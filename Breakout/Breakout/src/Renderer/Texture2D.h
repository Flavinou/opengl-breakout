#pragma once

class Texture2D
{
public:
    Texture2D(int width, int height, int nbChannels);
    ~Texture2D() = default;

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void SetData(const void* data, int internalFormat, unsigned int dataFormat, unsigned int type) const;

    void SetFilterMode(int mode);
    void SetWrapMode(int mode);

    const unsigned int& GetID() const { return m_ID; }
    unsigned int GetWidth() const { return m_Width; }
    unsigned int GetHeight() const { return m_Height; }
private:
    unsigned int m_ID;
    int m_Width, m_Height, m_Channels;
};
