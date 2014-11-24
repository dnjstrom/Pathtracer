#pragma once
#include "float3.h"
#include <float.h>

namespace chag 
{
	class ray
	{
	public:
		ray(void) : mint(0.0f), maxt(FLT_MAX) {};
		ray(const float3 &origin, const float3 direction, float start = 0.0f, float end = FLT_MAX) : 
			o(origin), d(direction), mint(start), maxt(end) {}; 
		~ray(void);
		float3	o, d;
		mutable float mint, maxt; 
		float3 operator()(float t) const { return o + d * t; }
		// In pbrt we have: 
		// float time;	// For motion blur
		// int depth;	// Number of bounces
	};
}