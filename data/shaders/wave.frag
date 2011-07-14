varying vec2		texCoordinate;

uniform sampler2D	sceneTexture;
uniform sampler2D	waveTexture;

void main() 
{
	vec4 offset		= texture2D(waveTexture, texCoordinate);
	vec4 combined	= vec4(texture2D(sceneTexture, texCoordinate.st - (offset.st - 0.5) * offset.w));
	//vec3 combined	= vec3(texture2D(sceneTexture, texCoordinate) + texture2D(waveTexture, texCoordinate));

	gl_FragColor	= vec4(combined.rgb, 1.0);
}