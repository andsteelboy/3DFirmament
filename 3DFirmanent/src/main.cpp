#include<glad/glad.h>
#include<glfw3.h>

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

const unsigned int WIDTH = 640;
const unsigned int HEIGHT = 480;


void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
char* readShader(const char* filepath);

const char* vertexShaderCode = readShader("C:/3DFirmament/3DFirmanent/3DFirmanent/src/shader/Vertex.shader");

const char*  fragmentShaderCode = readShader("C:/3DFirmament/3DFirmanent/3DFirmanent/src/shader/Fragment.shader");


int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window= glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,frameBufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };  //in NDC coordinates for simplicity
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr<< "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader =glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderPorgram;
    shaderPorgram = glCreateProgram();
    glAttachShader(shaderPorgram, vertexShader);
    glAttachShader(shaderPorgram, fragmentShader);
    glLinkProgram(shaderPorgram);

    glGetProgramiv(shaderPorgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderPorgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::LINKING::COMPILATION FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    

    //Vertex attributes-->layout(location=0)
    unsigned int VBO,VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        processInput(window);
        glClearColor(0.4f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Render here */
        glUseProgram(shaderPorgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //glClear(GL_COLOR_BUFFER_BIT);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderPorgram);
    glfwTerminate();


    return 0;
}


void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

char* readShader(const char* filepath)
{
    std::ifstream file(filepath);
    std::string content;
    std::stringstream buffer;
    if (!file.is_open()) {
        std::cerr << "Unable to open file";
    }
    buffer << file.rdbuf();
    file.close();
    content = buffer.str();
    char* result = new char[content.size() + 1];
    std::memcpy(result, content.c_str(), content.size() + 1); 

    return result;
}


