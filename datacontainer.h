#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QQuickItem>
#include <QSGTextureProvider>
#include "qsgfloattexture.h"

class DataTexture;
class DataTextureProvider;

class DataContainer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int dataWidth READ dataWidth NOTIFY dataChanged)
    Q_PROPERTY(int dataHeight READ dataHeight NOTIFY dataChanged)

public:
    explicit DataContainer(QQuickItem *parent = 0);
    virtual ~DataContainer();

    int dataWidth() const {return m_width;}
    int dataHeight() const {return m_height;}

    virtual bool isTextureProvider() const;
    virtual QSGTextureProvider* textureProvider();

signals:
    void dataChanged();

public slots:
    bool setDataFloat32(void* data, int width, int height);

protected:
    float* m_data;
    int m_width;
    int m_height;
    bool m_new_data;
    DataTextureProvider* m_provider;
    friend class DataTexture;
};

class DataTexture : public QSGFloatTexture
{
    Q_OBJECT

public:
    DataTexture(DataContainer* datacontainer);
    virtual ~DataTexture() {}

    bool updateTexture();
    DataContainer* m_datacontainer;
};

class DataTextureProvider : public QSGTextureProvider
{
    Q_OBJECT

public:
    DataTextureProvider(DataContainer* datacontainer);
    virtual ~DataTextureProvider();

    virtual QSGTexture* texture() const;
    DataTexture* m_datatexture;
};

#endif // DATACONTAINER_H
