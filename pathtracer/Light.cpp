#include "Light.h"
#include "MCSampling.h"
using namespace chag; 

Light::Light(void)
{
}

Light::~Light(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// Rejection sample a uniform position inside the sphere. There is 
// a better way.
///////////////////////////////////////////////////////////////////////////////
float3 Light::sample()
{
	float3 p = make_vector(2.0f, 0.0f, 0.0f); 
	while(length(p) > 1.0) {
		p = 2.0f * make_vector(randf(), randf(), randf());
		p -= make_vector(1.0f, 1.0f, 1.0f); 
	}
	return m_radius * p + m_position; 
}

///////////////////////////////////////////////////////////////////////////////
// Calculate the radiance that is emitted from the light and reaches point p 
///////////////////////////////////////////////////////////////////////////////
float3 Light::Le(const float3 &p)
{
	float dist = length(m_position - p); 
	return m_intensity / (dist * dist); 
}
