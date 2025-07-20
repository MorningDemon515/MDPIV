#ifndef SCENE_H
#define SCENE_H

class Scene
{
private:
    unsigned int VBO, VAO,texture;
public:
    Scene();
    ~Scene();
    
    void InitEnv();
    void Render();

};


#endif