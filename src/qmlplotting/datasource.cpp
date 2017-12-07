#include <QThread>
#include <QOpenGLContext>
#include "datasource.h"
#include "qsgdatatexture.h"

#include <math.h>


class DataTexture : public QSGDataTexture<float>
{
public:
    DataTexture(const DataSource *source) : QSGDataTexture<float>(), m_source(source) {}
    virtual ~DataTexture() {}

    virtual bool updateTexture() {
        if (m_source->m_new_data) {
            // copy/convert data to float texture buffer
            float* data = allocateData(m_source->m_dims, m_source->m_num_dims, 1);
            int num_elements = 1;
            for (int i = 0; i < m_source->m_num_dims; ++i)
                num_elements *= m_source->m_dims[i];
            for (int i = 0; i < num_elements; ++i) {
                data[i] = m_source->m_data[i];
            }
            commitData();
            return true;
        } else  {
            return false;
        }
    }

private:
    const DataSource *m_source;
};


class DataTextureProvider : public QSGTextureProvider
{
public:
    DataTextureProvider(const DataSource *source) : QSGTextureProvider() {
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
    m_data_buffer(),
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

QSGTextureProvider *DataSource::textureProvider() const
{
    if (!QOpenGLContext::currentContext()) {
        qWarning("DataSource::textureProvider needs OpenGL context");
        return 0;
    }
    if (!m_provider) {
        // TODO: use destroyed signal instead of m_provider for cleanup?
        ((DataSource*)this)->m_provider = new DataTextureProvider(this);
        m_provider->m_datatexture->updateTexture();
    }
    return m_provider;
}

bool DataSource::setData(double *data, const int *dims, int num_dims)
{
    if (num_dims <= 0 || num_dims > 3) {
        qWarning("DataSource::setData invalid number of dimensions");
        return false;
    }

    bool num_dims_changed = (m_num_dims != num_dims);
    bool size_changed = num_dims_changed;
    m_num_dims = num_dims;
    for (int i = 0; i < 3; ++i) {
        if (i < num_dims) {
            size_changed = size_changed || (m_dims[i] != dims[i]);
            m_dims[i] = dims[i];
        } else {
            m_dims[i] = 0;
        }
    }
    m_data = data;
    if (num_dims_changed) emit dataimensionsChanged();
    if (size_changed) emit dataSizeChanged();
    commitData();
    return true;
}

bool DataSource::setData1D(void* data, int size) {
    return setData((double*) data, &size, 1);
}

bool DataSource::setData2D(void* data, int width, int height) {
    int dims[] = {width, height};
    return setData((double*) data, dims, 2);
}

bool DataSource::setData3D(void* data, int width, int height, int depth){
    int dims[] = {width, height, depth};
    return setData((double*) data, dims, 3);
}

void* DataSource::allocateData1D(int size)
{
    int num_bytes = size * sizeof(double);
    if (m_data_buffer.size() != num_bytes) {
        m_data_buffer.resize(num_bytes);
    }
    double *data = (double*) m_data_buffer.data();
    setData((double*) data, &size, 1);
    return data;
}

void* DataSource::allocateData2D(int width, int height)
{
    int num_bytes = width * height * sizeof(double);
    if (m_data_buffer.size() != num_bytes) {
        m_data_buffer.resize(num_bytes);
    }
    int dims[] = {width, height};
    double *data = (double*) m_data_buffer.data();
    setData((double*) data, dims, 2);
    return data;
}

void* DataSource::allocateData3D(int width, int height, int depth)
{
    int num_bytes = width * height * depth * sizeof(double);
    if (m_data_buffer.size() != num_bytes) {
        m_data_buffer.resize(num_bytes);
    }
    int dims[] = {width, height, depth};
    double *data = (double*) m_data_buffer.data();
    setData((double*) data, dims, 3);
    return data;
}

bool DataSource::commitData()
{
    m_new_data = true;
    emit dataChanged();
    return true;
}

bool DataSource::ownsData()
{
    return m_data == (double*) m_data_buffer.data();
}

bool DataSource::setTestData1D()
{
    int size = 512;
    double* d = (double *) allocateData1D(2*size);

    // gauss + noise
    for (int ix = 0; ix < size; ++ix) {
        double x = -1. + 2.*(ix * (1./size));
        double r = rand() * (1./RAND_MAX);
        d[2*ix+0] = x;
        d[2*ix+1] = exp(-(x*x)*5.) * (1. + .2 * (r-.5));
    }
    return true;
}

bool DataSource::setTestData2D()
{
    int w = 512, h = 512;
    double* d = (double *) allocateData2D(w, h);

    // gauss + noise
    for (int iy = 0; iy < h; ++iy) {
        for (int ix = 0; ix < w; ++ix) {
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
    return true;
}
