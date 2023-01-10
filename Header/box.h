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
#include "Uniform_Buffer_Class.h"

class Box
{
private:
    std::vector<float> vertices;
    glm::vec3 position;
    glm::vec3 sizes;
    glm::vec3 speed;
    Shader shader;
    texture_color texture_color_0;

    VAO mesh;
public:
    Box(const glm::vec3& position, const glm::vec3& sizes) :
        shader("shaders/vertex_shader_phong", "shaders/fragment_shader_phong"), 
        mesh(false),
        texture_color_0(texture_color::TYP_0 |texture_color::TYP_RGB, "texture/pexels-pixmike-413195.jpg")

    {
        /* this->vertices = {
            // positions           // texture coords       //normals 
              -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,         -1.0f,  0.0f,  0.0f,
               0.5f, -0.5f, -0.5f,  0.0f,  0.0f,         -1.0f,  1.0f,  0.0f,
               0.5f,  0.5f, -0.5f,  0.0f,  0.0f,         -1.0f,  1.0f,  1.0f,
               0.5f,  0.5f, -0.5f,  0.0f,  0.0f,         -1.0f,  1.0f,  1.0f,
              -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,         -1.0f,  0.0f,  1.0f,
              -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,         -1.0f,  0.0f,  0.0f,
                                                        
              -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,          1.0f,  0.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  0.0f,  0.0f,          1.0f,  1.0f,  0.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  0.0f,          1.0f,  1.0f,  1.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  0.0f,          1.0f,  1.0f,  1.0f,
              -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,          1.0f,  0.0f,  1.0f,
              -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,          1.0f,  0.0f,  0.0f,
                                                        
              -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,          0.0f,  1.0f,  0.0f,
              -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,          0.0f,  1.0f,  1.0f,
              -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,          0.0f,  0.0f,  1.0f,
              -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,          0.0f,  0.0f,  1.0f,
              -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,          0.0f,  0.0f,  0.0f,
              -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,          0.0f,  1.0f,  0.0f,
                                                        
               0.5f,  0.5f,  0.5f,  1.0f,  0.0f,          0.0f,  1.0f,  0.0f,
               0.5f,  0.5f, -0.5f,  1.0f,  0.0f,          0.0f,  1.0f,  1.0f,
               0.5f, -0.5f, -0.5f,  1.0f,  0.0f,          0.0f,  0.0f,  1.0f,
               0.5f, -0.5f, -0.5f,  1.0f,  0.0f,          0.0f,  0.0f,  1.0f,
               0.5f, -0.5f,  0.5f,  1.0f,  0.0f,          0.0f,  0.0f,  0.0f,
               0.5f,  0.5f,  0.5f,  1.0f,  0.0f,          0.0f,  1.0f,  0.0f,
                                                        
              -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,          0.0f,  0.0f,  1.0f,
               0.5f, -0.5f, -0.5f,  0.0f, -1.0f,          0.0f,  1.0f,  1.0f,
               0.5f, -0.5f,  0.5f,  0.0f, -1.0f,          0.0f,  1.0f,  0.0f,
               0.5f, -0.5f,  0.5f,  0.0f, -1.0f,          0.0f,  1.0f,  0.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,          0.0f,  0.0f,  0.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,          0.0f,  0.0f,  1.0f,
                                                        
              -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,          0.0f,  0.0f,  1.0f,
               0.5f,  0.5f, -0.5f,  0.0f,  1.0f,          0.0f,  1.0f,  1.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  1.0f,          0.0f,  1.0f,  0.0f,
               0.5f,  0.5f,  0.5f,  0.0f,  1.0f,          0.0f,  1.0f,  0.0f,
              -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,          0.0f,  0.0f,  0.0f,
              -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,          0.0f,  0.0f,  1.0f
        };
     */
        this->vertices =
        {
            // back face
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };

        this->position = position;
        this->sizes = sizes;
        this->speed = glm::vec3(0.0, 0.0, 0.0);
        VAO_setup();
        uniform_setup();
    }
   


    void bind_shader()
    {
        shader.bind();
    }
    unsigned int get_shader_ID()
    {
        return shader.ID;
    }
    void bind_VAO()
    {
        mesh.Bind_data();
    }

    void set_uniform_model()
    {
        UBO_MVP::Write_Data_Model_matrix(get_model_matrix());
    }

    void bind_texture()
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->texture_color_0.get_id());
    }
    void uniform_setup()
    {
        UBO::Bind_Uniform_Block_To_Binding_Point(shader, "Matrices"    , 0);
        UBO::Bind_Uniform_Block_To_Binding_Point(shader, "Light_Data_8", 8);
        UBO::Bind_Uniform_Block_To_Binding_Point(shader, "Light_Data_9", 9);
        shader.bind();
        glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 10.0f);
        glUniform1i(glGetUniformLocation(shader.ID, "material.materialTexture"), 1);
        glUniform1i(glGetUniformLocation(shader.ID, "shadow_map_8"), 8);
        glUniform1i(glGetUniformLocation(shader.ID, "shadow_map_9"), 9);
        glUniform1i(glGetUniformLocation(shader.ID, "shadow_map_10"), 10);
        glUniform1i(glGetUniformLocation(shader.ID, "shadow_map_11"), 11);
        
    }
    
    glm::mat4 get_model_matrix()
    {
        return  (glm::scale(glm::translate(glm::mat4(1.0f), position), sizes));
    }
    unsigned int get_texture_id()
    {
        return texture_color_0.get_id();
    }
   
    void VAO_setup()
    {
        mesh.Bind_data();


        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float)
            , &this->vertices[0], GL_STATIC_DRAW);

#pragma region attribute_setting

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)0);


        glEnableVertexAttribArray(1);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
            (void*)(6 * sizeof(float)));


#pragma endregion



    }

    void Draw()
    {
        bind_and_setting();
        DrawSimple();
    }
    void bind_and_setting()
    {
        bind_shader();
        bind_texture();
    }
    void DrawSimple()
    {
        set_uniform_model();
        bind_VAO();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    void DrawShadow()
    {
        DrawSimple();
    }

    void update(float delta_time)
    {
        this->position.x = sin(delta_time)*15;
     

        
    }
    
   
    void PostProcessing()
    {

    }
   
};

