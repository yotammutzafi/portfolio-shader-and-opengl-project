#pragma once
#include "light_box.h"
#include "box.h"
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_operation.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "Uniform_Buffer_Class.h"
#include "Cubemap.h"
#include "character_type_1_class.h"
#include "character_type_2_class.h"
#include "Voxel_3D_Graph.h"
#include "perlin_noise.h"
class game
{
public:
	float currentFrame;
	float lastFrame;
	float delta_time;
	
	Camera camera;
	UBO_MVP  Matrices_Buffer;
	std::vector<UBO_LIGHT_SHADOW>  Light_Buffer_arr;

	

	std::vector<Box_light> Box_light_arr;
	std::vector<Box> Box_arr;
	perlin_noise pn;
	Voxel_3D_Graph v3g;
	square sq;
	character_type_1_class character;//regular character
	character_type_2_class character_rb;//character red blue
	Cubemap skycube;
	game() :
		delta_time{ 0 }, currentFrame{ 0 }, lastFrame{ 0 }, camera{}, Matrices_Buffer{ 0 },
		character("model/New folder/Dave/Dave.fbx", 6, glm::vec3(-2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))
		, character_rb("model/New folder/Dave/Dave.fbx", 6, glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f))
, skycube(), pn{}, sq{ pn.get_texture_ID() ,glm::vec3(8.0f, 5.0f, 8.0f) ,glm::vec3(4.0f,4.0f, 4.0f) },
		v3g{pn.get_texture_ID(),100,glm::vec3(22.0f, 12.0f, 0.0f) ,glm::vec3(20.0f,20.0f, 20.0f) }
	{
		
#pragma region boxes

		Box_arr.push_back(
			Box(glm::vec3(-3.0f, 3.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		Box_arr.push_back(
			Box(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f)));
		Box_arr.push_back(
			Box(glm::vec3(22.0f, 0.0f, 0.0f), glm::vec3(20.0f, 1.0f, 20.0f)));

	
#pragma endregion

#pragma region light_boxes
		
		Box_light_arr.push_back(
			Box_light(glm::vec3(0.0f, 7.0f, 6.0f), glm::vec3(0.2f, 0.2f, 0.2f)));
	

		
#pragma endregion
		
		Light_Buffer_arr.push_back(UBO_LIGHT_SHADOW(8));
		Light_Buffer_arr.push_back(UBO_LIGHT_SHADOW(9));
		Light_Buffer_arr.push_back(UBO_LIGHT_SHADOW(10));
		Light_Buffer_arr.push_back(UBO_LIGHT_SHADOW(11));


		setup();
	}
	void update_time()
	{
		this->lastFrame = this->currentFrame;
		this->currentFrame = static_cast<float>(glfwGetTime());
		this->delta_time = this->currentFrame - this->lastFrame;
		
		
	}
	void setup()
	{
		
		
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, Box_light_arr[0].get_shadow_texture_id());

		
		
	}

	void cycle()
	{
		update();
		
		PreDraw();
		


		Frame_Buffer::bind_window_0();
		glViewport(0, 0, 800, 600);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		glCullFace(GL_BACK);
		Draw();
	


		PostProcessing();
		

	}
	
	void Draw()
	{
		glCullFace(GL_BACK);
		this->Matrices_Buffer.Bind();
		this->camera.set_uniform();
		character_rb.Draw();
		for (int i=0;i< Box_light_arr.size();i++)
		{
			Box_light_arr[i].Draw();
		}
		for (int i = 0; i < Box_arr.size(); i++)
		{
			Box_arr[i].Draw();
		}
		v3g.Draw();
		character.Draw();
		skycube.Draw();
		sq.Draw();
	}
	void PreDraw()
	{
		Box_light_arr[0].bind_shadow_shader();
		glCullFace(GL_FRONT);
		for (int i=0;i< Box_light_arr.size(); i++)//using only one light source
		{
		Matrices_Buffer.Bind();
		Box_light_arr[i].set_view_matrix();
		Box_light_arr[i].set_projection_matrix();
		Box_light_arr[i].bind_shadow_frame_buffer();
		glViewport(0, 0, 1024, 1024);
		Box_light_arr[i].get_shadow_map()->is_frame_buffer_complete();
		glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

		DrawSimple();

		Light_Buffer_arr[i].Bind();
		Box_light_arr[i].set_light_data();
		Box_light_arr[i].set_lightView_matrix();
		}
	

	}
	void DrawSimple()
	{
		this->Matrices_Buffer.Bind();
		
	
		for (int i = 0; i < Box_arr.size(); i++)
		{
			Box_arr[0].DrawShadow();
		}
	
		character.DrawShadow();
		character_rb.DrawShadow();
	}
	void update()
	{
		update_time();
		for (int i = 0; i < Box_light_arr.size(); i++)
		{
			Box_light_arr[i].update(this->currentFrame);
		}
		
		this->v3g.update(this->delta_time);
		this->sq.update(this->delta_time);
	}
	
	
	void PostProcessing()
	{
		Box_light_arr[0].PostProcessing();
		Box_arr[0].PostProcessing();
		Box_arr[1].PostProcessing();
	}
};