#pragma once

#include <GLFW/glfw3.h>

class KeyboardHandler {
public:
    KeyboardHandler(GLFWwindow* window);
    //void update();
    bool isKeyPressed(int key) const;

private:
    GLFWwindow* window;
    bool keys[1024]; // Array to store key states
    void keyCallback(int key, int action);
};

