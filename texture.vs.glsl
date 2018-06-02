#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 vTexCoord;		// texture coordinate

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    ourColor = color;
    //vTexCoord = texCoord;		// texture image mapped upside down
    vTexCoord = vec2(texCoord.x, 1.0-texCoord.y);			// coordinate system difference, y is up/down -- flip y-coordinate
    //vTexCoord = vec2(1.0-texCoord.x, 1.0-texCoord.y);		// flipping the x-coordinate as well
    
}
