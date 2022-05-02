// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;

uniform mat4 _ViewProjection;
uniform mat4 _Transform;

out vec2 v_Texcoord;

void main()
{
	v_Texcoord	= a_Texcoord;
	gl_Position = _ViewProjection * _Transform * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Texcoord;

uniform sampler2D u_Texture;
uniform float u_TilingFactor;
uniform vec4 u_Color;

void main()
{
	color = texture(u_Texture, v_Texcoord * u_TilingFactor) * u_Color;
}