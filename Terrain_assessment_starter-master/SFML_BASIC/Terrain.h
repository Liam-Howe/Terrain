#pragma once
class Terrain
{
	//size of the mesh forming the terrain
	int gridWidth,gridDepth;
	int numVerts;
	//size of the terrain in world_coords
	float terrWidth,terrDepth;

	typedef  GLfloat vector[3];
	//array of vertices for the grid(this will be a triangle list)
	//I know, very inefficient, but let's run before we walk
	vector *vertices;
	vector *colors;
	vector *textureCoOrdinates;
	vector *normals;
	
	float getHeight(float x, float y);
	void setPoint(vector, float, float,float);
	
public:
	Terrain(void);
	~Terrain(void);
	void AddNormal(int lastIndex, GLfloat n[3]);
	void NormalVector(GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat n[3]);

	void Init();
	void Draw();
};

