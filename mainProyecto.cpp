#include "Ogre\ExampleApplication.h"
#include "OGRE\OgreManualObject.h"
#include <vector>
#include <string>

#define v3 Ogre::Vector3
#define v2 Ogre::Vector2

int number_wings = 4;

float wing_degree[4] = {0.0, 0.0, 0.0, 0.0},
	wing_degree_open[4][3] = {
		0.0,  16.0,  32.0,
		0.0, -16.0, -32.0,
		0.0, -16.0, -32.0,
		0.0,  16.0,  32.0};
v3 ship_position(0.0, 0.0, 0.0),
   cam_position(0.0, 10.0, 40.0), 
   wing_rotation_vector(0, 0, 1);

AnimationState * wingOpenState[4], * wingCloseState[4];
char* wingOpenName[] = {"wing_open_1", "wing_open_2", "wing_open_3", "wing_open_4"};
char* wingCloseName[] = {"wing_close_1", "wing_close_2", "wing_close_3", "wing_close_4"};
bool is_accelerating = false;

SceneNode * wing[4], * wing2[4];

Ogre::AnimationState* AnimLaser1;
Ogre::AnimationState* AnimLaser2;
Ogre::AnimationState* AnimLaser3;
Ogre::AnimationState* AnimLaser4;
Ogre::AnimationState* AnimLaser5;
Ogre::AnimationState* AnimLaser6;


class FrameListenerClase : public Ogre::FrameListener{

private:
	Ogre::SceneNode* _node;
	Ogre::AnimationState* _anim;
	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	OIS::Mouse* _mouse;
	Ogre::Camera* _cam;

public:
	FrameListenerClase(Ogre::SceneNode* node, Ogre::Camera* cam, RenderWindow* win){
		//Configuracion para captura de teclado y mouse 
		size_t windowHnd = 0;
		std::stringstream windowHndStr;
		win->getCustomAttribute("WINDOW",&windowHnd);
		windowHndStr << windowHnd;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"),windowHndStr.str()));

		//eventos
		_man = OIS::InputManager::createInputSystem(pl);
		_key = static_cast<OIS::Keyboard*>(_man->createInputObject(OIS::OISKeyboard,false));
		_mouse = static_cast<OIS::Mouse*>(_man->createInputObject(OIS::OISMouse,false));
		_cam = cam;
		_node = node;
	}


	~FrameListenerClase(){
		_man->destroyInputObject(_key);
		_man->destroyInputObject(_mouse);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent &evt){
		_key->capture();
		_mouse->capture();

		float speed_factor = 0.1f;
		Ogre::Vector3 tmov(0,0,0);

		//Camara
		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		if(_key->isKeyDown(OIS::KC_W)) {
			tmov += Ogre::Vector3(0,0,-10);
			if (_node->getPosition().z < -1100) {
				ship_position = v3(0.0, 0.0, 0.0);
				cam_position = v3(0.0, 10.0, 50.0);
			}
		}

		if(_key->isKeyDown(OIS::KC_A)) {
			if (_node->getPosition().x > -20)
				tmov += Ogre::Vector3(-10,0,0);
		}
		
		if(_key->isKeyDown(OIS::KC_D)) {
			if (_node->getPosition().x < 20)
				tmov += Ogre::Vector3(10,0,0);
		}

		if(_key->isKeyDown(OIS::KC_E) && !is_accelerating) {
			is_accelerating = true;
			
			for (int i=0; i<number_wings; i++) {
				wingOpenState[i]->setEnabled(true);
				wingCloseState[i]->setEnabled(false);
				wingOpenState[i]->setLoop(false);
				wingOpenState[i]->setTimePosition(0.0);
			}
		}

		if(_key->isKeyDown(OIS::KC_R) && is_accelerating) {
			is_accelerating = false;

			for (int i=0; i<number_wings; i++) {
				wingOpenState[i]->setEnabled(false);
				wingCloseState[i]->setEnabled(true);
				wingCloseState[i]->setLoop(false);
				wingCloseState[i]->setTimePosition(0.0);
			}
		}

		if(wingOpenState[0]->getEnabled()) {
			for (int i=0; i<number_wings; i++)
				wingOpenState[i]->addTime(evt.timeSinceLastFrame);
		}

		if(wingCloseState[0]->getEnabled()) {
			for (int i=0; i<number_wings; i++)
				wingCloseState[i]->addTime(evt.timeSinceLastFrame);
		}

		//camara control
		cam_position += (tmov * speed_factor);
		_cam->setPosition(cam_position);
		ship_position += (tmov * speed_factor);
		_node->setPosition(ship_position);

		
		AnimLaser1->addTime(evt.timeSinceLastFrame);
		AnimLaser2->addTime(evt.timeSinceLastFrame);
		AnimLaser3->addTime(evt.timeSinceLastFrame);
		AnimLaser4->addTime(evt.timeSinceLastFrame);
		AnimLaser5->addTime(evt.timeSinceLastFrame);
		AnimLaser6->addTime(evt.timeSinceLastFrame);

		return true;
	}
};

