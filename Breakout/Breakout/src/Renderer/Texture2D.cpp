#include "Texture2D.h"

#include <glad/glad.h>

Texture2D::Texture2D(const int width, const int height, const int nbChannels)
    : m_ID(0), m_Width(width), m_Height(height), m_Channels(nbChannels)
{
    glGenTextures(1, &m_ID);
}

void Texture2D::Bind(const unsigned int slot) const
{
    // Bind the texture to the specified slot
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SetData(const void* data, int internalFormat, unsigned int dataFormat, unsigned int type) const
{
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, type, data);
}

void Texture2D::SetFilterMode(const int mode)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
}

void Texture2D::SetWrapMode(const int mode)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
}
