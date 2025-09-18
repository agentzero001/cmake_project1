#include "keyboard.h"
#include <iostream>

KeyboardHandler::KeyboardHandler(GLFWwindow* window) : window(window) {
    // Initialize key states to false
    std::fill(std::begin(keys), std::end(keys), false);

    // Set the key callback function
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        // Get the KeyboardHandler instance associated with the window
        KeyboardHandler* handler = static_cast<KeyboardHandler*>(glfwGetWindowUserPointer(win));
        if (handler) {
            handler->keyCallback(key, action);
        }
        });

    // Set this instance as the user pointer for the window
    glfwSetWindowUserPointer(window, this);
}


bool KeyboardHandler::isKeyPressed(int key) const {
    return keys[key];
}


void KeyboardHandler::keyCallback(int key, int action) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}
