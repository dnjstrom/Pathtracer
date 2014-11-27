#include "Scene.h"
#include "MCSampling.h"
#include <iostream>

#include "BvhNode.h"

using namespace chag; 
using namespace std; 

Scene::Scene(void)
{
}


Scene::~Scene(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// Find the first intersection between a ray and the scene. 
///////////////////////////////////////////////////////////////////////////////
bool Scene::intersect(const chag::ray &r, Intersection &isect)
{
	bool foundIntersection = false; 
	for(unsigned int i=0; i<m_triangles.size(); i++)
	{
		foundIntersection |= m_triangles[i].intersect(r, isect); 
	}
	return foundIntersection; 
}

///////////////////////////////////////////////////////////////////////////////
// Return wether there there is ANY intersection between the ray and the scene
///////////////////////////////////////////////////////////////////////////////
bool Scene::intersectP(const chag::ray &r)
{
	Intersection isect; 
	for(unsigned int i=0; i<m_triangles.size(); i++)
	{
		if(m_triangles[i].intersect(r, isect)) return true; 
	}
	return false; 
}


///////////////////////////////////////////////////////////////////////////////
// Buid a Scene object from an OBJ file
///////////////////////////////////////////////////////////////////////////////
void Scene::buildFromObj(OBJModel *model)
{
	
	typedef std::map<std::string, OBJModel::Light>::iterator lightIterator; 
	for(lightIterator iter = model->m_lights.begin(); iter != model->m_lights.end(); iter++) {
		Light l;
		l.m_position = iter->second.position;
		l.m_intensity = iter->second.intensity * iter->second.color; 
		l.m_radius = iter->second.radius; 
		m_lights.push_back(l); 
	}

	typedef std::map<std::string, OBJModel::Camera>::iterator cameraIterator; 
	for(cameraIterator iter = model->m_cameras.begin(); iter != model->m_cameras.end(); iter++) {
		Camera c; 
		c.m_position = iter->second.position;
		c.m_direction = normalize(iter->second.target - iter->second.position); 
		c.m_up = normalize(iter->second.up); 
		c.m_fov = iter->second.fov; 
		m_cameras.push_back(c); 
	}


	for(unsigned int i=0; i<model->m_chunks.size(); i++) {
		OBJModel::Chunk &chunk = model->m_chunks[i]; 
		Material *material; 

		DiffuseMaterial *diffuse = new DiffuseMaterial; 
		diffuse->m_reflectance = chunk.material->diffuseReflectance; 
		if(chunk.material->diffuseReflectanceMap != "") {
			diffuse->m_reflectanceMap = new Texture; 
			diffuse->m_reflectanceMap->load(chunk.material->diffuseReflectanceMap); 
		}
		else diffuse->m_reflectanceMap = NULL; 

		SpecularReflectionMaterial *specularReflection = new SpecularReflectionMaterial; 
		specularReflection->m_reflectance = chunk.material->specularReflectance; 

		SpecularRefractionMaterial  *specularRefraction = new SpecularRefractionMaterial; 
		specularRefraction->m_ior = chunk.material->indexOfRefraction; 

		BlendMaterial *blend1 = new BlendMaterial; 
		blend1->m_w = chunk.material->transparency; 
		blend1->m_firstMaterial = specularRefraction; 
		blend1->m_secondMaterial = diffuse;

		FresnelBlendMaterial *fresnel = new FresnelBlendMaterial; 
		fresnel->m_R0 = chunk.material->reflAt0Deg; 
		fresnel->m_onReflectionMaterial = specularReflection; 
		fresnel->m_onRefractionMaterial = blend1; 
	
		BlendMaterial *blend0 = new BlendMaterial;
		blend0->m_w = chunk.material->reflAt90Deg; 
		blend0->m_firstMaterial = fresnel; 
		blend0->m_secondMaterial = blend1; 
			
		material = blend0; 

		for(unsigned int j=0; j<chunk.m_positions.size()/3; j++) {
			Triangle triangle; 
			triangle.v0 = chunk.m_positions[j*3 + 0];
			triangle.v1 = chunk.m_positions[j*3 + 1];
			triangle.v2 = chunk.m_positions[j*3 + 2];
			triangle.n0 = chunk.m_normals[j*3 + 0]; 
			triangle.n1 = chunk.m_normals[j*3 + 1]; 
			triangle.n2 = chunk.m_normals[j*3 + 2]; 
			triangle.uv0 = chunk.m_uvs[j*3+0]; 
			triangle.uv1 = chunk.m_uvs[j*3+1];
			triangle.uv2 = chunk.m_uvs[j*3+2];
			triangle.m_material = material; 
			m_triangles.push_back(triangle); 
		}
	}

	/* Construct BVH */
	bvh = new BvhNode(m_triangles);
}

///////////////////////////////////////////////////////////////////////////////
// Intersect a triangle and a ray
///////////////////////////////////////////////////////////////////////////////
bool Triangle::intersect(const chag::ray &r, Intersection &i)
{
	float3 d = r.d; 
	float3 o = r.o; 
	float3 e1 = v1-v0; 
	float3 e2 = v2-v0; 
	float3 q = cross(d, e2);
	float a = dot(e1, q);
	float epsilon = 0.00001f;
	//if(a < 0) return false; // Backface cull
	if(a > -epsilon && a < epsilon) return false;
	float f = 1.0f/a;
	float3 s = o - v0;
	float u = f * dot(s,q);
	if(u < 0.0 || u > 1.0) return false; 
	float3 R = cross(s, e1);
	float v = f * dot(d, R);
	if(v < 0.0 || u+v > 1.0) return false;
	float t = f * dot(e2, R); 
	if(t < r.mint || t > r.maxt) return false; 
	r.maxt = t;
	i.m_position = r(t);
	float w = 1.0f - (u+v);
	i.m_normal = normalize(w*n0 + u*n1 + v*n2); 
	i.m_material = m_material; 
	return true; 	
}

