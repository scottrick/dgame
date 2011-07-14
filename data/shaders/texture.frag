varying float		lightIntensity;
uniform sampler2D	texture;

void main() 
{
	vec3 lightColor = vec3(texture2D(texture, gl_TexCoord[0].st));
	gl_FragColor	= vec4(lightColor * lightIntensity, 1.0);
}