#include <QThread>
#include <QOpenGLContext>
#include "datacontainer.h"
#include "qsgfloattexture.h"

class DataTexture : public QSGFloatTexture
{
public:
    DataTexture(DataContainer* datacontainer) : QSGFloatTexture(), m_datacontainer(datacontainer) {}
    virtual ~DataTexture() {}

    virtual bool updateTexture() {
        if (m_datacontainer->m_new_data) {
            setDataSource(m_datacontainer->m_data,
                          m_datacontainer->m_width,
                          m_datacontainer->m_height,
                          1);
            return QSGFloatTexture::updateTexture();
        } else  {
            return false;
        }
    }
    DataContainer* m_datacontainer;
};


DataContainer::DataContainer(QQuickItem *parent) :
    QQuickItem(parent), m_data(nullptr), m_width(0), m_height(0), m_new_data(false),
    m_provider(nullptr)
{
}

DataContainer::~DataContainer()
{
    if (m_provider) {
        m_provider->deleteLater();
    }
}

bool DataContainer::isTextureProvider() const
{
    return true;
}

bool DataContainer::setDataFloat32(void *data, int width, int height)
{
    m_data = (float*) (data);
    m_width = width;
    m_height = height;
    m_new_data = true;
    emit dataChanged();
    return true;
}

QSGTextureProvider *DataContainer::textureProvider()
{
    if (!QOpenGLContext::currentContext()) {
        qWarning("DataContainer::textureProvider needs OpenGL context");
        return 0;
    }
    if (!m_provider) {
        m_provider = new DataTextureProvider(this);
        m_provider->m_datatexture->updateTexture();
    }
    return m_provider;
}

DataTextureProvider::DataTextureProvider(DataContainer *datacontainer) :
    QSGTextureProvider()
{
    m_datatexture = new DataTexture(datacontainer);
}

DataTextureProvider::~DataTextureProvider()
{
    delete m_datatexture;
}

QSGTexture *DataTextureProvider::texture() const
{
    return m_datatexture;
}
