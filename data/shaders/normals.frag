varying vec3 normal;
varying float intensity;

void main() 
{
	gl_FragColor = vec4(normalize(normal) / 2.0 + 0.5, intensity);
}