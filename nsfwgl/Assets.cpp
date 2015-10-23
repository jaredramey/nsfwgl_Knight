//preventing the error of an unsafe call
//#pragma warning(disable : 4996)
#include "ogl\gl_core_4_4.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "glfw\glfw3.h"
#include "STB\stb_image.h"
#include "FBX\FBXFile.h"
#include "TOL\tiny_obj_loader.h"


#include "nsfw.h"
#include <fstream>

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


bool nsfw::Assets::makeVAO(const char *name, const struct Vertex *verts, unsigned vsize,  const unsigned * tris, unsigned tsize)
{
	ASSET_LOG(GL_HANDLE_TYPE::VBO);
	ASSET_LOG(GL_HANDLE_TYPE::IBO);
	ASSET_LOG(GL_HANDLE_TYPE::VAO);
	ASSET_LOG(GL_HANDLE_TYPE::SIZE);
	//TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");

	GLuint vao, vbo, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	

	glGetError();
	glBufferData(GL_ARRAY_BUFFER, vsize, verts, GL_STATIC_DRAW);
	CheckGLError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glGetError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tsize, tris, GL_STATIC_DRAW);
	CheckGLError();

	//set up attribs
	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//normal
	glEnableVertexAttribArray(2);//tangent
	glEnableVertexAttribArray(3);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	setINTERNAL(nsfw::ASSET::VAO, (char*)name, vao);
	setINTERNAL(nsfw::ASSET::VBO, (char*)name, vbo);
	setINTERNAL(nsfw::ASSET::IBO, (char*)name, ibo);
	setINTERNAL(nsfw::ASSET::SIZE, (char*)name, tsize);

	return true;
}

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{
	ASSET_LOG(GL_HANDLE_TYPE::FBO);
	//TODO_D("Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.");

	unsigned int m_fbo;

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	std::vector<GLenum> drawBuffers;
	int colorAttachmentCount = 0;
	for (int i = 0; i < nTextures; i++)
	{
		makeTexture(names[i], w, h, depths[i], nullptr);

		GLenum attachment = (depths[i] == GL_DEPTH_COMPONENT) ? GL_DEPTH_ATTACHMENT : (GL_COLOR_ATTACHMENT0 + colorAttachmentCount);
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, get(TEXTURE, names[i]), 0);
		if (attachment != GL_DEPTH_ATTACHMENT)
		{
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachmentCount++);
		}
	}

	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

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

	setINTERNAL(FBO, (char*)name, m_fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const char *pixels)
{
	GLuint m_fboTexture;

	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	//TODO_D("Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.");

	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);

	if (nullptr == pixels && depth != GL_DEPTH_COMPONENT)
	{
		GLenum status = glGetError();
		assert(status == GL_NO_ERROR);

		glTexStorage2D(GL_TEXTURE_2D, 1, depth, w, h);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		assert(status == GL_NO_ERROR);
	}
	else   // otherwise, we're creating a normal texture
	{
		GLenum status = glGetError();
		assert(status == GL_NO_ERROR);

		glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		assert(status == GL_NO_ERROR);
	}

	CheckGLError();

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	setINTERNAL(TEXTURE, (char *)name, m_fboTexture);

	return true;
}

bool nsfw::Assets::loadTexture(const char * name, const char * path)
{
	//TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");
	
	GLuint tex;

	int imageWidth = 0, imageHeight = 0, imageForamt = 0, channels = 0;

	unsigned char* data = stbi_load(path, &imageWidth, &imageHeight, &imageForamt, STBI_default);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return true;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * fpath)
{
	ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	//TODO_D("Load shader from a file.");

	unsigned programID = glCreateProgram();
	unsigned vsahder = loadSubshader(GL_VERTEX_SHADER, vpath);
	unsigned fshader = loadSubshader(GL_FRAGMENT_SHADER, fpath);
	glAttachShader(programID, vsahder);
	glAttachShader(programID, fshader);
	glLinkProgram(programID);
	int success;

	freeSubShader(vsahder);
	freeSubShader(fshader);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);


	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("ERROR: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
		//setINTERNAL(SHADER, (char *)name, 0);
	}

	setINTERNAL(SHADER, (char *)name, programID);
	return true;
}

void nsfw::Assets::freeSubShader(unsigned int s) { glDeleteShader(s); }

