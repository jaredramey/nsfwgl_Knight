#version 410
//input is points but output is quad
layout(points) in;
layout (triangle_strip, max_vertices = 4) out;

//input data from vertex shader
in vec3 position[];
in float lifetime[];
in float lifespan[];

//output to fragment shader
out vec4 Color;
out vec2 UV;

uniform mat4 projectionView;
uniform mat4 cameraTransform;

uniform float sizeStart;
uniform float sizeEnd;

uniform vec4 colorStart;
uniform vec4 colorEnd;

void main()
{
	//LERP color
	Color = mix(colorStart, colorEnd, lifetime[0] / lifespan[0]);
	
	//calculate size and create corners of quad
	float halfSize = mix(sizeStart, sizeEnd, lifetime[0] / lifespan[0]) * 0.5f;
	
	vec3 corners[4];
	vec2 UVs[4];
	corners[0] = vec3(halfSize, -halfSize, 0);
	UVs[0] = vec2(1, 0);
	corners[1] = vec3(halfSize, halfSize, 0);
	UVs[1] = vec2(1, 1);
	corners[2] = vec3(-halfSize, -halfSize, 0);
	UVs[2] = vec2(0, 0);
	corners[3] = vec3(-halfSize, halfSize, 0);
	UVs[3] = vec2(0, 1);
	
	//billboard
	vec3 zAxis = normalize(cameraTransform[3].xyz - position[0]);
	vec3 xAxis = cross(cameraTransform[1].xyz, zAxis);
	vec3 yAxis = cross(zAxis, xAxis);
	mat3 billboard = mat3(xAxis, yAxis, zAxis);
	

	//emit the 4 vertices for the quad
	gl_Position = projectionView * vec4(billboard * corners[0] + position[0], 1);
	UV = UVs[0];
	EmitVertex();
	
	gl_Position = projectionView * vec4(billboard * corners[1] + position[0], 1);
	UV = UVs[1];
	EmitVertex();
	
	gl_Position = projectionView * vec4(billboard * corners[2] + position[0], 1);
	UV = UVs[2];
	EmitVertex();
	
	gl_Position = projectionView * vec4(billboard * corners[3] + position[0], 1);
	UV = UVs[3];
	EmitVertex();
}