#ifndef QSGFLOATTEXTURE_H
#define QSGFLOATTEXTURE_H

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
    virtual bool updateTexture();

    void setDataSource(float* data, int w, int h, int num_components);

private:
    unsigned int m_id_texture;
    float* m_data;
    int m_width;
    int m_height;
    int m_num_components;
};

#endif // QSGFLOATTEXTURE_H
