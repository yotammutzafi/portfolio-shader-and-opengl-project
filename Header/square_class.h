#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader_Class.h"
#include "VAOClass.h"
#include "light_struct.h"
#include "Texture_Class.h"
class square
{
private:
    std::vector<float> vertices;
    Shader shader;
    unsigned int texture_ID;
    VAO mesh;
    glm::vec3 position;
    glm::vec3 sizes;
public:
    float time;

    square(unsigned int texture_ID,glm::vec3 position,glm::vec3 sizes) :time{0.0f},
        position{ position }, sizes{sizes},
        shader("shaders/vertex_shader_square", "shaders/fragment_shader_square"), texture_ID{ texture_ID },
        mesh(false)
        
    {
        this->vertices = {
            // positions      // texture coords
              -1.0f, -1.0f,    0.0f, 0.0f,
              -1.0f,  0.0f,    0.0f,  1.0f,
               0.0f, -1.0f,     1.0f, 0.0f,
               0.0f,  0.0f,     1.0f,  1.0f,
             
        };
        setup();
        
    }
   
    void setup()
    {
        bind_shader();
        uniform_setup();
        setup_VAO();
    }
    
    void bind_shader()
    {
        shader.bind();
    }
    void bind_VAO()
    {
        mesh.Bind_data();
    }
    void bind_texture()
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_3D,texture_ID);
     
    }
    void uniform_setup()
    {
        UBO::Bind_Uniform_Block_To_Binding_Point(shader, "Matrices", 0);
        shader.bind();
        glUniform1i(glGetUniformLocation(shader.ID, "Texture"), 1);
    }

   
    void bind_and_setting()
    {
        bind_shader();
        set_uniform();
        bind_texture();
    }
    void DrawSimple()
    {
        mesh.Bind_data();
        set_model_matrix();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    void Draw()
    {
        glDisable(GL_CULL_FACE);
        bind_and_setting();
        DrawSimple();
        glEnable(GL_CULL_FACE);
    }
    void set_uniform()
    {
        glUniform1f(glGetUniformLocation(shader.ID, "get_current_state"),get_current_state());
        
    }
    void set_model_matrix()
    {
        UBO_MVP::Write_Data_Model_matrix(get_model_matrix());
    }
    glm::mat4 get_model_matrix()
    {
        return
            glm::rotate((glm::scale(glm::translate(glm::mat4(1.0f), position), sizes)),
                glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void update(float delta)
    {
        this->time += delta;
        time = std::fmod(time,10.0f);
    }
    float get_current_state()
    {
        return cos((time*2.0f*3.1415f)/10.0f)*0.5f+0.5f;
    }

    void delet_VAO()
    {
        mesh.delet_VAO();
    }
private:
    
  
    
    void setup_VAO()
    {
        mesh.Bind_data();


        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float)
            , &this->vertices[0], GL_STATIC_DRAW);

#pragma region attribute_setting

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0,2 , GL_FLOAT, GL_FALSE, 4 * sizeof(float),
            (void*)0);


        glEnableVertexAttribArray(1);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
            (void*)(2 * sizeof(float)));

       

    

#pragma endregion



    }
};