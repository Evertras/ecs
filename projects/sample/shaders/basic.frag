#version 330

in vec2 fragTexCoord;
out vec4 outColor;

uniform sampler2D image;
//uniform vec3 spriteColor;

void main()
{
	//outColor = vec4(spriteColor, 1.0) * texture(image, TexCoords);
	outColor = texture(image, fragTexCoord);
	//outColor = vec4(0.0, 0.0, 0.0, 1.0);
}