class Example1 : public ExampleApplication
{
public:
	Ogre::FrameListener* FrameListener01;
	SceneNode* ship;

	Example1(){
		FrameListener01 = NULL;
	}

	~Example1(){
		if(FrameListener01){
			delete FrameListener01;
		}
	}

	void createCamera() {
		mCamera = mSceneMgr->createCamera("MyCamera1");
		mCamera->setPosition(cam_position);
		mCamera->lookAt(0,0,-50);
		mCamera->setNearClipDistance(5);
	}

	void createFrameListener(){
		FrameListener01 = new FrameListenerClase(ship, mCamera, mWindow);
		mRoot->addFrameListener(FrameListener01);
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

		ship = manager->getRootSceneNode()->createChildSceneNode();
		ship->setPosition(ship_position);
		ship->setScale(ship_size, ship_size, ship_size);
		ship->yaw(Degree(180.0));

		SceneNode* back = ship->createChildSceneNode();
		drawOctagon(manager, back, std::string("back"));

		SceneNode* front = ship->createChildSceneNode();
		drawOctagon(manager, front, std::string("front"));
		front->yaw(Degree(180.0));
		front->setPosition(0.0, 0.0, start_length + mid_length);
		front->setScale(mid_ratio, mid_ratio, 1.0);

		// Create cylinder
		Ogre::Entity* entCylinderMotor01 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderMotor01 = mSceneMgr->createSceneNode("cylinder01");
		ship->addChild(cylinderMotor01);
		cylinderMotor01->attachObject(entCylinderMotor01);
		cylinderMotor01->setScale(0.25, 0.7, 0.25);
		cylinderMotor01->setPosition(2, 1.5, 3.9);
		cylinderMotor01->pitch(Degree(90));

		Ogre::Entity* entCylinderMotor02 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderMotor02 = mSceneMgr->createSceneNode("cylinder02");
		ship->addChild(cylinderMotor02);
		cylinderMotor02->attachObject(entCylinderMotor02);
		cylinderMotor02->setScale(0.25, 0.7, 0.25);
		cylinderMotor02->setPosition(-2, 1.5, 3.9);
		cylinderMotor02->pitch(Degree(90));

		Ogre::Entity* entCylinderMotor03 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderMotor03 = mSceneMgr->createSceneNode("cylinder03");
		ship->addChild(cylinderMotor03);
		cylinderMotor03->attachObject(entCylinderMotor03);
		cylinderMotor03->setScale(0.25, 0.7, 0.25);
		cylinderMotor03->setPosition(-2, -1.5, 3.9);
		cylinderMotor03->pitch(Degree(90));

		Ogre::Entity* entCylinderMotor04 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderMotor04 = mSceneMgr->createSceneNode("cylinder04");
		ship->addChild(cylinderMotor04);
		cylinderMotor04->attachObject(entCylinderMotor04);
		cylinderMotor04->setScale(0.25, 0.7, 0.25);
		cylinderMotor04->setPosition(2, -1.5, 3.9);
		cylinderMotor04->pitch(Degree(90));

		entCylinderMotor01->setMaterialName("matPropio03");
		entCylinderMotor02->setMaterialName("matPropio03");
		entCylinderMotor03->setMaterialName("matPropio03");
		entCylinderMotor04->setMaterialName("matPropio03");

		// Create Propeller
		Ogre::Entity* entCylinderPropeller01 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderPropeller01 = mSceneMgr->createSceneNode("cylinderPropeller01");
		ship->addChild(cylinderPropeller01);
		cylinderPropeller01->attachObject(entCylinderPropeller01);
		cylinderPropeller01->setScale(0.18, 0.2, 0.18);
		cylinderPropeller01->setPosition(2, 1.5, 0.1);
		cylinderPropeller01->pitch(Degree(90));

		Ogre::Entity* entCylinderPropeller02 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderPropeller02 = mSceneMgr->createSceneNode("cylinderPropeller02");
		ship->addChild(cylinderPropeller02);
		cylinderPropeller02->attachObject(entCylinderPropeller02);
		cylinderPropeller02->setScale(0.18, 0.2, 0.18);
		cylinderPropeller02->setPosition(-2, 1.5, 0.1);
		cylinderPropeller02->pitch(Degree(90));

		Ogre::Entity* entCylinderPropeller03 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderPropeller03 = mSceneMgr->createSceneNode("cylinderPropeller03");
		ship->addChild(cylinderPropeller03);
		cylinderPropeller03->attachObject(entCylinderPropeller03);
		cylinderPropeller03->setScale(0.18, 0.2, 0.18);
		cylinderPropeller03->setPosition(-2, -1.5, 0.1);
		cylinderPropeller03->pitch(Degree(90));

		Ogre::Entity* entCylinderPropeller04 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderPropeller04 = mSceneMgr->createSceneNode("cylinderPropeller04");
		ship->addChild(cylinderPropeller04);
		cylinderPropeller04->attachObject(entCylinderPropeller04);
		cylinderPropeller04->setScale(0.18, 0.2, 0.18);
		cylinderPropeller04->setPosition(2, -1.5, 0.1);
		cylinderPropeller04->pitch(Degree(90));

		entCylinderPropeller01->setMaterialName("matPropio06");
		entCylinderPropeller02->setMaterialName("matPropio06");
		entCylinderPropeller03->setMaterialName("matPropio06");
		entCylinderPropeller04->setMaterialName("matPropio06");


		SceneNode* sides = ship->createChildSceneNode();
		drawSides(manager, sides, std::string("sides"), start_length, mid_ratio, mid_length);

		SceneNode* upper_case = ship->createChildSceneNode();
		drawCase(manager, upper_case, std::string("upper_case"), start_length, mid_ratio, mid_length);

		SceneNode* lower_case = ship->createChildSceneNode();
		drawCase(manager, lower_case, std::string("lower_case"), start_length, mid_ratio, mid_length);

		wing2[0] = ship->createChildSceneNode();
		wing[0] = wing2[0]->createChildSceneNode();
		float left_degrees[] = {180.0, -90.0, 90.0};
		createWing(manager, wing[0], std::string("ship_left_wing1"), wing_size, left_degrees, v3(wing_proximity, 0.0, 0.0));
		wing[0]->pitch(Degree(wing_degree[0]));

		wing2[1] = ship->createChildSceneNode();
		wing[1] = wing2[1]->createChildSceneNode();

		// Create cylinder left wing
		Ogre::Entity* entCylinderWing01 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing01 = mSceneMgr->createSceneNode("cylinderWing01");
		wing[0]->addChild(cylinderWing01);
		cylinderWing01->attachObject(entCylinderWing01);
		cylinderWing01->setScale(0.025, 0.35, 0.25);
		cylinderWing01->setPosition(2.3, 5, 1);
		cylinderWing01->roll(Degree(90));
		
		createWing(manager, wing[1], std::string("ship_left_wing2"), wing_size, left_degrees, v3(wing_proximity, -1.0, 0.0));
		wing[1]->pitch(Degree(wing_degree[1]));

		// Create cylinder left wing 02
		Ogre::Entity* entCylinderWing02 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing02 = mSceneMgr->createSceneNode("cylinderWing02");
		wing[1]->addChild(cylinderWing02);
		cylinderWing02->attachObject(entCylinderWing02);
		cylinderWing02->setScale(0.025, 0.35, 0.25);
		cylinderWing02->setPosition(2.3, 5, 0.5);
		cylinderWing02->roll(Degree(90));
		
		wing2[2] = ship->createChildSceneNode();
		wing[2] = wing2[2]->createChildSceneNode();
		float right_degrees[] = {180.0, -90.0, -90.0};
		createWing(manager, wing[2], std::string("ship_right_wing1"), wing_size, right_degrees, v3(-wing_proximity, 0.0, 8.0));
		wing[2]->pitch(Degree(wing_degree[2]));

		wing2[3] = ship->createChildSceneNode();
		wing[3] = wing2[3]->createChildSceneNode();

		// Create cylinder right wing 01
		Ogre::Entity* entCylinderWing03 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing03 = mSceneMgr->createSceneNode("cylinderWing03");
		wing[2]->addChild(cylinderWing03);
		cylinderWing03->attachObject(entCylinderWing03);
		cylinderWing03->setScale(0.025, 0.35, 0.25);
		cylinderWing03->setPosition(-0.3, 5, 0.9);
		cylinderWing03->roll(Degree(90));

		createWing(manager, wing[3], std::string("ship_right_wing2"), wing_size, right_degrees, v3(-wing_proximity, -1.0, 8.0));
		wing[3]->pitch(Degree(wing_degree[3]));

		// Create cylinder right wing 01
		Ogre::Entity* entCylinderWing04 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing04 = mSceneMgr->createSceneNode("cylinderWing04");
		wing[3]->addChild(cylinderWing04);
		cylinderWing04->attachObject(entCylinderWing04);
		cylinderWing04->setScale(0.025, 0.35, 0.25);
		cylinderWing04->setPosition(-0.3, 5, 0.2);
		cylinderWing04->roll(Degree(90));

		entCylinderWing01->setMaterialName("matPropio04");
		entCylinderWing02->setMaterialName("matPropio04");
		entCylinderWing03->setMaterialName("matPropio04");
		entCylinderWing04->setMaterialName("matPropio04");

	}

