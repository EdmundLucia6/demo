#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 128

// 模拟 sensors_event_t 数据结构
typedef struct {
    int32_t sensor;        // 传感器类型
    int32_t type;          // 数据类型
    float data[3];         // 数据内容（如加速度 x, y, z）
    int64_t timestamp;     // 时间戳
} sensors_event_t;

void fill_sensor_data(sensors_event_t *event) {
    event->sensor = 1;  // 模拟传感器类型
    event->type = 1;    // 模拟事件类型
    event->data[0] = 9.81;   // x 轴数据
    event->data[1] = 0.0;    // y 轴数据
    event->data[2] = 0.0;    // z 轴数据
    event->timestamp = time(NULL);  // 当前时间戳
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 创建 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 配置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // 绑定 socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(server_fd, 1) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n", SERVER_PORT);

    // 等待客户端连接
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");

    // 循环发送传感器数据
    while (1) {
        sensors_event_t event;
        fill_sensor_data(&event);

        // 发送数据
        if (send(client_fd, &event, sizeof(event), 0) == -1) {
            perror("Send failed");
            break;
        }

        printf("Sent data: sensor=%d, type=%d, data[0]=%.2f, timestamp=%lld\n",
               event.sensor, event.type, event.data[0], (long long)event.timestamp);

        // 每秒发送一次
        sleep(1);
    }

    // 关闭连接
    close(client_fd);
    close(server_fd);

    return 0;
}
