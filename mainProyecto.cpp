#include "Ogre\ExampleApplication.h"
#include "OGRE\OgreManualObject.h"
#include <vector>
#include <string>

#define v3 Ogre::Vector3
#define v2 Ogre::Vector2
float pitch_degree[4] = {30.0, -30.0, 30.0, -30.0};


float xlaser1 = 0, zlaser1 = 5;
Ogre::AnimationState* AnimationLaser01;

class FrameListenerClase : public Ogre::FrameListener{

private:
	Ogre::SceneNode* _nodoF01;
	Ogre::Entity* _entOgre;
	Ogre::AnimationState* _anim;
	OIS::InputManager* _man;
	OIS::Keyboard* _key;
	OIS::Mouse* _mouse;
	Ogre::Camera* _cam;

public:
	FrameListenerClase(Ogre::SceneNode* nodo01,Ogre::Entity* entOgre01 ,Ogre::Camera* cam, RenderWindow* win){

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
		_nodoF01 = nodo01;
		_entOgre = entOgre01;

		_anim = _entOgre->getAnimationState("Dance");
		_anim->setEnabled(true);
		_anim->setLoop(true);
	}


	~FrameListenerClase(){
		_man->destroyInputObject(_key);
		_man->destroyInputObject(_mouse);
		OIS::InputManager::destroyInputSystem(_man);
	}

