#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;
out vec3 fragPos;
out vec2 vTexCoords;

uniform int ct=1;	// multiplication coefficient for texture (for repeating the texture)

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    fragPos = vec3(model * vec4(position, 1.0f));
    //vNormal = normal;
    vNormal = mat3(transpose(inverse(model))) * normal;
    vTexCoords = ct*texCoords;
}
