#pragma once
#include <float3.h>

///////////////////////////////////////////////////////////////////////////////
// For now, a light is an emitting sphere that radiates uniformly in all 
// directions. 
///////////////////////////////////////////////////////////////////////////////

class Light
{
public:
	Light(void);
	~Light(void);

	chag::float3 m_position; 
	float m_radius;
	chag::float3 sample(); 
	chag::float3 m_intensity; 
	chag::float3 Le(const chag::float3 &p); 
};

