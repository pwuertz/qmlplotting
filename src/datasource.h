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
    Q_PROPERTY(int dataDimensions MEMBER m_num_dims NOTIFY dataimensionsChanged)
    Q_PROPERTY(int dataWidth READ dataWidth NOTIFY dataSizeChanged)
    Q_PROPERTY(int dataHeight READ dataHeight  NOTIFY dataSizeChanged)
    Q_PROPERTY(int dataDepth READ dataDepth  NOTIFY dataSizeChanged)

public:
    explicit DataSource(QQuickItem *parent = 0);
    virtual ~DataSource();

    virtual bool isTextureProvider() const;
    virtual QSGTextureProvider *textureProvider() const;

    int dataWidth() const {return m_dims[0];}
    int dataHeight() const {return m_dims[1];}
    int dataDepth() const {return m_dims[2];}

public slots:
    bool setTestData1D();
    bool setTestData2D();
    bool setData1D(void* data, int size);
    bool setData2D(void* data, int width, int height);
    bool setData3D(void* data, int width, int height, int depth);
    void* allocateData1D(int size);
    void* allocateData2D(int width, int height);
    void* allocateData3D(int width, int height, int depth);
    void* data() const {return m_data;}
    bool commitData();
    bool ownsData();

signals:
    void dataimensionsChanged();
    void dataSizeChanged();
    void dataChanged();

protected:
    bool setData(double* data, const int* dims, int num_dims);

    double* m_data;
    int m_num_dims;
    int m_dims[3];
    QByteArray m_data_buffer;

private:
    bool m_new_data;
    DataTextureProvider* m_provider;
    friend class DataTexture;
};

#endif // DATASOURCE_H
