#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <bitset>


class texture
{
private:
	unsigned int texture_ID;

public:
	texture()
	{
		glGenTextures(1, &texture_ID);
		glBindTexture(GL_TEXTURE_2D, texture_ID);
	}
	unsigned int get_id()
	{
		return texture_ID;
	}
};
class texture_color : public texture
{
	

public:
	static constexpr std::bitset<8> TYP_0   { 0b00000000 };
	static constexpr std::bitset<8> TYP_1   { 0b00000001 };
	static constexpr std::bitset<8> flip    { 0b00010000 };
	static constexpr std::bitset<8> TYP_RGB { 0b00000100 };
	static constexpr std::bitset<8> TYP_RGBA{ 0b00000000 };


	texture_color(std::bitset<8> TYP = TYP_0, std::string path = ""):
		texture()
	{
		
		color_setup_path(path, TYP);
		color_setup_type(TYP);
	
	}


private:

	void color_setup_path(std::string path , std::bitset<8> TYP)
	{
		if ((TYP & std::bitset<8> { 0b00010000 }) == flip)
		{
				stbi_set_flip_vertically_on_load(true);
		}
		else
		{
			stbi_set_flip_vertically_on_load(false);
		}
		if (path == "")
		{
			if ((TYP & std::bitset<8> { 0b00001100 }) == TYP_RGB)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);//allocating memory 

			}
			else if ((TYP & std::bitset<8> { 0b00001100 }) == TYP_RGBA)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);//allocating memory 

			}
			else
			{
				std::cout << "error: TYP is not recognized";
			}
		}
		else
		{
			int width, height, nrChannels;
			char* array = &((path)[0]);
			unsigned char* data = stbi_load(array, &width, &height, &nrChannels, 0);

			if (data)
			{
				if ((TYP & std::bitset<8> { 0b00001100 }) == TYP_RGB)
				{
					
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//allocating memory 
					glGenerateMipmap(GL_TEXTURE_2D);

				}
				else if ((TYP & std::bitset<8> { 0b00001100 }) == TYP_RGBA)
				{
					
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);//allocating memory 
					glGenerateMipmap(GL_TEXTURE_2D);

				}
				else
				{
					std::cout << "error: TYP is not recognized";
				}
				
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);//delete data because we dont need it any more
		}		
	}

	void color_setup_type(std::bitset<8> TYP)
	{
		if ((TYP & std::bitset<8> { 0b00000011 }) == TYP_0)
		{
			color_setup_type_0();
		}
		else if ((TYP & std::bitset<8> { 0b00000011 }) == TYP_1)
		{
			color_setup_type_1();

		}
		else
		{
			std::cout << "error: TYP is not recognized";
		}
	}

	void color_setup_type_0()
	{

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	void color_setup_type_1() 
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	
};
class texture_depth : public texture
{
public:
	static constexpr std::bitset<8> TYP_0{ 0b00000000 };

	texture_depth(std::bitset<8> TYP = TYP_0):
		texture()
	{
		if ((TYP & std::bitset<8> { 0b00000011 }) == TYP_0)
		{
			depth_setup_type_0();
		}
		else
		{
			std::cout << "error: TYP is not recognized";
		}
	}
private:
	void depth_setup_type_0()
	{
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0,
GL_DEPTH_COMPONENT, GL_FLOAT, NULL);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
};