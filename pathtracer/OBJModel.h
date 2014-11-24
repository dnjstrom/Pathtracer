#ifndef __OBJModel_h_
#define __OBJModel_h_

#include "GL/glew.h"
#include "GL/glut.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <float2.h>
#include <float3.h>
#include <float4.h>


class OBJModel
{
public:
	OBJModel(void);
	~OBJModel(void);
	/**
	* When called, renders the OBJModel
	*/
	void render();
	/**
	* Load the OBJModel from disk
	*/
	void load(std::string fileName); 

protected:

	size_t getNumVerts();

	void loadOBJ(std::ifstream &file, std::string basePath);
	void loadMaterials(std::string fileName, std::string basePath);
	unsigned int loadTexture(std::string fileName, std::string basePath);

	struct Material
	{
    chag::float3 diffuseReflectance;
		std::string diffuseReflectanceMap; 
		chag::float3 specularReflectance;
		chag::float3 emittance; 
		float specularRoughness; 
		float transparency; 
		float reflAt0Deg; 
		float reflAt90Deg; 
		float indexOfRefraction; 
	};

	std::map<std::string, Material> m_materials;


public: 
	struct Light
	{
		chag::float3 position; 
		chag::float3 color; 
		float radius; 
		float intensity; 
	};
	std::map<std::string, Light> m_lights; 

	struct Camera
	{
		chag::float3 position; 
		chag::float3 target; 
		chag::float3 up; 
		float fov; 
	};
	std::map<std::string, Camera> m_cameras; 

	struct Chunk
	{
		Material *material;
		// Data on host
		std::vector<chag::float3> m_positions;
		std::vector<chag::float3> m_normals;
		std::vector<chag::float2> m_uvs; 
		// Data on GPU
		GLuint	m_positions_bo; 
		GLuint	m_normals_bo; 
		GLuint	m_uvs_bo; 
		// Vertex Array Object
		GLuint	m_vaob; 
	};
	std::vector<Chunk> m_chunks;
};

#endif // __OBJModel_h_
