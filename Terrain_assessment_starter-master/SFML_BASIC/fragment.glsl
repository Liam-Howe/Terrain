uniform sampler2D rock;
uniform sampler2D sea;
uniform sampler2D grass;
varying float height;
void main()
{
	vec4 rockColour = texture2D(rock, gl_TexCoord[0].st);
	vec4 seaColour = texture2D(sea, gl_TexCoord[0].st);
	vec4 grassColour = texture2D(grass, gl_TexCoord[0].st);
	
	if(height > 5.5)
	{
		gl_FragColor = rockColour;
	}
	 if( height > 1 && height < 5.5)
	{
		gl_FragColor = grassColour;
	}
	 if(height >0 && height <1)
	{
		gl_FragColor = seaColour;
	}
	//gl_FragColor = gl_Color;
}