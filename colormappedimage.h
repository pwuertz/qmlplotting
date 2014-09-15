#ifndef COLORMAPPEDIMAGE_H
#define COLORMAPPEDIMAGE_H

#include "dataclient.h"
#include "qsgfloattexture.h"

class ColormappedImage : public DataClient
{
    Q_OBJECT
    Q_PROPERTY(double minimumValue READ minimumValue WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(double maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)

public:
    explicit ColormappedImage(QQuickItem *parent = 0);
    ~ColormappedImage();

    double minimumValue() const {return m_min_value;}
    double maximumValue() const {return m_max_value;}
    void setMinimumValue(double value);
    void setMaximumValue(double value);

signals:
    void minimumValueChanged(double value);
    void maximumValueChanged(double value);
    void dataContainerChanged(DataContainer* datacontainer);

protected:
    virtual QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    double m_min_value;
    double m_max_value;
    QSGFloatTexture* m_texture_cmap;
};


#endif // COLORMAPPEDIMAGE_H

