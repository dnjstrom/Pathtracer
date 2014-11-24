#version 130

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
precision highp float;

uniform sampler2D framebuffer; 
uniform int framebufferSamples; 

in vec2 texCoord; 
out vec4 fragmentColor;

void main() 
{
	fragmentColor = (1.0 / float(framebufferSamples)) * texture2D(framebuffer, texCoord);
}