#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "VAOClass.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>




class AnimateModel
{
struct vertex
	{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

		float jointIndex[6];
		float weights[6];

		
	};
private:	
	unsigned int max_num_per_vertex = 6;
	VAO mesh;
	unsigned int max_num_bones_per_vertex;

	
public:
	//Joint rootJoint;
	unsigned int number_of_vertices;
	unsigned int number_of_indices;
	std::string scene_path;
	std::vector<glm::mat4> bonesMat;

	AnimateModel( std::string path,
		int max_num_bones_per_vertex):
		scene_path(path), 
		max_num_bones_per_vertex(max_num_bones_per_vertex),
		mesh(true)
	{
		
		
		set_model();
	}
	void set_model()
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(this->scene_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;

		}
#pragma region initialize_VAO

		//scene->mMeshes[0]->mBones[0].
		std::vector<vertex> VBO_temp;
		std::vector<unsigned int> EBO_temp;

		vertex tempver{};
		unsigned int index_temp[3];
		this->number_of_vertices = 0;
		//VBO_temp.resize(); need to clac the number of vertexes 
		//EBO_temp.resize(); need to clac the number of faces
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//setting VBO not includng bones
			for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
			{

				tempver.position = glm::vec3(scene->mMeshes[i]->mVertices[j].x, scene->mMeshes[i]->mVertices[j].y, scene->mMeshes[i]->mVertices[j].z);

				tempver.normal = glm::vec3(scene->mMeshes[i]->mNormals[j].x, scene->mMeshes[i]->mNormals[j].y, scene->mMeshes[i]->mNormals[j].z);


				tempver.texCoords = glm::vec2(scene->mMeshes[i]->mTextureCoords[0][j].x, scene->mMeshes[i]->mTextureCoords[0][j].y);



				VBO_temp.push_back(tempver);


			}


			

			//setting EBO
			for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				//its always a triangle
				index_temp[0] = scene->mMeshes[i]->mFaces[j].mIndices[0] + this->number_of_vertices;
				index_temp[1] = scene->mMeshes[i]->mFaces[j].mIndices[1] + this->number_of_vertices;
				index_temp[2] = scene->mMeshes[i]->mFaces[j].mIndices[2] + this->number_of_vertices;

				EBO_temp.push_back(index_temp[0]);
				EBO_temp.push_back(index_temp[1]);
				EBO_temp.push_back(index_temp[2]);
			}


			this->number_of_vertices += scene->mMeshes[i]->mNumVertices;
		}
		this->number_of_indices = EBO_temp.size();
#pragma endregion
#pragma region revarse_map_for_bones_wieghts&bonesMat_initialize
		int bones_index_counter = 0, vertex_index_counter = 0, Weights_array_index = 0;

		//resizing bonesMat
		{
			int numOfBones = 0;
			
			for (int i=0;i< scene->mNumMeshes;i++)
			{
				numOfBones += scene->mMeshes[i]->mNumBones;
			}
			bonesMat.resize(numOfBones);
		}		

		std::vector<vertex> VBO_TEST = VBO_temp;//delet

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++)
			{
				bonesMat[bones_index_counter+j]=aiMatrix_to_mat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix);

				for (int k = 0; k < scene->mMeshes[i]->mBones[j]->mNumWeights; k++)
				{
					Weights_array_index = 0;
					while (Weights_array_index < this->max_num_bones_per_vertex)
					{
						if (VBO_temp[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId + vertex_index_counter].weights[Weights_array_index] < 0.001f)//find a location to store the weight of the bone in the vertex
						{break;}
						Weights_array_index++;
					}
					if (Weights_array_index >= this->max_num_bones_per_vertex) {
						printf("error: too much bones ber vertex, need to increase  max_num_bones_per_vertex");
					}
					else {
						VBO_temp[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId + vertex_index_counter]
							.jointIndex[Weights_array_index] = j + bones_index_counter;

						VBO_temp[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId + vertex_index_counter]
							.weights[Weights_array_index] = scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
					}
				}

			}

			vertex_index_counter += scene->mMeshes[i]->mNumVertices;
			bones_index_counter += scene->mMeshes[i]->mNumBones;
		}


		for (int i=0;i<scene->mNumMeshes;i++)
		{
			for(int j=0;j<scene->mMeshes[i]->mNumBones;j++)
			{
				for (int k=0;k<scene->mMeshes[i]->mBones[j]->mNumWeights;k++)
				{
					
if (VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[0] < 0.001)
					{
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[0]=
		scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].jointIndex[0]=
		j;
					}
else if(VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[1] < 0.001)
{
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[1] =
		scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].jointIndex[1] =
		j;
}
else if (VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[2] < 0.001)
{
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[2] =
		scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].jointIndex[2] =
		j;
}
else if (VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[3] < 0.001)
{
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[3] =
		scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].jointIndex[3] =
		j;
}
else if (VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[4] < 0.001)
{
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[4] =
		scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].jointIndex[4] =
		j;
}
else if (VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[5] < 0.001)
{
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].weights[5] =
		scene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
	VBO_TEST[scene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId].jointIndex[5] =
		j;
}
				
				}
			
			}
		}





#pragma endregion
		
		

		
		
	set_VAO(&VBO_temp, &EBO_temp);
		
	}




	void bind_VAO()
	{
		mesh.Bind_data();
	}
	

	void set_VAO(std::vector<vertex>* VBO_data_pointer, std::vector<unsigned int>* EBO_data_pointer)
	{
	
		mesh.Bind_data();




		glBufferData(GL_ARRAY_BUFFER, (*VBO_data_pointer).size() * sizeof(vertex)
			, &(*VBO_data_pointer)[0], GL_STATIC_DRAW);

#pragma region attribute_setting
	/*	glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;

		float jointIndex[6];
		float weights[6];*/
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)offsetof(vertex, position));


		glEnableVertexAttribArray(1);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)offsetof(vertex, normal));


		glEnableVertexAttribArray(2);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)offsetof(vertex, texCoords));


		glEnableVertexAttribArray(3);

		glVertexAttribPointer(3, max_num_per_vertex, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)offsetof(vertex, jointIndex));


		glEnableVertexAttribArray(4);

		glVertexAttribPointer(4, max_num_per_vertex, GL_FLOAT, GL_FALSE, sizeof(vertex),
			(void*)offsetof(vertex, weights));

#pragma endregion




		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (*EBO_data_pointer).size() * sizeof(unsigned int), &(*EBO_data_pointer)[0], GL_STATIC_DRAW);

	}
	glm::mat4 aiMatrix_to_mat4(aiMatrix4x4 m)
	{
		return glm::mat4(m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1, m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);
	}

};
