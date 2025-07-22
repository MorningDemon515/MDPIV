#ifndef SCENE2_H
#define SCENE2_H

class Scene2
{
private:
    unsigned int VBO, VAO;
    unsigned int LightVAO;
public:
    Scene2();
    ~Scene2();
    
    void InitEnv();
    void Render();

};


#endif