// **********************************
// ****       Kontomire 2D       ****
// **** Renderer2D Circle Shader ****
// **********************************

#type vertex
#version 420 core

layout(location = 0) in vec4 a_Color;
layout(location = 1) in vec3 a_WorldPosition;
layout(location = 2) in vec3 a_LocalPosition;

layout(location = 3) in float a_Fade;
layout(location = 4) in float a_Thickness;

uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec3 LocalPosition;

	float Fade;
	float Thickness;
};

layout (location = 0) out VertexOutput Output;

void main()
{
	Output.Fade = a_Fade;
	Output.Thickness = a_Thickness;

	Output.Color = a_Color;
	Output.LocalPosition = a_LocalPosition;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 420 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec3 LocalPosition;
	
	float Fade;
	float Thickness;
};

layout (location = 0) in VertexOutput Input;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(Input.LocalPosition);
	float alpha = smoothstep(0.0, Input.Fade, distance);
	alpha *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (alpha <= 0)
		discard;

    // Set output color
    o_Color = Input.Color;
	o_Color.a *= alpha;
}