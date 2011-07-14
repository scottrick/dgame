varying vec3 normal;
varying float intensity;
	
void main()
{
	if (gl_Color.r > 0.0) {
		float maxIntensity			= gl_Color.r * 100.0;
		float xSquaredPlusYSquared	= gl_Vertex.x * gl_Vertex.x + gl_Vertex.y * gl_Vertex.y;
		float zSquared				= gl_Vertex.z * gl_Vertex.z;

		float fThing	= 1.0 - (xSquaredPlusYSquared + zSquared) / (maxIntensity * maxIntensity);
		if (fThing > 0.2) {
			fThing = 0.2;
		}

		intensity = xSquaredPlusYSquared / (xSquaredPlusYSquared + zSquared);
		intensity = intensity * fThing;
	}
	else
	{
		intensity = 0.0;
	}

	normal		= gl_NormalMatrix * gl_Normal;
	gl_Position	= ftransform();
}