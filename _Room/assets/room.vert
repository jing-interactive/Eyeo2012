uniform vec3 eyePos;
uniform mat4 ciModelViewProjection;
uniform mat4 ciModelMatrix;

in vec4 ciPosition;
in vec3 ciNormal;
in vec2 ciTexCoord0;

out vec3 vEyeDir;
out vec4 vVertex;
out vec3 vNormal;
out vec2 vTexCoord0;

void main()
{
	vVertex			= ciModelMatrix * ciPosition;
	vNormal			= ciNormal;//normalize( vec3( mMatrix * vec4( gl_Normal, 0.0 ) ) );
	
	vEyeDir			= normalize( eyePos - vVertex.xyz );
	
	gl_Position		= ciModelViewProjection * ciPosition;
	vTexCoord0	= ciTexCoord0;
}


