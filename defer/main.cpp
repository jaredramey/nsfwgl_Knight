
#include "ogl\gl_core_4_4.h"

#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "Keyboard.h"
#include "ParticleEmitter.h"

#include "GPass.h"
#include "CPass.h"
#include "LPassD.h"
#include "LPassP.h"
#include "SPassPre.h"
#include "SPassPost.h"

using namespace nsfw;

int main()
{
	DeferredApplication d;

	d.init();
	d.play();
	d.term();

	system("pause");
}


void DeferredApplication::onInit()
{
	auto &w = nsfw::Window::instance();
	auto &a = nsfw::Assets::instance();

	// Setup FBOs
	const char *gpassTextureNames[] = { "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
	//if you're makefbo code is right, you use the specific ones here...
	const unsigned gpassDepths[] = { GL_RGB8,GL_RGB32F,GL_RGB32F,GL_DEPTH_COMPONENT }; // GL_RGB8, GL_RGB32, GL_RGB32, GL_DEPTH_COMPONENT
	a.makeFBO("GeometryPass", w.getWidth(), w.getHeight(), 4, gpassTextureNames, gpassDepths);

	//same as above
	const char *lpassTextureNames[] = { "LPassColor" };
	const unsigned lpassDepths[] = { GL_RGB8 }; // GL_RGB8
	a.makeFBO("LightPass", w.getWidth(), w.getHeight(), 1, lpassTextureNames, lpassDepths);

	//For shadow pass
	const char *spassTextureNames[] = { "ShadowMap" };
	const unsigned spassDepths[] = { GL_DEPTH_COMPONENT }; // GL_RGB8
	a.makeFBO("ShadowPass", 1024, 1024, 1, spassTextureNames, spassDepths);

	// Load Shaders
	a.loadShader("GeometryPassPhong", "./shaders/geoVert.txt", "./shaders/geoFrag.txt");
	a.loadShader("LightPassDirectional", "./shaders/lightVert.txt", "./shaders/lightFrag.txt");
	a.loadShader("LightPointPass", "./shaders/LightPVert.txt", "./shaders/LightPFrag.txt");
	a.loadShader("CompPass", "./shaders/compVert.txt", "./shaders/compFrag.txt");
	a.loadShader("ShadowPassPre", "./shaders/shadowPrepVert.txt", "./shaders/shadowPrepFrag.txt");
	a.loadShader("ShadowPassPost", "./shaders/shadowRendVert.txt", "./shaders/shadowRendFrag.txt");
	a.loadShader("GPUParticleReg", "./shaders/", "./shaders/", "./shaders/");

	// Load any other textures and geometry we want to use
	a.loadFBX("Soulspear", "./resources/models/soulspear/soulspear.fbx");
	a.loadOBJ("Bunny", "./resources/stanford_objs/bunny.obj");

	m_camera = new Camera;
	m_camera->StartupPerspective(45, (float)w.getWidth() / w.getHeight(), .1f, 100.0f);
	m_camera->SetView(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	Keyboard::Init();
}

void DeferredApplication::onPlay()
{
	//TODO_D("Initialize our scene objects!");
	m_light      = new LightD;
	m_lightP	 = new LightP;
	m_soulspear  = new Geometry;
	m_soulspear2 = new Geometry;
	m_bunny		 = new Geometry;
	m_floor		 = new Geometry;
	m_testParticle = new ParticleEmitter(100, 12, 2.0f);

	/*
	* Filling out Light data
	*/
	m_light->color			 = glm::vec3(0.0f, 1.f, 1.f);
	m_light->direction		 = glm::normalize(glm::vec3(1, 1, 1));
	m_light->lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	m_light->m_lightMatrix   = glm::lookAt(m_light->direction, glm::vec3(0), glm::vec3(0, 1, 0));

	m_lightP->color			 = glm::vec3(1.f, 1.f, 0.0f);
	m_lightP->position		 = glm::vec4(1, 2, 2, 1);
	m_lightP->attenuation.kC = 0;

	/*
	* Filling out Particle Data
	*/

	//CPU Test Particle
	m_testParticle->mesh = "Quad";
	m_testParticle->tris = "Quad";
	m_testParticle->color = "White";
	m_testParticle->transform = glm::mat4(1);
	m_testParticle->velocity = glm::vec3(0, 2, 0);

	/*
	* Filling out Geometry Data 
	*/
	//SoulSpear 1
	m_soulspear->mesh		 = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->tris	     = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->diffuse     = "soulspear_diffuse.tga";	// loadFBX will need to name every handle it creates,
	m_soulspear->normal      = "soulspear_normal.tga";		// These handle names may not be what your loadFBX sets 
	m_soulspear->specular    = "soulspear_specular.tga";	// them as! (Assets will report what the key names are though)
	m_soulspear->specPower   = 40.0f;
	m_soulspear->transform	 = mat4(1);
	m_soulspear->transform   = glm::translate(m_soulspear->transform, glm::vec3(3.0f, -0.87f, -2.0f));

	//SoulSpear 2
	m_soulspear2->mesh		 = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear2->tris		 = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear2->diffuse	 = "soulspear_diffuse.tga";	// loadFBX will need to name every handle it creates,
	m_soulspear2->normal	 = "soulspear_normal.tga";		// These handle names may not be what your loadFBX sets 
	m_soulspear2->specular   = "soulspear_specular.tga";	// them as! (Assets will report what the key names are though)
	m_soulspear2->specPower  = 40.0f;
	m_soulspear2->transform	 = glm::mat4(1); // <-- Use trnaslate to move over 
	m_soulspear2->transform  = glm::translate(m_soulspear2->transform, glm::vec3(0.25f, -0.87f, -0.5f));

	//bunny
	m_bunny->mesh	   = "Bunny";
	m_bunny->tris	   = "Bunny";
	m_bunny->diffuse   = "White";
	m_bunny->normal    = "Blue";
	m_bunny->specular  = "White";
	m_bunny->specPower = 128.f;
	m_bunny->transform = glm::translate(glm::mat4(1), glm::vec3(-10, 0, 2));

	//floor
	m_floor->mesh	   = "Quad";
	m_floor->tris	   = "Quad";
	m_floor->diffuse   = "White";
	m_floor->normal	   = "Blue";
	m_floor->specular  = "White";
	m_floor->specPower = 40.0f;
	m_floor->transform = glm::rotate(90.0f, glm::vec3(-1, 0, 0)) * glm::scale(glm::vec3(10, 10, 1));

	//TODO_D("Initialize our render passes!");

	m_geometryPass			 = new GPass ("GeometryPassPhong", "GeometryPass");
	m_spotLightPass			 = new LPassP("LightPointPass", "LightPass");
	m_directionalLightPass   = new LPassD("LightPassDirectional", "LightPass");
	m_compositePass			 = new CPass ("CompPass", "Screen"); // Screen is defined in nsfw::Assets::init()
	m_shadowPre				 = new SPassPre("ShadowPassPre", "ShadowPass");
}

void DeferredApplication::onStep()
{


	//m_floor->transform = glm::rotate(nsfw::Window::instance().getTime() * 100,glm::vec3(1.f,0.f,0.f));

	//TODO_D("Update our game objects-- IF THEY EVEN DO ANYTHING");
	m_light->update();
	m_lightP->update(nsfw::Window::instance().getTime());
	UpdateFlyCamControls(nsfw::Window::instance().getTime(), 10);
	m_camera->Update(0);
	m_soulspear->update();
	m_soulspear2->update();
	m_testParticle->update();
	
	//TODO_D("Draw all of our renderpasses!");
	/*
	* GEOMETRY PASS
	*/
	m_geometryPass->prep();
	m_geometryPass->draw(*m_camera, *m_soulspear);
	m_geometryPass->draw(*m_camera, *m_soulspear2);
	m_geometryPass->draw(*m_camera, *m_bunny);
	m_geometryPass->draw(*m_camera, *m_floor);
	m_geometryPass->draw(*m_camera, *m_testParticle);
	m_geometryPass->post();

	/*
	* SHADOW PASSES	
	*/
	m_shadowPre->prep();
	m_shadowPre->draw(*m_light, *m_soulspear);
	m_shadowPre->draw(*m_light, *m_soulspear2);
	m_shadowPre->draw(*m_light, *m_bunny);
	m_shadowPre->draw(*m_light, *m_floor);
	m_shadowPre->post();

	/*
	* LIGHT PASSES
	*/
	m_directionalLightPass->prep();
	m_directionalLightPass->draw(*m_camera, *m_light);
	m_directionalLightPass->post();

	m_spotLightPass->prep();
	m_spotLightPass->draw(*m_camera, *m_lightP);
	m_spotLightPass->post();

	/*
	* COMPOSITE PASSES
	*/
	m_compositePass->prep();
	m_compositePass->draw();
	m_compositePass->post();
}

void DeferredApplication::onTerm()
{
	delete m_camera;
	delete m_light;
	delete m_lightP;
	delete m_soulspear;
	delete m_soulspear2;
	delete m_bunny;
	delete m_floor;

	delete m_compositePass;
	delete m_geometryPass;
	delete m_directionalLightPass;
	delete m_spotLightPass;
	delete m_shadowPre;
	delete m_shadowPost;
}

void DeferredApplication::UpdateFlyCamControls(float deltaTime, float moveSpeed)
{
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		m_camera->Move(moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		m_camera->Move(-moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		m_camera->Slide(-moveSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		m_camera->Slide(-moveSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
	{
		m_camera->Slide(0, moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
	{
		m_camera->Slide(0, -moveSpeed * deltaTime);
	}
}