#include "Model.h"

#include "LoadImage.h"

void Model::loadModel(std::string const &path)
{
        
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    aiMesh* mesh;
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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Vertex vertex = {MD_Math::VECTOR3(0.0f, 0.0f, 0.0f),MD_Math::VECTOR3(0.0f, 0.0f, 0.0f),MD_Math::VECTOR2(0.0f, 0.0f)};//, MD_Math::VECTOR3(0.0f, 0.0f, 0.0f), MD_Math::VECTOR3(0.0f, 0.0f, 0.0f)};
    MD_Math::VECTOR3 vector(0.0f, 0.0f, 0.0f); 
    MD_Math::VECTOR2 vec(0.0f, 0.0f);
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
          
        if(mesh->mTextureCoords[0]) 
        {
     
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        
        }
        else
            vertex.TexCoords = MD_Math::VECTOR2(0.0f, 0.0f);
            vertices.push_back(vertex);
        
    }
  
    aiFace face;

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        face = mesh->mFaces[i];
          
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);        
    }
      
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    aiString str;
    bool skip = false;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        
        mat->GetTexture(type, i, &str);
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
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); 
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    
    Image tex;
    Load_Image_GL(filename.c_str(), &tex);

    return tex.id;
}