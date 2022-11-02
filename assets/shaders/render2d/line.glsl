// **********************************
// ****       Kontomire 2D       ****
// **** Renderer2D Circle Shader ****
// **********************************

#type vertex
#version 420 core

layout(location = 0) in vec4 a_Color;
layout(location = 1) in vec3 a_Position;

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) out VertexOutput Output;

uniform mat4 u_ViewProjection;

void main()
{
	Output.Color = a_Color;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) in VertexOutput Input;

void main()
{
	o_Color = Input.Color;
}