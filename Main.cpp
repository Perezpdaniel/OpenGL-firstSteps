#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex Shader source code: transforms each vertex by a 4x4 matrix (for rotation)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

// Fragment Shader source code: outputs a solid black color
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

int main() {
    // Initialize GLFW library (window/context management)
    glfwInit();

    // Set OpenGL version to 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 800, "Rotating Triangle", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers with GLAD
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the viewport size (drawing area in the window)
    glViewport(0, 0, 800, 800);

    // Define triangle vertices (equilateral triangle centered on screen)
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left
         0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right
         0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f  // Upper
    };

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Shaders are linked into the program, so we can delete them now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create and bind Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Describe vertex data layout to OpenGL
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind for safety
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Set background color and clear the screen
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Calculate rotation angle based on elapsed time
        float timeValue = glfwGetTime();
        float angle = timeValue; // radians

        float cosA = cos(angle);
        float sinA = sin(angle);

        // Manually build a 4x4 rotation matrix (Z axis)
        float transform[16] = {
             cosA, -sinA, 0.0f, 0.0f,
             sinA,  cosA, 0.0f, 0.0f,
             0.0f,  0.0f, 1.0f, 0.0f,
             0.0f,  0.0f, 0.0f, 1.0f
        };
                                                
        // Pass the rotation matrix to the vertex shader
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

        // Draw the triangle
        glBindVertexArray(VAO);
		//glLineWidth(3); // Set line width for outline
		glDrawArrays(GL_TRIANGLES, 0, 3); //GL_TRIANGLES for fully filled triangle GL_LINE_LOOP for just the outline

        // Swap front and back buffers and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup: delete OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
