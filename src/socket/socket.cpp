#include "socket.hpp"

Socket::Socket() {
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
    unlink(socket_name);

    local_addr.sun_family = AF_UNIX;
    strcpy(local_addr.sun_path, socket_name);

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

    return success;
}

bool Socket::send_data(std::string& data) {
    bool success = true;

    if (data.length() >= SOCKET_BUFF_SIZE)
    {
        success = false;
        debug_log("ERROR: Socket buffer overflowed");
    }
    else
    {
        memset(send_buff, 0, SOCKET_BUFF_SIZE * sizeof(char));

        strcpy(send_buff, data.c_str());

        success = write(data_socket, send_buff, strlen(send_buff) * sizeof(char)) != -1;
    }

    return success;
}

Socket::~Socket() {
    close(data_socket);
    unlink(socket_name);

    debug_log("DEBUG: Socket has been closed");
}
