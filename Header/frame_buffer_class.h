#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <bitset>
#include "Texture_Class.h"

class Frame_Buffer
{
private:
	unsigned int fbo;


public:

	Frame_Buffer()
	{
		glGenFramebuffers(1, &fbo);
		bind();
	}
	
	void is_complete()
	{
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "frame buffer is not complete" ;
		}
	}
	void bind()//bind shadow texure bind frame buffer
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	}
	static void bind_window_0()//bind shadow texure bind frame buffer
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}
};
class Frame_Buffer_Depth : public Frame_Buffer
{



public:

	Frame_Buffer_Depth(unsigned int texture_id):
		Frame_Buffer()
	{
		set_depth_texture(texture_id);
		Set_Attributes_Depth();

	}

	void set_depth_texture(unsigned int texture_id)
	{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);
	}
	
	void Set_Attributes_Depth()
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	

	
};
class Frame_Buffer_color_Depth : public Frame_Buffer
{



public:

	Frame_Buffer_color_Depth(unsigned int texture_color_id, unsigned int texture_depth_id) :
		Frame_Buffer()
	{
		set_color_texture(texture_color_id);
		set_depth_texture(texture_depth_id);
	}

	void set_depth_texture(unsigned int texture_id)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture_id, 0);
	}
	void set_color_texture(unsigned int texture_id)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
	}

	



};
