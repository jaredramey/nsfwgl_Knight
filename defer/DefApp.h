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
	Camera    *m_camera;
	Geometry  *m_soulspear;
	Geometry  *m_soulspear2;
	LightD	  *m_light;

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