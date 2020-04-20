#ifndef PLOTGROUP_H
#define PLOTGROUP_H

#include <QQuickItem>

class PlotGroup : public QQuickItem
{   
    Q_OBJECT

public:
    enum class ViewMode {
        AutoAspect,
        PreserveAspectFit,
        PreserveAspectCrop,
    };
    Q_ENUM(ViewMode)

public:
    Q_PROPERTY(ViewMode viewMode MEMBER m_viewMode WRITE setViewMode NOTIFY viewModeChanged)
    Q_PROPERTY(double aspectRatio MEMBER m_aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged)
    Q_PROPERTY(QRectF viewRect MEMBER m_viewRect WRITE setViewRect NOTIFY viewRectChanged)
    Q_PROPERTY(bool logY MEMBER m_logY WRITE setLogY NOTIFY logYChanged)
    Q_PROPERTY(QQmlListProperty<QQuickItem> plotItems READ plotItems CONSTANT)

public:
    explicit PlotGroup(QQuickItem* parent = nullptr);

    QQmlListProperty<QQuickItem> plotItems();

signals:
    void viewModeChanged(ViewMode viewMode);
    void aspectRatioChanged(double aspectRatio);
    void viewRectChanged(const QRectF& viewRect);
    void logYChanged(bool logY);

public slots:
    void setViewMode(ViewMode viewMode);
    void setAspectRatio(double aspectRatio);
    void setViewRect(const QRectF& viewRect);
    void setLogY(bool logY);

protected:
    void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;

private:
    ViewMode m_viewMode = ViewMode::AutoAspect;
    double m_aspectRatio = 1.0;
    QRectF m_viewRect = {0., 0., 1., 1.};
    QRectF m_preferredViewRect = {0., 0., 1., 1.};
    bool m_logY = false;
    QVector<QQuickItem*> m_plotItems;

    QRectF adjustViewRect(const QRectF& viewRect) const;
    void updateViewRects();
};

#endif // PLOTGROUP_H
