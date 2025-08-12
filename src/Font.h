#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <glad/glad.h>
#include "Shader.h"

#include <iostream>
#include <map>
#include <vector>

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
    Font(std::string text, const char* fontpath, int size);
    ~Font();

    void Draw(float x, float y, float z, float scale);
private:
    std::map<unsigned int, Character> Characters; 
    unsigned int textVAO, textVBO;

//From https://github.com/Saalvage/Blitz3D/commit/232d253f5b3bf84fcada619e2666d6004d3fe606
    int measureCodepoint(char chr);
    int decodeCharacter(const char* buf, int index);

    std::vector<unsigned int> codepoints;
    std::string textt;
};

#endif