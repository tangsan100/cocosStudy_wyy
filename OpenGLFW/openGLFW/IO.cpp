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
		for (uint i = 0; i < texVec.size(); i++)
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

			if (typeName == TEXTURE_HEIGHT_STR)
			{
				num = std::to_string(0); // 法线贴图
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
		glGenBuffers(1, &EBO);


		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertexVec.size()*sizeof(ffVertex), &vertexVec[0], GL_STATIC_DRAW);

		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*indexVec.size(), &indexVec[0], GL_STATIC_DRAW);

		// 顶点
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)0);
		// 法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)(offsetof(ffVertex,normal)));
		// uv
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)(offsetof(ffVertex, texCoord)));

		//切线
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)(offsetof(ffVertex, tangent)));

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
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
		// 参数2：后处理，三角形| 反转UV 坐标|模型顶点对于有tangent 信息做预处理
		const aiScene* scene = importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs|aiProcess_CalcTangentSpace);

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
			// 顶点
			glm::vec3 pos;
			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertext.pos = pos;

			//  法线
			glm::vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertext.normal = normal;

			// 贴图uv
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 texCoord(0.0f);
				texCoord.x = mesh->mTextureCoords[0][i].x;
				texCoord.y = mesh->mTextureCoords[0][i].y;
				vertext.texCoord = texCoord;
			}

			// 法线贴图，切线
			if (mesh->mTangents)
			{
				glm::vec3 tangent;
				tangent.x = mesh->mTangents[i].x;
				tangent.y = mesh->mTangents[i].y;
				tangent.z = mesh->mTangents[i].z;
				vertext.tangent = tangent;
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

			std::vector<ffTexture> normalMaps = loadMaterialTextures(mat, aiTextureType_HEIGHT, TEXTURE_HEIGHT_STR);
			texVec.insert(texVec.end(), normalMaps.begin(), normalMaps.end());
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
