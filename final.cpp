#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <thread>
#include <chrono>
#include <asio.hpp>
#include <atomic>
#include <boost/asio/strand.hpp>
#include <boost/bind/bind.hpp>
#include <memory>
#include <vector>

#include "common_header.h"
#include "json.hpp"
using json = nlohmann::json;

using boost::asio::ip::tcp;
using namespace std;
using namespace boost;

#define BUFFER_SIZE 1024
#define PORT 8080

// Global atomic flag for socket validity
std::atomic<bool> socket_valid(true);

string handle_request(const string &json_request, Board &board)
{
    std::cout << "Raw JSON request: " << json_request << std::endl;

    json request = json::parse(json_request);
    json response;

    string purpose = request["purpose"];

    if (purpose == "generateMove")
    {
        string position = request["position"];
        // Extract column and row from position (e.g., 'b3' -> col = 'b', row = 3)
        char col = position[0];
        int row = position[1] - '0';
        string pieceName = request["pieceName"];
        vector<vector<string>> possible_moves;
        if (pieceName == "knight")
        {
            possible_moves = get_knight_moves(board, col, row);
        }
        if (pieceName == "king")
        {
            possible_moves = get_king_moves(board, col, row);
        }
        if (pieceName == "queen")
        {
            possible_moves = get_queen_moves(board, col, row);
        }
        if (pieceName == "pawn")
        {
            possible_moves = get_pawn_moves(board, col, row);
        }
        if (pieceName == "rook")
        {
            possible_moves = get_rook_moves(board, col, row);
        }
        if (pieceName == "bishop")
        {
            possible_moves = get_bishop_moves(board, col, row);
        }

        response["possibleMoves"] = possible_moves[0];
        response["possibleCaptures"] = possible_moves[1];
    }
    else if (purpose == "updateBoard")
    {
        string move = request["position"];
        // Process the move on the board
        // bool success = update_board(board, move);
        bool success = moveGeneration(move, board);
        printBoard(board);

        response["status"] = success ? "success" : "error";
        response["position"] = move;
    }

    return response.dump(); // Convert JSON response to string
}

string base64_encode(unsigned char *input, int length)
{
    BIO *bmem = BIO_new(BIO_s_mem());
    BIO *b64 = BIO_new(BIO_f_base64());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);

    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);

    string encoded(bptr->data, bptr->length - 1);
    BIO_free_all(b64);

    return encoded;
}

string sha1_hash(const string &input)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(input.c_str()), input.length(), hash);
    return string(reinterpret_cast<char *>(hash), SHA_DIGEST_LENGTH);
}

string websocket_handshake_response(const string &key)
{
    string magic_string = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    string hashed_key = sha1_hash(magic_string);
    return base64_encode(reinterpret_cast<unsigned char *>(&hashed_key[0]), hashed_key.size());
}

string decode_websocket_frame(const char *buffer, int length)
{
    unsigned char payload_length = buffer[1] & 0x7F;
    int mask_index = 2;
    if (payload_length == 126)
    {
        mask_index = 4;
    }
    else if (payload_length == 127)
    {
        mask_index = 10;
    }

    char masking_key[4];
    memcpy(masking_key, &buffer[mask_index], 4);

    string decoded_message;
    for (int i = mask_index + 4; i < length; ++i)
    {
        decoded_message += buffer[i] ^ masking_key[(i - (mask_index + 4)) % 4];
    }

    return decoded_message;
}

string encode_websocket_frame(const string &message)
{
    string frame;
    frame += 0x81; // First byte: FIN bit set, opcode for text frame (0x1)
    
    // Payload length encoding
    if (message.size() <= 125)
    {
        frame += static_cast<char>(message.size()); // No masking and length <= 125
    }
    else if (message.size() <= 65535)
    {
        frame += 126;
        frame += static_cast<char>((message.size() >> 8) & 0xFF);
        frame += static_cast<char>(message.size() & 0xFF);
    }
    else
    {
        frame += 127;
        for (int i = 7; i >= 0; --i)
        {
            frame += static_cast<char>((message.size() >> (8 * i)) & 0xFF);
        }
    }

    // Append the actual message
    frame += message;
    return frame;
}


