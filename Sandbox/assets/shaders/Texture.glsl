// Basic Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_TilingFactor;
layout(location = 4) in float a_TexIndex;


uniform mat4 _ViewProjection;
uniform mat4 _Transform;

out vec2 v_Texcoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
	v_Color 		= a_Color;
	v_Texcoord		= a_Texcoord;
	v_TexIndex 		= a_TexIndex;
	v_TilingFactor 	= a_TilingFactor;
	gl_Position 	= _ViewProjection * vec4(a_Position, 1.0);	
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_Texcoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	color = texture(u_Textures[int(v_TexIndex)], v_Texcoord * v_TilingFactor) * v_Color;
	//color = vec4(v_Texcoord, 0.0, 1.0);
}