#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <QQuickItem>
#include "datacontainer.h"

class DataClient : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* dataContainer READ dataContainer WRITE setDataContainer NOTIFY dataContainerChanged)

signals:
    void dataContainerChanged(QQuickItem* item);

public:
    explicit DataClient(QQuickItem *parent = 0);
    virtual ~DataClient();

    QQuickItem* dataContainer() const {return m_datacontainer;}
    void setDataContainer(QQuickItem* item);

protected:
    void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry);
    Q_INVOKABLE void dataChanged();

    bool m_new_geometry;
    bool m_new_data;
    bool m_new_container;
    DataContainer* m_datacontainer;
};

#endif // DATACLIENT_H