class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
{
public:
    WebSocketSession(tcp::socket socket, asio::io_context &context, Board &board)
        : socket_(std::move(socket)),
          strand_(asio::make_strand(context)),
          timer_(context), board(board) {}

    void start()
    {
        do_handshake();
    }

private:
    void do_handshake()
    {
        auto self = shared_from_this();
        asio::async_read_until(socket_, asio::dynamic_buffer(buffer_), "\r\n\r\n",
                               asio::bind_executor(strand_, [this, self](boost::system::error_code ec, std::size_t length)
                                                   {
                if (!ec) {
                    std::string request(buffer_.substr(0, length));
                    buffer_.erase(0, length);

                    // Extract WebSocket key and send handshake response
                    size_t start_key = request.find("Sec-WebSocket-Key: ") + 19;
                    size_t end_key = request.find("\r\n", start_key);
                    std::string key = request.substr(start_key, end_key - start_key);
                    std::string accept_key = websocket_handshake_response(key);
                    std::string response =
                        "HTTP/1.1 101 Switching Protocols\r\n"
                        "Upgrade: websocket\r\n"
                        "Connection: Upgrade\r\n"
                        "Sec-WebSocket-Accept: " + accept_key + "\r\n\r\n";

                    do_write(response);
                } }));
    }

    void do_write(const std::string &message)
    {
        auto self = shared_from_this();
        asio::async_write(socket_, asio::buffer(message),
                          asio::bind_executor(strand_, [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                              {
                if (!ec) {
                    cout<<"handshake completed and socket id:"<<socket_.native_handle()<<endl;
                     
                    // Start reading messages from the client
                    do_read();
                    // Start sending periodic messages to the client
                    do_send_periodic_messages();
                } }));
    }

    void do_read()
    {
        auto self = shared_from_this();
        socket_.async_read_some(asio::buffer(read_buffer_),
                                asio::bind_executor(strand_, [this, self](boost::system::error_code ec, std::size_t length)
                                                    {
                if (!ec) {
                    std::string message = decode_websocket_frame(read_buffer_, length);
                    std::cout << "Received: " << message << std::endl;

                    json parsedMessage = json::parse(message);
                    cout<<parsedMessage.dump()<<endl;
                    string response = handle_request(message,board);



                    std::string response_frame = encode_websocket_frame(response);
                    do_write(response_frame);

                    // Continue reading
                    do_read();
                } else {
                    std::cout << "Client disconnected or error occurred: " << ec.message() << std::endl;
                    socket_.close();
                } }));
    }

    void do_send_periodic_messages()
    {
        auto self = shared_from_this();
        timer_.expires_after(std::chrono::seconds(10));
        timer_.async_wait(asio::bind_executor(strand_, [this, self](boost::system::error_code ec)
                                              {
            if (!ec) {
                std::string message = "Hello from the server!";
                std::string encoded_message = encode_websocket_frame(message);
                asio::async_write(socket_, asio::buffer(encoded_message),
                    asio::bind_executor(strand_, [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                        if (!ec) {
                            std::cout << "Sent periodic message to client: Hello from the server!" << std::endl;
                            // Continue sending periodic messages
                            do_send_periodic_messages();
                        } else {
                            socket_.close();
                        }
                    })
                );
            } }));
    }

    tcp::socket socket_;
    asio::strand<asio::io_context::executor_type> strand_;
    asio::steady_timer timer_;
    std::string buffer_;
    char read_buffer_[BUFFER_SIZE];
    Board board;
};

class WebSocketServer
{
public:
    WebSocketServer(asio::io_context &context, short port, Board &board)
        : acceptor_(context, tcp::endpoint(tcp::v4(), port)),
          context_(context), board(board)
    {
        cout << "Server is started and Listening on port " << port << endl;
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<WebSocketSession>(std::move(socket), context_, board)->start();
                }
                do_accept(); // Continue accepting new connections
            });
    }

    tcp::acceptor acceptor_;
    asio::io_context &context_;
    Board board;
};

int main()
{
    try
    {
        Board board;
        string moves;
        initializeBoard(board);
        // printBoard(board);
        asio::io_context context;
        WebSocketServer server(context, PORT, board);

        asio::thread_pool pool(4); // Thread pool with 4 threads
        for (int i = 0; i < 4; ++i)
        {
            asio::post(pool, [&context]
                       { context.run(); });
        }

        pool.join();
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}