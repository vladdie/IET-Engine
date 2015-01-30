#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord; 
layout (location = 3) in vec4 color; 

vec4 transform(vec4 position); 
 
uniform mat4 mvp; 

out vec2 TexCoords;
out vec4 outColor;

void main() 
{  
	gl_PointSize = 10.0;

	gl_Position = transform( vec4(position, 1.0) );
	outColor = color; 
	TexCoords = texCoord;
}