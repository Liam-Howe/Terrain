#include "stdafx.h"
#include "SFML/OpenGL.hpp"
#include "Terrain.h"
#include <cmath>
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"


Terrain::Terrain(void)
{
	gridWidth=100;
	gridDepth=100;

	terrWidth=50; //size of terrain in world units
	terrDepth=50;
	vertices=NULL;
	colors=NULL;	
	normals = NULL;
	textureCoOrdinates = NULL;
	
	
	//num squares in grid will be width*height, two triangles per square
	//3 verts per triangle
	 numVerts=gridDepth*gridWidth*2*3;


}


Terrain::~Terrain(void)
{
	delete [] vertices;
	delete [] colors;
	delete[] textureCoOrdinates;
}

//interpolate between two values
float lerp(float start, float end, float t){
	return start+(end-start)*t;
}


void Terrain::setPoint(vector v,float x, float y, float z){

		v[0]=x;
		v[1]=y;
		v[2]=z;
}

//helper function to calculate height of terrain at a given point in space
//you will need to modify this significantly to pull height from a map
float  Terrain::getHeight(float x, float y){

	//for the sample we will calculate height based on distance form origin
	float dist=sqrt(x*x+y*y);

	//center will be the highest point
	dist=30-dist;
	//put a nice curve in it
	dist*=dist;
	dist*=dist;
	//whoah, way to high, make it smaller
	dist/=50000;

	return dist;
}

void Terrain::Init(){
	
	delete [] vertices;//just in case we've called init before
	vertices=new vector[numVerts];
	delete[] normals;
	delete [] colors;
	colors=new vector[numVerts];
	normals = new vector[numVerts];
	delete[] textureCoOrdinates;
	textureCoOrdinates = new vector[numVerts];

	GLfloat * a, *b, *c;
	GLfloat normal[3];

	GLfloat materialAmbDiff[] = { 0.9f, 0.5f, 0.3f, 1.0f }; // create an array of RGBA values



	//interpolate along the edges to generate interior points
	for(int i=0;i<gridWidth-1;i++){ //iterate left to right
		for(int j=0;j<gridDepth-1;j++){//iterate front to back
			int sqNum=(j+i*gridDepth);
			int vertexNum=sqNum*3*2; //6 vertices per square (2 tris)
			float front=lerp(-terrDepth/2,terrDepth/2,(float)j/gridDepth);
			float back =lerp(-terrDepth/2,terrDepth/2,(float)(j+1)/gridDepth);
			float left=lerp(-terrWidth/2,terrWidth/2,(float)i/gridDepth);
			float right=lerp(-terrDepth/2,terrDepth/2,(float)(i+1)/gridDepth);
			
			/*
			back 0,0  +-----+1,0	looking from above, the grid is made up of regular squares
			       |tri1/|	'left & 'right' are the x cooded of the edges of the square
				   |   / |	'back' & 'front' are the y coords of the square
				   |  /  |	each square is made of two trianlges (1 & 2)
				   | /   |	
				   |/tri2|
			front 0,1+-----+1,1
			     left   right
				 */

			//tri 1
			a = vertices[vertexNum];
			setPoint(textureCoOrdinates[vertexNum], 0, 1, 0);
			setPoint(vertices[vertexNum++], left, getHeight(left, front), front);

			b = vertices[vertexNum];
			setPoint(textureCoOrdinates[vertexNum], 1, 1, 0);
			setPoint(vertices[vertexNum++], right, getHeight(right, front), front);
			
			c = vertices[vertexNum];
			setPoint(textureCoOrdinates[vertexNum], 1, 0, 0);
			setPoint(vertices[vertexNum++], right, getHeight(right, back), back);

			NormalVector(a,b,c, normal);
			AddNormal(vertexNum, normal);
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//tri 2
			a = vertices[vertexNum];
			setPoint(textureCoOrdinates[vertexNum], 0, 1, 0);
			setPoint(vertices[vertexNum++], left, getHeight(left, front), front);

			b = vertices[vertexNum];
			setPoint(textureCoOrdinates[vertexNum], 1, 0, 0);
			setPoint(vertices[vertexNum++], right, getHeight(right, back), back);
			
			c = vertices[vertexNum];
			setPoint(textureCoOrdinates[vertexNum], 0, 0, 0);
			setPoint(vertices[vertexNum++], left, getHeight(left, back), back);
			NormalVector(a, b, c, normal);
			AddNormal(vertexNum, normal);

		}
	}
}

void Terrain::AddNormal(int lastIndex, GLfloat n[3])
{
	for (int i = 0; i < 3; i++)
	{
		setPoint(normals[lastIndex - i], n[0], n[1], n[2]);
	}
}

void Terrain::Draw( ){

	glBegin(GL_TRIANGLES);
	for(int i =0;i<numVerts;i++){
			//glColor3fv(colors[i]);
			glTexCoord2fv(textureCoOrdinates[i]);
			glVertex3fv(vertices[i]);
			glNormal3fv(normals[i]);
	}
	glEnd();
}
void Terrain::NormalVector(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat n[3])
{

	GLfloat v1[3], v2[3]; // two vectors

	//calculate two vectors lying on the surface
	// v1=p1-p2
	// v2=p3-p2

	for (int i = 0; i<3; i++){
		v1[i] = p2[i] - p1[i];
		v2[i] = p3[i] - p2[i];
	}

	// calculate cross product of two vectors
	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	n[2] = v1[0] * v2[1] - v2[0] * v1[1];

} //done