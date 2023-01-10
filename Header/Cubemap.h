#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader_Class.h"
#include "VAOClass.h"
class Cube_map_texture
{
private:
	unsigned int texture_ID;

public:
	Cube_map_texture()
	{
		glGenTextures(1, &texture_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);
	}
	void bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture_ID);
	}
	unsigned int get_id()
	{
		return texture_ID;
	}
};

class Cubemap
{
private:
	VAO mesh;
	Cube_map_texture texture;
	Shader shader;
public:
	Cubemap() :
		texture(),mesh(false),
		shader("shaders/vertex_shader_skybox","shaders/fragment_shader_skybox")
	{
	VAO_setup();
	uniform_setup();
#pragma region texture_setup	
	stbi_set_flip_vertically_on_load(false);

		int width, height, nrChannels;
		unsigned char* data;
			data = stbi_load("texture/skybox/front.jpg", &width, &height, &nrChannels, 0);
			
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
stbi_image_free(data);

			data = stbi_load("texture/skybox/bottom.jpg", &width, &height, &nrChannels, 0);
glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
stbi_image_free(data);

			data = stbi_load("texture/skybox/back.jpg", &width, &height, &nrChannels, 0);
glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
stbi_image_free(data);

			data = stbi_load("texture/skybox/left.jpg", &width, &height, &nrChannels, 0);
glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
stbi_image_free(data);

			data = stbi_load("texture/skybox/right.jpg", &width, &height, &nrChannels, 0);
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
stbi_image_free(data);

data = stbi_load("texture/skybox/top.jpg", &width, &height, &nrChannels, 0);
glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
stbi_image_free(data);

glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#pragma endregion
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

	void set_uniform_model()//delet unnacacery
	{
		UBO_MVP::Write_Data_Model_matrix(get_model_matrix());
	}

	void bind_texture()
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture.get_id());
	}
	void uniform_setup()
	{
		UBO::Bind_Uniform_Block_To_Binding_Point(shader, "Matrices", 0);
		shader.bind();
glUniform1i(glGetUniformLocation(shader.ID, "skybox"), 1);
	}

	glm::mat4 get_model_matrix()
	{
		return  glm::diagonal4x4(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
	}
	unsigned int get_texture_id()
	{
		return texture.get_id();
	}

	
	void Draw()//, glm::mat4 lightSpaceMatrix)
	{
		glDepthFunc(GL_LEQUAL);
		bind_and_setting();
		DrawSimple();
		glDepthFunc(GL_LESS);
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


	
	void VAO_setup()
	{
		std::vector<float> vertices = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
		mesh.Bind_data();


		glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float)
			, &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
			(void*)0);
	}
};
