#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader_Class.h"
#include "VAOClass.h"
#include "Uniform_Buffer_Class.h"
class Voxel_3D_Graph
{
private:
	Shader shader;
	VAO mesh;
	glm::vec3 position;
	glm::vec3 sizes;
	unsigned int texture_3d_id;
	unsigned int resolution;
	float time;

public :
	Voxel_3D_Graph(unsigned int texture_3d_id,unsigned int resolution, glm::vec3 position,
	glm::vec3 sizes) :
		texture_3d_id{ texture_3d_id }, position{ position }, sizes{ sizes }
		, resolution{ resolution }, time{0.0f},
		shader("shaders/vertex_shader_voxel", "shaders/fragment_shader_voxel"), mesh{true}
	{
	    uniform_setup();
		VAO_setup();
	}
	void set_uniform_model()
	{
		shader.bind();
		glUniform1f(glGetUniformLocation(shader.ID, "resolution"), resolution);
		glUniform1f(glGetUniformLocation(shader.ID, "cut"),get_current_state());
		UBO_MVP::Write_Data_Model_matrix(get_model_matrix());
	}
	void Draw()//, glm::mat4 lightSpaceMatrix)
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
	
		glDrawArraysInstanced(GL_TRIANGLES, 0, 108, resolution* resolution* resolution);
		 
	}
	
	void bind_texture()
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_3D, this->texture_3d_id);
	}
	void uniform_setup()
	{
		UBO::Bind_Uniform_Block_To_Binding_Point(shader, "Matrices", 0);
		shader.bind();
		glUniform1f(glGetUniformLocation(shader.ID, "Texture"), texture_3d_id);
	}
	glm::mat4 get_model_matrix()
	{
		return  
			glm::rotate((glm::scale(glm::translate(glm::mat4(1.0f), position), sizes)),
				glm::radians(-90.0f),glm::vec3(0.0f,1.0f,0.0f));
	}
	void update(float delta)
	{
		this->time += delta;
		time = std::fmod(time, 10.0f);
	}
	float get_current_state()
	{
		return cos((time * 2.0f * 3.1415f) / 10.0f) * 0.5f+0.5f ;
	}
	void set_texture_3d_id(unsigned int texture_3d_id)
	{
		this->texture_3d_id = texture_3d_id;
	}
	void set_size(unsigned int size)
	{
		this->resolution = size;
	}
	void bind_shader()
	{
		shader.bind();
	}
	void bind_VAO()
	{
		mesh.Bind_data();
	}
	void VAO_setup()
	{
		mesh.Bind_data();
		
		std::vector<float> vertices = 
		{
			// back face
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,0.0f,-0.4f,// bottom-left
		  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,0.0f,-0.4f,// top-right
		  0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,0.0f,-0.4f,// bottom-right         
		  0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,0.0f,-0.4f,// top-right
		 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,0.0f,-0.4f,// bottom-left
		 -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,	0.0f,0.0f,-0.4f,// top-left
		 //square base pyramid
		 0.0f,0.0f,0.0f,		0.0f,1.0f,1.0f,			0.0f,0.0f,-0.4f,
		 0.5f,0.5f,-0.5f,		0.0f,1.0f,1.0f,			0.0f,0.0f,-0.4f,
		 -0.5f,0.5f,-0.5f,		0.0f,1.0f,1.0f, 		0.0f,0.0f,-0.4f,

		0.0f,0.0f,0.0f,			0.0f,-1.0f,1.0f,		0.0f,0.0f,-0.4f,
		-0.5f,-0.5f,-0.5f,		0.0f,-1.0f,1.0f,		0.0f,0.0f,-0.4f,
		0.5f,-0.5f,-0.5f,		0.0f,-1.0f,1.0f,		0.0f,0.0f,-0.4f,

		0.0f,0.0f,0.0f,			1.0f,0.0f,1.0f,		0.0f,0.0f,-0.4f,
		0.5f,-0.5f,-0.5f,		1.0f,0.0f,1.0f,		0.0f,0.0f,-0.4f,
		0.5f,0.5f,-0.5f,		1.0f,0.0f,1.0f,		0.0f,0.0f,-0.4f,

		0.0f,0.0f,0.0f,			-1.0f,0.0f,1.0f,		0.0f,0.0f,-0.4f,
		-0.5f,0.5f,-0.5f,		-1.0f,0.0f,1.0f,		0.0f,0.0f,-0.4f,
		-0.5f,-0.5f,-0.5f,		-1.0f,0.0f,1.0f,		0.0f,0.0f,-0.4f,


		 // front face
 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,		0.0f,0.0f,0.4f,	// bottom-left
  0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,		0.0f,0.0f,0.4f,	// bottom-right
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,		0.0f,0.0f,0.4f,	// top-right
  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,		0.0f,0.0f,0.4f,	// top-right
 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,		0.0f,0.0f,0.4f, // top-left
 -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,		0.0f,0.0f,0.4f,	// bottom-left
 

  //square base pyramid
		 0.0f,0.0f,0.0f,		0.0f,1.0f,-1.0f,			0.0f,0.0f,0.4f,
		 -0.5f,0.5f,0.5f,		0.0f,1.0f,-1.0f,			0.0f,0.0f,0.4f,
		 0.5f,0.5f,0.5f,		0.0f,1.0f,-1.0f, 		0.0f,0.0f,0.4f,

		0.0f,0.0f,0.0f,			0.0f,1.0f,1.0f,		0.0f,0.0f,0.4f,
		0.5f,-0.5f,0.5f,		0.0f,1.0f,1.0f,		0.0f,0.0f,0.4f,
		-0.5f,-0.5f,0.5f,		0.0f,1.0f,1.0f,		0.0f,0.0f,0.4f,
		//
		0.0f,0.0f,0.0f,			1.0f,0.0f,-1.0f,		0.0f,0.0f,0.4f,
		0.5f,0.5f,0.5f,		    1.0f,0.0f,-1.0f,		0.0f,0.0f,0.4f,
		0.5f,-0.5f,0.5f,		1.0f,0.0f,-1.0f,		0.0f,0.0f,0.4f,

		0.0f,0.0f,0.0f,			-1.0f,0.0f,-1.0f,		0.0f,0.0f,0.4f,
		-0.5f,-0.5f,0.5f,		-1.0f,0.0f,-1.0f,		0.0f,0.0f,0.4f,
		-0.5f,0.5f,0.5f,		-1.0f,0.0f,-1.0f,		0.0f,0.0f,0.4f,



 // left face									
 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,		-0.4f,0.0f,0.0f, // top-right
 -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,		-0.4f,0.0f,0.0f, // top-left
 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,		-0.4f,0.0f,0.0f, // bottom-left
 -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,		-0.4f,0.0f,0.0f, // bottom-left
 -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,		-0.4f,0.0f,0.0f, // bottom-right
 -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,		-0.4f,0.0f,0.0f, // top-right


 
  //square base pyramid
		 0.0f,0.0f,0.0f,		1.0f,1.0f,0.0f,			-0.4f,0.0f,0.0f,
		 -0.5f,0.5f,-0.5f,		1.0f,1.0f,0.0f,			-0.4f,0.0f,0.0f,
		 -0.5f,0.5f,0.5f,		1.0f,1.0f,0.0f, 		-0.4f,0.0f,0.0f,

		0.0f,0.0f,0.0f,			1.0f,-1.0f,0.0f,		-0.4f,0.0f,0.0f,
		-0.5f,-0.5f,0.5f,		1.0f,-1.0f,0.0f,		-0.4f,0.0f,0.0f,
		-0.5f,-0.5f,-0.5f,		1.0f,-1.0f,0.0f,		-0.4f,0.0f,0.0f,
		//
		0.0f,0.0f,0.0f,			1.0f,0.0f,1.0f,		-0.4f,0.0f,0.0f,
		-0.5f,0.5f,0.5f,        1.0f,0.0f,1.0f,		-0.4f,0.0f,0.0f,
		-0.5f,-0.5f,0.5f,		1.0f,0.0f,1.0f,		-0.4f,0.0f,0.0f,

		0.0f,0.0f,0.0f,			1.0f,0.0f,-1.0f,		-0.4f,0.0f,0.0f,
		-0.5f,-0.5f,-0.5f,		1.0f,0.0f,-1.0f,		-0.4f,0.0f,0.0f,
		-0.5f,0.5f,-0.5f,		1.0f,0.0f,-1.0f,		-0.4f,0.0f,0.0f,


		
 // right face									
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,		0.4f,0.0f,0.0f,	// top-left
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,		0.4f,0.0f,0.0f,	// bottom-right
  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,		0.4f,0.0f,0.0f,	 // top-right         
  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,		0.4f,0.0f,0.0f,	 // bottom-right
  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,		0.4f,0.0f,0.0f,	 // top-left
  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,		0.4f,0.0f,0.0f,	 // bottom-left   

	 
  //square base pyramid
		 0.0f,0.0f,0.0f,		-1.0f,1.0f,0.0f,			0.4f,0.0f,0.0f,
		 0.5f,0.5f,0.5f,		-1.0f,1.0f,0.0f,			0.4f,0.0f,0.0f,
		 0.5f,0.5f,-0.5f,		-1.0f,1.0f,0.0f, 		0.4f,0.0f,0.0f,

		0.0f,0.0f,0.0f,			-1.0f,-1.0f,0.0f,		0.4f,0.0f,0.0f,
		0.5f,-0.5f,-0.5f,		-1.0f,-1.0f,0.0f,		0.4f,0.0f,0.0f,
		0.5f,-0.5f,0.5f,		-1.0f,-1.0f,0.0f,		0.4f,0.0f,0.0f,
		//
		0.0f,0.0f,0.0f,			-1.0f,0.0f,-1.0f,		0.4f,0.0f,0.0f,
		0.5f,0.5f,-0.5f,        -1.0f,0.0f,-1.0f,		0.4f,0.0f,0.0f,
		0.5f,-0.5f,-0.5f,		-1.0f,0.0f,-1.0f,		0.4f,0.0f,0.0f,

		0.0f,0.0f,0.0f,			-1.0f,0.0f,1.0f,		0.4f,0.0f,0.0f,
		0.5f,-0.5f,0.5f,		-1.0f,0.0f,1.0f,		0.4f,0.0f,0.0f,
		0.5f,0.5f,0.5f,		-1.0f,0.0f,1.0f,		0.4f,0.0f,0.0f,


 // bottom face									
 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,		0.0f,-0.4f,0.0f,	 // top-right
  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,		0.0f,-0.4f,0.0f,	 // top-left
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,		0.0f,-0.4f,0.0f,	 // bottom-left
  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,		0.0f,-0.4f,0.0f,	 // bottom-left
 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,		0.0f,-0.4f,0.0f,	 // bottom-right
 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,		0.0f,-0.4f,0.0f,	 // top-right



	 //square base pyramid
	 0.0f,0.0f,0.0f,		0.0f,1.0f,1.0f,			0.0f,-0.4f,0.0f,
	 -0.5f,-0.5f,0.5f,		0.0f,1.0f,1.0f,			0.0f,-0.4f,0.0f,
	 0.5f,-0.5f,0.5f,		0.0f,1.0f,1.0f, 		0.0f,-0.4f,0.0f,
														
	0.0f,0.0f,0.0f,			0.0f,1.0f,-1.0f,			0.0f,-0.4f,0.0f,
	0.5f,-0.5f,-0.5f,		0.0f,1.0f,-1.0f,			0.0f,-0.4f,0.0f,
	-0.5f,-0.5f,-0.5f,		0.0f,1.0f,-1.0f,			0.0f,-0.4f,0.0f,
	//													
	0.0f,0.0f,0.0f,			1.0f,1.0f,0.0f,			0.0f,-0.4f,0.0f,
	0.5f,-0.5f,0.5f,        1.0f,1.0f,0.0f,			0.0f,-0.4f,0.0f,
	0.5f,-0.5f,-0.5f,		1.0f,1.0f,0.0f,			0.0f,-0.4f,0.0f,
														
	0.0f,0.0f,0.0f,			-1.0f,1.0f,0.0f,			0.0f,-0.4f,0.0f,
	-0.5f,-0.5f,-0.5f,		-1.0f,1.0f,0.0f,			0.0f,-0.4f,0.0f,
	-0.5f,-0.5f,0.5f,		-1.0f,1.0f,0.0f,			0.0f,-0.4f,0.0f,


 // top face									
 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,		0.0f,0.4f,0.0f,	 // top-left
  0.5f,  0.5f , 0.5f,  0.0f,  1.0f,  0.0f,		0.0f,0.4f,0.0f,	 // bottom-right
  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,		0.0f,0.4f,0.0f,	 // top-right     
  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,		0.0f,0.4f,0.0f,	 // bottom-right
 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,		0.0f,0.4f,0.0f,	 // top-left
 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,		0.0f,0.4f,0.0f, // bottom-left    



	  //square base pyramid
 0.0f,0.0f,0.0f,		0.0f,-1.0f,1.0f,			0.0f,0.4f,0.0f,
 0.5f,0.5f,0.5f,		0.0f,-1.0f,1.0f,			0.0f,0.4f,0.0f,
 -0.5f,0.5f,0.5f,		0.0f,-1.0f,1.0f, 		0.0f,0.4f,0.0f,
													
0.0f,0.0f,0.0f,			0.0f,-1.0f,-1.0f,		0.0f,0.4f,0.0f,
-0.5f,0.5f,-0.5f,		0.0f,-1.0f,-1.0f,		0.0f,0.4f,0.0f,
0.5f,0.5f,-0.5f,		0.0f,-1.0f,-1.0f,		0.0f,0.4f,0.0f,
//													
0.0f,0.0f,0.0f,			1.0f,-1.0f,0.0f,			0.0f,0.4f,0.0f,
0.5f,0.5f,-0.5f,        1.0f,-1.0f,0.0f,			0.0f,0.4f,0.0f,
0.5f,0.5f,0.5f,		    1.0f,-1.0f,0.0f,			0.0f,0.4f,0.0f,
													
0.0f,0.0f,0.0f,			-1.0f,-1.0f,0.0f,		0.0f,0.4f,0.0f,
-0.5f,0.5f,0.5f,		-1.0f,-1.0f,0.0f,		0.0f,0.4f,0.0f,
-0.5f,0.5f,-0.5f,		-1.0f,-1.0f,0.0f,		0.0f,0.4f,0.0f

		};

glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		
glEnableVertexAttribArray(0);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
	(void*)0);


glEnableVertexAttribArray(1);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
	(void*)(3 * sizeof(float)));

glEnableVertexAttribArray(2);

glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
	(void*)(6 * sizeof(float)));


	}
};