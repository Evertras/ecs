#version 330

out vec4 outColor;

uniform vec4 rectColor;

void main()
{
	outColor = rectColor;
	outColor = vec4(1.0, 0.0, 0.0, 1.0);
}
