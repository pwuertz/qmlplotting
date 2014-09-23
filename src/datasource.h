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
    bool setTestData1D();
    bool setTestData2D();
    bool setData1D(const void* data, int size);
    bool setData2D(const void* data, int width, int height);
    bool setData3D(const void* data, int width, int height, int depth);

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

#endif // DATASOURCE_H
