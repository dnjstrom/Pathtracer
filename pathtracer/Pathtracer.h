#pragma once
#include <vector>
#include <float3.h>
#include <int2.h>
#include "Scene.h"
#include "ray.h"
#include "MCSampling.h"

#define PT_MAX_BOUNCES 16
#define PT_EPSILON 0.00001f 

class Pathtracer
{
public: 
	Pathtracer(void);
	~Pathtracer(void);

	Scene *m_scene; 
	int m_selectedCamera; 
	chag::float3 *m_frameBuffer; 
	chag::int2	  m_frameBufferSize; 
	int			  m_frameBufferSamples; 
	void restart();
	void resize(int width, int height);
	void tracePrimaryRays();
	chag::float3 Li(const chag::ray &r, const Intersection &isect); 
	chag::float3 Lenvironment(const chag::ray &r); 
};