	void createOpenWingsAnimation(int wing_index) {
		// create animation to open wings
		Real duration = 2.0;
		Real step = duration/2.0;
		Animation* animation = mSceneMgr->createAnimation(wingOpenName[wing_index], duration);
		animation->setInterpolationMode(Animation::IM_SPLINE);
		NodeAnimationTrack* track = animation->createNodeTrack(0, wing2[wing_index]);

		// add keyframes
		TransformKeyFrame* key;
 
		key = track->createNodeKeyFrame(0.0f);
		key->setRotation(Quaternion(Degree(wing_degree_open[wing_index][0]), wing_rotation_vector));
 
		key = track->createNodeKeyFrame(step);
		key->setRotation(Quaternion(Degree(wing_degree_open[wing_index][1]), wing_rotation_vector));
 
		key = track->createNodeKeyFrame(step * 2.0);
		key->setRotation(Quaternion(Degree(wing_degree_open[wing_index][2]), wing_rotation_vector));

		wingOpenState[wing_index] = mSceneMgr->createAnimationState(wingOpenName[wing_index]);
	}

	void createCloseWingsAnimation(int wing_index) {
		// create animation to open wings
		Real duration = 2.0;
		Real step = duration/2.0;
		Animation* animation = mSceneMgr->createAnimation(wingCloseName[wing_index], duration);
		animation->setInterpolationMode(Animation::IM_SPLINE);
		NodeAnimationTrack* track = animation->createNodeTrack(0, wing2[wing_index]);

		// add keyframes
		TransformKeyFrame* key;
 
		key = track->createNodeKeyFrame(0.0f);
		key->setRotation(Quaternion(Degree(wing_degree_open[wing_index][2]), wing_rotation_vector));
 
		key = track->createNodeKeyFrame(step);
		key->setRotation(Quaternion(Degree(wing_degree_open[wing_index][1]), wing_rotation_vector));
 
		key = track->createNodeKeyFrame(step * 2.0);
		key->setRotation(Quaternion(Degree(wing_degree_open[wing_index][0]), wing_rotation_vector));

		wingCloseState[wing_index] = mSceneMgr->createAnimationState(wingCloseName[wing_index]);
	}

