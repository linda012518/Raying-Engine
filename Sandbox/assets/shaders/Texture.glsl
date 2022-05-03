// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec4 a_Color;

uniform mat4 _ViewProjection;
uniform mat4 _Transform;

out vec2 v_Texcoord;
out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	v_Texcoord	= a_Texcoord;
	gl_Position = _ViewProjection * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Texcoord;
in vec4 v_Color;

uniform sampler2D u_Texture;
uniform float u_TilingFactor;
uniform vec4 u_Color;

void main()
{
	//color = texture(u_Texture, v_Texcoord * u_TilingFactor) * u_Color;
	color = v_Color;
}