#ifndef COLORMAPPEDIMAGE_H
#define COLORMAPPEDIMAGE_H

#include <QQuickItem>
#include "qsgfloattexture.h"
#include "datacontainer.h"

class ColormappedImage : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(ColormappedImage)
    Q_PROPERTY(double minimumValue READ minimumValue WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(double maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(DataContainer* dataContainer READ dataContainer WRITE setDataContainer NOTIFY dataContainerChanged)

public:
    explicit ColormappedImage(QQuickItem *parent = 0);
    ~ColormappedImage();

    double minimumValue() const {return m_min_value;}
    double maximumValue() const {return m_max_value;}

    DataContainer* dataContainer() const {return m_datacontainer;}
    void setDataContainer(DataContainer* datacontainer);

    void setMinimumValue(double value);
    void setMaximumValue(double value);

signals:
    void minimumValueChanged(double value);
    void maximumValueChanged(double value);
    void dataContainerChanged(DataContainer* datacontainer);

protected:
    virtual QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    Q_INVOKABLE void handleDataChanged();

private:
    bool m_new_data;
    bool m_new_container;
    double m_min_value;
    double m_max_value;
    QSGFloatTexture* m_texture_cmap;
    DataContainer* m_datacontainer;
};


#endif // COLORMAPPEDIMAGE_H

