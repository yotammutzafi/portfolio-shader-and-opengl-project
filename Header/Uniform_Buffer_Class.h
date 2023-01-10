#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_operation.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <bitset>
#include "shader_Class.h"
#include "light_struct.h"








class UBO
{
private:
	unsigned int  UBO_ID;
	
public:
	 


	UBO()
	{
			glGenBuffers(1, &UBO_ID);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID);
	}
	void Bind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID);
	}
	void Unbind()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	unsigned int get_UBO_ID()
	{
		return UBO_ID;
	}
	static	void Bind_Uniform_Block_To_Binding_Point(const Shader& shader, const char uniform_block_Name[], unsigned int Binding_Point)
	{
		unsigned int uniform_block_ID = glGetUniformBlockIndex(shader.ID, uniform_block_Name);
		glUniformBlockBinding(shader.ID, uniform_block_ID, Binding_Point);
	}
	

};

class UBO_MVP:public UBO
{

public:

	static const unsigned int size{ sizeof(glm::mat4) * 3 };

	UBO_MVP(const unsigned int Binding_Point):
		UBO()
	{
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		Bind_UBO_To_Binding_Point(Binding_Point);
	}
	void Bind_UBO_To_Binding_Point(const unsigned int Binding_Point)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, Binding_Point, this->get_UBO_ID(), 0, size);
	}
	
	static void Write_Data_Model_matrix(glm::mat4 Model_Matrix)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Model_Matrix));

	}
	static void Write_Data_View_matrix(glm::mat4 view_matrix)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view_matrix));

	}
	static void Write_Data_Projection_matrix(glm::mat4 Projection_matrix)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Projection_matrix));
	}
};

class UBO_LIGHT_DATA :public UBO//currently only design for 3 matrices(MVP)
{

public:

	static const unsigned int size{ sizeof(glm::vec4) * 5 };

	UBO_LIGHT_DATA(const unsigned int Binding_Point) :
		UBO()
	{
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		Bind_UBO_To_Binding_Point(Binding_Point);
	}
	
	
	void Bind_UBO_To_Binding_Point(const unsigned int Binding_Point)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, Binding_Point, this->get_UBO_ID(), 0, size);
	}

	static void Write_Light_Data(light_data light)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(light.position));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.direction));
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.ambient));
		glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.diffuse));
		glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.specular));

	}
};

class UBO_LIGHT_SHADOW : public UBO
{



public:

	static const unsigned int size{ sizeof(glm::vec4) * 5 + sizeof(glm::mat4) + sizeof(glm::mat4) };

	UBO_LIGHT_SHADOW(const unsigned int Binding_Point) :
		UBO()
	{
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
		Bind_UBO_To_Binding_Point(Binding_Point);
	}


	void Bind_UBO_To_Binding_Point(const unsigned int Binding_Point)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, Binding_Point, this->get_UBO_ID(), 0, size);
	}

	static void Write_Light_Data(light_data light)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(light.position));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.direction));
		glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.ambient));
		glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.diffuse));
		glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(light.specular));

	}
	static void Write_LightView(glm::mat4 lightView)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4), sizeof(glm::mat4), glm::value_ptr(lightView));
	}
	

	void bind_texture_unit_to_texture(unsigned int texture_id)
	{
	
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, texture_id);

	}


};