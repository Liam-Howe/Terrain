//////////////////////////////////////////////////////////// 
// Headers 
//////////////////////////////////////////////////////////// 
#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 
 
#include "SFML/Graphics.hpp" 
#include "SFML/Graphics/Shader.hpp"
#include "SFML/OpenGL.hpp" 
#include <iostream> 
  
 
#include "Terrain.h"
#include "Camera.h"




int main() 
{ 
    // Create the main window 
	// Create the main window
	//sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML First Program");
	sf::Texture rock, sea, grass;
	rock.loadFromFile("rock.jpg");
	sea.loadFromFile("water.png");
	grass.loadFromFile("grass.png");
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	int width = 800, height = 600;
	sf::RenderWindow App(sf::VideoMode(width, height, 32), "SFML OpenGL", sf::Style::Close, Settings);
 
	bool debug_mode = false;
	
    // Create a clock for measuring time elapsed     
    sf::Clock Clock; 

	aiVector3D position(0,10,-30);
	Camera camera;
    camera.Init(position); //create a camera
      
    //prepare OpenGL surface for HSR 
    glClearDepth(1.f); 
    glClearColor(0.3f, 0.3f, 0.6f, 0.f); //background colour
    glEnable(GL_DEPTH_TEST); 
	glEnable(GL_NORMALIZE);
    glDepthMask(GL_TRUE); 
	glEnable(GL_TEXTURE_2D);
    //// Setup a perspective projection & Camera position 
    glMatrixMode(GL_PROJECTION);
	
    glLoadIdentity(); 
	glEnable(GL_LIGHTING); // switch
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    //set up a 3D Perspective View volume
    gluPerspective(90.f, (float)width/height, 1.f, 300.0f);//fov, aspect, zNear, zFar 
	
	GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color); // set color of diffuse component
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color); // set color of specular component
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   // set position
	//GLfloat materialAmbDiff[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialAmbDiff);

	//GLfloat light_color2[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat light_position2[] = { 0.0, 0.0, 0.0, 0.0 };
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color2); // set color of diffuse component
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light_color2); // set color of specular component
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position2);   // set position

	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create an array of RGBA values (White)
	GLfloat materialShininess[] = { 128.0f }; // select value between 0-128, 128=shiniest
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); // set the colour of specular reflection
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess); // set shininess of the
	//load & bind the shader
	sf::Shader shader;
	//all the lighting & texture blending code should  be put in 'fragment.glsl'
	if(!shader.loadFromFile("vertex.glsl","fragment.glsl")){
        exit(1);
    }
	sf::Shader::bind(&shader);

	//Create our Terrain
	Terrain terrain;
	terrain.Init();

    // Start game loop 
    while (App.isOpen()) 
    { 
        // Process events 
        sf::Event Event; 
        while (App.pollEvent(Event)) 
        { 
            // Close window : exit 
            if (Event.type == sf::Event::Closed) 
                App.close(); 
   
            // Escape key : exit 
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) 
                App.close(); 
             
			//update the camera
			camera.Update(Event);
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::I))
			{
				debug_mode = !debug_mode;
			}
        } 
	
		
        //Prepare for drawing 
        // Clear color and depth buffer 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
		shader.setParameter("rock", rock);
		shader.setParameter("sea", sea);
		shader.setParameter("grass", grass);

        // Apply some transformations 
        //initialise the worldview matrix
		glMatrixMode(GL_MODELVIEW); 
        glLoadIdentity(); 

		//get the viewing transform from the camera
		camera.ViewingTransform();
		if (debug_mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw only lines no fill
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // normal mode
		}
		//make the world spin
		//TODO:probably should remove this in final
		static float ang=0.0;
		ang+=0.01f;
		glRotatef(ang*2,0,1,0);//spin about y-axis
		
		sf::Shader::bind(&shader);
		
		//draw the world
		terrain.Draw();

		   
        // Finally, display rendered frame on screen 
        App.display(); 
    } 
   
    return EXIT_SUCCESS; 

	
}
