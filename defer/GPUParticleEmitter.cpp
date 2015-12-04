#include "GPUParticleEmitter.h"

//GPUParticleEmitter::GPUParticleEmitter()
//{
//}
//
//
//GPUParticleEmitter::~GPUParticleEmitter()
//{
//}

void GPUParticleEmitter::init(const char *nameOne, const char *nameTwo, float a_maxParticles, float lifeTimeMin, float lifeTimeMax, float velocityMin, float velocityMax, float StartSize, float EndSize, const glm::vec4 StartColor, const glm::vec4 EndColor, unsigned size)
{
	startColor = StartColor;
	endColor = EndColor;
	sizeStart = StartSize;
	sizeEnd = EndSize;
	veloMin = velocityMin;
	veloMax = velocityMax;
	lifeMin = lifeTimeMin;
	lifeMax = lifeTimeMax;
	maxParticles = a_maxParticles;

	particles = new nsfw::ParticleVertex[maxParticles];

	//Create VAO's
	//nsfw::Assets::makeVAO(nameOne, particles, size);
	nsfw::Assets::instance().makeVAO(nameOne, particles, maxParticles);
	nsfw::Assets::instance().makeVAO(nameTwo, particles, maxParticles);

	activeBuffer = 0;

	vao[0] = nsfw::Assets::instance().get<nsfw::ASSET::VAO>(nameOne);
	vao[1] = nsfw::Assets::instance().get<nsfw::ASSET::VAO>(nameTwo);
	vbo[0] = nsfw::Assets::instance().get<nsfw::ASSET::VBO>(nameOne);
	vbo[1] = nsfw::Assets::instance().get<nsfw::ASSET::VBO>(nameTwo);

	FinishUpdateShaderCreation();
}

void GPUParticleEmitter::FinishUpdateShaderCreation()
{
	glUseProgram(*updateShader);

	setUniform("lifeMin", nsfw::UNIFORM::TYPE::FLO1, &lifeMin);
	setUniform("lifeMax", nsfw::UNIFORM::TYPE::FLO1, &lifeMax);
}

void GPUParticleEmitter::prep()
{
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	glEnable(GL_RASTERIZER_DISCARD);

	glUseProgram(*shader);
}

void GPUParticleEmitter::post()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);

	delete[] particles;
}

void GPUParticleEmitter::draw(float dT, Camera &c)
{
	setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
	setUniform("deltaTime", nsfw::UNIFORM::TYPE::FLO1, &dT);
	setUniform("emmiterPosition", nsfw::UNIFORM::TYPE::FLO3, &pos[0]);

	glBindVertexArray(vao[activeBuffer]);

	unsigned int otherBuffer = (activeBuffer + 1) % 2;

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vao[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, maxParticles);

	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	glUseProgram(*shader);

	setUniform("projectionView", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
	setUniform("cameraTransform", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetWorldTransform()));

	glBindVertexArray(vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, maxParticles);

	activeBuffer = otherBuffer;
}

void GPUParticleEmitter::update()
{

}