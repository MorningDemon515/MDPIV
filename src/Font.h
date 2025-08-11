#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glad/glad.h>
#include "Shader.h"

#include <iostream>
#include <map>

struct Character {
    unsigned int TextureID;
    MD_Math::IVECTOR2   Size; 
    MD_Math::IVECTOR2   Bearing;   
    unsigned int Advance;  
};

class Font
{
public:
    Font(){};
    Font(const char* fontpath, int size);
    ~Font();

    void Draw(std::string text, float x, float y, float z, float scale);
private:
    std::map<char, Character> Characters;  
    unsigned int textVAO, textVBO;
};

#endif