#version 430 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform bool first;


void main()
{
    if(!first)
	    FragColor = texture(tex0, texCoord);
	else
	    FragColor = texture(tex1, texCoord);
}