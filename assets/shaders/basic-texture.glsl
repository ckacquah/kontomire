// *************************************
// ****         Kontomire 2D        ****
// ****     Basic Texture Shader    ****
// *************************************

#type vertex
#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinates;

out vec2 v_texture_coordinates;

uniform mat4 model;
uniform mat4 view_projection;

void main()
{
   v_texture_coordinates = texture_coordinates;
   gl_Position = view_projection * model * vec4(position, 1.0);
}

#type fragment
#version 420 core

in vec2 v_texture_coordinates;

out vec4 output_color;

uniform vec4 color;
uniform sampler2D texture0;

void main()
{
   output_color = texture(texture0, v_texture_coordinates) * color;
};