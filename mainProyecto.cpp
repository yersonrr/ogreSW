#include "Ogre\ExampleApplication.h"
#include "OGRE\OgreManualObject.h"
#include <vector>

#define v3 Ogre::Vector3
#define v2 Ogre::Vector2

class Example1 : public ExampleApplication
{

public:

	void createCamera() {

		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(0,10,50);
		mCamera->lookAt(0,0,-50);
		mCamera->setNearClipDistance(5);

	}

	void drawPlane(ManualObject* obj, std::vector<v3> points, std::vector<v2> textures_coords) {
		obj->begin("Examples/Rockwall", RenderOperation::OT_TRIANGLE_STRIP);
			for(int i=0; i<points.size(); i++) {
				obj->position(points[i].x, points[i].y, points[i].z);
				obj->textureCoord(textures_coords[i].x, textures_coords[i].y);
				obj->normal(points[i].x, points[i].y, points[i].z);
			}
        obj->end();
	}

	void drawCube(ManualObject* obj, float x, float y, float z) {
		// front
		std::vector<v3> points;
		std::vector<v2> textures;
		points.push_back(v3(-x, y, -z)); points.push_back(v3(x, y, -z)); points.push_back(v3(-x, -y, -z)); points.push_back(v3(x, -y, -z));
		textures.push_back(v2(0,0)); textures.push_back(v2(0,1)); textures.push_back(v2(1,0)); textures.push_back(v2(1,1));
		drawPlane(obj, points, textures);
		points.clear();
		
		//back
		points.push_back(v3(-x, -y, z)); points.push_back(v3(x, -y, z)); points.push_back(v3(-x, y, z)); points.push_back(v3(x, y, z));
		drawPlane(obj, points, textures); points.clear();

		//top
		points.push_back(v3(-x, y, z)); points.push_back(v3(x, y, z)); points.push_back(v3(-x, y, -z)); points.push_back(v3(x, y, -z));
		drawPlane(obj, points, textures);
		points.clear();

		//bottom
		points.push_back(v3(-x, -y, -z)); points.push_back(v3(x, -y, -z)); points.push_back(v3(-x, -y, z)); points.push_back(v3(x, -y, z));
		drawPlane(obj, points, textures);
		points.clear();

		//left
		points.push_back(v3(-x, y, z)); points.push_back(v3(-x, y, -z)); points.push_back(v3(-x, -y, z)); points.push_back(v3(-x, -y, -z));
		drawPlane(obj, points, textures);
		points.clear();

		//right
		points.push_back(v3(x, -y, z)); points.push_back(v3(x, -y, -z)); points.push_back(v3(x, y, z)); points.push_back(v3(x, y, -z));
		drawPlane(obj, points, textures);
		points.clear();
	}

	void createScene()
	{
		Ogre::SceneNode* torreta01;
		Ogre::SceneNode* torreta02;
		Ogre::SceneNode* torreta03;
		Ogre::SceneNode* torreta04;

		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		

		Ogre::Entity* ent01 = mSceneMgr->createEntity("MyEntity1","ejes01.mesh");
		Ogre::SceneNode* node01 = mSceneMgr->createSceneNode("node01");
		mSceneMgr->getRootSceneNode()->addChild(node01);
		node01->attachObject(ent01);
		

		Ogre::Entity* entEscena01 = mSceneMgr->createEntity("ogre_base01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena01);

		Ogre::Entity* entEscena02 = mSceneMgr->createEntity("ogre_fondo01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena02);

		Ogre::Entity* entEscena03 = mSceneMgr->createEntity("ogre_pared01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena03);

		Ogre::Entity* entEscena04 = mSceneMgr->createEntity("ogre_pared02.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena04);

		Ogre::Entity* entEscena05 = mSceneMgr->createEntity("ogre_pared03.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena05);

		Ogre::Entity* entEscena06 = mSceneMgr->createEntity("ogre_torretas01.mesh");
		mSceneMgr->getRootSceneNode()->attachObject(entEscena06);

		Ogre::Entity* entTorreta01 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta01 = mSceneMgr->createSceneNode("torreta01");
		mSceneMgr->getRootSceneNode()->addChild(torreta01);
		torreta01->attachObject(entTorreta01);
		torreta01->setScale(0.5, 3, 0.5);
		torreta01->setPosition(-22, -8, -227.5);

		Ogre::Entity* entTorreta02 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta02 = mSceneMgr->createSceneNode("torreta02");
		mSceneMgr->getRootSceneNode()->addChild(torreta02);
		torreta02->attachObject(entTorreta02);
		torreta02->setScale(0.5, 3, 0.5);
		torreta02->setPosition(-22, -8, -637.5);

		Ogre::Entity* entTorreta03 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta03 = mSceneMgr->createSceneNode("torreta03");
		mSceneMgr->getRootSceneNode()->addChild(torreta03);
		torreta03->attachObject(entTorreta03);
		torreta03->setScale(0.5, 3, 0.5);
		torreta03->setPosition(22, -8, -407.75);

		Ogre::Entity* entTorreta04 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta04 = mSceneMgr->createSceneNode("torreta04");
		mSceneMgr->getRootSceneNode()->addChild(torreta04);
		torreta04->attachObject(entTorreta04);
		torreta04->setScale(0.5, 3, 0.5);
		torreta04->setPosition(22, -8, -883.5);

		// Manual object for the body of the x-wing ship
		ManualObject* cube = mSceneMgr->createManualObject("cube");
        drawCube(cube, 10.0, 5.0, 15.0);
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(cube);
	}
};


int main (void)
{
	Example1 app;
	app.go();
	return 0;
}