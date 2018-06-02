#version 330 core
in vec3 ourColor;
in vec2 vTexCoord;

out vec4 color;

uniform sampler2D texture1;

void main()
{
    //color = vec4(ourColor, 1.0f);                   // colored square
    color = texture(texture1, vTexCoord);        // square shape with texture
    //color = texture(texture1, vTexCoord) * vec4(ourColor, 1.0f);     // mixing colors and texture (multiply)

}
