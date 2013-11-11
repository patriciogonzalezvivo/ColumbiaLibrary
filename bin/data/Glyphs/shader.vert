float minDistance = 0.0;
float maxDistance = 100.0;
float maxPointSize = 1.0;

float map(float value, float inputMin, float inputMax, float outputMin, float outputMax) {;
	return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
}

const float epsilon = 1e-6;
void main(void){
	vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * eyeCoord;

	float dist = length(eyeCoord.xyz);
	float att  = 500.0 / dist;
	gl_FrontColor = gl_Color * clamp(map(gl_Position.z, minDistance, maxDistance, 1.0, 0.0), 0.0, 1.0);
}