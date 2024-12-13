#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char* SOCKET_PATH = "/tmp/callback_socket";

int main() {
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        return 1;
    }

    std::string message = "Registering callback";
    write(client_socket, message.c_str(), message.size());

    char buffer[256] = {0};
    read(client_socket, buffer, sizeof(buffer));
    std::cout << "Client received: " << buffer << std::endl;

    close(client_socket);
    return 0;
}
