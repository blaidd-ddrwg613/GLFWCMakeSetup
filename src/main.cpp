#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(void)
{
    // Init GLFW and set OpenGL version/profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // We create the GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create glfw window" << "\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load the OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shaders
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

    const char *fragmentShaderSource2 = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                       "}\n\0";


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION\n" << infoLog << "\n";
    }

    unsigned int fragmentShader, fragmentShader2;
    fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader2 =glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION\n" << infoLog << "\n";
    }
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION\n" << infoLog << "\n";
    }

    // Create Shader Program
    unsigned int shaderProgram, shaderProgram2;
    shaderProgram = glCreateProgram();
    shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram);
    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::FAILED\n" << infoLog << "\n";
    }
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::FAILED\n" << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);

    // Triangle vertices
    float vertices[] =
            {
            // First Triangle
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f,
            0.0f,0.5f,0.0f,
            };

    // Second Triangle
    float triangle2[] =
            {
                    0.5f,-0.5f,0.0f,
                    1.0f,0.0f,0.0f,
                    0.5f,0.5f,0.0f
            };

    // Element Buffer Vertices
    float vertices2[] =
            {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
            };
    unsigned int indices[] =
            {
            0,1,3,
            1,2,3
            };

    unsigned int VBOs[2],VAOs[2],EBO;

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);

    // First Triangle
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    // Second Triangle
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(0);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Enable wireframe mode
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Rendering
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // Draw First Triangle
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES,0, 3); // To draw two triangles out of the same array we change the 3 to 6
        // Draw second triangle
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);

//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

