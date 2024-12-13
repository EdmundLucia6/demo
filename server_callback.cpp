#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char* SOCKET_PATH = "/tmp/callback_socket";

void handle_client(int client_socket) {
    char buffer[256] = {0};
    // 读取客户端发送的消息
    read(client_socket, buffer, sizeof(buffer));
    std::cout << "Server received: " << buffer << std::endl;

    // 模拟事件触发，回调
    std::string response = "Event occurred. Callback triggered.";
    write(client_socket, response.c_str(), response.size());
}

int main() {
    int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_un server_addr;
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    unlink(SOCKET_PATH);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_socket, 5);
    std::cout << "Server is listening..." << std::endl;

    int client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    handle_client(client_socket);

    close(client_socket);
    close(server_socket);
    unlink(SOCKET_PATH);
    return 0;
}
