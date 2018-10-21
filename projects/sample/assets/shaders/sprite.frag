#version 330

in vec2 fragTexCoord;
out vec4 outColor;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{
	outColor = spriteColor * texture(image, fragTexCoord);
}
