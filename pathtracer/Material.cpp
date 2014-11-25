#include "Material.h"
#include "MCSampling.h"

using namespace chag; 

///////////////////////////////////////////////////////////////////////////////
// Diffuse reflection
///////////////////////////////////////////////////////////////////////////////
float3 DiffuseMaterial::f(const float3 &i, const float3 &o, const Intersection &isect) const
{
	return m_reflectance * (1.0f / M_PI); 
}

float3 DiffuseMaterial::sample_f(const float3 &i, float3 &o, const Intersection &isect, float &pdf) const
{
	const float3 &n = isect.m_normal; 
	float3 tangent = normalize(perpendicular(n)); 
	float3 bitangent = cross(n, tangent);
	float3 s = cosineSampleHemisphere();
	o = normalize(s.x * tangent + s.y * bitangent + s.z * n);
	pdf = cos(dot(isect.m_normal, o)) / M_PI;
	return f(i,o,isect);  
};

///////////////////////////////////////////////////////////////////////////////
// Perfect specular reflection
///////////////////////////////////////////////////////////////////////////////
float3 SpecularReflectionMaterial::f(const float3 &i, const float3 &o, const Intersection &isect) const
{
	return make_vector(0.0f, 0.0f, 0.0f); 
}

float3 SpecularReflectionMaterial::sample_f(const float3 &i, float3 &o, const Intersection &isect, float &pdf) const
{
	const float3 &n = isect.m_normal; 
	o = normalize(2.0f * abs(dot(i,n)) * n - i); 
	pdf = sameHemisphere(i, o, n) ? abs(dot(o, n)) : 0.0f;
	return m_reflectance; 
}

///////////////////////////////////////////////////////////////////////////////
// Perfect specular refraction
///////////////////////////////////////////////////////////////////////////////
float3 SpecularRefractionMaterial::f(const float3 &i, const float3 &o, const Intersection &isect) const
{
	return make_vector(0.0f, 0.0f, 0.0f); 
}

float3 SpecularRefractionMaterial::sample_f(const float3 &i, float3 &o, const Intersection &isect, float &pdf) const
{
	const float3 &n = isect.m_normal; 
	float eta; 
	if(dot(-i, n) < 0.0f) eta = 1.0f/m_ior; 
	else eta = m_ior; 

	float3 N = dot(-i, n) < 0.0 ? n : -n; 

	float w = -dot(-i,N) * eta;
	float k = 1.0f + (w-eta)*(w+eta); 
	if(k < 0.0f) {
		// Total internal reflection
		SpecularReflectionMaterial refMat; 
		refMat.m_reflectance = make_vector(1.0f, 1.0f, 1.0f); 
		Intersection newIntersection = isect; 
		newIntersection.m_normal = N; 
		return refMat.sample_f(i, o, newIntersection, pdf); 
	}	
	k = sqrt(k); 
	o = normalize(-eta*i + (w-k)*N); 
	pdf = 1.0; 
	return make_vector(1.0f, 1.0f, 1.0f); 
};

///////////////////////////////////////////////////////////////////////////////
// Fresnel Blending
///////////////////////////////////////////////////////////////////////////////
float FresnelBlendMaterial::R(const chag::float3 &wo, const float3 &n) const 
{
	return m_R0 + (1.0f - m_R0) * pow(1.0f - abs(dot(wo,n)), 5.0f); 
}

chag::float3 FresnelBlendMaterial::f(const chag::float3 &wo, const chag::float3 &wi, const Intersection &isect) const 
{
	const float3 &n = isect.m_normal; 
	float _R = R(wo,n); 
	return _R * m_onReflectionMaterial->f(wo, wi, isect) + 
		   (1.0f - _R) * m_onRefractionMaterial->f(wo, wi, isect); 
}

chag::float3 FresnelBlendMaterial::sample_f(const chag::float3 &wo, chag::float3 &wi, const Intersection &isect, float &pdf) const 
{
	const float3 &n = isect.m_normal; 
	if(randf() < R(wo, n)) 
		return m_onReflectionMaterial->sample_f(wo, wi, isect, pdf); 
	else 
		return m_onRefractionMaterial->sample_f(wo, wi, isect, pdf); 
}

///////////////////////////////////////////////////////////////////////////////
// Linear Blending
///////////////////////////////////////////////////////////////////////////////
chag::float3 BlendMaterial::f(const chag::float3 &wo, const chag::float3 &wi, const Intersection &isect) const 
{
	const float3 &n = isect.m_normal; 
	return m_w * m_firstMaterial->f(wo, wi, isect) + (1.0f - m_w) * m_secondMaterial->f(wo, wi, isect); 
}

chag::float3 BlendMaterial::sample_f(const chag::float3 &wo, chag::float3 &wi, const Intersection &isect, float &pdf) const 
{
	const float3 &n = isect.m_normal; 
	if(randf() < m_w) 
		return m_firstMaterial->sample_f(wo, wi, isect, pdf); 
	else 
		return m_secondMaterial->sample_f(wo, wi, isect, pdf); 
}

