//preventing the error of an unsafe call
#pragma warning(disable : 4996)
#include "ogl\gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "glfw\glfw3.h"

#include "nsfw.h"

using namespace nsfw::ASSET;

const char *nsfw::TYPE_NAMES[eSIZE + 1] = { "NONE","vao","ibo","vbo","tri-size","fbo","rbo","texture","shader","SIZE" };

#ifdef _DEBUG
#define ASSET_LOG(type) do { std::cerr << "Trying to create key: <" << TYPE_NAMES[type] << ">" << name << std::endl; } while(0)
#else 
#define ASSET_LOG(type) do {} while (0)
#endif


nsfw::GL_HANDLE nsfw::Assets::getVERIFIED(const AssetKey &key) const
{
#ifdef _DEBUG
			if (!handles.count(key))
			{
				std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
				return 0;
			}
#endif
			return handles.at(key);
}

bool nsfw::Assets::setINTERNAL(ASSET::GL_HANDLE_TYPE t, char *name, GL_HANDLE handle)
{
	AssetKey key(t, name);
#ifdef _DEBUG
	if (handles.count(key))
	{
		std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
		return false;
	}
	else std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
	handles[key] = handle;
	return true;
}


bool nsfw::Assets::makeVAO(const char * name, const struct Vertex *verts, unsigned vsize,  const unsigned * tris, unsigned tsize)
{
	ASSET_LOG(GL_HANDLE_TYPE::VBO);
	ASSET_LOG(GL_HANDLE_TYPE::IBO);
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);
	TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");

	unsigned int m_vbo, m_ibo, m_vao, m_size = vsize;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vsize, verts, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize, tris, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, vsize, GL_FLOAT, GL_FALSE, sizeof(Vertex) * vsize, tris);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, tsize, GL_FLOAT, GL_FALSE, sizeof(Vertex) * tsize, tris);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	setINTERNAL(nsfw::ASSET::VAO, (char*)name, m_vao);
	setINTERNAL(nsfw::ASSET::VBO, (char*)name, m_vbo);
	setINTERNAL(nsfw::ASSET::IBO, (char*)name, m_ibo);
	setINTERNAL(nsfw::ASSET::SIZE, (char*)name, m_size);

	return false;
}

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{
	ASSET_LOG(GL_HANDLE_TYPE::FBO);
	TODO_D("Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.");

	unsigned int m_fbo;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	for (int i = 0; i < nTextures; i++)
	{
		makeTexture(names[i], w, h, depths[i], nullptr);

		glFramebufferTexture(GL_FRAMEBUFFER, (depths[i] == GL_DEPTH_COMPONENT) ? GL_DEPTH_ATTACHMENT : (GL_COLOR_ATTACHMENT0 + i++), get(nsfw::ASSET::TEXTURE, names[i]), 0);
	}

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum drawBuffers[] = 
	{ GL_COLOR_ATTACHMENT0,
	  GL_COLOR_ATTACHMENT1,
	  GL_COLOR_ATTACHMENT2,
	  GL_COLOR_ATTACHMENT3,
	  GL_COLOR_ATTACHMENT4,
	  GL_COLOR_ATTACHMENT5,
	  GL_COLOR_ATTACHMENT6,
      GL_COLOR_ATTACHMENT7 };

	glDrawBuffers(nTextures, drawBuffers);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	//super debuggy status check
	switch (status)
	{
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("Incomplete Attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("Missing Attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("Incomplete Draw Buffer");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf("Incomplete Read Buffer");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:\
		printf("Unsuported Framebuffer");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf("Incomplete Multisample");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		printf("Incomplete Layer Targets");
		break;
	}

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		glGetError();
		printf("FrameBuffer Error!\n");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const char *pixels)
{
	unsigned int m_fboTexture;

	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	TODO_D("Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.");

	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);

	glTexStorage2D(GL_TEXTURE_2D, 1, depth, w, h);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);

	setINTERNAL(nsfw::ASSET::TEXTURE, (char*)name, m_fboTexture);

	return true;
}

bool nsfw::Assets::loadTexture(const char * name, const char * path)
{
	TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");



	return false;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * fpath)
{
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	TODO_D("Load shader from a file.");



	return false;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	//name/meshName
	//name/textureName
	TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");
	return false;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)
{
	TODO_D("OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n");
	return false;
}

void nsfw::Assets::init()
{
	TODO_D("Load up some default assets here if you want.");
	
	setINTERNAL(FBO,"Screen",0);
	
	makeVAO("Cube",CubeVerts,24,CubeTris,36);
	makeVAO("Quad",QuadVerts, 4, QuadTris,6);
	/*
	char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGB8, w);
	*/

}

void nsfw::Assets::term()
{
	TODO();
	for each(std::pair<AssetKey,unsigned> k in handles)
	{
		switch (k.first.first)
		{
		case VBO:		TODO_D("VBO deletion");		break;
		case IBO:		TODO_D("IBO deletion");		break;
		case VAO:		TODO_D("VAO deletion");		break;
		case SHADER:	TODO_D("Shader deletion");	break;
		case TEXTURE:	TODO_D("Texture deletion"); break;
		case RBO:		TODO_D("RBO deletion");		break;
		case FBO:		TODO_D("FBO deletion");		break;
		}
	}
}


