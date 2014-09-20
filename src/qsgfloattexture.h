#ifndef QSGFLOATTEXTURE_H
#define QSGFLOATTEXTURE_H

#include <QOpenGLContext>
#include <QSGDynamicTexture>

class QSGFloatTexture : public QSGDynamicTexture
{
public:
    QSGFloatTexture();
    virtual ~QSGFloatTexture();

    virtual int textureId() const;
    virtual QSize textureSize() const;
    virtual bool hasAlphaChannel() const;
    virtual bool hasMipmaps() const;
    virtual void bind();

    void setData1D(double* data, int size, int num_components);
    void setData2D(double* data, int width, int height, int num_components);
    void setData3D(double* data, int width, int height, int depth, int num_components);
    virtual bool updateTexture();

private:
    unsigned int m_id_texture;
    int m_dims[3];
    int m_num_components;
    GLfloat* m_buffer;
    int m_buffer_size;
};

#endif // QSGFLOATTEXTURE_H
