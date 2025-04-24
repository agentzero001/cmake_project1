#include "app.h"



int main() {
    VulkanApp app;

    try {
        app.run();
        std::cout << "success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}