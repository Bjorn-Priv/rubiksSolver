# rubiksSolver
as the name says


shader files naming conventions: 
https://stackoverflow.com/questions/6432838/what-is-the-correct-file-extension-for-glsl-shaders


//shader code
const char* vertexShaderSrc = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core

out vec4 FragColor;

void main()
{
  FragColor = vec4(0.8, 0.2, 0.2, 1.0);
}
)";
