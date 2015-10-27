#ifndef QSGDATATEXTURE_H
#define QSGDATATEXTURE_H

#include <QSGDynamicTexture>
#include <QByteArray>
#include <QOpenGLFunctions_2_0>


template<typename T>
class QSGDataTexture : public QSGDynamicTexture, protected QOpenGLFunctions_2_0
{
public:
    QSGDataTexture();
    virtual ~QSGDataTexture();

    virtual int textureId() const;
    virtual QSize textureSize() const;
    virtual bool hasAlphaChannel() const;
    virtual bool hasMipmaps() const;
    virtual void bind();

    T* allocateData(int* dims, int num_dims, int num_components);
    T* allocateData1D(int size, int num_components);
    T* allocateData2D(int width, int height, int num_components);
    T* allocateData3D(int width, int height, int depth, int num_components);
    void commitData();

    int getDim(int dim);

    virtual bool updateTexture();

private:
    unsigned int m_id_texture;
    int m_num_dims;
    int m_dims[3];
    int m_num_components;
    QByteArray m_buffer;
    bool m_needs_upload;

    static const int glformat_internal[5];
    static const int glformat[5];
    static const int gltype;
};

#endif // QSGDATATEXTURE_H
