#pragma once
#include <iostream>
#include <float3.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif 


inline float randf() {
	return rand() / float(RAND_MAX); 
}

inline chag::float3 uniformSampleHemisphere() {
	float r1 = randf(); 
	float r2 = randf(); 
	return chag::make_vector(
		2.0f * cos(2.0f * M_PI * r1)*sqrt(r2 * (1.0f - r2)),
		2.0f * sin(2.0f * M_PI * r1)*sqrt(r2 * (1.0f - r2)),
		abs(1.0f - 2.0f * r2)); 
}

inline void concentricSampleDisk(float *dx, float *dy) {
	float r, theta;
	float u1 = randf(); 
	float u2 = randf(); 
	// Map uniform random numbers to $[-1,1]^2$
	float sx = 2 * u1 - 1;
	float sy = 2 * u2 - 1;
	// Map square to $(r,\theta)$
	// Handle degeneracy at the origin
	if (sx == 0.0 && sy == 0.0) {
		*dx = 0.0;
		*dy = 0.0;
		return;
	}
	if (sx >= -sy) {
		if (sx > sy) {
			// Handle first region of disk
			r = sx;
			if (sy > 0.0) theta = sy/r;
			else          theta = 8.0f + sy/r;
		}
		else {
			// Handle second region of disk
			r = sy;
			theta = 2.0f - sx/r;
		}
	}
	else {
		if (sx <= sy) {
			// Handle third region of disk
			r = -sx;
			theta = 4.0f - sy/r;
		}
		else {
			// Handle fourth region of disk
			r = -sy;
			theta = 6.0f + sx/r;
		}
	}
	theta *= M_PI / 4.f;
	*dx = r * cosf(theta);
	*dy = r * sinf(theta);
}

inline chag::float3 cosineSampleHemisphere() {
	chag::float3 ret;
	concentricSampleDisk(&ret.x, &ret.y);
	ret.z = sqrt(std::max(0.f, 1.f - ret.x*ret.x - ret.y*ret.y));
	return ret;
}

