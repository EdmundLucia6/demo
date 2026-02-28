#include <iostream>

extern "C" void my_callback_function() {
    std::cout << "Callback function executed!" << std::endl;
}
