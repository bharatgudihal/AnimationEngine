#include "ModelImporter.h"
#include <Engine/Mesh/Mesh.h>
#include <Engine/Material/Material.h>
#include <Engine/Texture/Texture2D.h>
#include <Externals/assimp/include/assimp/Importer.hpp>
#include <Externals/assimp/include/assimp/scene.h>
#include <Externals/assimp/include/assimp/postprocess.h>
#include <iostream>
#include <vector>
#include <Engine/Actor/Actor.h>

void GetMeshes(aiNode* node, const aiScene* scene, std::vector<aiMesh*>& o_meshes);
void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Engine::Graphics::Mesh*>& o_meshes, std::vector<Engine::Graphics::Material*>& o_materials, const std::string& directory);
void ExtractVertexData(aiMesh * aiMesh, std::vector<Engine::Graphics::Mesh *> &o_meshes);
void ExtractMaterialData(const aiScene * scene, aiMesh * aiMesh, const std::string& directory, std::vector<Engine::Graphics::Material *> &o_materials);

void Engine::Utility::ImportModel(const std::string path, Actor*& o_actor)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	std::string directory = path.substr(0, path.find_last_of('/') + 1);

	std::vector<aiMesh*> aiMeshes;

	std::vector<Engine::Graphics::Mesh*> meshes;
	std::vector<Engine::Graphics::Material*> materials;

	GetMeshes(scene->mRootNode, scene, aiMeshes);

	for (unsigned int i = 0; i < aiMeshes.size(); i++) {
		ProcessMesh(aiMeshes[i], scene, meshes, materials, directory);
	}

	o_actor = new Actor(meshes, materials);

	for (unsigned int i = 0; i < meshes.size(); i++) {
		Graphics::Mesh::DestroyMesh(meshes[i]);
	}

	for (unsigned int i = 0; i < materials.size(); i++) {
		Graphics::Material::DestroyMaterial(materials[i]);
	}

	aiMeshes.clear();
	meshes.clear();
	materials.clear();

	importer.FreeScene();
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

void ProcessMesh(aiMesh * aiMesh, const aiScene * scene, std::vector<Engine::Graphics::Mesh*>& o_meshes, std::vector<Engine::Graphics::Material*>& o_materials, const std::string& directory)
{

	//Extract Vertex data
	ExtractVertexData(aiMesh, o_meshes);
	
	//Extract Material Data
	ExtractMaterialData(scene, aiMesh, directory, o_materials);

}

void ExtractVertexData(aiMesh * aiMesh, std::vector<Engine::Graphics::Mesh *> &o_meshes)
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
			// Grey color
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

		if (aiMesh->HasTangentsAndBitangents()) {
			vertexData[i].tangent.x = aiMesh->mTangents[i].x;
			vertexData[i].tangent.y = aiMesh->mTangents[i].y;
			vertexData[i].tangent.z = aiMesh->mTangents[i].z;

			vertexData[i].bitangent.x = aiMesh->mBitangents[i].x;
			vertexData[i].bitangent.y = aiMesh->mBitangents[i].y;
			vertexData[i].bitangent.z = aiMesh->mBitangents[i].z;
		}
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

Engine::Graphics::Texture2D* GetTextureFromMaterial(aiMaterial* material, const aiTextureType textureType, const std::string& directory){
	aiString file;
	material->GetTexture(textureType, 0, &file);
	std::string filePath = std::string(file.C_Str());
	filePath = filePath.substr(filePath.find_last_of("\\") + 1);
	filePath = directory + filePath;
	return Engine::Graphics::Texture2D::CreateTexture(filePath.c_str());
}

void ExtractMaterialData(const aiScene * scene, aiMesh * aiMesh, const std::string& directory, std::vector<Engine::Graphics::Material *> &o_materials)
{
	aiMaterial* aiMaterial = scene->mMaterials[aiMesh->mMaterialIndex];
	unsigned int diffuseCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
	unsigned int specularCount = aiMaterial->GetTextureCount(aiTextureType_SPECULAR);
	unsigned int normalCount = aiMaterial->GetTextureCount(aiTextureType_HEIGHT);
	
	Engine::Graphics::Texture2D* diffuseTexture = nullptr;
	Engine::Graphics::Texture2D* specularTexture = nullptr;
	Engine::Graphics::Texture2D* normalMap = nullptr;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	if (diffuseCount > 0) {
		diffuseTexture = GetTextureFromMaterial(aiMaterial, aiTextureType_DIFFUSE, directory);
	}
	else {
		aiColor3D aiDiffuse;
		aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);
		diffuseColor.r = aiDiffuse.r;
		diffuseColor.g = aiDiffuse.g;
		diffuseColor.b = aiDiffuse.b;
	}

	if (specularCount > 0) {		
		specularTexture = GetTextureFromMaterial(aiMaterial, aiTextureType_SPECULAR, directory);
	}
	else {
		aiColor3D aiSpecular;
		aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular);
		specularColor.r = aiSpecular.r;
		specularColor.g = aiSpecular.g;
		specularColor.b = aiSpecular.b;
	}

	if (normalCount > 0) {
		normalMap = GetTextureFromMaterial(aiMaterial, aiTextureType_HEIGHT, directory);
	}

	Engine::Graphics::Material* material = Engine::Graphics::Material::CreateMaterial(diffuseTexture, specularTexture, diffuseColor, specularColor);
	material->SetNormalMap(normalMap);
	o_materials.push_back(material);

	if (diffuseTexture) {
		Engine::Graphics::Texture2D::DestroyTexture(diffuseTexture);
	}

	if (specularTexture) {
		Engine::Graphics::Texture2D::DestroyTexture(specularTexture);
	}

	if (normalMap) {
		Engine::Graphics::Texture2D::DestroyTexture(normalMap);
	}
}