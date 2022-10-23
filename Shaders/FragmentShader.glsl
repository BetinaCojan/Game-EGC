#version 330


in vec2 textures;
uniform float Time;

void main()
{
	float noise = Time;
	out_color = vec4(vec3(textures * (2. - 1. * noise), 0.0), 1);
}
