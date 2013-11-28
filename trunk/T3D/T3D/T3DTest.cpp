// =========================================================================================
// KXG363 - Advanced Games Programming, 2012
// =========================================================================================
//
// Author: Robert Ollington
//
// t3dtest.cpp
//
// Example t3dapplication
// Creates cube, sphere, camera and light
// Adds keyboard controller to camera game object
// Adds perflogtask

#include "t3dtest.h"
#include "Cube.h"
#include "PlaneMesh.h"
#include "Sphere.h"
#include "KeyboardController.h"
#include "TerrainFollower.h"
#include "RotateBehaviour.h"
#include "LookAtBehaviour.h"
#include "Terrain.h"
#include "Camera.h"
#include "PerfLogTask.h"
#include "Material.h"
#include "Animation.h"
#include "Billboard.h"
#include "Math.h"
#include "Sweep.h"
#include "SweepPath.h"

namespace T3D{

	T3DTest::T3DTest(void)
	{
	}


	T3DTest::~T3DTest(void)
	{
	}

	bool T3DTest::init(){
		WinGLApplication::init();
		root->name = "Root";
		//test scene

		renderer->loadSkybox("Resources/Sunny1");	

		renderer->setFog(0.007,0.8,0.8,0.8,1.0);
		renderer->toggleFog();

		Material *red = new Material(1,0,0,1);
		Material *green = new Material(0,1,0,1);
		green->setSpecular(0,0,0,0);
		Material *blue = new Material(0,0,1,1);

		Texture *smileytex = new Texture("Resources/Smiley.bmp");
		renderer->loadTexture(smileytex);
		Material *smiley = new Material(1,1,1,1);
		smiley->setTexture(smileytex);
				
		Texture *proctex = new Texture(512,512);
		proctex->createFractal(Colour(40,150,50,255),Colour(120,220,100,255),25.0f,false);

		renderer->loadTexture(proctex, true);		
		Material *procmat = new Material(1,1,1,1);
		procmat->setTexture(proctex,50);
		procmat->setSpecular(0,0,0,0);


		cout << "creating camera\n";
		GameObject *camObj = new GameObject(this);
		renderer->camera = new Camera();
		camObj->setCamera(renderer->camera);
		camObj->getTransform()->setLocalPosition(Vector3(0,0,20));
		camObj->getTransform()->setLocalRotation(Vector3(0,0,0));
		camObj->getTransform()->setParent(root);
		camObj->addComponent(new KeyboardController());

		cout << "creating light\n";
		GameObject *lightObj = new GameObject(this);
		Light *light = new Light(Light::DIRECTIONAL); 
		light->setAmbient(1,1,1);
		light->setDiffuse(1,1,1);
		light->setSpecular(1,1,1);
		lightObj->setLight(light);
		lightObj->getTransform()->setLocalRotation(Vector3(-45*Math::DEG2RAD,135*Math::DEG2RAD,0));
		lightObj->getTransform()->setParent(root);

		cout << "creating cube\n";
		GameObject *cube = new GameObject(this);
		cube->setMesh(new Cube(1));
		cube->setMaterial(smiley);
		cube->getTransform()->setLocalPosition(Vector3(0,0,0));
		cube->getTransform()->setParent(root);
		cube->getTransform()->name = "Cube";

		cout << "creating plane\n";
		GameObject *terrain = new GameObject(this);
		Terrain *terrainComponent = new Terrain();
		terrain->addComponent(terrainComponent);
		//terrainComponent->createTerrain("Resources/terrain.bmp",500,25);
		terrainComponent->createFractalTerrain(256,500,20,2.0);
		terrain->setMaterial(procmat);
		terrain->getTransform()->setLocalPosition(Vector3(0,-20,0));
		terrain->getTransform()->setParent(root);
		terrain->getTransform()->name = "Terrain";

		//camObj->addComponent(new TerrainFollower(terrainComponent,2.0));
		
		cout << "creating sphere\n";
		GameObject *sphere = new GameObject(this);
		sphere->setMesh(new Sphere(1.0,32));
		sphere->setMaterial(smiley);
		sphere->getTransform()->setLocalPosition(Vector3(15,0,0));		
		sphere->getTransform()->setParent(root);
		sphere->getTransform()->name = "Sphere";


		cout << "creating sphere2 as child\n";
		GameObject *sphere2 = new GameObject(this);
		sphere2->setMesh(new Sphere(1.0,32));
		sphere2->setMaterial(red);
		sphere2->getTransform()->setLocalPosition(Vector3(1,0,0));		
		sphere2->getTransform()->setParent(sphere->getTransform());
		sphere2->getTransform()->name = "Child";

		cout << "creating billboard\n";
		GameObject *billboard = new GameObject(this);
		Billboard *billboardComponent = new Billboard(renderer->camera->gameObject->getTransform(),true);
		billboard->addComponent(billboardComponent);
		billboard->setMaterial(smiley);
		billboard->getTransform()->setLocalPosition(Vector3(-10,0,0));
		billboard->getTransform()->setLocalScale(Vector3(2,2,2));
		billboard->getTransform()->setParent(root);
		billboard->getTransform()->name = "Billboard";

		cout << "creating sweep\n";
		SweepPath p;
		Transform a,b,c,d;
		a.setLocalPosition(Vector3(0,0,0));
		b.setLocalPosition(Vector3(1,0,0));
		c.setLocalPosition(Vector3(2,0,0));
		d.setLocalPosition(Vector3(3,0,0));
		p.addTransform(a);
		p.addTransform(b);
		p.addTransform(c);
		p.addTransform(d);
		GameObject *sweep = new GameObject(this);
		vector<Vector3> points;
		points.push_back(Vector3(0,1,0));
		points.push_back(Vector3(0,0,1));
		points.push_back(Vector3(0,-1,0));
		points.push_back(Vector3(0,0,-1));
		sweep->setMesh(new Sweep(points,p));
		sweep->getTransform()->setLocalPosition(Vector3(0,10,0));
		sweep->getTransform()->setParent(root);
		sweep->getTransform()->name = "Sweep";
		
		cube->addComponent(new LookAtBehaviour(sphere2->getTransform()));

		addTask(new PerfLogTask());

		Animation *anim = new Animation(10,5);
		sphere->addComponent(anim);
		anim->addKey("Child",9,Quaternion(),Vector3(1,0,0));
		anim->addKey("Child",0,Quaternion(),Vector3(1,0,0));
		anim->addKey("Child",7,Quaternion(),Vector3(3,0,0));
		anim->addKey("Child",5,Quaternion(),Vector3(10,0,0));
		anim->addKey("Sphere",9,Quaternion(),Vector3(5,0,0));
		anim->addKey("Sphere",0,Quaternion(),Vector3(5,0,0));
		anim->addKey("Sphere",7,Quaternion(),Vector3(5,0,10));
		anim->addKey("Sphere",5,Quaternion(Vector3(0,0,Math::HALF_PI)),Vector3(5,0,5));
		anim->loop(true);
		anim->interpolate();		

		/*
		Animation *b = new Animation(10,5);
		cube->addComponent(b);
		b->addKey("Cube",9,Quaternion(),Vector3(0,0,0));
		b->addKey("Cube",0,Quaternion(),Vector3(0,0,0));
		b->addKey("Cube",3,Quaternion(Vector3(Math::HALF_PI,0,Math::HALF_PI)),Vector3(0,0,0));
		b->addKey("Cube",7,Quaternion(Vector3(0,Math::HALF_PI,0)),Vector3(0,0,0));
		b->addKey("Cube",5,Quaternion(Vector3(0,-Math::HALF_PI,0)),Vector3(0,0,0));
		b->loop(true);
		b->interpolate();	
		*/

		return true;
	}
}