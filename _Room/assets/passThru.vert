uniform mat4    ciModelViewProjection;

in vec4 ciPosition;
in vec2 ciTexCoord0;

out vec4 vVertex;
out vec4 vPosition;
out vec2 vTexCoord0;

void main()
{
	vVertex		= ciPosition;
	vTexCoord0	= ciTexCoord0;
	vPosition	= ciModelViewProjection * ciPosition;
}