#version 330
 
 struct Material{
 	sampler2D texture_diffuse1;
 };


uniform Material material;
 
in vec2 tex_coord;
in vec4 out_color;
out vec4 vFragColor;
 
void main() 
{
	vFragColor = texture(material.texture_diffuse1,tex_coord);
}