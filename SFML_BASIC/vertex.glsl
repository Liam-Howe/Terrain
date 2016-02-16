varying float height;
uniform float highest;
void main()
{

	vec3 normal, lightDir, viewVector;
	vec4 diffuse, ambient, globalAmbient, specular = vec4(0.0);
	float NdotL,NdotHV;
	height = gl_Vertex.y / highest;
	
	/* first transform the normal into eye space and normalize the result */
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	/* now normalize the light's direction. Note that according to the
	OpenGL specification, the light is stored in eye space. Also since 
	we're talking about a directional light, the position field is actually 
	direction */
	//lightDir = normalize(vec3(gl_LightSource[0].position));
	
	vec4 pointEyeSpace=(gl_ModelViewMatrix*gl_Vertex); 	
	lightDir = normalize(vec3(gl_LightSource[0].position)-pointEyeSpace.xyz);
	
	/* compute the cos of the angle between the normal and lights direction. 
	The light is directional so the direction is constant for every vertex.
	Since these two are normalized the cosine is the dot product. We also 
	need to clamp the result to the [0,1] range. */
	
	NdotL = max(dot(normal, lightDir), 0.0);

	vec4 cameraDir=-pointEyeSpace;
	vec3 halfVector=normalize(lightDir + cameraDir.xyz);

	/* Compute the diffuse, ambient and globalAmbient terms */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;
	
	/* compute the specular term if NdotL is  larger than zero */
	if (NdotL > 0.0) {

		NdotHV = max(dot(normal, halfVector.xyz),0.0);		
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}
	
	gl_FrontColor = globalAmbient + NdotL * diffuse + ambient + specular;
	
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}