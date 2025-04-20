#version 330 core
in vec3 col;
in vec2 tex;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, tex);
}
