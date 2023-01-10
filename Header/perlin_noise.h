#pragma once
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader_Class.h"
#include "Uniform_Buffer_Class.h"
#include "VAOClass.h"
#include "frame_buffer_class.h"

class perlin_noise
{
private:
 
 

    unsigned int random_texture_id;
    unsigned int random_texture_size;
    
    unsigned int frame_buffer_id;
    unsigned int color_texture_id;
    unsigned int depth_texture_id;

	unsigned int perlin_size;
    
    Shader shader;
    VAO mesh;//that one point
public:
    perlin_noise():
      shader("shaders/vertex_shader_perlin", "shaders/fragment_shader_perlin"), mesh(false),  perlin_size{ 200 }
        , random_texture_size{6}
          
	{
     
        
        glEnable(GL_TEXTURE_3D);

        //random texture
        glGenTextures(1, &random_texture_id);
        glBindTexture(GL_TEXTURE_3D, random_texture_id);
        
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  
        generate_random_texture();
        

        
        //color_texture
        glGenTextures(1, &color_texture_id);
        glBindTexture(GL_TEXTURE_3D, color_texture_id);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, perlin_size, perlin_size, perlin_size, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        
    
        
                                   
  

        //frame_buffer
        glGenFramebuffers(1, &frame_buffer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
        glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, color_texture_id, 0, 0);
    
        
	
        VAO_setup();
        
        uniform_setup();

        render_perlin_noise();
      
        
	}
    unsigned int get_texture_ID()
    {
        return color_texture_id;
    }
    void generate_random_texture()
    {
        glBindTexture(GL_TEXTURE_3D, random_texture_id);
        unsigned int const size = 6;
        float random_arr[size][size][size * 3];

        
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++) {
                
                for (int k = 0; k < size * 3; k += 3)
                {
                    for (int s = 0; s < 2; s++){rand();}//to change random pattern
                    
                        random_arr[i][j][k + 0] = (float)rand() / RAND_MAX;
                        random_arr[i][j][k + 1] = (float)rand() / RAND_MAX;
                        random_arr[i][j][k + 2] = (float)rand() / RAND_MAX;
                    
 
                }
  
            }

        }
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, size, size, size, 0, GL_RGB, GL_FLOAT, random_arr);
        glGenerateMipmap(GL_TEXTURE_3D);



    }
    
    void render_perlin_noise()
    {
        glDisable(GL_DEPTH_TEST);

        glDisable(GL_CULL_FACE);

       glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);

        shader.bind();
        glViewport(0, 0, perlin_size, perlin_size);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        bind_VAO();
        bind_texture();
        for (unsigned int z = 0; z < perlin_size; z++)
        {
            Draw_slice( z);
    
        }
        
        glBindTexture(GL_TEXTURE_3D, color_texture_id);
        glGenerateMipmap(GL_TEXTURE_3D);

        
     
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        Frame_Buffer::bind_window_0();
    }
    
    void reshuffle_perlin_noise()
    {
        generate_random_texture();
        render_perlin_noise();
    }
    void Draw_slice(unsigned z)
    {
        set_uniform(z);
        glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, color_texture_id, 0, z);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   }
    void set_uniform(unsigned int z)
    {
        glUniform1f(glGetUniformLocation(shader.ID, "z_slice"), z);
    }
    void bind_texture()
    {
           glActiveTexture(GL_TEXTURE1);
           glBindTexture(GL_TEXTURE_3D, this->random_texture_id);
    }
    unsigned int get_perlin_texture_id()
    {   
        return color_texture_id;
    }
    unsigned int get_random_texture_id()
    {
        return random_texture_id;
    }
    void uniform_setup()
    {
        shader.bind();
        glUniform1f(glGetUniformLocation(shader.ID, "a_perlin_size"),(float) this->perlin_size);
        glUniform1f(glGetUniformLocation(shader.ID, "random_size"), (float)this->random_texture_size);
        glUniform1i(glGetUniformLocation(shader.ID, "Texture"), 1);


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
    
    
  

 
  
    
    void VAO_setup()
    {
        std::vector<float> vertices={ -1.0f, -1.0f,    0.0f, 0.0f,
                                      -1.0f,  1.0f,    0.0f,  1.0f,
                                       1.0f, -1.0f,     1.0f, 0.0f,
                                       1.0f,  1.0f,     1.0f,  1.0f, };

       mesh.Bind_data();


       glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float)
           , &vertices[0], GL_STATIC_DRAW);

#pragma region attribute_setting

       glEnableVertexAttribArray(0);

       glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
           (void*)0);


       glEnableVertexAttribArray(1);

       glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
           (void*)(2 * sizeof(float)));


       






    }

   

    void update(float delta_time)
    {
    }
    void PostProcessing()
    {
    }

	
};