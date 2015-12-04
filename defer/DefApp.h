#pragma once

#include "../nsfwgl/nsfw.h"

struct Camera;
struct Geometry;
struct LightD;
struct LightP;

class GPass;
class LPassD;
class LPassP;
class CPass;
class SPassPre;
class SPassPost;
class ParticleEmitter;
class GPUParticleEmitter;

class DeferredApplication : public nsfw::Application
{
	/*
	* Cameras
	*/
	Camera    *m_camera;

	/*
	* Geometry
	*/
	Geometry  *m_soulspear;
	Geometry  *m_soulspear2;
	Geometry  *m_bunny;
	Geometry  *m_floor;

	/*
	* Particles
	*/
	ParticleEmitter *m_testParticle;
	GPUParticleEmitter *m_GPUParticle;

	/*
	* Lights
	*/
	LightD	  *m_light;
	LightP    *m_lightP;

	/*
	* Passes
	*/
	CPass     *m_compositePass;
	GPass     *m_geometryPass;
	LPassD    *m_directionalLightPass;
	LPassP    *m_spotLightPass;
	SPassPre  *m_shadowPre;
	SPassPost *m_shadowPost;

public:
	void onInit();
	void onStep();
	void onTerm();
	void onPlay();
	void UpdateFlyCamControls(float deltaTime, float moveSpeed);
};