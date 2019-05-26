#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
	vec4 img = texture(image, TexCoords);
	if (img.w < 0.2)
		discard;
	color = vec4(spriteColor, 1.0) * img;
}