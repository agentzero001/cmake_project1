#include "VulkanContext.h"
// #include <iostream>
// #include <GLFW/glfw3.h>


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

// int main() {
//     std::cout << "Starting app\n";

//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW\n";
//         return -1;
//     }

//     GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Window", nullptr, nullptr);
//     if (!window) {
//         std::cerr << "Failed to create GLFW window\n";
//         glfwTerminate();
//         return -1;
//     }

//     glfwMakeContextCurrent(window);

//     while (!glfwWindowShouldClose(window)) {
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwDestroyWindow(window);
//     glfwTerminate();
//     return 0;
// }