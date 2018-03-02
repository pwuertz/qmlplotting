#ifndef COLORMAPPEDIMAGE_H
#define COLORMAPPEDIMAGE_H

#include "dataclient.h"
#include <QVector4D>

class ColormappedImage : public DataClient
{
    Q_OBJECT
    Q_PROPERTY(double minimumValue MEMBER m_min_value WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(double maximumValue MEMBER m_max_value WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(QRectF viewRect MEMBER m_view_rect WRITE setViewRect NOTIFY viewRectChanged)
    Q_PROPERTY(QVector4D extent MEMBER m_extent WRITE setExtent NOTIFY extentChanged)
    Q_PROPERTY(QString colormap MEMBER m_colormap WRITE setColormap NOTIFY colormapChanged)
    Q_PROPERTY(QString filter READ getFilter WRITE setFilter NOTIFY filterChanged)

public:
    explicit ColormappedImage(QQuickItem *parent = nullptr);
    ~ColormappedImage() override;

    void setMinimumValue(double value);
    void setMaximumValue(double value);
    void setViewRect(const QRectF& viewrect);
    void setExtent(const QVector4D& extent);
    void setColormap(const QString& colormap);
    void setFilter(const QString& filter);
    QString getFilter() const;

signals:
    void minimumValueChanged(double value);
    void maximumValueChanged(double value);
    void viewRectChanged(const QRectF& viewrect);
    void extentChanged(const QVector4D& extent);
    void colormapChanged(const QString& colormap);
    void filterChanged(const QString& filter);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

private:
    double m_min_value = 0.;
    double m_max_value = 1.;
    QRectF m_view_rect = {0., 0., 1., 1.};
    QVector4D m_extent = {0., 1., 0., 1.};
    QString m_colormap;
    bool m_new_colormap = false;
    QSGTexture* m_texture_cmap = nullptr;
    QSGTexture::Filtering m_filter = QSGTexture::Linear;
};


#endif // COLORMAPPEDIMAGE_H

