#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>




class VAO
{
private :
      unsigned int  VAO_ID;
	  unsigned int  VBO_ID;
	  unsigned int  EBO_ID;
public:
	

	VAO(bool is_EBO)
	{
	

		glGenVertexArrays(1, &VAO_ID);
		glGenBuffers(1, &VBO_ID);
		glBindVertexArray(VAO_ID);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
		EBO_ID = 0;
		if (is_EBO) {
		
		glGenBuffers(1, &EBO_ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
		}
	}

	
	unsigned int get_VAO()
	{
		return this->VAO_ID;
	}
	
	void Bind_data()//bind the VAO
	{	
		glBindVertexArray(VAO_ID);
	
	}
	void delet_VAO()//do not use ~destructor so i wont delet IDs in game constructor
	{
		glDeleteBuffers(1, &VAO_ID);
		glDeleteBuffers(1, &VBO_ID);
		glDeleteBuffers(1, &EBO_ID);

	}
	
};

