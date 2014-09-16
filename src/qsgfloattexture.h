#ifndef QSGFLOATTEXTURE_H
#define QSGFLOATTEXTURE_H

#include <QSGTexture>

class QSGFloatTexture : public QSGTexture
{
public:
    QSGFloatTexture();
    virtual ~QSGFloatTexture();

    virtual int textureId() const;
    virtual QSize textureSize() const;
    virtual bool hasAlphaChannel() const;
    virtual bool hasMipmaps() const;
    virtual void bind();
    void updateTextureData(float* data, int w, int h, int num_components);

private:
    unsigned int m_id_texture;
    unsigned int m_width;
    unsigned int m_height;
};

#endif // QSGFLOATTEXTURE_H
