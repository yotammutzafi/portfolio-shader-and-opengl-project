#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_operation.hpp>
#include "glm/gtc/type_ptr.hpp"



#include "frame_buffer_class.h"
#include "Texture_Class.h"

#include "Camera.h"
#include "light_struct.h"
#include "shader_Class.h"
class shadow_map
{
private:
	unsigned int SHADOW_WIDTH, SHADOW_HEIGHT;
	texture_depth depth_texture;
	texture_color color_texture;
	Frame_Buffer_color_Depth frame_buffer;

	Shader Shadow_Shader;
public:
	
	

	shadow_map() : 
		color_texture(texture_color::TYP_1 | texture_color::TYP_RGB),
		depth_texture(texture_depth::TYP_0),

		frame_buffer(color_texture.get_id(), depth_texture.get_id())
		
		, Shadow_Shader
	{  "shaders/vertex_shader_depth", "shaders/fragment_shader_depth" }

	{
		
		SHADOW_WIDTH = 1024;
		SHADOW_HEIGHT = 1024;
		setup();
	}
	
	void bind_shadow_shader()
	{
		Shadow_Shader.bind();
	}
	unsigned int Get_Shadow_Shader_ID()
	{
		return Shadow_Shader.ID;
	}
	void setup()
	{
		uniform_setup();
	}

	void bind_frame_buffer()
	{
		frame_buffer.bind();
	}

	
	unsigned int get_texture_id()
	{
		return depth_texture.get_id();
	}
	texture_depth* get_texture_depth()
	{
		return &depth_texture;
	}
	texture_color* get_texture_color()
	{
		return &color_texture;
	}
	void uniform_setup()
	{
		UBO::Bind_Uniform_Block_To_Binding_Point(Shadow_Shader, "Matrices", 0);
	}
	void is_frame_buffer_complete()
	{
		frame_buffer.is_complete();
	}
private:

	


};
