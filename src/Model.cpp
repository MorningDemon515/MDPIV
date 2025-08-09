#include "Model.h"
#include "Texture.h"

#include <assert.h>

using namespace MD_Math;

void Model::LoadModel(std::string path)
{
    scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);    

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));       
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vers;

    Vertex temp = {
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR2(0.0f, 0.0f),
        VECTOR3(0.0f, 0.0f, 0.0f),
        VECTOR3(0.0f, 0.0f, 0.0f)
    };

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        temp.Position.x = mesh->mVertices[i].x;
        temp.Position.y = mesh->mVertices[i].y;
        temp.Position.z = mesh->mVertices[i].z;

        temp.Normals.x = mesh->mNormals[i].x;
        temp.Normals.y = mesh->mNormals[i].y;
        temp.Normals.z = mesh->mNormals[i].z;

        temp.Tangent.x = mesh->mTangents[i].x;
        temp.Tangent.y = mesh->mTangents[i].y;
        temp.Tangent.z = mesh->mTangents[i].z;

        temp.Bitangent.x = mesh->mBitangents[i].x;
        temp.Bitangent.y = mesh->mBitangents[i].y;
        temp.Bitangent.z = mesh->mBitangents[i].z;

        if(mesh->mTextureCoords[0]) 
        {
            temp.TexCoords.x = mesh->mTextureCoords[0][i].x; 
            temp.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            temp.TexCoords = MD_Math::VECTOR2(0.0f, 0.0f);
        }

        vers.push_back(temp);
    }

    std::vector<unsigned int> indices;

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    std::vector<Texture> textures;

    material = scene->mMaterials[mesh->mMaterialIndex];    

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
   
    texCount = textures.size();

   return Mesh(vers, textures, indices);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {   
            std::string filename = std::string(str.C_Str());
            filename = this->directory + "/" + filename;

            Texture texture;
            texture.id = TextureFromFile(filename.c_str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}