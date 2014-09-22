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
            setData(m_source->m_data, m_source->m_dims, m_source->m_num_dims, 1);
            return true;
        } else  {
            return false;
        }
    }

private:
    DataSource* m_source;
};


class DataTextureProvider : public QSGTextureProvider
{
public:
    DataTextureProvider(DataSource* source) : QSGTextureProvider() {
        m_datatexture = new DataTexture(source);
    }

    virtual ~DataTextureProvider() {
        delete m_datatexture;
    }

    virtual QSGTexture* texture() const {
        return m_datatexture;
    }

    DataTexture* m_datatexture;
};


DataSource::DataSource(QQuickItem *parent) :
    QQuickItem(parent),
    m_data(nullptr), m_num_dims(0),
    m_test_data_buffer(),
    m_new_data(false),
    m_provider(nullptr)
{
    for (int i = 0; i < 3; ++i) {
        m_dims[i] = 0;
    }
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

bool DataSource::setData(const double *data, const int *dims, int num_dims)
{
    if (num_dims <= 0 || num_dims > 3) {
        qWarning("DataSource::setData invalid number of dimensions");
        return false;
    }
    m_num_dims = num_dims;
    bool dims_changed = false;
    for (int i = 0; i < num_dims; ++i) {
        dims_changed = dims_changed || (m_dims[i] != dims[i]);
        m_dims[i] = dims[i];
    }
    m_data = data;
    m_new_data = true;
    emit dimensionsChanged();
    emit dataChanged();
    return true;
}


DataSource1D::DataSource1D(QQuickItem *parent) : DataSource(parent)
{
    connect(this, SIGNAL(dimensionsChanged()), this, SLOT(handleDimensionsChanged()));
}

void DataSource1D::handleDimensionsChanged()
{
    emit dataSizeChanged(m_dims[0]);
}

bool DataSource1D::setDataFloat64(const void *data, int size)
{
    return setData(reinterpret_cast<const double*>(data), &size, 1);
}

bool DataSource1D::setTestData()
{
    int size = 512;
    int num_bytes = size * sizeof(double);
    if (m_test_data_buffer.size() != num_bytes) {
        m_test_data_buffer.resize(num_bytes);
    }
    double* d = reinterpret_cast<double*>(m_test_data_buffer.data());

    // gauss + noise
    for (int ix = 0; ix < size; ++ix) {
        double x = -1. + 2.*(ix * (1./size));
        double r = rand() * (1./RAND_MAX);
        d[ix] = exp(-(x*x)*2.) + .2 * (r-.5);
    }

    return setData(d, &size, 1);
}


DataSource2D::DataSource2D(QQuickItem *parent) : DataSource(parent)
{
    connect(this, SIGNAL(dimensionsChanged()), this, SLOT(handleDimensionsChanged()));
}

void DataSource2D::handleDimensionsChanged()
{
    emit dataSizeChanged(dataSize());
}

bool DataSource2D::setDataFloat64(const void *data, int width, int height)
{
    int dims[2] = {width, height};
    return setData(reinterpret_cast<const double*>(data), dims, 2);
}

bool DataSource2D::setTestData()
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

    int dims[2] = {w, h};
    return setData(d, dims, 2);
}
