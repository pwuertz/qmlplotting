#include "dataclient.h"

DataClient::DataClient(QQuickItem *parent) :
    QQuickItem(parent),
    m_new_geometry(false), m_new_data(false), m_new_source(false),
    m_source(nullptr)
{

}

DataClient::~DataClient()
{

}

void DataClient::dataChanged()
{
    m_new_data = true;
    update();
}

void DataClient::setDataSource(QQuickItem *item)
{
    DataSource* d = dynamic_cast<DataSource*>(item);
    if (d == m_source) return;

    // disconnect from previous data source
    if (m_source) {
        disconnect(m_source, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    }
    // connect to new data source
    if (d) {
        connect(d, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    }
    m_source = d;
    m_new_source = true;
    emit dataSourceChanged(d);
    update();
}

void DataClient::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    m_new_geometry = true;
    update();
}
