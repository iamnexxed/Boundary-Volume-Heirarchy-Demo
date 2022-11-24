#include <glad/glad.h>

float trivertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragShaderSource = "#version 330 core\n"
                                 "out vec4 FragColor;\n"

                                 "void main()\n"
                                 "{\n"
                                 " FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
                                 "}\0";

unsigned int triVBO;
unsigned int trishaderProgram;
unsigned int triVAO;

void setupTri()
{

    glGenBuffers(1, &triVBO);
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trivertices), trivertices, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragmentShader);

    trishaderProgram = glCreateProgram();
    glAttachShader(trishaderProgram, vertexShader);
    glAttachShader(trishaderProgram, fragmentShader);
    glLinkProgram(trishaderProgram);
    glUseProgram(trishaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &triVAO);

    glBindVertexArray(triVAO);
    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, triVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trivertices), trivertices, GL_STATIC_DRAW);
    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void drawTri() 
{
    glUseProgram(trishaderProgram);
    glBindVertexArray(triVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
