#version 330 core

uniform sampler2D pic;

in vec2 FragUV;
out vec4 FragColor;

void main() {
   vec3 picture = texture(pic, FragUV).rgb;
   FragColor = vec4(picture, 1.0f);
   //FragColor = vec4(1.0f);
}
