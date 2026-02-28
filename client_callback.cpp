#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

const int SHM_SIZE = 256;

int main() {
    // 连接到共享内存
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory creation failed");
        return 1;
    }

    char* shared_memory = (char*)shmat(shmid, nullptr, 0);
    if (shared_memory == (char*)-1) {
        perror("Shared memory attach failed");
        return 1;
    }

    // 将回调函数的名称写入共享内存
    const char* callback_name = "my_callback_function";
    strcpy(shared_memory, callback_name);
    std::cout << "Registered callback function: " << callback_name << std::endl;

    // 等待 Server 执行
    sleep(5);

    // 清理资源
    shmdt(shared_memory);
    return 0;
}
