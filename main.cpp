#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_operation.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <map>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
#include "Header/AnimateModelClass.h"
#include "Header/shader_Class.h"
#include "Header/Camera.h"
#include "Header/character_type_1_class.h"
#include "Header/box.h"
#include "Header/light_box.h"
#include "Header/frame_buffer_class.h"
#include "Header/square_class.h"
#include "Header/shadow_generator_class.h"
#include "Header/game.h"
#include "Header/Cubemap.h"
#include "Header/perlin_noise.h"


game* GM_R;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



bool firstMouse = true;
float lastX = 400.0f;
float lastY = 300.0f;


void scroll_callback( GLFWwindow* window, double xoffset, double yoffset)
{
    GM_R->camera.ProcessMouseScroll(yoffset);
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 4.0f * GM_R->delta_time; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        GM_R->camera.ProcessKeyboard(FORWARD, GM_R->delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        GM_R->camera.ProcessKeyboard(BACKWARD, GM_R->delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        GM_R->camera.ProcessKeyboard(LEFT, GM_R->delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        GM_R->camera.ProcessKeyboard(RIGHT, GM_R->delta_time);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        GM_R->camera.ProcessKeyboard(UP, GM_R->delta_time);
        
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    GM_R->camera.ProcessMouseMovement(xpos - lastX, lastY - ypos);
    lastX = xpos;
    lastY = ypos;
    
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}




int main()
{
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__//delet
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback( window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);





 





    

#pragma endregion

    


    
    game GM{};
    GM_R = &GM ;
   
  
  
   

   

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_3D);

    



   
    Frame_Buffer::bind_window_0();
    glViewport(0, 0, 800, 600);
    

    while (!glfwWindowShouldClose(window))
    {
       
        // input
       // -----
        processInput(window);//esc close the window
        glfwSwapBuffers(window);//swap Buffers
        glfwPollEvents();//checks if any events 
                        //are triggered like keyboard input 
       glfwSetCursorPosCallback(window, mouse_callback);
        // render
        // ------
       
   
      
      

        GM.cycle();
        
  
      
    }

}

