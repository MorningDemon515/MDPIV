#include "Font.h"

Font::Font(std::string text, const char* fontPath, int size)
{
    textt = text;

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;

    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font at " << fontPath << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 48); 
    
    size_t textLen = strlen(text.c_str());
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned int i = 0; i < textLen;)
    {
        unsigned int charLen = measureCodepoint(text[i]);
        unsigned int unicode = decodeCharacter(text.c_str(), i);
        
        unsigned int c = unicode;
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph: " << c << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            MD_Math::IVECTOR2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            MD_Math::IVECTOR2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<unsigned int, Character>(c, character));
 
        codepoints.push_back(unicode);
        i += charLen;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Font::~Font()
{
    glDeleteVertexArrays(1, &textVAO);
    glDeleteBuffers(1, &textVBO);

    for (auto& ch : Characters) {
        glDeleteTextures(1, &ch.second.TextureID);
    }
}

void Font::Draw(float x, float y, float z, float scale)
{

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (unsigned int c : codepoints) {

        auto it = Characters.find(c);
        if (it == Characters.end()) {
            std::cerr << "WARN::Font: Cann't Load Character: 0x" << std::hex << c << std::dec << std::endl;
            continue;
        }
        Character ch = it->second;

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][5] = {
            { xpos,     ypos + h, z,  0.0f, 0.0f },
            { xpos,     ypos,     z,  0.0f, 1.0f },
            { xpos + w, ypos,     z,  1.0f, 1.0f },
            { xpos,     ypos + h, z,  0.0f, 0.0f },
            { xpos + w, ypos,     z,  1.0f, 1.0f },
            { xpos + w, ypos + h, z,  1.0f, 0.0f }
        };
        
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;

    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
    
}

int Font::measureCodepoint(char chr)
{
    if ((chr & 0x80) == 0x00) {
        //first bit is 0: treat as ASCII
        return 1;
    }

    //first bit is 1, number of consecutive 1 bits at the start is length of codepoint
    int len = 0;
    while (((chr >> (7 - len)) & 0x01) == 0x01) {
        len++;
    }
    return len;
}

int Font::decodeCharacter(const char* buf, int index)
{
     int codepointLen = measureCodepoint(buf[index]);

    if (codepointLen == 1) {
        return buf[index];
    } else {
        //decode first byte by skipping all bits that indicate the length of the codepoint
        int newChar = buf[index] & (0x7f >> codepointLen);
        for (int j = 1; j < codepointLen; j++) {
            //decode all of the following bytes, fixed 6 bits per byte
            newChar = (newChar << 6) | (buf[index + j] & 0x3f);
        }
        return newChar;
    }
}