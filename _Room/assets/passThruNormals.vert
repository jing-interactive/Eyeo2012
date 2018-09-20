uniform mat4    ciModelViewProjection;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

out vec4 vVertex;
out vec3 vNormal;
out vec4 vPosition;
out vec2 vTexCoord0;

void main()
{
	vVertex		= ciPosition;
	vNormal		= ciNormal;
	vTexCoord0	= ciTexCoord0;
	vPosition	= ciModelViewProjection * ciPosition;
}