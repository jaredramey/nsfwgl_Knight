#pragma once

#include "../nsfwgl/nsfw.h"

struct Camera;
struct Geometry;
struct LightD;

class GPass;
class LPassD;
class CPass;
class SPassPre;
class SPassPost;

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
	* Lights
	*/
	LightD	  *m_light;

	/*
	* Passes
	*/
	CPass     *m_compositePass;
	GPass     *m_geometryPass;
	LPassD    *m_directionalLightPass;
	LPassD    *m_spotLightPass;
	SPassPre  *m_shadowPre;
	SPassPost *m_shadowPost;

public:
	void onInit();
	void onStep();
	void onTerm();
	void onPlay();
};