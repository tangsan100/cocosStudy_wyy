#pragma once
#include "Base.h"
#include <vector>
#include "Shader.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <map>

namespace FF {
	
#define TEXTURE_DIFFUSE_STR "diffuse"
#define TEXTURE_SPECULAR_STR "specular"
#define TEXTURE_HEIGHT_STR "tangent"


	struct ffVertex 
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
		glm::vec3 tangent;

		ffVertex() {
			pos = glm::vec3(0.0f);
			normal = glm::vec3(0.0f);
			texCoord = glm::vec2(0.0f);
			tangent = glm::vec3(0.0f);
		}
	};

	struct ffTexture
	{
		uint id;
		std::string path;
		std::string type;

	};

	class ffMesh
	{
	public:
		ffMesh(std::vector<ffVertex> vertexVec, std::vector<uint> indexVec, std::vector<ffTexture> texVec);
		~ffMesh();


		void draw(Shader* shader);
	public:
		
		std::vector<ffVertex>	vertexVec;// ������Ϣ
		std::vector<uint>		indexVec;// ���������
		std::vector<ffTexture>	texVec; // ������Ϣ
	
		uint					VAO;

	private:
		void setupMesh();
	};

	class ffModel {
	public:
		ffModel(const char* path) {
			loadModel(path);
		}

		void draw(Shader* shader);
		
	public:
		std::vector<ffMesh> meshVec;
		std::string			dir;

	private:
		
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		ffMesh processMesh(aiMesh*mesh, const aiScene* scene);

		std::vector<ffTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

	class ffTextureManager
	{
	public:
		void SINGLE_OVER();
		uint createTexture(std::string path);
		uint createTexture(std::string path,std::string dir);
	private:
		SINGLE_INSTANCE(ffTextureManager)
		ffTextureManager(){}

	private:
		std::map<std::string, uint> textureMap;

	};

}

