#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <QQuickItem>
#include "datasource.h"

class DataClient : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)

signals:
    void dataSourceChanged(QQuickItem* item);

public:
    explicit DataClient(QQuickItem *parent = 0);
    virtual ~DataClient();

    QQuickItem* dataSource() const {return m_source;}
    virtual void setDataSource(QQuickItem* item);

protected:
    void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry);
    Q_INVOKABLE void dataChanged();

    bool m_new_geometry;
    bool m_new_data;
    bool m_new_source;
    DataSource* m_source;
};

#endif // DATACLIENT_H
