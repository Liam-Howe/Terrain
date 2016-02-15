uniform sampler2D rock;
uniform sampler2D sea;
uniform sampler2D grass;
varying float height;

void main()
{

	vec4 rockColour = texture2D(rock, gl_TexCoord[0].st);
	vec4 seaColour = texture2D(sea, gl_TexCoord[0].st);
	vec4 grassColour = texture2D(grass, gl_TexCoord[0].st);
	vec4 tempColour;

	if (height < 0.5f){ //water texture
		gl_FragColor = seaColour;
	}
	if (height > 0.5f && height < 1f){ //mix of water and grass
		float grassStrength = height / 1.f;
		gl_FragColor = mix(seaColour, grassColour, grassStrength);
	}
	if (height > 1f){
		float rockStrength = height / 12f;
		gl_FragColor = mix(grassColour, rockColour, rockStrength);
	}
	
	gl_FragColor = gl_FragColor * gl_Color;
}