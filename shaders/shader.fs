#version 330 core
out vec4 FragColor;

in vec3 posVertex;

void main()
{
    FragColor = vec4(posVertex, 1.0);
}