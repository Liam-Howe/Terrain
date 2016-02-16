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

	if (height < 0.05f){ //water texture
		gl_FragColor = seaColour;
	}
	if (height > 0.05f && height < 0.1f){ //mix of water and grass
		float blendingHeight = height - 0.05f;
		float grassStrength = blendingHeight / 0.05f;
		gl_FragColor = mix(seaColour, grassColour, grassStrength);
	}
	if (height > 0.1f && height < 0.6f){
		gl_FragColor = grassColour;
	}
	if (height > 0.6f && height < 0.75f){
		float blendingHeight = height - 0.6f;
		float rockStrength = blendingHeight / 0.15f;
		gl_FragColor = mix(grassColour, rockColour, rockStrength);
	}
	if (height > 0.75f){
		gl_FragColor = rockColour;
	}
	gl_FragColor = gl_FragColor * gl_Color;
}