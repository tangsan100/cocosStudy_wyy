#include "IO.h"
#include "AImage.h"
#include <string>

namespace FF {
	ffMesh::ffMesh(std::vector<ffVertex> vertexVec, std::vector<uint> indexVec, std::vector<ffTexture> texVec) {
		this->vertexVec = vertexVec;
		this->indexVec = indexVec;
		this->texVec = texVec;

		setupMesh();
	}
	ffMesh::~ffMesh() {

	}


	void ffMesh::draw(Shader* shader) {

		uint diffuseN = 1, specularN = 1;
		for (int i = 0; i < texVec.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string typeName = texVec[i].type;
			std::string num;
			if (typeName == TEXTURE_DIFFUSE_STR)
			{
				num = std::to_string(diffuseN++);
			}
			if (typeName == TEXTURE_SPECULAR_STR)
			{
				num = std::to_string(specularN++);
			}

			shader->setInt("myMaterial."+ typeName + num, i);
			glBindTexture(GL_TEXTURE_2D, texVec[i].id);
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(VAO);

		// ebo 的渲染方式， 
		//参数1： 三角形;
		//参数2: 顶点数; 
		//参数3：参数类型; 
		//参数4： 从哪个位置开始绘制
		glDrawElements(GL_TRIANGLES, indexVec.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void ffMesh::setupMesh() {
		uint VBO = 0, EBO = 0;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexVec), &vertexVec[0], GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexVec), &indexVec[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)(offsetof(ffVertex,normal)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)(offsetof(ffVertex, texCoord)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void ffModel::draw(Shader* shader) {
		for (uint i = 0; i < meshVec.size(); ++ i)
		{
			ffMesh mesh = meshVec[i];
			mesh.draw(shader);
		}
	}

	void ffModel::loadModel(std::string path) {
		Assimp::Importer importer;
		//读取模型， 参数1：路径
		// 参数2：后处理，三角形| 反转UV 坐标
		const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "model read fail" << std::endl;
		}

		dir = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);

	}
	void ffModel::processNode(aiNode* node, const aiScene* scene) {
		for (uint i =0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshVec.push_back(processMesh(mesh, scene));
		}

		for (uint i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	ffMesh ffModel::processMesh(aiMesh*mesh, const aiScene* scene) {
		std::vector<ffVertex>	vertexVec;// 顶点信息
		std::vector<uint>		indexVec;// 顶点的索引
		std::vector<ffTexture>	texVec; // 纹理信息

		for (uint i = 0; i < mesh->mNumVertices; ++i)
		{
			ffVertex vertext;
			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertext.pos = pos;

			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertext.normal = normal;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoord(0.0f);
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;
				vertext.texCoord = texCoord;
			}

			vertexVec.push_back(vertext);
		}

		// 解析indices
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (uint j = 0; j < face.mNumIndices; ++j)
			{
				indexVec.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >=0)
		{
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<ffTexture> diffuse = loadMaterialTextures(mat, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE_STR);
			texVec.insert(texVec.end(), diffuse.begin(), diffuse.end());

			std::vector<ffTexture> specular = loadMaterialTextures(mat, aiTextureType_SPECULAR, TEXTURE_SPECULAR_STR);
			texVec.insert(texVec.end(), specular.begin(), specular.end());
		}

		return ffMesh(vertexVec, indexVec, texVec);
	}

	std::vector<ffTexture> ffModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<ffTexture> texVec;

		for (uint i= 0; i < mat->GetTextureCount(type); ++i)
		{
			ffTexture tex;
			aiString path;
			// 获取文件的相对路径，相对于模型的路径
			mat->GetTexture(type, i, &path);

			tex.id = ffTextureManager::getInstance()->createTexture(path.C_Str(), dir);
			tex.path = path.C_Str();
			tex.type = typeName;

			texVec.push_back(tex);
		
		}

		return texVec;
	}

	ffTextureManager* ffTextureManager::mInstance = nullptr;
	void ffTextureManager::SINGLE_OVER() {

	}
	uint ffTextureManager::createTexture(std::string path) {
		std::map<std::string, uint>::iterator it = textureMap.find(path);
		if (it != textureMap.end())
		{
			return it->second;
		}

		AImage * image = AImage::loadImage(path);
		uint texID = 0;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData()
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		delete image;
		textureMap[path] = texID;

		return texID;
	}
	uint ffTextureManager::createTexture(std::string path, std::string dir) {
		return createTexture(dir + '/' + path);
	}
}
