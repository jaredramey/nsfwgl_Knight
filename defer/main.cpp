
#include "ogl\gl_core_4_4.h"

#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"

#include "GPass.h"
#include "CPass.h"
#include "LPassD.h"
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
	const char *spassTextureNames[] = { "LPassDepth" };
	const unsigned spassDepths[] = { GL_DEPTH_COMPONENT }; // GL_RGB8
	a.makeFBO("ShadowPass", w.getWidth(), w.getHeight(), 1, spassTextureNames, spassDepths);

	// Load Shaders
	a.loadShader("GeometryPassPhong", "./shaders/geoVert.txt", "./shaders/geoFrag.txt");
	a.loadShader("LightPassDirectional", "./shaders/lightVert.txt", "./shaders/lightFrag.txt");
	//a.loadShader("LightPassPoint", "/path/to/lpass/Point/vertex", "/path/to/lpass/Point/fragment");
	a.loadShader("CompPass", "./shaders/compVert.txt", "./shaders/compFrag.txt");
	a.loadShader("ShadowPassPre", "./shaders/shadowPrepVert.txt", "./shaders/shadowPrepFrag.txt");
	a.loadShader("ShadowPassPost", "./shaders/shadowRendVert.txt", "./shaders/shadowRendFrag.txt");

	// Load any other textures and geometry we want to use
	a.loadFBX("Soulspear", "./resources/models/soulspear/soulspear.fbx");
	//a.loadOBJ("Bunny", "");

	m_camera = new Camera;
	m_camera->StartupPerspective(45, (float)w.getWidth() / w.getHeight(), .1f, 1000.0f);
	m_camera->SetView(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void DeferredApplication::onPlay()
{
	//TODO_D("Initialize our scene objects!");
	m_light     = new LightD;
	m_soulspear = new Geometry;

	m_light->color			 = glm::vec3(1, 1, 1);
	m_light->direction		 = glm::vec3(1, 2.5f, 1);
	m_light->lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
	m_light->m_lightMatrix   = glm::lookAt(m_light->direction, glm::vec3(0), glm::vec3(0, 1, 0));

	m_soulspear->mesh		 = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->tris	     = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->diffuse     = "soulspear_diffuse.tga";	// loadFBX will need to name every handle it creates,
	m_soulspear->normal      = "soulspear_normal.tga";		// These handle names may not be what your loadFBX sets 
	m_soulspear->specular    = "soulspear_specular.tga";	// them as! (Assets will report what the key names are though)
	m_soulspear->specPower   = 40.0f;
	m_soulspear->transform   = mat4(1);

	//TODO_D("Initialize our render passes!");

	m_geometryPass			 = new GPass ("GeometryPassPhong", "GeometryPass");
	//m_spotLightPass		 = new LPassD("", "");
	m_directionalLightPass   = new LPassD("LightPassDirectional", "LightPass");
	m_compositePass			 = new CPass ("CompPass", "Screen"); // Screen is defined in nsfw::Assets::init()
	m_shadowPre				 = new SPassPre("ShadowPassPre", "ShadowPass");
	m_shadowPost			 = new SPassPost("ShadowPassPost", "ShadowPass");
}

void DeferredApplication::onStep()
{
	//TODO_D("Update our game objects-- IF THEY EVEN DO ANYTHING");
	m_light->update();
	m_camera->Update(0);
	m_soulspear->update();
	
	//TODO_D("Draw all of our renderpasses!");
	m_geometryPass->prep();
	m_geometryPass->draw(*m_camera, *m_soulspear);
	m_geometryPass->post();

	m_shadowPre->prep();
	m_shadowPre->draw(*m_light, *m_soulspear);
	m_shadowPre->post();

	m_shadowPost->prep();
	m_shadowPost->draw(*m_light, *m_camera);
	m_shadowPost->post();

	m_directionalLightPass->prep();
	m_directionalLightPass->draw(*m_camera, *m_light);
	m_directionalLightPass->post();

	m_compositePass->prep();
	m_compositePass->draw();
	m_compositePass->post();
}

void DeferredApplication::onTerm()
{
	delete m_camera;
	delete m_light;
	delete m_soulspear;

	delete m_compositePass;
	delete m_geometryPass;
	delete m_directionalLightPass;
	delete m_shadowPre;
	delete m_shadowPost;
}
