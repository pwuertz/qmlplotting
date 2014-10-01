#ifndef QSGUINT8TEXTURE_H
#define QSGUINT8TEXTURE_H

#include <QSGTexture>
#include <QOpenGLContext>

class QSGUint8Texture : public QSGTexture
{
    Q_OBJECT
public:
    QSGUint8Texture();
    virtual ~QSGUint8Texture();

    virtual int textureId() const;
    virtual QSize textureSize() const;
    virtual bool hasAlphaChannel() const;
    virtual bool hasMipmaps() const;
    virtual void bind();

    uint8_t* allocateData1D(int size, int components);
    uint8_t* allocateData2D(int width, int height, int components);
    void commitData();

private:
    unsigned int m_id_texture;
    int m_num_dims;
    int m_dims[2];
    int m_num_components;
    QByteArray m_buffer;
    bool m_needs_upload;
};

#endif // QSGUINT8TEXTURE_H
