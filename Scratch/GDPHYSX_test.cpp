#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Model3D.hpp"
#include "MyCamera.hpp"
#include "light.hpp"

#define CAMERA2_POS orthoPos

#define WINDOW_HEIGHT 700.f
#define WINDOW_WIDTH 700.f

glm::vec3 objectPosition = glm::vec3(0.f, 0.f, 0.f); //for position of the object
glm::vec3 lighPosition1 = glm::vec3(0.f, 35.f, 0.f); //for light

OrthographicCamera ortho;

std::vector<Model3D*> models;

glm::vec3 orthoPos = ortho.getCameraPos();

/////////////////////////////////////////////////////////////////////////////////////////////

void Key_CallBack(GLFWwindow* window, //pointer to the window
    int key, //keycode of the press
    int scancode, //physical position of the press
    int action, //either press / release
    int mods) {// which modifier keys is held down

    if (key == GLFW_KEY_ESCAPE) { //close window
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void)

{
    std::cout << " Hello World";

    GLFWwindow* window;
    int colorIndex = 0;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bumanglag, Nikos Railey", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    ortho.setCamera(true);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //creating vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //assign the source to the vertex shader
    glShaderSource(vertexShader, 1, &v, NULL);
    //Compile the Vertex Shader
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &f, NULL);
    glCompileShader(fragShader);

    //Creating shader program
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*****************************************
    *                                        *
    *                                        *
    *      Listening for Keyboard Input      *
    *                                        *
    *                                        *
    *****************************************/

    glfwSetKeyCallback(window, Key_CallBack); 

    pointlight pointlight(5.0f);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto firstModel = new Model3D(objectPosition, "3D/white.png", "3D/sphere.obj", glm::vec3(2.f, 2.f, 2.f), 1);
    firstModel->spawn();

    models.push_back(firstModel);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*****************************************
     *                                       *
     * Loop until the user closes the window *
     *                                       *
     *****************************************/

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        models[0]->setYTheta(0.f);
        models[0]->setPosition(objectPosition);

        ortho.setCameraPos(orthoPos);
        ortho.setProjectionMatrix();
        ortho.setViewMatrix(ortho.getCameraPos() + glm::vec3(0.1f, -1.f, 0.0f));
        ortho.projection_viewMatrix(shaderProg);

        pointlight.lightColor = glm::vec3(0, 1, 2);

        glUseProgram(shaderProg);
        models[0]->update(shaderProg);

        glBindVertexArray(models[0]->getVAO());

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, models[0]->getTexture());
        glUniform1i(tex0Address, 0);

        models[0]->draw();

        pointlight.setLightPosition(lighPosition1);
        pointlight.setbrightness(shaderProg);
        pointlight.setLight(shaderProg, ortho.getCameraPos());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}