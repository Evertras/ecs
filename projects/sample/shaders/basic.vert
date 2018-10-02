#version 330

layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexCoord;

out vec2 fragTexCoord;

uniform vec4 uTextureRect;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(inPosition, 1.0);

	fragTexCoord = vec2(
		mix(uTextureRect.x, uTextureRect.x + uTextureRect.z, inTexCoord.x),
		mix(uTextureRect.y, uTextureRect.y + uTextureRect.w, inTexCoord.y)
	);
}
