#pragma once
#include "Intersection.h"
#include "OBJModel.h"
#include "ray.h"
#include "Material.h"
#include "float3.h"
#include "Aabb.h"
#include "Light.h"
#include "Camera.h"
#include "BvhNode.h"

class Triangle
{
public: 
	chag::float3 v0, v1, v2; 
	chag::float3 n0, n1, n2; 
	chag::float2 uv0, uv1, uv2; 
	Material *m_material; 
	bool intersect(const chag::ray &r, Intersection &i);
};

class Scene
{
	void constructBvh();
public:
	Scene(void);
	~Scene(void);
	std::vector<Light> m_lights; 
	std::vector<Camera> m_cameras; 
	std::vector<Triangle> m_triangles; 
	virtual void buildFromObj(OBJModel *model); 
	virtual bool intersect(const chag::ray &r, Intersection &isect); 
	virtual bool intersectP(const chag::ray &r);
	BvhNode *bvh;
};

