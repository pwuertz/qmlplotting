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
    ~QSGDataTexture() override;

    int textureId() const override;
    QSize textureSize() const override;
    bool hasAlphaChannel() const override;
    bool hasMipmaps() const override;
    void bind() override;

    T* allocateData(const int *dims, int num_dims, int num_components);
    T* allocateData1D(int size, int num_components);
    T* allocateData2D(int width, int height, int num_components);
    T* allocateData3D(int width, int height, int depth, int num_components);
    void commitData();

    int getDim(int dim);

    bool updateTexture() override;

private:
    unsigned int m_id_texture = 0;
    int m_num_dims = 0;
    int m_dims[3] = {0, 0, 0};
    int m_num_components = 0;
    QByteArray m_buffer;
    bool m_needs_upload = false;
};

extern template class QSGDataTexture<float>;
extern template class QSGDataTexture<uint8_t>;

#endif // QSGDATATEXTURE_H
