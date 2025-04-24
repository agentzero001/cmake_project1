#include "VulkanContext.h"



int main() {
    VulkanContext app;

    glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f) );
    

    try {
        app.run();
        std::cout << "success" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}