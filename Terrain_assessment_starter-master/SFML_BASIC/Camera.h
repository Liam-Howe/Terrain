#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glu.h" 



class Camera{
    static aiVector3D zero,yaxis,zaxis;
public:
    aiVector3D position;
    aiVector3D forward;
    aiVector3D up;
	aiVector3D xAxis;
	aiVector3D lookat;
    float forwardSpeed;
    float roationSpeed;
     
    Camera():forwardSpeed(0.5f),roationSpeed(0.1f){}
 
    void Init(aiVector3D& p=zero, aiVector3D& f=zaxis, aiVector3D& u=yaxis){
        position=p;
        forward=f;
        up=u;
		xAxis = aiVector3D(1.0f, 0.0f, 0.0f);
    }
 
	void Update(sf::Event e){//respond to keyboard events
		if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::D)){
                MoveLeftRight(-1);
            }

			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::A)){
                MoveLeftRight(+1);
            }
 
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::W)){
                MoveForwardBack(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::S)){
                MoveForwardBack(-1);
            }

			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Q)){
                MoveUpDown(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::E)){
                MoveUpDown(-1);
            }
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::O)){
				OrthographicProjection();
			}
			if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::I)){
				perspectiveProjection();
			}

            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Right)){
                TurnRightLeft(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Left)){
                TurnRightLeft(-1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Up)){
               TurnUpDown(1);
            }
            if ((e.type == sf::Event::KeyPressed) && (e.key.code == sf::Keyboard::Down)){
                TurnUpDown(-1);
 
            }
	
	}
    void MoveLeftRight(int dir){ //Dir=+1=>Right, dir=-1=> Left
            //TODO
		position += (xAxis*(forwardSpeed*dir));
	
    }

	void MoveUpDown(int dir){ //Dir=+1=>Right, dir=-1=> Left
		position += (up*(forwardSpeed*dir));
		
    }
 
    void MoveForwardBack(int dir){ //Dir=+1=>Forward, dir=-1=> Back
 
        position+=(forward*(forwardSpeed*dir));
		
    }

	void OrthographicProjection(){ //Dir=+1=>Forward, dir=-1=> Back

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//set up a 3D Perspective View volume
		//gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar 

		//set up a  orthographic projection same size as window
		//this means the vertex coordinates are in pixel space
		//glOrtho(0, 800, 0, 600, 0, 1); // use pixel coordinates

		glOrtho(0, 800, 0, 600,0, 300);
	}
	void perspectiveProjection(){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90.f, (float)800 / 600, 1.f, 300.0f);//fov, aspect, zNear, zFar
	}
 
    void TurnRightLeft(int dir){ //Dir=+1=>Right, dir=-1=> Left
   
		forward.z = forward.z*cos(roationSpeed*dir) - forward.x*sin(roationSpeed*dir);
		forward.x = forward.z*sin(roationSpeed*dir) + forward.x*cos(roationSpeed*dir);
    }
         
    void TurnUpDown(int dir){ //Dir=+1=>Up, dir=-1=> Down
		forward.y = forward.y*cos(roationSpeed*dir) - forward.z*sin(roationSpeed*dir);
		forward.z = forward.y*sin(roationSpeed*dir) + forward.z*cos(roationSpeed*dir);
    }


 
    void ViewingTransform(){
        gluLookAt(	position.x,position.y,position.z,// camera position
				  forward.x + position.x , forward.y + position.y, forward.z + position.z, //look at this point //TODO: BUG here!! what is it ??
					0,1,0); //camera up
    }
 
};

//create some default vectors
aiVector3D Camera::zero(0.0f);
aiVector3D Camera::yaxis(0.0f,1.0f,0.0f);
aiVector3D Camera::zaxis(0.0f,0.0f,1.0f);
