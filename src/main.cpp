#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <vector>
#include <random>

#include "shaderClass.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

const unsigned int SCREEN_WIDTH = 900;
const unsigned int SCREEN_HEIGHT = 900;

class Box {
public:
    Box(GLfloat x, GLfloat y, GLfloat z, GLfloat edge);
    GLfloat edge;
    GLfloat x, y, z;
};

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;
std::vector<Box*> boxes;

Box::Box(GLfloat x_, GLfloat y_, GLfloat z_, GLfloat ed) {

    x = x_;
    y = y_;
    z = z_;
    edge = ed;

    vertices.insert(vertices.end(),{
        x_,         y_,         z_,         0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        x_ + ed,    y_,         z_,         0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        x_ + ed,    y_,         z_ - ed,    0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        x_,         y_,         z_ - ed,    0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        x_,         y_ + ed,    z_,         0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        x_ + ed,    y_ + ed,    z_,         0.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        x_ + ed,    y_ + ed,    z_ - ed,    0.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        x_,         y_ + ed,    z_ - ed,    0.0f, 0.0f, 0.0f,   0.0f, 1.0f
    });

    if(boxes.empty()) {
        indices.insert(indices.end(), {
                0, 1, 4,
                1, 5, 4,
                3, 4, 7,
                0, 3, 4,
                0, 2, 3,
                0, 1, 2,
                1, 2, 6,
                1, 5, 6,
                3, 6, 7,
                2, 3, 6,
                4, 5, 7,
                5, 6, 7
        });
    }
    else {
        indices.insert(indices.end(), {
                0 + 8, 1 + 8, 4 + 8,
                1 + 8, 5 + 8, 4 + 8,
                3 + 8, 4 + 8, 7 + 8,
                0 + 8, 3 + 8, 4 + 8,
                0 + 8, 2 + 8, 3 + 8,
                0 + 8, 1 + 8, 2 + 8,
                1 + 8, 2 + 8, 6 + 8,
                1 + 8, 5 + 8, 6 + 8,
                3 + 8, 6 + 8, 7 + 8,
                2 + 8, 3 + 8, 6 + 8,
                4 + 8, 5 + 8, 7 + 8,
                5 + 8, 6 + 8, 7 + 8
        });

    }

}

int main()
{
    // Initialize GLFW
    glfwInit();

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "zadanie 1", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    Box *box1 = new Box(-1.0f, -1.0f, 1.0f, 1.0f);
    boxes.push_back(box1);

    Box *box2 = new Box(1.0f, -1.0f, 1.0f, 1.0f);
    boxes.push_back(box2);

    // Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("OpenGLPAG/res/shaders/basic.vert", "OpenGLPAG/res/shaders/basic.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, vertices.size() * sizeof(GLfloat));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, indices.size() * sizeof(GLuint));

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    Texture stoneTex("OpenGLPAG/res/textures/stone.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    stoneTex.texUnit(shaderProgram, "tex0", 0);
    Texture spaceTex("OpenGLPAG/res/textures/space.jpg", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);
    spaceTex.texUnit(shaderProgram, "tex1", 1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(-0.5f, 0.0f, -6.0f));
    proj = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();

        model = glm::mat4(1.0f);

        // Outputs the matrices into the Vertex Shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
//        int colLoc = glGetUniformLocation(shaderProgram.ID, "inColor");
//        glUniform3fv(colLoc, 1, glm::value_ptr(col));


        glActiveTexture(GL_TEXTURE0);
        stoneTex.Bind();
        glActiveTexture(GL_TEXTURE1);
        spaceTex.Bind();
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();

        // Draw primitives, number of indices, datatype of indices, index of indices
        //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "first"), 0);
        glDrawElementsBaseVertex(GL_TRIANGLES, indices.size()/2, GL_UNSIGNED_INT, 0, 0);
        glUniform1i(glGetUniformLocation(shaderProgram.ID, "first"), 1);
        glDrawElementsBaseVertex(GL_TRIANGLES, indices.size()/2, GL_UNSIGNED_INT, 0, 8);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window);

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    stoneTex.Delete();
    spaceTex.Delete();
    shaderProgram.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}