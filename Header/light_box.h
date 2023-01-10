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
#include "Uniform_Buffer_Class.h"
#include "shadow_generator_class.h"
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Uniform_Buffer_Class.h"
class Box_light
{
private:
    light_data light;
    std::vector<float> vertices;
    glm::vec3 position;
    glm::vec3 sizes;
    Shader shader;
    shadow_map shadow;
    VAO mesh;
public:
    Box_light(const glm::vec3& position, const glm::vec3& sizes) :
        shader("shaders/vertex_shader", "shaders/fragment_shader")
        
        ,shadow()
        , mesh(false)
    {
       
        this->position = position;
        this->light.position = position;
        this->light.direction = -1.0f * position;
        this->light.ambient  = glm::vec3(0.3f, 0.3f, 0.3f);
        this->light.diffuse  = glm::vec3(0.6f, 0.6f, 0.6f);
        this->light.specular = glm::vec3(0.8f, 0.8f, 0.8f);
        this->sizes = sizes;
        VAO_setup();
        uniform_setup();
    }
    
   
    unsigned int get_shadow_texture_id()
    {
        return shadow.get_texture_id();
    }
    void Draw()
    {
        bind_and_setting();
        DrawSimple();
    }
    void bind_and_setting()//bind shader  bind mesh 
    {
        bind_shader();
    }
    void bind_shader()
    {
        shader.bind();
    }
    unsigned int get_shader_ID()
    {
        return shader.ID;
    }
    void bind_shadow_shader()
    {
        shadow.bind_shadow_shader();
    }
    void DrawSimple()//you need to bind the UBO first
    {
        set_model_matrix(); 
            bind_VAO();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    void uniform_setup()
    {   
        UBO::Bind_Uniform_Block_To_Binding_Point(shader,"Matrices",0);
    }
    void bind_VAO()
    {
        mesh.Bind_data();
    }
    void set_light_data()
    {
        UBO_LIGHT_SHADOW::Write_Light_Data(this->light);
    }
    void bind_shadow_frame_buffer()//you need to bind shadow shader too
    {
        this->shadow.bind_frame_buffer();
    }
    void set_model_matrix()
    {
        UBO_MVP::Write_Data_Model_matrix(get_model_matrix());
    }
    glm::mat4 get_view_matrix()
    {
       
        return glm::lookAt(this->light.position,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

      
    }
    glm::mat4 get_projection_matrix()
    {
        return glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
       // return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 100.0f);
    }
    glm::mat4 get_lightView_matrix()
    {
        
        return  get_projection_matrix() * get_view_matrix();
    }
    void set_view_matrix()
    {  
         UBO_MVP::Write_Data_View_matrix(get_view_matrix());
    }
    void set_projection_matrix()
    {  
        UBO_MVP::Write_Data_Projection_matrix(get_projection_matrix());
    }

    void set_lightView_matrix()
    {
        UBO_LIGHT_SHADOW::Write_LightView(get_lightView_matrix());
        
        
    }
    glm::mat4 get_model_matrix()
    {
        return  (glm::scale(glm::translate(glm::mat4(1.0f), position), sizes));
    }
    void update(float delta_time)
    {
      //  light.position.x = sin(delta_time/2.0) * 5.0;
       //       position.x = sin(delta_time / 2.0) * 5.0;
    }
    void PostProcessing()
    {

    }
    shadow_map* get_shadow_map()
    {
        return &shadow;
    }

   
private:
    



    void VAO_setup()
    {
        mesh.Bind_data();
        
        std::vector<float> verticesss = {
            // back face
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, // bottom-left
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, // top-right
 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, // bottom-right 
 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, // top-right
-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, // bottom-left
-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, // top-left
// front face
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-left
 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-right
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, // top-right
 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, // top-right
-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, // top-left
-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-left
// left face
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f, // top-right
-0.5f,  0.5f, -0.5f, -1.0f,  0.0f, // top-left
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f, // bottom-left
-0.5f, -0.5f, -0.5f, -1.0f,  0.0f, // bottom-left
-0.5f, -0.5f,  0.5f, -1.0f,  0.0f, // bottom-right
-0.5f,  0.5f,  0.5f, -1.0f,  0.0f, // top-right
// right face
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f, // top-left
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f, // bottom-right
 0.5f,  0.5f, -0.5f,  1.0f,  0.0f, // top-right    
 0.5f, -0.5f, -0.5f,  1.0f,  0.0f, // bottom-right
 0.5f,  0.5f,  0.5f,  1.0f,  0.0f, // top-left
 0.5f, -0.5f,  0.5f,  1.0f,  0.0f, // bottom-left  
// bottom face
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, // top-right
 0.5f, -0.5f, -0.5f,  0.0f, -1.0f, // top-left
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, // bottom-left
 0.5f, -0.5f,  0.5f,  0.0f, -1.0f, // bottom-left
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, // bottom-right
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, // top-right
// top face
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // top-left
 0.5f,  0.5f , 0.5f,  0.0f,  1.0f, // bottom-right
 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // top-right    
 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, // bottom-right
-0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // top-left
-0.5f,  0.5f,  0.5f,  0.0f,  1.0f // bottom-left  

        };
        glBufferData(GL_ARRAY_BUFFER, verticesss.size() * sizeof(float)
            , &(verticesss[0]), GL_STATIC_DRAW);

#pragma region attribute_setting

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void*)0);


        glEnableVertexAttribArray(1);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
            (void*)(3 * sizeof(float)));



#pragma endregion






    }

};