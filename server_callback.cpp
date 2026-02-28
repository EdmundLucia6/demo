#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dlfcn.h>
#include <cstring>
#include <unistd.h>

const int SHM_SIZE = 256;

int main() {
    // 创建共享内存
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("Shared memory creation failed");
        return 1;
    }

    // 连接到共享内存
    char* shared_memory = (char*)shmat(shmid, nullptr, 0);
    if (shared_memory == (char*)-1) {
        perror("Shared memory attach failed");
        return 1;
    }

    std::cout << "Server is waiting for callback registration..." << std::endl;

    // 等待回调函数名称
    while (strlen(shared_memory) == 0) {
        sleep(1); // 等待客户端注册回调
    }

    std::cout << "Received callback function name: " << shared_memory << std::endl;

    // 动态加载回调函数
    void* handle = dlopen("./libcallback.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Failed to load library: " << dlerror() << std::endl;
        return 1;
    }

    // 使用函数名找到对应函数
    void (*callback)() = (void (*)())dlsym(handle, shared_memory);
    if (!callback) {
        std::cerr << "Failed to find function: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }

    // 调用回调函数
    std::cout << "Calling the registered callback function..." << std::endl;
    callback();

    // 清理资源
    dlclose(handle);
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
