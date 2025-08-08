#include "Model.h"
#include "Texture.h"

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
    std::vector<MD_Math::VECTOR3> pos;
    std::vector<MD_Math::VECTOR2> texc;
    std::vector<MD_Math::VECTOR3> normal; 
    std::vector<MD_Math::VECTOR3> tangent; 
    std::vector<MD_Math::VECTOR3> bitangent;

    MD_Math::VECTOR3 temp(0.0f, 0.0f, 0.0f);
    MD_Math::VECTOR3 temp_n(0.0f, 0.0f, 0.0f);
    MD_Math::VECTOR3 temp_t(0.0f, 0.0f, 0.0f);
    MD_Math::VECTOR3 temp_bt(0.0f, 0.0f, 0.0f);

    MD_Math::VECTOR2 temp2(0.0f, 0.0f);

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {

        temp.x = mesh->mVertices[i].x;
        temp.y = mesh->mVertices[i].y;
        temp.z = mesh->mVertices[i].z;
        pos.push_back(temp);

        temp_n.x = mesh->mNormals[i].x;
        temp_n.y = mesh->mNormals[i].y;
        temp_n.z = mesh->mNormals[i].z;
        normal.push_back(temp_n);

        temp_t.x = mesh->mTangents[i].x;
        temp_t.y = mesh->mTangents[i].y;
        temp_t.z = mesh->mTangents[i].z;
        tangent.push_back(temp_t);

        temp_bt.x = mesh->mBitangents[i].x;
        temp_bt.y = mesh->mBitangents[i].y;
        temp_bt.z = mesh->mBitangents[i].z;
        bitangent.push_back(temp_bt);

        if(mesh->mTextureCoords[0]) 
        {
            temp2.x = mesh->mTextureCoords[0][i].x; 
            temp2.y = mesh->mTextureCoords[0][i].y;
            texc.push_back(temp2);
        }
        else
        {
            texc.push_back(MD_Math::VECTOR2(0.0f, 0.0f));
        }
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

   return Mesh(pos, texc, normal, tangent, bitangent, textures, indices);
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