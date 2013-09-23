#version 120

uniform float 	nearClip;
uniform float 	farClip;

uniform float	focusDistance;
uniform float	focusAperture;

uniform float 	size;
uniform float 	maxPointSize;
uniform float 	minPointSize;

float PI = 3.14159265359;
float HALF_PI = 1.57079632679;

void main(){
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	float attenuation = ((farClip-nearClip)/2.)/distance(vec3(0),gl_Position.xyz);
	gl_PointSize = max( minPointSize, min( maxPointSize, size * attenuation ) );

	gl_FrontColor = gl_Color;
	// float radius = (1.0-(gl_PointSize/maxPointSize));
	float radius = min( 1.0 + abs(gl_Position.z - focusDistance) * focusAperture, maxPointSize)*0.5;
	// radius *= focus;
	gl_FrontColor.a /= PI * radius * radius;
}
