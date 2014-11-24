#include "Texture.h"
#include "FreeImage.h"
using namespace std; 
using namespace chag; 
#include <string>

Texture::Texture(void)
{
	m_image = NULL; 
}


Texture::~Texture(void)
{
}

void Texture::load(string filename)
{
	FIBITMAP *bitmap = FreeImage_Load(FIF_PNG, filename.c_str()); 
	if(!bitmap) {
		cout << "Failed to load texture " << filename << endl; 
	}
	FIBITMAP *rgbabitmap = FreeImage_ConvertTo32Bits(bitmap); 
	FreeImage_Unload(bitmap); 
	m_width = FreeImage_GetWidth(rgbabitmap); 
	m_height = FreeImage_GetHeight(rgbabitmap); 
	int scan_width = FreeImage_GetPitch(rgbabitmap); 
	BYTE *data = new BYTE[m_height*scan_width]; 
	if(m_image != NULL) delete [] m_image; 
	m_image = new float3[m_width * m_height];
	int bytespp = FreeImage_GetLine(rgbabitmap) / FreeImage_GetWidth(rgbabitmap);
	for(int y=0; y<m_height; y++){
		BYTE *bits = FreeImage_GetScanLine(rgbabitmap, y);
		for(int x=0; x<m_width; x++){
			m_image[y*m_width+x].x = float(bits[FI_RGBA_RED])/255.0f; 
			m_image[y*m_width+x].y = float(bits[FI_RGBA_GREEN])/255.0f; 
			m_image[y*m_width+x].z = float(bits[FI_RGBA_BLUE])/255.0f; 
			bits += bytespp;
		}
	}
	FreeImage_Unload(rgbabitmap); 
}

