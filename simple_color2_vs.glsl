#version 330 core

in vec3 aPos;
out vec4 passColor;
uniform mat4 uModlMat, uViewMat, uProjMat;

void main()
{
    gl_Position = projMat * viewMat * modlMat * vec4(aPos, 1.0);
    passColor = vec4(1.0, 0.0, 0.0, 1.0);
}