	bool frameStarted(const Ogre::FrameEvent &evt){
		_key->capture();
		_mouse->capture();

		float movSpeed=10.0f;
		Ogre::Vector3 tmov(0,0,0);
		Ogre::Vector3 tcam(0,0,0);

		//Camara
		if (_key->isKeyDown(OIS::KC_ESCAPE))
			return false;

		if(_key->isKeyDown(OIS::KC_W))
			tcam += Ogre::Vector3(0,0,-10);
		
		if(_key->isKeyDown(OIS::KC_S))
			tcam += Ogre::Vector3(0,0,10);

		if(_key->isKeyDown(OIS::KC_A))
			tcam += Ogre::Vector3(-10,0,0);
		
		if(_key->isKeyDown(OIS::KC_D))
			tcam += Ogre::Vector3(10,0,0);


		//camara control
		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame*-1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame*-1;
		_cam->yaw(Ogre::Radian(rotX));
		_cam->pitch(Ogre::Radian(rotY));
		_cam->moveRelative(tcam*movSpeed*evt.timeSinceLastFrame);
		_nodoF01->translate(tmov * evt.timeSinceLastFrame);
		_anim->addTime(evt.timeSinceLastFrame*3);

		return true;
	}
};

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

		SceneNode* left_wing1 = ship->createChildSceneNode();
		float left_degrees[] = {180.0, -90.0, 90.0};
		createWing(manager, left_wing1, std::string("ship_left_wing1"), wing_size, left_degrees, v3(wing_proximity, 0.0, 0.0));
		left_wing1->pitch(Degree(pitch_degree[0]));

		// Create cylinder left wing
		Ogre::Entity* entCylinderWing01 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing01 = mSceneMgr->createSceneNode("cylinderWing01");
		left_wing1->addChild(cylinderWing01);
		cylinderWing01->attachObject(entCylinderWing01);
		cylinderWing01->setScale(0.025, 0.35, 0.25);
		cylinderWing01->setPosition(2.3, 5, 1);
		cylinderWing01->roll(Degree(90));
		

		SceneNode* left_wing2 = ship->createChildSceneNode();
		createWing(manager, left_wing2, std::string("ship_left_wing2"), wing_size, left_degrees, v3(wing_proximity, -1.0, 0.0));
		left_wing2->pitch(Degree(pitch_degree[1]));

		// Create cylinder left wing 02
		Ogre::Entity* entCylinderWing02 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing02 = mSceneMgr->createSceneNode("cylinderWing02");
		left_wing2->addChild(cylinderWing02);
		cylinderWing02->attachObject(entCylinderWing02);
		cylinderWing02->setScale(0.025, 0.35, 0.25);
		cylinderWing02->setPosition(2.3, 5, 0.5);
		cylinderWing02->roll(Degree(90));
		
		SceneNode* right_wing1 = ship->createChildSceneNode();
		float right_degrees[] = {180.0, -90.0, -90.0};
		createWing(manager, right_wing1, std::string("ship_right_wing1"), wing_size, right_degrees, v3(-wing_proximity, 0.0, 8.0));
		right_wing1->pitch(Degree(pitch_degree[2]));

		// Create cylinder right wing 01
		Ogre::Entity* entCylinderWing03 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing03 = mSceneMgr->createSceneNode("cylinderWing03");
		right_wing1->addChild(cylinderWing03);
		cylinderWing03->attachObject(entCylinderWing03);
		cylinderWing03->setScale(0.025, 0.35, 0.25);
		cylinderWing03->setPosition(-0.3, 5, 0.9);
		cylinderWing03->roll(Degree(90));

		SceneNode* right_wing2 = ship->createChildSceneNode();
		createWing(manager, right_wing2, std::string("ship_right_wing2"), wing_size, right_degrees, v3(-wing_proximity, -1.0, 8.0));
		right_wing2->pitch(Degree(pitch_degree[3]));

		// Create cylinder right wing 01
		Ogre::Entity* entCylinderWing04 = mSceneMgr->createEntity("usb_cilindro.mesh");
		Ogre:: SceneNode* cylinderWing04 = mSceneMgr->createSceneNode("cylinderWing04");
		right_wing2->addChild(cylinderWing04);
		cylinderWing04->attachObject(entCylinderWing04);
		cylinderWing04->setScale(0.025, 0.35, 0.25);
		cylinderWing04->setPosition(-0.3, 5, 0.2);
		cylinderWing04->roll(Degree(90));

		entCylinderWing01->setMaterialName("matPropio04");
		entCylinderWing02->setMaterialName("matPropio04");
		entCylinderWing03->setMaterialName("matPropio04");
		entCylinderWing04->setMaterialName("matPropio04");

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

		xlaser1 += 0.1;
		zlaser1 += 0.1;

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
		laser1->setPosition(xlaser1, 2.8, zlaser1);
		laser1->pitch(Degree(90));
	
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
		canon2->pitch(Degree(75));

		Ogre::Entity* entCanon2_2 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon2_2 = mSceneMgr->createSceneNode("canon2_2");
		torreta02->addChild(canon2_2);
		canon2_2->attachObject(entCanon2_2);
		canon2_2->setScale(0.2, 0.5, 0.2);
		canon2_2->setPosition(1, 3.2, 3);
		canon2_2->pitch(Degree(75));


		entTorreta02->setMaterialName("matPropio04");
		entSuperficie2->setMaterialName("matPropio04");
		entCanon2->setMaterialName("matPropio03");
		entCanon2_2->setMaterialName("matPropio03");
		torreta02->yaw(Degree(30));

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
		canon3->pitch(Degree(75));

		Ogre::Entity* entCanon4 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon4 = mSceneMgr->createSceneNode("canon4");
		torreta03->addChild(canon4);
		canon4->attachObject(entCanon4);
		canon4->setScale(0.4, 0.6, 0.4);
		canon4->setPosition(0, 9, 3);
		canon4->pitch(Degree(75));

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
		canon4_1->pitch(Degree(75));

		Ogre::Entity* entCanon4_2 = mSceneMgr->createEntity("usb_cilindro.mesh");
		canon4_2 = mSceneMgr->createSceneNode("canon4_2");
		torreta04->addChild(canon4_2);
		canon4_2->attachObject(entCanon4_2);
		canon4_2->setScale(0.7, 0.27, 0.7);
		canon4_2->setPosition(-5, 2, 3);
		canon4_2->pitch(Degree(75));

		entTorreta04->setMaterialName("matPropio02");
		entSuperficie4->setMaterialName("matPropio04");
		entCanon4_1->setMaterialName("matPropio03");
		entCanon4_2->setMaterialName("matPropio03");
		torreta04->yaw(Degree(-30));

		//SPACE
		mSceneMgr->setSkyDome(true, "matPropio05", 5, 8);

		drawShip(mSceneMgr);
	}
};

int main (void)
{
	Example1 app;
	app.go();
	return 0;
}