	void createScene()
	{
		Ogre::SceneNode* cubo1;
		Ogre::SceneNode* cubo2;
		Ogre::SceneNode* cubo3;
		Ogre::SceneNode* cubo4;
		Ogre::SceneNode* torreta01;
		Ogre::SceneNode* torreta02;
		Ogre::SceneNode* torreta03;
		Ogre::SceneNode* torreta04;
		Ogre::SceneNode* canon1;
		Ogre::SceneNode* canon1_1;
		Ogre::SceneNode* canon2;
		Ogre::SceneNode* canon2_2;
		Ogre::SceneNode* canon3;
		Ogre::SceneNode* canon4;

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


		// Torreta numero 1
		Ogre::Entity* entTorreta01 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta01 = mSceneMgr->createSceneNode("torreta01");
		mSceneMgr->getRootSceneNode()->addChild(torreta01);
		torreta01->attachObject(entTorreta01);
		torreta01->setScale(0.5, 3, 0.5);
		torreta01->setPosition(-22, -8, -227.5);
		
		Ogre::Entity* entSuperficie = mSceneMgr->createEntity("cube.mesh");
		cubo1 = mSceneMgr->createSceneNode("cubo1");
		torreta01->addChild(cubo1);
		cubo1->attachObject(entSuperficie);
		cubo1->setScale(0.1, 0.05, 0.07);
		cubo1->setPosition(0, 0, 0);

		Ogre::Entity* entCanon = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon1 = mSceneMgr->createSceneNode("canon1");
		torreta01->addChild(canon1);
		canon1->attachObject(entCanon);
		canon1->setScale(1.5, 0.3, 1.5);
		canon1->setPosition(0, 2.5, 3);
		canon1->pitch(Degree(90));

		Ogre::Entity* entCanon1_1 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon1_1 = mSceneMgr->createSceneNode("canon1_1");
		torreta01->addChild(canon1_1);
		canon1_1->attachObject(entCanon1_1);
		canon1_1->setScale(0.75, 0.33, 0.75);
		canon1_1->setPosition(0, 2.8, 5);
		canon1_1->pitch(Degree(90));

		entTorreta01->setMaterialName("matPropio04");
		entSuperficie->setMaterialName("matPropio03");
		entCanon->setMaterialName("matPropio04");
		entCanon1_1->setMaterialName("matPropio03");
		torreta01->yaw(Degree(30));
			
		// Create laser and animation 01
		Ogre::SceneNode* laser1;
		Ogre::Entity* entLaser = mSceneMgr->createEntity("usb_laser.mesh");
		laser1 = mSceneMgr->createSceneNode("laser1");
		torreta01->addChild(laser1);
		laser1->attachObject(entLaser);
		laser1->setScale(1.2, 0.16, 1.2);
		laser1->setPosition(0, 2.8, 5);
		laser1->pitch(Degree(90));
	
		float duration = 0.3;
		Ogre::Animation* animationLaser01 = mSceneMgr->createAnimation("AnimLaser01",duration);
		animationLaser01->setInterpolationMode(Animation::IM_SPLINE);
		
		Ogre::NodeAnimationTrack* Laser01track = animationLaser01->createNodeTrack(0, laser1);
		Ogre::TransformKeyFrame* key;
		
		key = Laser01track->createNodeKeyFrame(0.0);
		key->setTranslate(Vector3(0, 2.8, 5));
		key->setScale(Vector3(1.2, 0.16, 1.2));
		key->setRotation(Ogre::Quaternion(Degree(90), v3(1,0,0)));

		key = Laser01track->createNodeKeyFrame(0.3);
		key->setTranslate(Vector3(5, 2.8, 225));
		key->setScale(Vector3(1.2, 0.16, 1.2));
		key->setRotation(Ogre::Quaternion(Degree(90), v3(1,0,0)));

		AnimLaser1 = mSceneMgr->createAnimationState("AnimLaser01");
		AnimLaser1->setEnabled(true);
		AnimLaser1->setLoop(true);

		// Torreta Numero 2
		Ogre::Entity* entTorreta02 = mSceneMgr->createEntity("usb_cubomod01.mesh");
		torreta02 = mSceneMgr->createSceneNode("torreta02");
		mSceneMgr->getRootSceneNode()->addChild(torreta02);
		torreta02->attachObject(entTorreta02);
		torreta02->setScale(1.7, 1.5, 1.7);
		torreta02->setPosition(-22.5, -6, -637.5);

		Ogre::Entity* entSuperficie2 = mSceneMgr->createEntity("cube.mesh");
		cubo2 = mSceneMgr->createSceneNode("cubo2");
		torreta02->addChild(cubo2);
		cubo2->attachObject(entSuperficie2);
		cubo2->setScale(0.05, 0.04, 0.05);
		cubo2->setPosition(0, 2.5, 0);
		
		
		Ogre::Entity* entCanon2 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon2 = mSceneMgr->createSceneNode("canon2");
		torreta02->addChild(canon2);
		canon2->attachObject(entCanon2);
		canon2->setScale(0.2, 0.5, 0.2);
		canon2->setPosition(-1, 3.2, 3);
		canon2->pitch(Degree(90));

		// Create laser and animation 02
		Ogre::SceneNode* laser2;
		Ogre::Entity* entLaser2 = mSceneMgr->createEntity("usb_laser.mesh");
		laser2 = mSceneMgr->createSceneNode("laser2");
		canon2->addChild(laser2);
		laser2->attachObject(entLaser2);
		laser2->setScale(2, 0.7, 2);
		laser2->setPosition(0, -2, 0);
		
		float duration2 = 0.7;
		Ogre::Animation* animationLaser02 = mSceneMgr->createAnimation("AnimLaser02",duration2);
		animationLaser02->setInterpolationMode(Animation::IM_SPLINE);
		
		Ogre::NodeAnimationTrack* Laser02track = animationLaser02->createNodeTrack(0, laser2);
		Ogre::TransformKeyFrame* key2;
		
		key2 = Laser02track->createNodeKeyFrame(0.0);
		key2->setTranslate(Vector3(0, 0, 0));
		key2->setScale(Vector3(2, 0.7, 2));

		key2 = Laser02track->createNodeKeyFrame(0.7);
		key2->setTranslate(Vector3(0, 300, 0));
		key2->setScale(Vector3(2, 0.7, 2));

		AnimLaser2 = mSceneMgr->createAnimationState("AnimLaser02");
		AnimLaser2->setEnabled(true);
		AnimLaser2->setLoop(true);

		// Canon2_2
		Ogre::Entity* entCanon2_2 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon2_2 = mSceneMgr->createSceneNode("canon2_2");
		torreta02->addChild(canon2_2);
		canon2_2->attachObject(entCanon2_2);
		canon2_2->setScale(0.2, 0.5, 0.2);
		canon2_2->setPosition(1, 3.2, 3);
		canon2_2->pitch(Degree(90));

		entTorreta02->setMaterialName("matPropio04");
		entSuperficie2->setMaterialName("matPropio04");
		entCanon2->setMaterialName("matPropio03");
		entCanon2_2->setMaterialName("matPropio03");
		torreta02->yaw(Degree(30));
		
		// Laser 2_2
		Ogre::SceneNode* laser3;
		Ogre::Entity* entLaser3 = mSceneMgr->createEntity("usb_laser.mesh");
		laser3 = mSceneMgr->createSceneNode("laser3");
		canon2_2->addChild(laser3);
		laser3->attachObject(entLaser3);
		laser3->setScale(2, 0.7, 2);
		laser3->setPosition(0, -2, 0);
		
		float duration3 = 0.5;
		Ogre::Animation* animationLaser03 = mSceneMgr->createAnimation("AnimLaser03",duration3);
		animationLaser03->setInterpolationMode(Animation::IM_SPLINE);
		
		Ogre::NodeAnimationTrack* Laser03track = animationLaser03->createNodeTrack(0, laser3);
		Ogre::TransformKeyFrame* key3;
		
		key3 = Laser03track->createNodeKeyFrame(0.0);
		key3->setTranslate(Vector3(0, 0, 0));
		key3->setScale(Vector3(2, 0.7, 2));

		key3 = Laser03track->createNodeKeyFrame(0.5);
		key3->setTranslate(Vector3(0, 300, 0));
		key3->setScale(Vector3(2, 0.7, 2));

		AnimLaser3 = mSceneMgr->createAnimationState("AnimLaser03");
		AnimLaser3->setEnabled(true);
		AnimLaser3->setLoop(true);

		// Torreta Numero 3
		Ogre:: SceneNode* turretbase3;
		Ogre:: SceneNode* turretbase3_1;

		Ogre::Entity* entTorreta03 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta03 = mSceneMgr->createSceneNode("torreta03");
		mSceneMgr->getRootSceneNode()->addChild(torreta03);
		torreta03->attachObject(entTorreta03);
		torreta03->setScale(0.75, 1, 0.75);
		torreta03->setPosition(22, -9, -407.75);

		Ogre::Entity* entbase03 = mSceneMgr->createEntity("usb_cilindro.mesh");
		turretbase3 = mSceneMgr->createSceneNode("turretbase3");
		torreta03->addChild(turretbase3);
		turretbase3->attachObject(entbase03);
		turretbase3->setScale(2, 0.2, 2);
		turretbase3->setPosition(0, 1, 0);

		Ogre::Entity* entbase03_1 = mSceneMgr->createEntity("usb_cilindro.mesh");
		turretbase3_1 = mSceneMgr->createSceneNode("turretbase3_1");
		torreta03->addChild(turretbase3_1);
		turretbase3_1->attachObject(entbase03_1);
		turretbase3_1->setScale(1.3, 1.2, 1.3);
		turretbase3_1->setPosition(0, 1, 0);

		Ogre::Entity* entSuperficie3 = mSceneMgr->createEntity("usb_cilindro02.mesh");
		cubo3 = mSceneMgr->createSceneNode("cubo3");
		torreta03->addChild(cubo3);
		cubo3->attachObject(entSuperficie3);
		cubo3->setScale(2, 0.23, 2);
		cubo3->setPosition(0, 9, 0);

		Ogre::Entity* entCanon3 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon3 = mSceneMgr->createSceneNode("canon3");
		torreta03->addChild(canon3);
		canon3->attachObject(entCanon3);
		canon3->setScale(0.5, 0.5, 0.5);
		canon3->setPosition(0, 9, 3);
		canon3->pitch(Degree(90));

		// Laser 3_1
		Ogre::SceneNode* laser4;
		Ogre::Entity* entLaser4 = mSceneMgr->createEntity("usb_laser.mesh");
		laser4 = mSceneMgr->createSceneNode("laser4");
		canon3->addChild(laser4);
		laser4->attachObject(entLaser4);
		laser4->setScale(2, 0.7, 2);
		laser4->setPosition(0, -2, 0);
		
		float duration4 = 0.7;
		Ogre::Animation* animationLaser04 = mSceneMgr->createAnimation("AnimLaser04",duration4);
		animationLaser04->setInterpolationMode(Animation::IM_SPLINE);
		
		Ogre::NodeAnimationTrack* Laser04track = animationLaser04->createNodeTrack(0, laser4);
		Ogre::TransformKeyFrame* key4;
		
		key4 = Laser04track->createNodeKeyFrame(0.0);
		key4->setTranslate(Vector3(0, 0, 0));
		key4->setScale(Vector3(2, 0.7, 2));

		key4 = Laser04track->createNodeKeyFrame(0.7);
		key4->setTranslate(Vector3(0, 300, 0));
		key4->setScale(Vector3(2, 0.7, 2));

		AnimLaser4 = mSceneMgr->createAnimationState("AnimLaser04");
		AnimLaser4->setEnabled(true);
		AnimLaser4->setLoop(true);

		Ogre::Entity* entCanon4 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon4 = mSceneMgr->createSceneNode("canon4");
		torreta03->addChild(canon4);
		canon4->attachObject(entCanon4);
		canon4->setScale(0.4, 0.6, 0.4);
		canon4->setPosition(0, 9, 3);
		canon4->pitch(Degree(90));

		entTorreta03->setMaterialName("matPropio04");
		entSuperficie3->setMaterialName("matPropio04");
		entbase03->setMaterialName("matPropio04");
		entbase03_1->setMaterialName("matPropio04");
		entCanon3->setMaterialName("matPropio01");
		entCanon4->setMaterialName("matPropio03");
		torreta03->yaw(Degree(-30));


		// Torreta numero 4
		Ogre:: SceneNode* canon4_1;
		Ogre:: SceneNode* canon4_2;

		Ogre::Entity* entTorreta04 = mSceneMgr->createEntity("usb_pipe.mesh");
		torreta04 = mSceneMgr->createSceneNode("torreta04");
		mSceneMgr->getRootSceneNode()->addChild(torreta04);
		torreta04->attachObject(entTorreta04);
		torreta04->setScale(0.5, 3.7, 0.5);
		torreta04->setPosition(22, -8, -883.5);

		Ogre::Entity* entSuperficie4 = mSceneMgr->createEntity("cube.mesh");
		cubo4 = mSceneMgr->createSceneNode("cubo4");
		torreta04->addChild(cubo4);
		cubo4->attachObject(entSuperficie4);
		cubo4->setScale(0.25, 0.015, 0.17);
		cubo4->setPosition(0, 2.1, 0);

		Ogre::Entity* entCanon4_1 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon4_1 = mSceneMgr->createSceneNode("canon4_1");
		torreta04->addChild(canon4_1);
		canon4_1->attachObject(entCanon4_1);
		canon4_1->setScale(0.7, 0.27, 0.7);
		canon4_1->setPosition(5, 2, 3);
		canon4_1->pitch(Degree(90));

		// Laser 4_1
		Ogre::SceneNode* laser5;
		Ogre::Entity* entLaser5 = mSceneMgr->createEntity("usb_laser.mesh");
		laser5 = mSceneMgr->createSceneNode("laser5");
		canon4_1->addChild(laser5);
		laser5->attachObject(entLaser5);
		laser5->setScale(2, 0.7, 2);
		laser5->setPosition(0, -2, 0);
		
		float duration5 = 0.5;
		Ogre::Animation* animationLaser05 = mSceneMgr->createAnimation("AnimLaser05",duration5);
		animationLaser05->setInterpolationMode(Animation::IM_SPLINE);
		
		Ogre::NodeAnimationTrack* Laser05track = animationLaser05->createNodeTrack(0, laser5);
		Ogre::TransformKeyFrame* key5;
		
		key5 = Laser05track->createNodeKeyFrame(0.0);
		key5->setTranslate(Vector3(0, 0, 0));
		key5->setScale(Vector3(2, 0.7, 2));

		key5 = Laser05track->createNodeKeyFrame(0.5);
		key5->setTranslate(Vector3(0, 300, 0));
		key5->setScale(Vector3(2, 0.7, 2));

		AnimLaser5 = mSceneMgr->createAnimationState("AnimLaser05");
		AnimLaser5->setEnabled(true);
		AnimLaser5->setLoop(true);

		// Canon 4_2
		Ogre::Entity* entCanon4_2 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon4_2 = mSceneMgr->createSceneNode("canon4_2");
		torreta04->addChild(canon4_2);
		canon4_2->attachObject(entCanon4_2);
		canon4_2->setScale(0.7, 0.27, 0.7);
		canon4_2->setPosition(-5, 2, 3);
		canon4_2->pitch(Degree(90));

		// Laser 4_2
		Ogre::SceneNode* laser6;
		Ogre::Entity* entLaser6 = mSceneMgr->createEntity("usb_laser.mesh");
		laser6 = mSceneMgr->createSceneNode("laser6");
		canon4_2->addChild(laser6);
		laser6->attachObject(entLaser6);
		laser6->setScale(2, 0.7, 2);
		laser6->setPosition(0, -2, 0);
		
		float duration6 = 0.3;
		Ogre::Animation* animationLaser06 = mSceneMgr->createAnimation("AnimLaser06",duration6);
		animationLaser06->setInterpolationMode(Animation::IM_SPLINE);
		
		Ogre::NodeAnimationTrack* Laser06track = animationLaser06->createNodeTrack(0, laser6);
		Ogre::TransformKeyFrame* key6;
		
		key6 = Laser06track->createNodeKeyFrame(0.0);
		key6->setTranslate(Vector3(0, 0, 0));
		key6->setScale(Vector3(2, 0.7, 2));

		key6 = Laser06track->createNodeKeyFrame(0.3);
		key6->setTranslate(Vector3(0, 300, 0));
		key6->setScale(Vector3(2, 0.7, 2));

		AnimLaser6 = mSceneMgr->createAnimationState("AnimLaser06");
		AnimLaser6->setEnabled(true);
		AnimLaser6->setLoop(true);

		entTorreta04->setMaterialName("matPropio02");
		entSuperficie4->setMaterialName("matPropio04");
		entCanon4_1->setMaterialName("matPropio03");
		entCanon4_2->setMaterialName("matPropio03");
		torreta04->yaw(Degree(-30));

		entLaser->setMaterialName("matPropio07");
		entLaser2->setMaterialName("matPropio07");
		entLaser3->setMaterialName("matPropio08");
		entLaser4->setMaterialName("matPropio09");
		entLaser5->setMaterialName("matPropio09");
		entLaser6->setMaterialName("matPropio08");

		//SPACE
		mSceneMgr->setSkyDome(true, "matPropio05", 5, 8);
		
		drawShip(mSceneMgr);

		for (int i=0; i<number_wings; i++) {
			createCloseWingsAnimation(i);
			createOpenWingsAnimation(i);
		}
	}
};

int main (void)
{
	Example1 app;
	app.go();
	return 0;
}