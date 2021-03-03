#include "mvTexture.h"
#include <string>
#include <assert.h>
#include "mvGraphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Marvel {

    mvTexture::mvTexture(mvGraphics& graphics, const std::string& path, GLenum slot)
    {

        m_slot = slot;

        // Load from file
        int image_width = 0;
        int image_height = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image_data = stbi_load(path.c_str(), &image_width, &image_height, nullptr, 4);
        if (image_data == nullptr)
            return;

        // Create a OpenGL texture identifier
        glGenTextures(1, &m_texture);
        glActiveTexture(m_slot);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image_data);

        glUniform1i(m_slot, m_slot); // set it manually
    }

    void mvTexture::bind(mvGraphics& graphics)
    {
        glActiveTexture(m_slot);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        
    }

}