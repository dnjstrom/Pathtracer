#pragma once
#include <float3.h>
#include "MCSampling.h"
#include "Texture.h"
#include "Intersection.h"

///////////////////////////////////////////////////////////////////////////////
// Abstract base class for materials
///////////////////////////////////////////////////////////////////////////////
class Material
{
protected: 
	// A few helper functions that are useful for all Materials
	float sign(float v) const { return v < 0.0f ? -1.0f : 1.0f; }; 
	bool sameHemisphere(const chag::float3 &i, const chag::float3 &o, const chag::float3 &n) const {
		return sign(dot(o,n)) == sign(dot(i,n));
	}
public: 
	// The public interface which all Materials must implement
	virtual chag::float3 f(const chag::float3 &i, const chag::float3 &o, const Intersection &isect) const = 0;
	virtual chag::float3 sample_f(const chag::float3 &i, chag::float3 &o, const Intersection &isect, float &pdf) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
// Diffuse reflection
// ===========================================================================
// The BRDF for diffuse reflection is constant. It can be importance sampled 
// on the cosine term to improve sampling quality somewhat. 
///////////////////////////////////////////////////////////////////////////////
class DiffuseMaterial : public Material
{
public: 
	// The reflectance (color) of the material
	chag::float3 m_reflectance;
	Texture *m_reflectanceMap; 
	// Material Interface
	chag::float3 f(const chag::float3 &i, const chag::float3 &o, const Intersection &isect) const; 
	chag::float3 sample_f(const chag::float3 &i, chag::float3 &o, const Intersection &isect, float &pdf) const; 
};

///////////////////////////////////////////////////////////////////////////////
// Perfect specular reflection
// ===========================================================================
// This BRDF will allways sample EXACTLY one direction given a normal and an
// incident vector. Therefore, pdf = 1.0 and f can just be set to 
// zero as we will never select a light sample which is exactly in that 
// direction. 
///////////////////////////////////////////////////////////////////////////////
class SpecularReflectionMaterial : public Material
{
public: 
	// The reflectance (color) of the specular reflection
	chag::float3 m_reflectance;
	// Material Interface
	chag::float3 f(const chag::float3 &i, const chag::float3 &o, const Intersection &isect) const; 
	chag::float3 sample_f(const chag::float3 &i, chag::float3 &o, const Intersection &isect, float &pdf) const; 
};

///////////////////////////////////////////////////////////////////////////////
// Perfect specular refraction
// ===========================================================================
// This BTDF oll allways sample EXACTLY one direction given a normal, 
// incident vector and index of refraction. Therefore, pdf = 1.0 and f can 
// just be set to zero as we oll never select a light sample which is exactly 
// in that direction. 
///////////////////////////////////////////////////////////////////////////////
class SpecularRefractionMaterial : public Material
{
public: 
	// Index of refraction
	float m_ior; 
	// Material Interface
	chag::float3 f(const chag::float3 &i, const chag::float3 &o, const Intersection &isect) const; 
	chag::float3 sample_f(const chag::float3 &i, chag::float3 &o, const Intersection &isect, float &pdf) const; 
};

///////////////////////////////////////////////////////////////////////////////
// Fresnel blending
// ===========================================================================
// This Material actually combines two bxdfs oth a view dependent fresnel
// term. We use the Schlick approximation to the real fresnel equations, 
// which irks quite well for conductors. 
///////////////////////////////////////////////////////////////////////////////
class FresnelBlendMaterial : public Material
{
public: 
	Material *m_onReflectionMaterial; 
	Material *m_onRefractionMaterial; 
	float m_R0; 
	float R(const chag::float3 &i, const chag::float3 &n) const;
	// Material Interface
	chag::float3 f(const chag::float3 &i, const chag::float3 &o, const Intersection &isect) const; 
	chag::float3 sample_f(const chag::float3 &i, chag::float3 &o, const Intersection &isect, float &pdf) const; 
};

///////////////////////////////////////////////////////////////////////////////
// Linear blending
// ===========================================================================
// This Material combines two bxdfs linearly as: w*M1 + (1.0-w)*M2
///////////////////////////////////////////////////////////////////////////////
class BlendMaterial : public Material
{
public: 
	Material *m_firstMaterial; 
	Material *m_secondMaterial; 
	float m_w; 
	// Material Interface
	chag::float3 f(const chag::float3 &i, const chag::float3 &o, const Intersection &isect) const; 
	chag::float3 sample_f(const chag::float3 &i, chag::float3 &o, const Intersection &isect, float &pdf) const; 
};

