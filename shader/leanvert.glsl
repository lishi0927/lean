#version 450 core

layout(location = 0) in vec3 Position_VS_in;
layout(location = 1) in vec2 TexCoord_VS_in;

uniform mat4 gWorldMatrix;
uniform mat4 gVPMatrix;

out vec2 TexCoord_FS_in;
out vec3 WorldPos;

void main()
{
  WorldPos = gWorldMatrix * vec4(Position_VS_in, 1.0).xyz;
  gl_Position = gVPMatrix * vec4(WorldPos, 1.0);

  TexCoord_FS_in = TexCoord_VS_in;
}