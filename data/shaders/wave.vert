varying vec2 texCoordinate;

void main() 
{
	texCoordinate		= gl_MultiTexCoord0.st;
	gl_Position			= ftransform();
}