unsigned int nsfw::Assets::loadSubshader(unsigned int type, const char* path)
{
	std::ifstream in(path);
	std::string contents((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	const char *src = contents.c_str();

	unsigned int shader = glCreateShader(type);

	glShaderSource(shader, 1, &src, 0);

	glCompileShader(shader);
	return shader;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	//name/meshName
	//name/textureName
	//TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");

	FBXFile fbx;
	//fbx.initialiseOpenGLTextures();

	std::vector<Vertex> verts;
	std::vector<unsigned> indicies;

	bool success = fbx.load(path, FBXFile::UNITS_METER, true, false, false);
	if (!success)
	{
		std::cout << "Error loading fbx file" << std::endl;
		return false;
	}

	assert(fbx.getMeshCount() > 0);
	for (int i = 0; i < fbx.getMeshCount(); i++)
	{
		FBXMeshNode* mesh = fbx.getMeshByIndex(0);

		for (int j = 0; j < mesh->m_vertices.size(); j++)
		{
			auto Vert = mesh->m_vertices[j];
			Vertex v;
			v.position = Vert.position;
			v.normal = Vert.normal;
			v.texCoord = Vert.texCoord1;
			verts.push_back(v);
		}

		indicies = mesh->m_indices;

		makeVAO(name, verts.data(), verts.size(), indicies.data(), indicies.size());
	}

	for (int i = 0; i < fbx.getTextureCount(); i++)
	{
		FBXTexture* tex = fbx.getTextureByIndex(i);
		switch (tex->format) {
		case 1:
			makeTexture(tex->name.c_str(), tex->width, tex->height,  GL_RG, (char*)tex->data);
			break;
		case 2:
			makeTexture(tex->name.c_str(), tex->width, tex->height, GL_RG, (char*)tex->data);
			break;
		case 3:
			makeTexture(tex->name.c_str(), tex->width, tex->height, GL_RGB, (char*)tex->data);
			break;
		case 4:
			makeTexture(tex->name.c_str(), tex->width, tex->height, GL_RGBA, (char*)tex->data);
			break;
		default:
			assert(false && "Invalid texture depth found while loading FBX object.");
			break;
		}
	}

	fbx.unload();

	return true;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)
{
	//TODO_D("OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n");
	std::vector<Vertex> verts;
	std::vector<unsigned int> indicies;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, path);
	if (err.length() != 0)
	{
		std::cout << "Error loading obj File: " << err << std::endl;
		return false;
	}

	for (int i = 0; i < shapes.size(); i++)
	{
		auto shape = shapes[i];
		auto mesh = shape.mesh;
		unsigned int posIndex = 0;
		unsigned int normalIndex = 0;
		unsigned int UVIndex = 0;
		bool hasNormals = mesh.normals.size() == mesh.positions.size();
		bool hasUVs = mesh.texcoords.size() == mesh.positions.size();

		for (unsigned int vertexCount = 0; posIndex < mesh.positions.size(); vertexCount++)
		{
			Vertex vertex;

			float x = mesh.positions[posIndex++];
			float y = mesh.positions[posIndex++];
			float z = mesh.positions[posIndex++];

			vertex.position = vec4(x, y, z, 1);

			if (hasNormals)
			{
				x = mesh.normals[normalIndex++];
				y = mesh.normals[normalIndex++];
				z = mesh.normals[normalIndex++];
				vertex.normal = vec4(x, y, z, 1);
			}

			if (hasUVs)
			{
				x = mesh.texcoords[UVIndex++];
				y = mesh.texcoords[UVIndex++];
				vertex.texCoord = vec2(x, y);
			}
			
			verts.push_back(vertex);
		}
		indicies = mesh.indices;
		makeVAO(shape.name.c_str(), verts.data(), verts.size(), indicies.data(), indicies.size());
	}

	return true;
}

void nsfw::Assets::init()
{
	//TODO_D("Load up some default assets here if you want.");
	
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
	//TODO();
	for each(std::pair<AssetKey,unsigned> k in handles)
	{
		switch (k.first.first)
		{
		case VBO:
			glDeleteBuffers(1, &k.second);
			break;
		case IBO:
			glDeleteBuffers(1, &k.second);
			break;
		case VAO:
			glDeleteVertexArrays(1, &k.second);
			break;
		case SHADER:
			glDeleteProgram(k.second);
			break;
		case TEXTURE:
			glDeleteTextures(1, &k.second);
			break;
		case RBO:
			glDeleteRenderbuffers(1, &k.second);
			break;
		case FBO:
			glDeleteFramebuffers(1, &k.second);
			break;
		}
	}
}

void nsfw::Assets::CheckGLError()
{
	unsigned error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "ERRROR: " << std::hex << error << std::endl;
		assert(false);
	}
}


