/***
* Phong Lighting (basic)
* @author mubastan
* @date   07.12.2015
****/

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>

Light light;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void move_camera();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

// Camera
Camera  camera(glm::vec3(0.0f, 1.5f, 15.0f));

GLfloat lastX  =  WIDTH  / 2.0;
GLfloat lastY  =  HEIGHT / 2.0;
bool    keys[1024];

// Light position
glm::vec3 lightPos(2.5f, 3.0f, 3.0f);
glm::vec3 lightColor(1.0f,1.0f, 0.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame



void draw(Model& objModel, Shader& shader, double x,double y,double z,double s,char o)
{       float y_c;
        if(o == 't') y_c =0.0;
        if(o == 'c') y_c=1.0;
       //Shader shader("model_loading.vs.glsl", "model_loading.fs.glsl");
        //Model treeModel("./tree/tree.obj");
        //glm::mat4 model;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0*x, y_c*y, 1.2f*z));
        model = glm::rotate(model, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.08f*s, 0.08f*s, 0.08f*s));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glm::vec3 objColor(0.3f, 0.8f, 0.0f);
        glUniform3fv(glGetUniformLocation(shader.Program, "objectColor"), 1, glm::value_ptr(objColor));
        objModel.Draw(shader);


}


// The MAIN function, from here we start the application and run the game loop
int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting2", 0, 0);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program
    Shader modelShader("model.vs.glsl", "model.fs.glsl");
    Shader lampShader("model.vs.glsl", "lamp.fs.glsl");     // vertex shader is the same as the object
/////////////////////////////////////////////

    Shader textureLightShader("textured_lighting.vs.glsl", "textured_lighting.fs.glsl");


////////////////////////////////////////////
    /// Load object models
    //Model cubeModel("./models/cube.obj");
    Model sphereModel("./models/sphere.obj");
    Model cylModel("./models/cylinder.obj");
    Model cubeModel("./cubex/cube.obj");
    Model treeModel("./tree/tree.obj");
    Model tcubeModel("./texcube/cube.obj"); // textured cube
    // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // solid
    //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    // Event loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        move_camera();

        // Clear the colorbuffer
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// move light source with time
        GLfloat radius = 5.0f;
        lightPos.x = 1.0+radius*sin(0.5f*glfwGetTime());
        lightPos.y = 1.0+radius*cos(0.5f*glfwGetTime());
        lightColor.x = lightPos.y/6.0f;
        lightColor.y = lightPos.y/6.0f;
        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

        /// Draw the lamp object, as a sphere
        lampShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
        GLint viewLoc  = glGetUniformLocation(lampShader.Program, "view");
        GLint projLoc  = glGetUniformLocation(lampShader.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLint lightColorLoc  = glGetUniformLocation(lampShader.Program, "lightColor");
        /// color of the light

        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));  // send color of the light to frag. shader of lamp
        sphereModel.Draw(lampShader);

        /// Draw objects
        modelShader.Use();  // use object/model shaders

        GLint objectColorLoc = glGetUniformLocation(modelShader.Program, "objectColor");
        GLint lightColorLoc2  = glGetUniformLocation(modelShader.Program, "lightColor");
        GLint lightPosLoc    = glGetUniformLocation(modelShader.Program, "lightPos");
        GLint viewPosLoc     = glGetUniformLocation(modelShader.Program, "viewPos");

        glUniform3fv(lightColorLoc2, 1, glm::value_ptr(lightColor));  // send color of the light to frag. shader of object
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(camera.Position));

        // Get the uniform locations from vertex shader
        modelLoc = glGetUniformLocation(modelShader.Program, "model");
        viewLoc  = glGetUniformLocation(modelShader.Program, "view");
        projLoc  = glGetUniformLocation(modelShader.Program, "projection");
        // Pass the matrices to vertex shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        ///cubeFLoor

        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(20.0f, 0.1f, 20.0f));	    // It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cubeModel.Draw(modelShader);
        /*
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(5.0f, 0.1f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(lshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glm::vec3 objColor(0.0f, 1.0f, 0.0f);
        glUniform3fv(glGetUniformLocation(lshader.Program, "objectColor"), 1, glm::value_ptr(objColor));
        cubeModel.Draw(shader);
        */
        /*/// Draw cube object
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.3f); // color of the cube
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cubeModel.Draw(modelShader);*/
//////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////



        /// Draw cylinder object
        glUniform3f(objectColorLoc, 0.9f, 0.5f, 1.0f);  // color of the cylinder
        model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 2.0f));
       //  model = glm::scale(model, glm::vec3(2.0f, 3.1f, 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cylModel.Draw(modelShader);

            /// Draw cylinder object
        glUniform3f(objectColorLoc, 1.0f, 0.0f, 0.0f);  // color of the cylinder
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.0f, 0.0f, -2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        cylModel.Draw(modelShader);

        draw(treeModel, modelShader, 1.0,1.0,1.0,1.0,'t');
        draw(treeModel, modelShader, 2.0,1.0,1.0,1.0,'t');
        draw(treeModel, modelShader, 3.0,1.0,1.0,1.0,'t');
        draw(treeModel, modelShader, 1.5,1.2,1.0,0.8,'t');
        draw(treeModel, modelShader, 2.5,1.2,2.0,0.8,'t');
        draw(treeModel, modelShader, 1.9,1.2,-2.0,0.8,'t');
        draw(treeModel, modelShader,-1.0,1.0,2.0,1.0,'t');
        draw(treeModel, modelShader,-1.5,2.0,2.0,1.0,'t');
        draw(treeModel, modelShader,-2.0,1.0,1.0,1.0,'t');
        draw(treeModel, modelShader,-2.5,1.0,2.1,1.0,'t');
        draw(treeModel, modelShader,-3.0,-1.0,1.2,1.0,'t');
        draw(treeModel, modelShader,-3.5,-1.0,1.5,1.0,'t');
        draw(treeModel, modelShader,-4.0,-1.0,1.9,1.0,'t');
        /*for(int i =0;i<=10;i++)
        {
            for(int j=0;j<=10;j++)
            {
            draw(treeModel, modelShader,i,0.0,j,1.0,'t');
            }
        }*/





        textureLightShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(textureLightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(textureLightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


         /// textured cube
        light.position = lightPos;
        model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, -2.5f));
        GLfloat angle = 0.5*glfwGetTime() * 45.0f;
        model = glm::rotate(model,angle, glm::vec3(0.0f, 4.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(textureLightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(textureLightShader.Program, "ct"), 1);
        light.send2shader(textureLightShader.Program);
        tcubeModel.Draw(textureLightShader);
        /*GLfloat angle = (1+i%3)*glfwGetTime() * 45.0f;
            model = glm::rotate(model, angle, glm::vec3(i%3, 0.f, 1.f));*/
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void move_camera()
{
    // Camera controls
    if(keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(keys[GLFW_KEY_U] || keys[GLFW_KEY_PAGE_UP])
        camera.ProcessKeyboard(UP, deltaTime);
    if(keys[GLFW_KEY_J] || keys[GLFW_KEY_PAGE_DOWN])
        camera.ProcessKeyboard(DOWN, deltaTime);
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}



