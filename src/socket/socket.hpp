#pragma once

#include "../shared.hpp"

class Socket {
public:
    Socket();
    ~Socket();

    bool accept_client();
    bool send_data(std::string& data);

private:
    const char* socket_name{SOCKET_NAME};

    char send_buff[SOCKET_BUFF_SIZE];

    int n_incoming_conns{1};

    int listen_socket{-1};
    int data_socket{-1};

    struct sockaddr_un local_addr;
};
