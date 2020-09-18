#pragma once

extern GLFWwindow* window;

glm::vec3 position = glm::vec3 (0, 0, 5);
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFOV = 45.0f;
float speed = 3.0f;
float mouseSpeed = 0.005f;

glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;

glm::mat4 GetViewMatrix ()
{
    return viewMatrix;
}

glm::mat4 GetProjectionMatrix ()
{
    return projectionMatrix;
}

/*
void ScrollCallback(GLFWwindow* win, double xoff, double yoff) 
{
    float FOV = initialFOV - (5 * glfwGetMouseWheel());
}*/

void ComputeMatricesFromInputs ()
{
    // Only gotten the first time this function runs
    static double lastTime = glfwGetTime();
    
    // Delta Time Set Up
    double currentTime = glfwGetTime();
    float deltaTime = (float) (currentTime - lastTime);
    
    // Get Mouse Position
    double xpos, ypos;
    glfwGetCursorPos (window, &xpos, &ypos);
    
    // Get Screen Dimensions
    int windowWidth, windowHeight;
    glfwGetWindowSize (window, &windowWidth, &windowHeight);
    glfwSetCursorPos  (window, windowWidth/2, windowHeight/2);
    
    // Get Mouse Delta
    float dx =  (((float) windowWidth  / (float) 2)  - xpos);
    float dy =  (((float) windowHeight / (float) 2)  - ypos);
    
    // Set Angles
    horizontalAngle -= mouseSpeed * dx;
    verticalAngle   -= mouseSpeed * dy;
    
    verticalAngle = glm::clamp (verticalAngle, -glm::radians(90.0f), glm::radians(90.0f));
    
    // Get Forward, Right, and Up Vectors
    glm::vec3 forward  (
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
        );
    
    glm::vec3 right (
        sin(horizontalAngle - (3.14f / 2.0f)),
        0,
        cos(horizontalAngle - (3.14f / 2.0f))
        );
    
    glm::vec3 up = glm::cross(forward, right);
    
    // Keyboard Movement 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += forward * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= forward * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position -= right   * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position += right   * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) position -= up      * speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) position += up      * speed * deltaTime;
    
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);
    viewMatrix       = glm::lookAt(
        position,
        position + forward,
        up
        );
    lastTime = currentTime;
}
