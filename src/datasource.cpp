#include <QThread>
#include <QOpenGLContext>
#include "datasource.h"
#include "qsgfloattexture.h"

#include <math.h>

class DataTexture : public QSGFloatTexture
{
public:
    DataTexture(DataSource* source) : QSGFloatTexture(), m_source(source) {}
    virtual ~DataTexture() {}

    virtual bool updateTexture() {
        if (m_source->m_new_data) {
            setData2D(m_source->m_data,
                      m_source->m_width,
                      m_source->m_height,
                      1);
            return true;
        } else  {
            return false;
        }
    }
    DataSource* m_source;
};


DataSource::DataSource(QQuickItem *parent) :
    QQuickItem(parent), m_data(nullptr), m_width(0), m_height(0), m_new_data(false),
    m_provider(nullptr), m_test_data_buffer()
{
}

DataSource::~DataSource()
{
    if (m_provider) {
        m_provider->deleteLater();
    }
}

bool DataSource::isTextureProvider() const
{
    return true;
}

bool DataSource::setDataFloat64(void *data, int width, int height)
{
    m_data = (double*) (data);
    m_width = width;
    m_height = height;
    m_new_data = true;
    emit dataChanged();
    return true;
}

bool DataSource::setTestData()
{
    int w = 512, h = 512;
    int num_bytes = w * h * sizeof(double);
    if (m_test_data_buffer.size() != num_bytes) {
        m_test_data_buffer.resize(num_bytes);
    }
    double* d = reinterpret_cast<double*>(m_test_data_buffer.data());

    // gauss + noise
    for (int iy = 0; iy < w; ++iy) {
        for (int ix = 0; ix < h; ++ix) {
            double x = -1. + 2.*(ix * (1./w));
            double y = -1. + 2.*(iy * (1./h));
            double r = rand() * (1./RAND_MAX);
            d[iy*w + ix] = exp(-(x*x+y*y)*2.) + .2 * (r-.5);
        }
    }
    // linear strip
    for (int iy = h-30; iy < h; ++iy) {
        for (int ix = 0; ix < w; ++ix) {
            double x = (ix * (1./w));
            d[iy*w + ix] = x;
        }
    }

    setDataFloat64(d, w, h);
    return true;
}

QSGTextureProvider *DataSource::textureProvider()
{
    if (!QOpenGLContext::currentContext()) {
        qWarning("DataSource::textureProvider needs OpenGL context");
        return 0;
    }
    if (!m_provider) {
        m_provider = new DataTextureProvider(this);
        m_provider->m_datatexture->updateTexture();
    }
    return m_provider;
}

DataTextureProvider::DataTextureProvider(DataSource* source) :
    QSGTextureProvider()
{
    m_datatexture = new DataTexture(source);
}

DataTextureProvider::~DataTextureProvider()
{
    delete m_datatexture;
}

QSGTexture *DataTextureProvider::texture() const
{
    return m_datatexture;
}
