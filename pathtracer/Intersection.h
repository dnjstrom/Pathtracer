#pragma once
#include "float3.h"
#include "float2.h"
class Material; 

class Intersection
{
public:
	chag::float3 m_position; 
	chag::float3 m_normal; 
	Material *m_material;
};

