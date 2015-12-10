#include "GPUParticleEmitter.h"

GPUParticleEmitter::GPUParticleEmitter()
	: mParticles(nullptr), mMaxParticles(0), mPosition(0, 0, 0),
	mDrawShader(0),
	mUpdateShader(0),
	mLastDrawTime(0)
{
	mVAO[0] = 0;
	mVAO[1] = 0;
	mVBO[0] = 0;
	mVBO[1] = 0;
}

GPUParticleEmitter::~GPUParticleEmitter()
{
	delete[] mParticles;

	glDeleteVertexArrays(2, mVAO);
	glDeleteBuffers(2, mVBO);

	//delete the shaders
	glDeleteProgram(mDrawShader);
	glDeleteProgram(mUpdateShader);
}

void GPUParticleEmitter::Init(unsigned int a_maxParticles, float a_lifespanMin, float a_lifespanMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const glm::vec4 & a_startColor, const glm::vec4 & a_endColor)
{
	mStartColor = a_startColor;
	mEndColor = a_endColor;
	mStartSize = a_startSize;
	mEndSize = a_endSize;
	mVelocityMin = a_velocityMin;
	mVelocityMax = a_velocityMax;
	mLifespanMin = a_lifespanMin;
	mLifespanMax = a_lifespanMax;
	mMaxParticles = a_maxParticles;

	mParticles = new GPUParticle[a_maxParticles];

	mActiveBuffer = 0;

	CreateBuffers();

	CreateUpdateShader();

	CreateDrawShader();
}

void GPUParticleEmitter::Draw(float time, const glm::mat4 & a_cameraTransform, const glm::mat4 & a_projectionView)
{
	//update particles using transform feedback
	glUseProgram(mUpdateShader);

	//bind time info
	int location = glGetUniformLocation(mUpdateShader, "time");
	glUniform1f(location, time);
	float deltaTime = time - mLastDrawTime;
	mLastDrawTime = time;
	location = glGetUniformLocation(mUpdateShader, "deltaTime");
	glUniform1f(location, deltaTime);

	//bind emitters position
	location = glGetUniformLocation(mUpdateShader, "emitterPosition");
	glUniform3fv(location, 1, &mPosition[0]);

	//disable rasterization
	glEnable(GL_RASTERIZER_DISCARD);

	//bind the buffer to update
	glBindVertexArray(mVAO[mActiveBuffer]);

	//work out the other buffer
	unsigned int otherBuffer = (mActiveBuffer + 1) % 2;

	//bind the update buffer as points and start transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mVBO[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, mMaxParticles);

	//disable transform feedback and enable rasterization
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	//draw the particles using geometry shader to billboard them
	glUseProgram(mDrawShader);

	location = glGetUniformLocation(mDrawShader, "projectionView");
	glUniformMatrix4fv(location, 1, false, &a_projectionView[0][0]);

	location = glGetUniformLocation(mDrawShader, "cameraTransform");
	glUniformMatrix4fv(location, 1, false, &a_cameraTransform[0][0]);

	location = glGetUniformLocation(mDrawShader, "myTexture");
	glUniform1i(location, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, nsfw::Assets::instance().get<nsfw::ASSET::TEXTURE>("soulspear_diffuse.tga"));
	glBindTexture(GL_TEXTURE_2D, nsfw::Assets::instance().get<nsfw::ASSET::TEXTURE>("crate_Sample"));

	//draw particles
	glBindVertexArray(mVAO[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, mMaxParticles);

	//swap for next frame
	mActiveBuffer = otherBuffer;
}

void GPUParticleEmitter::CreateBuffers()
{
	glGenVertexArrays(2, mVAO);
	glGenBuffers(2, mVBO);

	glBindVertexArray(mVAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(GPUParticle), mParticles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//velocity
	glEnableVertexAttribArray(2);//lifetime
	glEnableVertexAttribArray(3);//lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 28);

	glBindVertexArray(mVAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(GPUParticle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//velocity
	glEnableVertexAttribArray(2);//lifetime
	glEnableVertexAttribArray(3);//lifespan

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GPUParticle), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GPUParticleEmitter::CreateUpdateShader()
{
	//create a shader
	unsigned int vertexShader = LoadShader(GL_VERTEX_SHADER, "./shaders/gpuVertUpdate.txt");
	mUpdateShader = glCreateProgram();
	glAttachShader(mUpdateShader, vertexShader);

	//specify the data will stream back
	const char* varyings[] = { "position", "velocity", "lifetime", "lifespan" };
	glTransformFeedbackVaryings(mUpdateShader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(mUpdateShader);

	//remove unneeded handles
	glDeleteShader(vertexShader);

	//bind the shader so can set some uniforms that dont change per frame
	glUseProgram(mUpdateShader);

	//bind lifetime min / max
	int location = glGetUniformLocation(mUpdateShader, "lifeMin");
	glUniform1f(location, mLifespanMin);
	location = glGetUniformLocation(mUpdateShader, "lifeMax");
	glUniform1f(location, mLifespanMax);
}

void GPUParticleEmitter::CreateDrawShader()
{
	unsigned int vertexShader = LoadShader(GL_VERTEX_SHADER, "./shaders/gpuVert.txt");
	unsigned int geometryShader = LoadShader(GL_GEOMETRY_SHADER, "./shaders/gpuGeom.txt");
	unsigned int fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "./shaders/gpuFrag.txt");

	mDrawShader = glCreateProgram();
	glAttachShader(mDrawShader, vertexShader);
	glAttachShader(mDrawShader, fragmentShader);
	glAttachShader(mDrawShader, geometryShader);
	glLinkProgram(mDrawShader);

	int success = GL_FALSE;
	glGetProgramiv(mDrawShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(mDrawShader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(mDrawShader, length, 0, log);
		assert(false && "Error linking shader:" && log);
		delete[] log;
	}

	//remove unneeded handles
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);

	//bind the shader so that we can set some uniforms that don't change per-frame
	glUseProgram(mDrawShader);

	//bind size info for lerp that wont change
	int location = glGetUniformLocation(mDrawShader, "sizeStart");
	glUniform1f(location, mStartSize);
	location = glGetUniformLocation(mDrawShader, "sizeEnd");
	glUniform1f(location, mEndSize);

	//bind colorinfo for lerp that wont change
	location = glGetUniformLocation(mDrawShader, "colorStart");
	glUniform4fv(location, 1, &mStartColor[0]);
	location = glGetUniformLocation(mDrawShader, "colorEnd");
	glUniform4fv(location, 1, &mEndColor[0]);
}

unsigned int GPUParticleEmitter::LoadShader(unsigned int type, const char * path)
{
	FILE* file = fopen(path, "rb");
	if (file == nullptr)
	{
		assert(false);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	GLint success = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << "Error compiling shader.\n" << log << std::endl;
		delete[] log;
		assert(false);
	}
	delete[] source;

	return shader;
}