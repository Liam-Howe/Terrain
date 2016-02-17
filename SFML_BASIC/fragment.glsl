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
		tempColour = seaColour;
	}
	if (height > 0.05f && height < 0.1f){ //mix of water and grass
		float blendingHeight = height - 0.05f;
		float grassStrength = blendingHeight / 0.05f;
		tempColour = mix(seaColour, grassColour, grassStrength);
	}
	if (height > 0.1f && height < 0.6f){
		tempColour = grassColour;
	}
	if (height > 0.6f && height < 0.75f){
		float blendingHeight = height - 0.6f;
		float rockStrength = blendingHeight / 0.15f;
		tempColour = mix(grassColour, rockColour, rockStrength);
	}
	if (height > 0.75f){
		tempColour = rockColour;
	}
	gl_FragColor = tempColour * gl_Color;
}