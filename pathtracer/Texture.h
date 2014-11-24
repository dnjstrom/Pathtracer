#pragma once
#include <iostream>
#include "float3.h"
#include "float2.h"

class Texture
{
public:
	Texture(void);
	~Texture(void);
	chag::float3 *m_image; 
	int m_width, m_height; 
	void load(std::string filename); 
};

