#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 vertexColors;
layout(location=2) in vec2 texCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Perspective;

out vec3 v_vertexColors;
out vec2 v_texCoords; 

void main()
{
	v_vertexColors = vertexColors;
	vec4 newPosition = u_Perspective * u_ViewMatrix * u_ModelMatrix * vec4(position,1.0f);
	gl_Position = vec4(newPosition.x, newPosition.y, newPosition.z, newPosition.w);
	v_texCoords = texCoords;
}