#include "ModelImporter.h"
#include <Engine/Mesh/Mesh.h>
#include <Engine/Material/Material.h>
#include <Externals/assimp/include/assimp/Importer.hpp>
#include <Externals/assimp/include/assimp/scene.h>
#include <Externals/assimp/include/assimp/postprocess.h>
#include <iostream>
#include <vector>
#include <Engine/Actor/Actor.h>

void GetMeshes(aiNode* node, const aiScene* scene, std::vector<aiMesh*>& o_meshes);
void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Engine::Graphics::Mesh*>& o_meshes, std::vector<Engine::Graphics::Material*>& o_materials);

void Engine::Utility::ImportModel(const std::string path, Actor*& o_actor)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	std::string directory = path.substr(0, path.find_last_of('/'));

	std::vector<aiMesh*> aiMeshes;

	std::vector<Engine::Graphics::Mesh*> meshes;
	std::vector<Engine::Graphics::Material*> materials;

	GetMeshes(scene->mRootNode, scene, aiMeshes);

	for (unsigned int i = 0; i < aiMeshes.size(); i++) {
		ProcessMesh(aiMeshes[i], scene, meshes, materials);
	}

	o_actor = new Actor(meshes, materials);

	for (unsigned int i = 0; i < meshes.size(); i++) {
		Graphics::Mesh::DestroyMesh(meshes[i]);
	}

}

void GetMeshes(aiNode * node, const aiScene * scene, std::vector<aiMesh*>& o_meshes)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		o_meshes.push_back(scene->mMeshes[node->mMeshes[i]]);
	}

	for (unsigned int j = 0; j < node->mNumChildren; j++) {
		GetMeshes(node->mChildren[j], scene, o_meshes);
	}
}

void ProcessMesh(aiMesh * aiMesh, const aiScene * scene, std::vector<Engine::Graphics::Mesh*>& o_meshes, std::vector<Engine::Graphics::Material*>& o_materials)
{
	Engine::Graphics::VertexFormat::Mesh* vertexData = new Engine::Graphics::VertexFormat::Mesh[aiMesh->mNumVertices];
	uint32_t* indices = new uint32_t[aiMesh->mNumFaces * aiMesh->mFaces[0].mNumIndices];
	
	for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
		vertexData[i].position.x = aiMesh->mVertices[i].x;
		vertexData[i].position.y = aiMesh->mVertices[i].y;
		vertexData[i].position.z = aiMesh->mVertices[i].z;
		
		if (aiMesh->HasVertexColors(0)) {			
			vertexData[i].color.r = aiMesh->mColors[0][i].r;
			vertexData[i].color.g = aiMesh->mColors[0][i].g;
			vertexData[i].color.b = aiMesh->mColors[0][i].b;
		}
		else {
			vertexData[i].color.r = 80.0f / 255.0f;
			vertexData[i].color.g = 80.0f / 255.0f;
			vertexData[i].color.b = 80.0f / 255.0f;
		}
		
		if (aiMesh->HasTextureCoords(0)) {
			vertexData[i].UV.u = aiMesh->mTextureCoords[0][i].x;
			vertexData[i].UV.v = aiMesh->mTextureCoords[0][i].y;
		}

		vertexData[i].normal.x = aiMesh->mNormals[i].x;
		vertexData[i].normal.y = aiMesh->mNormals[i].y;
		vertexData[i].normal.z = aiMesh->mNormals[i].z;
	}

	for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
		for (unsigned int j = 0; j < aiMesh->mFaces[i].mNumIndices; j++) {
			indices[j + (i * 3)] = aiMesh->mFaces[i].mIndices[j];
		}
	}

	Engine::Graphics::Mesh* mesh = Engine::Graphics::Mesh::CreateMesh(vertexData, aiMesh->mNumVertices, indices, aiMesh->mNumFaces * aiMesh->mFaces[0].mNumIndices);
	o_meshes.push_back(mesh);

	delete[] vertexData;
	delete[] indices;
}
