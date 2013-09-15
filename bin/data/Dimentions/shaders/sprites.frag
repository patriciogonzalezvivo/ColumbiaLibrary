uniform sampler2D tex;

uniform float 	nearClip;
uniform float 	farClip;

float linearizeDepth( in float d ) {
    return (2.0 * nearClip) / (farClip + nearClip - d * (farClip - nearClip));
}

void main(){
	float depthVal = 1. - pow( linearizeDepth( gl_FragCoord.z ), 2.);
	gl_FragColor = ( texture2D(tex, gl_TexCoord[0].st) * gl_Color ) * depthVal;
}