#version 410

in vec4 Color;
in vec2 UV;
out vec4 fragColor;

uniform bool useTexture = true;
uniform sampler2D myTexture;

int count = 0;

void main()
{
	if (useTexture)
	{
		//fragColor = vec4(0, 1, 0, 1);
		fragColor = texture(myTexture, UV);
		count++;
	}
	else
	{
		fragColor = Color;
	}
	
}