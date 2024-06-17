#include "socket.hpp"

Socket::Socket(std::string socket_name) {
    this->socket_name = socket_name;
    
    listen_socket = socket(
        AF_UNIX,        // Unix domain socket
        SOCK_STREAM,    // TCP
        0
    );

    if (listen_socket == -1) {
        debug_log("FATAL: Failed to create socket");
        exit(1);
    }

    // Make sure that there is no previous socket file
    unlink(socket_name.c_str());

    local_addr.sun_family = AF_UNIX;
    strcpy(local_addr.sun_path, socket_name.c_str());

    int len = strlen(local_addr.sun_path) + sizeof(local_addr.sun_family);

    // Bind
    if (bind(listen_socket, (struct sockaddr*)&local_addr, len) != 0) {
        debug_log("FATAL: Error on binding socket");
        exit(1);
    }

    // Listen
    if (listen(listen_socket, n_incoming_conns) != 0) {
        debug_log("FATAL: Error on listen call");
        exit(1);
    }
}

Socket::~Socket() {}

void Socket::lock() { mtx_.lock(); }
void Socket::unlock() { mtx_.unlock(); }

bool Socket::accept_client() {
    bool success = true;

    debug_log("DEBUG: Waiting for the client to connect");
    debug_log("DEBUG: Socket name is {}", socket_name);

    data_socket = accept(listen_socket, NULL, NULL);

    if (data_socket == -1)
    {
        success = false;
        debug_log("ERROR: Error on accept call");
    }
    else
    {
        debug_log("DEBUG: Socket connection has been established");
    }

    if (!success) {        
        close(data_socket);
        unlink(socket_name.c_str());
    }

    return success;
}

bool Socket::send_data(std::string& data) {
    bool success = true;

    if (data.length() >= SOCK_SEND_BUFF_SIZE)
    {
        success = false;
        debug_log("ERROR: Socket buffer overflowed");
    }
    else
    {
        memset(send_buff, 0, SOCK_SEND_BUFF_SIZE * sizeof(char));

        strcpy(send_buff, data.c_str());

        success = write(data_socket, send_buff, strlen(send_buff) * sizeof(char)) != -1;
    }

    return success;
}

std::string& Socket::get_socket_name() { return socket_name; }

ClientSocket::ClientSocket(std::string socket_name) {
    if ((data_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        debug_log("FATAL: Error on socket() call");
        exit(1);
    }

    remote_addr.sun_family = AF_UNIX;
    strcpy(remote_addr.sun_path, socket_name.c_str());

    int len = strlen(remote_addr.sun_path) + sizeof(remote_addr.sun_family);

    if (connect(data_socket, (struct sockaddr*)&remote_addr, len) == -1) {
        debug_log("FATAL: Failed to connect to server socket");
        exit(1);    
    }

    debug_log("DEBUG: Client socket is ready");
}

ClientSocket::~ClientSocket() {
    close(data_socket);
}

std::optional<std::string> ClientSocket::recv_data() {
    memset(recv_buff, 0, SOCK_RECV_BUFF_SIZE * sizeof(char));

    if ((recv(data_socket, recv_buff, SOCK_RECV_BUFF_SIZE, 0)) > 0) {
        auto data_str = std::string(recv_buff);
        
        return data_str;
    }

    return std::nullopt;
} 
