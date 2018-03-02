#include "plotgroup.h"
#include <QDebug>

PlotGroup::PlotGroup(QQuickItem* parent) : QQuickItem(parent)
{

}

QQmlListProperty<QQuickItem> PlotGroup::plotItems()
{
    const auto append = [](QQmlListProperty<QQuickItem>* list, QQuickItem* plotItem) {
        auto* self = reinterpret_cast<PlotGroup*>(list->data);
        self->m_plotItems.append(plotItem);
        // Reparent and anchor to plot group size
        plotItem->setParentItem(self);
        plotItem->setPosition({0., 0.});
        plotItem->setSize({self->width(), self->height()});
        // Initialize properties from plot group
        plotItem->setProperty("viewRect", self->m_viewRect);
        plotItem->setProperty("logY", self->m_logY);
    };
    const auto count = [](QQmlListProperty<QQuickItem>* list) -> int {
        auto* self = reinterpret_cast<PlotGroup*>(list->data);
        return self->m_plotItems.count();
    };
    const auto at = [](QQmlListProperty<QQuickItem>* list, int index) -> QQuickItem* {
        auto* self = reinterpret_cast<PlotGroup*>(list->data);
        return self->m_plotItems.at(index);
    };
    const auto clear = [](QQmlListProperty<QQuickItem>* list) {
        auto* self = reinterpret_cast<PlotGroup*>(list->data);
        self->m_plotItems.clear();
    };
    return {this, this, append, count, at, clear};
}

void PlotGroup::setAspectAuto(bool aspectAuto)
{
    if (m_aspectAuto != aspectAuto) {
        m_aspectAuto = aspectAuto;
        if (!m_aspectAuto) {
            setViewRect(correctAspectRatio(m_viewRect));
        }
        emit aspectAutoChanged(aspectAuto);
    }
}

void PlotGroup::setAspectRatio(double aspectRatio)
{
    if (m_aspectRatio != aspectRatio) {
        m_aspectRatio = std::max(aspectRatio, 0.);
        if (!m_aspectAuto) {
            setViewRect(correctAspectRatio(m_viewRect));
        }
        emit aspectRatioChanged(aspectRatio);
    }
}

void PlotGroup::setViewRect(const QRectF &viewRect)
{
    if (m_viewRect != viewRect) {
        m_viewRect = viewRect;  // TODO: Enforce correct aspect ratio here?
        // Forward viewRect change to plot items
        for (auto& item: m_plotItems) {
            item->setProperty("viewRect", viewRect);
        }
        emit viewRectChanged(viewRect);
    }
}

void PlotGroup::setLogY(bool logY)
{
    if (m_logY != logY) {
        m_logY = logY;
        // Forward logY change to plot items
        for (auto& item: m_plotItems) {
            item->setProperty("logY", logY);
        }
        emit logYChanged(logY);
    }
}

void PlotGroup::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    // Resize plot items to new item size
    for (auto& item: m_plotItems) {
        item->setSize({width(), height()});
    }
    // If aspect ratio is fixed, correct viewRect after resize
    if (!m_aspectAuto) {
        setViewRect(correctAspectRatio(m_viewRect));
    }
}

QRectF PlotGroup::correctAspectRatio(const QRectF &viewRect)
{
    // Assert item size is valid before calculating new viewRect
    if (!(width() > 0 && height() > 0)) {
        return viewRect;
    }
    // TODO: What is the 'natural' way of applying view rect constraints?
    // For now fix y-axis and adjust width according to target aspect ratio.
    const double viewAspect = m_aspectRatio * width() / height();
    const double targetWidth = viewRect.height() * viewAspect;
    const double widthDelta = targetWidth - viewRect.width();
    return {viewRect.x() - 0.5 * widthDelta, viewRect.y(), targetWidth, viewRect.height()};
}
