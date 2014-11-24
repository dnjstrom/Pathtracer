#pragma once
#include <float3.h>

class Camera
{
public:
	Camera(void);
	~Camera(void);
	chag::float3 m_position; 
	chag::float3 m_direction; 
	chag::float3 m_up; 
	float m_fov; 
};

