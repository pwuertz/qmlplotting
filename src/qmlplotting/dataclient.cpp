#include "dataclient.h"

DataClient::DataClient(QQuickItem *parent)
    : QQuickItem(parent)
    , m_new_geometry(false)
    , m_new_data(false)
    , m_new_source(false)
    , m_source(nullptr)
{

}

DataClient::~DataClient() = default;

void DataClient::dataChanged()
{
    m_new_data = true;
    update();
}

void DataClient::setDataSource(QQuickItem *item)
{
    auto* d = dynamic_cast<DataSource*>(item);
    if (d == m_source) {
        return;
    }

    // disconnect from previous data source
    if (m_source != nullptr) {
        disconnect(m_source, &DataSource::dataChanged, this, &DataClient::dataChanged);
    }
    // connect to new data source
    if (d != nullptr) {
        connect(d, &DataSource::dataChanged, this, &DataClient::dataChanged);
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
