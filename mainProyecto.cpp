#include "Ogre\ExampleApplication.h"
#include "OGRE\OgreManualObject.h"
#include <vector>
#include <string>

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

	void drawFan(ManualObject* obj, std::vector<v3> points, std::vector<v2> textures_coords, bool inverted_normal = false) {
		obj->begin("Examples/Rockwall", RenderOperation::OT_TRIANGLE_FAN);

			if (inverted_normal) {
				for (int i=points.size()-1; i>=0; i--) {
					obj->position(points[i]);
					int u = (i-2 < 0) ? points.size()-2:i-2;
					int v = (i-1 < 0) ? points.size()-1:i-1;
					Ogre::Vector3 dir0 = points[u] - points[0];
					Ogre::Vector3 dir1 = points[0] - points[v];
					Ogre::Vector3 normal = dir1.crossProduct(dir0).normalisedCopy();
					obj->normal(normal);
					//obj->normal(points[i]);
					//obj->textureCoord(textures_coords[i]);
				}
			}
			else {
				for (int i=0; i<points.size(); i++) {
					obj->position(points[i]);
					int u = (i+2 >= points.size()) ? 1:i+2;
					int v = (i+1 >= points.size()) ? points.size()-1:i+1;
					Ogre::Vector3 dir0 = points[u] - points[0];
					Ogre::Vector3 dir1 = points[0] - points[v];
					Ogre::Vector3 normal = dir1.crossProduct(dir0).normalisedCopy();
					obj->normal(normal);
					//obj->normal(points[i]);
					//obj->textureCoord(textures_coords[i]);
				}
			}
			//for (int i=0; i<points.size()-2; i++)
				//obj->triangle(0, i+1, i+2);
			//obj->triangle(0, points.size()-1, 1);
        obj->end();
	}

	void drawSides(SceneManager* manager, SceneNode* node, std::string name, float start_length, float mid_ratio, float mid_length) {
		ManualObject* obj = manager->createManualObject(name);
		std::vector<v3> points;
		std::vector<v2> textures;
		textures.push_back(v2(0,0)); textures.push_back(v2(0,1)); textures.push_back(v2(1,0)); textures.push_back(v2(1,1));

		// first left
		points.push_back(v3(-(1.0 + sqrt(2)),  1.0,   0.0));
		points.push_back(v3(-(1.0 + sqrt(2)),  1.0,   start_length));
		points.push_back(v3(-(1.0 + sqrt(2)),  -1.0,  start_length));
		points.push_back(v3(-(1.0 + sqrt(2)),  -1.0,  0.0));
		drawFan(obj, points, textures, true); points.clear();

		// first right
		points.push_back(v3(1.0 + sqrt(2),  1.0,   0));
		points.push_back(v3(1.0 + sqrt(2),  1.0,   start_length));
		points.push_back(v3(1.0 + sqrt(2),  -1.0,  start_length));
		points.push_back(v3(1.0 + sqrt(2),  -1.0,  0.0));
		drawFan(obj, points, textures); points.clear();

		// second left
		points.push_back(v3(-(1.0 + sqrt(2)),			    1.0,			  start_length));
		points.push_back(v3(-(1.0 + sqrt(2)) * mid_ratio,   1.0 * mid_ratio,  start_length + mid_length));
		points.push_back(v3(-(1.0 + sqrt(2)) * mid_ratio,  -1.0 * mid_ratio,  start_length + mid_length));
		points.push_back(v3(-(1.0 + sqrt(2)),			   -1.0,			  start_length));
		drawFan(obj, points, textures, true); points.clear();

		// second right
		points.push_back(v3(1.0 + sqrt(2),				  1.0,			     start_length));
		points.push_back(v3((1.0 + sqrt(2)) * mid_ratio,  1.0 * mid_ratio,   start_length + mid_length));
		points.push_back(v3((1.0 + sqrt(2)) * mid_ratio,  -1.0 * mid_ratio,  start_length + mid_length));
		points.push_back(v3(1.0 + sqrt(2),				  -1.0,			     start_length));
		drawFan(obj, points, textures); points.clear();

		node->attachObject(obj);
	}

	void drawCase(SceneManager* manager, SceneNode* node, std::string name, float start_length, float mid_ratio, float mid_length) {
		ManualObject* obj = manager->createManualObject(name);
		std::vector<v3> points;
		std::vector<v2> textures; 
		textures.push_back(v2(0,0)); textures.push_back(v2(0,1)); textures.push_back(v2(1,0)); textures.push_back(v2(1,1));
		
		// first top
		points.push_back(v3(-1.0,  1.0 + sqrt(2),  0.0));
		points.push_back(v3(-1.0,  1.0 + sqrt(2),  start_length));
		points.push_back(v3(1.0,   1.0 + sqrt(2),  start_length));
		points.push_back(v3(1.0,   1.0 + sqrt(2),  0.0));
		drawFan(obj, points, textures); points.clear();

		// first inclined right
		points.push_back(v3(1.0,			 1.0 + sqrt(2), 0.0));
		points.push_back(v3(1.0,			 1.0 + sqrt(2), start_length));
		points.push_back(v3(1.0 + sqrt(2),   1.0, start_length));
		points.push_back(v3(1.0 + sqrt(2),   1.0, 0.0));
		drawFan(obj, points, textures); points.clear();

		// first inclined left
		points.push_back(v3(-1.0,			   1.0 + sqrt(2),  0.0));
		points.push_back(v3(-1.0,			   1.0 + sqrt(2),  start_length));
		points.push_back(v3(-(1.0 + sqrt(2)),  1.0,			  start_length));
		points.push_back(v3(-(1.0 + sqrt(2)),  1.0,			  0.0));
		drawFan(obj, points, textures, true); points.clear();

		// second top
		points.push_back(v3(-1.0,			   1.0 + sqrt(2),			     start_length));
		points.push_back(v3(-1.0 * mid_ratio,  (1.0 + sqrt(2)) * mid_ratio,  start_length + mid_length));
		points.push_back(v3(1.0 * mid_ratio,   (1.0 + sqrt(2)) * mid_ratio,  start_length + mid_length));
		points.push_back(v3(1.0,			   1.0 + sqrt(2),			     start_length));
		drawFan(obj, points, textures); points.clear();

		// second inclined right
		points.push_back(v3(1.0,					      1.0 + sqrt(2),				start_length));
		points.push_back(v3(1.0 * mid_ratio,			  (1.0 + sqrt(2)) * mid_ratio,  start_length + mid_length));
		points.push_back(v3((1.0 + sqrt(2)) * mid_ratio,  1.0 * mid_ratio,			    start_length + mid_length));
		points.push_back(v3((1.0 + sqrt(2)),			  1.0,						    start_length));
		drawFan(obj, points, textures); points.clear();

		// second inclined left
		points.push_back(v3(-1.0,						   1.0 + sqrt(2),			     start_length));
		points.push_back(v3(-1.0 * mid_ratio,			   (1.0 + sqrt(2)) * mid_ratio,  start_length + mid_length));
		points.push_back(v3(-(1.0 + sqrt(2)) * mid_ratio,  1.0 * mid_ratio,			     start_length + mid_length));
		points.push_back(v3(-(1.0 + sqrt(2)),			   1.0,						     start_length));
		drawFan(obj, points, textures, true); points.clear();

		node->attachObject(obj);

		if (name.find("lower") != std::string::npos) {
			node->roll(Degree(180.0));
		}
	}

	void drawRightWing(ManualObject* obj) {
		v3 arr[] = {v3(0.0, 0.0, 0.0), v3(2.0, 0.0, 0.0), v3(0.7, 5.0, 0.0), v3(0.0, 5.0, 0.0), v3(0.0, 0.0, 0.0)};
		drawWing(obj, arr);
	}

	void drawLeftWing(ManualObject* obj) {
		v3 arr[] = {v3(0.0, 0.0, 0.0), v3(2.0, 0.0, 0.0), v3(2.0, 5.0, 0.0), v3(1.3, 5.0, 0.0), v3(0.0, 0.0, 0.0)};
		drawWing(obj, arr);
	}

	void drawWing(ManualObject* obj, v3 arr[]) {
		std::vector<v3> points;
		std::vector<v2> textures;
		textures.push_back(v2(0,0)); textures.push_back(v2(0,1)); textures.push_back(v2(1,0)); textures.push_back(v2(1,1));

		// lower side
		for (int i=0; i<5; i++) points.push_back(arr[i]);
		drawFan(obj, points, textures, true);

		// upper side
		for (int i=0; i<5; i++) points[i].z = 1.0;
		drawFan(obj, points, textures); points.clear();

		// border
		points.push_back(arr[0]);
		points.push_back(arr[1]);
		points.push_back(arr[1] + v3(0.0, 0.0, 1.0));
		points.push_back(arr[0] + v3(0.0, 0.0, 1.0));
		drawFan(obj, points, textures); points.clear();

		points.push_back(arr[1]);
		points.push_back(arr[2]);
		points.push_back(arr[2] + v3(0.0, 0.0, 1.0));
		points.push_back(arr[1] + v3(0.0, 0.0, 1.0));
		drawFan(obj, points, textures); points.clear();

		points.push_back(arr[2]);
		points.push_back(arr[3]);
		points.push_back(arr[3] + v3(0.0, 0.0, 1.0));
		points.push_back(arr[2] + v3(0.0, 0.0, 1.0));
		drawFan(obj, points, textures); points.clear();

		points.push_back(arr[3]);
		points.push_back(arr[0]);
		points.push_back(arr[0] + v3(0.0, 0.0, 1.0));
		points.push_back(arr[3] + v3(0.0, 0.0, 1.0));
		drawFan(obj, points, textures); points.clear();
	}

	void createWing(SceneManager* manager, SceneNode* node, std::string name, float wing_size, float degrees[], v3 position) {
		ManualObject* obj = manager->createManualObject(name);
		if (name.find("left") != std::string::npos)
			drawLeftWing(obj);
		else
			drawRightWing(obj);
		node->yaw(Degree(degrees[0]));
		node->pitch(Degree(degrees[1]));
		node->roll(Degree(degrees[2]));
		node->setPosition(position);
		node->setScale(2*wing_size, wing_size, wing_size/4);
		node->attachObject(obj);
	}

	void drawOctagon(SceneManager* manager, SceneNode* node, std::string name) {
		ManualObject* obj = manager->createManualObject(name);
		std::vector<v3> points;
		std::vector<v2> textures;
		textures.push_back(v2(0,0)); textures.push_back(v2(0,1)); textures.push_back(v2(1,0)); textures.push_back(v2(1,1));
		
		//octagon with center 0.0
		points.push_back(v3(0.0, 0.0, 0.0));  // center
		points.push_back(v3(1.0,		       1.0 + sqrt(2),     0.0));
		points.push_back(v3(1.0 + sqrt(2),     1.0,			      0.0));
		points.push_back(v3(1.0 + sqrt(2),     -1,			      0.0));
		points.push_back(v3(1.0,			   -(1.0 + sqrt(2)),  0.0));
		points.push_back(v3(-1.0,			   -(1.0 + sqrt(2)),  0.0));
		points.push_back(v3(-(1.0 + sqrt(2)),  -1,				  0.0));
		points.push_back(v3(-(1.0 + sqrt(2)),  1.0,				  0.0));
		points.push_back(v3(-1.0,			   1.0 + sqrt(2),	  0.0));
		points.push_back(v3(1.0,			   1.0 + sqrt(2),	  0.0));
		drawFan(obj, points, textures); points.clear();

		node->attachObject(obj);
	}

	void drawShip(SceneManager* manager) {
		float ship_size = 1.0,
			  wing_size = 2.0,
			  wing_proximity = 2.40,
			  start_length = 8.0, 
			  mid_ratio = 0.2,
			  mid_length = 11.0;

		SceneNode* ship = manager->getRootSceneNode()->createChildSceneNode();
		ship->setScale(ship_size, ship_size, ship_size);
		ship->yaw(Degree(180.0));

		SceneNode* back = ship->createChildSceneNode();
		drawOctagon(manager, back, std::string("back"));

		SceneNode* front = ship->createChildSceneNode();
		drawOctagon(manager, front, std::string("front"));
		front->yaw(Degree(180.0));
		front->setPosition(0.0, 0.0, start_length + mid_length);
		front->setScale(mid_ratio, mid_ratio, 1.0);

		SceneNode* sides = ship->createChildSceneNode();
		drawSides(manager, sides, std::string("sides"), start_length, mid_ratio, mid_length);

		SceneNode* upper_case = ship->createChildSceneNode();
		drawCase(manager, upper_case, std::string("upper_case"), start_length, mid_ratio, mid_length);

		SceneNode* lower_case = ship->createChildSceneNode();
		drawCase(manager, lower_case, std::string("lower_case"), start_length, mid_ratio, mid_length);

		SceneNode* left_wing1 = ship->createChildSceneNode();
		float left_degrees[] = {180.0, -90.0, 90.0};
		createWing(manager, left_wing1, std::string("ship_left_wing1"), wing_size, left_degrees, v3(wing_proximity, 0.0, 0.0));

		SceneNode* left_wing2 = ship->createChildSceneNode();
		createWing(manager, left_wing2, std::string("ship_left_wing2"), wing_size, left_degrees, v3(wing_proximity, -1.0, 0.0));
		
		SceneNode* right_wing1 = ship->createChildSceneNode();
		float right_degrees[] = {180.0, -90.0, -90.0};
		createWing(manager, right_wing1, std::string("ship_right_wing1"), wing_size, right_degrees, v3(-wing_proximity, 0.0, 8.0));

		SceneNode* right_wing2 = ship->createChildSceneNode();
		createWing(manager, right_wing2, std::string("ship_right_wing2"), wing_size, right_degrees, v3(-wing_proximity, -1.0, 8.0));
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

		drawShip(mSceneMgr);
	}
};

int main (void)
{
	Example1 app;
	app.go();
	return 0;
}