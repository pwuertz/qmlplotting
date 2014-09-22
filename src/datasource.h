#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QQuickItem>
#include <QSGTextureProvider>
#include <QSGDynamicTexture>
#include <QByteArray>

class DataTexture;
class DataTextureProvider;


class DataSource : public QQuickItem
{
    Q_OBJECT

public:
    explicit DataSource(QQuickItem *parent = 0);
    virtual ~DataSource();

    virtual bool isTextureProvider() const;
    virtual QSGTextureProvider* textureProvider();

public slots:
    virtual bool setTestData() = 0;

signals:
    void dimensionsChanged();
    void dataChanged();

protected:
    bool setData(const double* data, const int* dims, int num_dims);

    const double* m_data;
    int m_num_dims;
    int m_dims[3];
    QByteArray m_test_data_buffer;

private:
    bool m_new_data;
    DataTextureProvider* m_provider;
    friend class DataTexture;
};


class DataSource1D : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(int dataSize READ dataSize NOTIFY dataSizeChanged)

public:
    explicit DataSource1D(QQuickItem *parent = 0);
    virtual ~DataSource1D() {}

    int dataSize() const {return m_dims[0];}

signals:
    void dataSizeChanged(int size);

public slots:
    virtual bool setTestData();
    bool setDataFloat64(const void* data, int size);

private slots:
    void handleDimensionsChanged();
};


class DataSource2D : public DataSource
{
    Q_OBJECT
    Q_PROPERTY(QSize dataSize READ dataSize NOTIFY dataSizeChanged)

public:
    explicit DataSource2D(QQuickItem *parent = 0);
    virtual ~DataSource2D() {}

    QSize dataSize() const {return QSize(m_dims[0], m_dims[1]);}

signals:
    void dataSizeChanged(const QSize& shape);

public slots:
    virtual bool setTestData();
    bool setDataFloat64(const void* data, int width, int height);

private slots:
    void handleDimensionsChanged();
};

#endif // DATASOURCE_H
