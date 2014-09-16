#include "dataclient.h"

DataClient::DataClient(QQuickItem *parent) :
    QQuickItem(parent),
    m_new_geometry(false), m_new_data(false), m_new_container(false),
    m_datacontainer(nullptr)
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

void DataClient::setDataContainer(QQuickItem *item)
{
    DataContainer* d = dynamic_cast<DataContainer*>(item);
    if (d == m_datacontainer) return;

    // disconnect from previous data source
    if (m_datacontainer) {
        disconnect(m_datacontainer, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    }
    // connect to new data source
    if (d) {
        connect(d, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    }
    m_datacontainer = d;
    m_new_container = true;
    emit dataContainerChanged(d);
    update();
}

void DataClient::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    m_new_geometry = true;
    update();
}
