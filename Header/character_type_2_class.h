#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <math.h>
#include "shader_Class.h"
#include "Texture_Class.h"
#include "AnimateModelClass.h"
#include "Uniform_Buffer_Class.h"
class character_type_2_class
{
public:


private:
	glm::vec3 position;
	glm::vec3 sizes;
	glm::vec3 rotation;
	texture_color texture_1;
	AnimateModel model_Data;
	Shader shader_right_blue;
	Shader shader_left_red;
public:
	character_type_2_class(std::string path, int max_num_bones_per_vertex, glm::vec3 position,
		glm::vec3 sizes) :
		texture_1{ texture_color::TYP_0 | texture_color::TYP_RGBA | texture_color::flip,"model/New folder/Dave/Character Texture.psd" },
		model_Data(path, max_num_bones_per_vertex),
		shader_right_blue("shaders/vertex_shader_model_right_eye", "shaders/fragment_shader_model"),
		shader_left_red("shaders/vertex_shader_model_left_eye", "shaders/fragment_shader_model"),
		position(position), sizes(sizes)
	{
		uniform_setup();
	}


	unsigned int get_shader_ID()
	{
		return shader_right_blue.ID;
	}

	void set_uniform_model()
	{
		UBO_MVP::Write_Data_Model_matrix(get_model_matrix());
	}

	void bind_texture()
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->texture_1.get_id());
	}
	void uniform_setup()
	{

		UBO::Bind_Uniform_Block_To_Binding_Point(shader_right_blue, "Matrices", 0);
		UBO::Bind_Uniform_Block_To_Binding_Point(shader_right_blue, "Light_Data_8", 8);
		UBO::Bind_Uniform_Block_To_Binding_Point(shader_right_blue, "Light_Data_9", 9);
		UBO::Bind_Uniform_Block_To_Binding_Point(shader_left_red, "Matrices", 0);
		UBO::Bind_Uniform_Block_To_Binding_Point(shader_left_red, "Light_Data_8", 8);
		UBO::Bind_Uniform_Block_To_Binding_Point(shader_left_red, "Light_Data_9", 9);
		shader_right_blue.bind();
	glUniform1f(glGetUniformLocation(shader_right_blue.ID, "material.shininess"), 10.0f);
	glUniform1i(glGetUniformLocation(shader_right_blue.ID, "material.materialTexture"), 1);
	glUniform1f(glGetUniformLocation(shader_right_blue.ID, "eye_distance"), 0.003f);

		glUniform1i(glGetUniformLocation(shader_right_blue.ID, "shadow_map_8"), 8);
		glUniform1i(glGetUniformLocation(shader_right_blue.ID, "shadow_map_9"), 9);

		shader_left_red.bind();
		glUniform1f(glGetUniformLocation(shader_left_red.ID, "material.shininess"), 10.0f);
		glUniform1i(glGetUniformLocation(shader_left_red.ID, "material.materialTexture"), 1);
		glUniform1f(glGetUniformLocation(shader_left_red.ID, "eye_distance"), 0.003f);

		glUniform1i(glGetUniformLocation(shader_left_red.ID, "shadow_map_8"), 8);
		glUniform1i(glGetUniformLocation(shader_left_red.ID, "shadow_map_9"), 9);

	}

	unsigned int number_of_indices()
	{
		return model_Data.number_of_indices;
	}
	glm::mat4 get_model_matrix()
	{

		return  glm::rotate((glm::scale(glm::translate(glm::mat4(1.0f), position), sizes)), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	}
	void bind()
	{

		bind_VAO();
		shader_right_blue.bind();
	}
	unsigned int get_texture_id()
	{
		return texture_1.get_id();
	}

	void bind_VAO()
	{

		model_Data.bind_VAO();
	}
	void bind_shader()
	{
		shader_right_blue.bind();
	}
	unsigned int get_shader_id()
	{
		return shader_right_blue.ID;
	}

	void Draw()
	{
		bind_texture();
		set_uniform_model();
		bind_VAO();
		
		glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
		shader_right_blue.bind();
		glDrawElements(GL_TRIANGLES, model_Data.number_of_indices, GL_UNSIGNED_INT, 0);


		glClear(GL_DEPTH_BUFFER_BIT);

		
		glColorMask(true,  false, false, true);
		shader_left_red.bind();
		glDrawElements(GL_TRIANGLES, model_Data.number_of_indices, GL_UNSIGNED_INT, 0);
		
		//to bring back the red depth. can be reblace with a simplar shader
		glColorMask(false, false, false, true);
		shader_right_blue.bind();
		glDrawElements(GL_TRIANGLES, model_Data.number_of_indices, GL_UNSIGNED_INT, 0);



		glColorMask(true, true, true, true);
	}
	
	
	void DrawShadow()
	{
		set_uniform_model();
		bind_VAO();
		glDrawElements(GL_TRIANGLES, model_Data.number_of_indices, GL_UNSIGNED_INT, 0);
	}


private:
};

