uniform vec3 lightDir;
varying vec3 normal;

void main() 
{
	float intensity;
	vec4 color;
	
	intensity = dot(normalize(lightDir), normalize(normal));
	
	if (intensity > 0.99) 
		color = vec4(1.0, 0.9, 0.9, 1.0);
	else if (intensity > 0.9)
		color = vec4(0.9, 0.5, 0.5, 1.0);
	else if (intensity > 0.75)
		color = vec4(0.65, 0.4, 0.4, 1.0);
	else if (intensity > 0.45)
		color = vec4(0.35, 0.15, 0.15, 1.0);
	else
		color = vec4(0.15, 0.05, 0.05, 1.0);
	
	gl_FragColor = color;
}