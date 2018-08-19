#version 450
//#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inOffset;
layout(location = 2) in vec3 inOffsetColor;

layout (location = 0) out vec3 fragColor;

void main()
{	
    gl_Position =  proj * view * model * vec4(inPosition + inOffset, 1.0);

	fragColor = inOffsetColor;
}
