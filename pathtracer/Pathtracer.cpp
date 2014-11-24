#include <iostream>
#include "Pathtracer.h"
#include "ray.h"
#include <float2.h>

#include "MCSampling.h"

using namespace chag; 
using namespace std; 

Pathtracer::Pathtracer(void)
{
	m_frameBuffer = NULL; 
}


Pathtracer::~Pathtracer(void)
{
}

///////////////////////////////////////////////////////////////////////////
// Restart the path-tracing
///////////////////////////////////////////////////////////////////////////
void Pathtracer::restart()
{
	for(int i=0; i<m_frameBufferSize.x*m_frameBufferSize.y; i++) {
		m_frameBuffer[i] = make_vector(0.0f, 0.0f, 0.0f);
	}
	m_frameBufferSamples = 0; 
}

///////////////////////////////////////////////////////////////////////////
// Resize the path-tracer framebuffer
///////////////////////////////////////////////////////////////////////////
void Pathtracer::resize(int width, int height)
{
	if(m_frameBuffer == NULL) delete[] m_frameBuffer; 
	m_frameBuffer = new float3[width * height]; 
	m_frameBufferSize = make_vector(width, height); 
	restart(); 
}

///////////////////////////////////////////////////////////////////////////
// Create and trace a ray per pixel
///////////////////////////////////////////////////////////////////////////
void Pathtracer::tracePrimaryRays()
{
	// Scene must have a camera
	if(m_scene->m_cameras.size() == 0) {
		cout << "Scene has no cameras!\n";
		exit(0); 
	}
	// Initialize selected camera
	float3 camera_pos = m_scene->m_cameras[m_selectedCamera % m_scene->m_cameras.size()].m_position; 
	float3 camera_dir = m_scene->m_cameras[m_selectedCamera % m_scene->m_cameras.size()].m_direction; 
	float3 camera_up = m_scene->m_cameras[m_selectedCamera % m_scene->m_cameras.size()].m_up; 
	float3 camera_right = normalize(cross(camera_dir, camera_up)); 
	camera_up = normalize(cross(camera_right, camera_dir)); 
	float camera_fov = m_scene->m_cameras[m_selectedCamera % m_scene->m_cameras.size()].m_fov; 
	float camera_aspectRatio = float(m_frameBufferSize.x) / float(m_frameBufferSize.y); 

	// Create a ray per pixel
	float3 Z = camera_dir * cos(camera_fov/2.0f * (M_PI/180.0f));
	float3 X = camera_up * sin(camera_fov/2.0f * (M_PI/180.0f));
	float3 Y = camera_right * sin(camera_fov/2.0f * (M_PI/180.0f)) * camera_aspectRatio;
	float3 min_d = Z - Y - X;
	float3 dX = 2.0f * ((Z - X) - min_d); 
	float3 dY = 2.0f * ((Z - Y) - min_d); 

	#pragma omp parallel for
	for(int y=0; y<m_frameBufferSize.y; y++) {
		for(int x=0; x<m_frameBufferSize.x; x++) {
			ray primaryRay; 
			primaryRay.o = camera_pos;

			float2 screenCoord = make_vector(float(x + randf())/float(m_frameBufferSize.x), float(y + randf())/float(m_frameBufferSize.y));

			primaryRay.d = normalize(min_d + 
						   screenCoord.x * dX +  
						   screenCoord.y * dY);
			
			// Intersect ray with scene
			Intersection isect; 
			if(m_scene->intersect(primaryRay, isect)) {
				// If it hit something, evaluate the radiance from that point
				m_frameBuffer[y*m_frameBufferSize.x+x] += Li(primaryRay, isect); 
			}
			else {
				// Otherwise evaluate environment
				m_frameBuffer[y*m_frameBufferSize.x+x] += Lenvironment(primaryRay); 
			}
		}
	}
	m_frameBufferSamples += 1;
}

///////////////////////////////////////////////////////////////////////////
// Evaluate the outgoing radiance from the first intersection point of 
// a primary ray. 
///////////////////////////////////////////////////////////////////////////
float3 Pathtracer::Li(const chag::ray &r, const Intersection &isect)
{
	const Intersection *isectp = &isect; 

	// Initialize return radiance
	float3 L = make_vector(0.0f, 0.0f, 0.0f);
	// p is the intersection point
	const float3	&p = isectp->m_position; 
	// n is the intersection normal
	const float3	&n = isectp->m_normal; 
	// mat is the material at the intersection
	Material			&mat = *isectp->m_material; 
	// wi is the incoming direction
	float3 wi = -r.d; 

	for(unsigned int i=0; i<m_scene->m_lights.size(); i++) {
		// Sample a position on the area light
		float3 lightSamplePos = m_scene->m_lights[i].sample(); 
		// Calculate the outgoing direction towards that sample
		float3 wo = normalize(lightSamplePos - isectp->m_position); 
		// Add this lights contribution to the radiance
		float3 Li =  m_scene->m_lights[i].Le(p); 
		L += mat.f(wi, wo, *isectp) * Li * abs(dot(wo, n)); 
	}
	return L; 
}

///////////////////////////////////////////////////////////////////////////
// Evaluate the outgoing radiance from the environment
///////////////////////////////////////////////////////////////////////////
float3 Pathtracer::Lenvironment(const ray &r)
{
	if(r.d.y > 0.0)
		return make_vector(0.5f, 0.6f, 0.7f);
	else 
		return make_vector(0.05f, 0.025f, 0.001f);
}