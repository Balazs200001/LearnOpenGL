#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

void frame_buffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLFWwindow* initialize_window()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
	    std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))  // NOLINT(clang-diagnostic-cast-function-type-strict)
    {
	    std::cout << "Failed to initialize GLAD" << '\n';
        return nullptr;
    }

    return window;
}

int main()
{
    GLFWwindow* window = initialize_window();
    if (window == nullptr)
    {
        return -1;
    }

	Shader ourShader("shaders/shader.vs", "shaders/shader.fs");

    // set up vertex data
    constexpr float firstTriangle[] = {
        // positions         // colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // left 
         0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // right
        -0.25f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top 
    };
    constexpr float secondTriangle[] = {
		// positions         // colors
        0.0f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // left
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // right
        0.25f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // top 
    };
    unsigned int vertexBufferObjects[2], vertexArrayObjects[2];

    glGenVertexArrays(2, vertexArrayObjects);
    glGenBuffers(2, vertexBufferObjects);

	// first triangle setup
    glBindVertexArray(vertexArrayObjects[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	// second triangle setup
    glBindVertexArray(vertexArrayObjects[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjects[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const float timeValue = static_cast<float>(glfwGetTime());
		ourShader.use();
		ourShader.setFloat("xOffset", timeValue / 10.f);
        glBindVertexArray(vertexArrayObjects[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(vertexArrayObjects[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, vertexArrayObjects);
    glDeleteBuffers(2, vertexBufferObjects);

    glfwTerminate();
    return 0;
}
