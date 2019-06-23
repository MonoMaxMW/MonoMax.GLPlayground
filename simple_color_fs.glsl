#version 330 core
in vec4 passColor;
out vec4 fragmentColor;
void main()
{
    fragmentColor = passColor;
}