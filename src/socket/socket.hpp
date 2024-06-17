#pragma once

#include "../shared.hpp"

class Socket {
public:
    Socket(std::string socket_name);
    ~Socket();

    Socket(Socket const&) = delete;
    Socket(Socket&&) = default;
    Socket& operator = (Socket const&) = delete;
    Socket& operator = (Socket&&) = default;

    void lock();
    void unlock();

    bool accept_client();
    bool send_data(std::string& data);

    std::string& get_socket_name();

private:
    std::mutex mtx_;
    std::string socket_name;

    char send_buff[SOCK_SEND_BUFF_SIZE];

    int n_incoming_conns{1};
    int listen_socket{-1};
    int data_socket{-1};

    struct sockaddr_un local_addr;
};

class ClientSocket {
public:
    ClientSocket(std::string socket_name);
    ~ClientSocket();

    std::optional<std::string> recv_data();

private:
    int data_socket{-1};

    char recv_buff[SOCK_RECV_BUFF_SIZE];

    struct sockaddr_un remote_addr;
};
