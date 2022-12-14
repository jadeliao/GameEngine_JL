#version 450
#extension GL_ARB_separate_shader_objects : enable

uniform sampler2D myTexture;

layout(location = 0) in vec2 texCoord; 

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = texture(myTexture,texCoord);
}