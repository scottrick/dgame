varying float	lightIntensity;
uniform vec3	lightDir;

const float specularContribution	= 0.1;
const float diffuseContribution		= 1.0 - specularContribution;

void main()
{
	vec3	ecPosition		= vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3	tnorm			= normalize(gl_NormalMatrix * gl_Normal);
	vec3	reflectVec		= reflect(-lightDir, tnorm);
	vec3	viewVec			= normalize(-ecPosition);
	
	float	spec			= clamp(dot(reflectVec, viewVec), 0.0, 1.0);
	spec					= pow(spec, 16.0);
	
	lightIntensity			= diffuseContribution * max(dot(lightDir, tnorm), 0.0) + specularContribution * spec;
	
	gl_TexCoord[0]	= gl_MultiTexCoord0;
	gl_Position		= ftransform();
}