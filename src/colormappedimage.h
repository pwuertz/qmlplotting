#ifndef COLORMAPPEDIMAGE_H
#define COLORMAPPEDIMAGE_H

#include "dataclient.h"

class ColormappedImage : public DataClient
{
    Q_OBJECT
    Q_PROPERTY(double minimumValue MEMBER m_min_value WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(double maximumValue MEMBER m_max_value WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(QRectF viewRect MEMBER m_view_rect WRITE setViewRect NOTIFY viewRectChanged)
    Q_PROPERTY(bool viewInvert MEMBER m_view_invert WRITE setViewInvert NOTIFY viewInvertChanged)
    Q_PROPERTY(QString colormap MEMBER m_colormap WRITE setColormap NOTIFY colormapChanged)

public:
    explicit ColormappedImage(QQuickItem *parent = 0);
    ~ColormappedImage();

    void setMinimumValue(double value);
    void setMaximumValue(double value);
    void setViewRect(const QRectF& viewrect);
    void setViewInvert(bool invert);
    void setColormap(const QString& colormap);

public slots:
    QPointF mapPointFromScene(const QPointF& spoint) const;
    QRectF mapRectFromScene(const QRectF& srect) const;
    QPointF mapPointToScene(const QPointF& vpoint) const;
    QRectF mapRectToScene(const QRectF& vrect) const;

signals:
    void minimumValueChanged(double value);
    void maximumValueChanged(double value);
    void viewRectChanged(const QRectF& viewrect);
    void viewInvertChanged(bool invert);
    void colormapChanged(const QString& colormap);

protected:
    virtual QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    double m_min_value;
    double m_max_value;
    QRectF m_view_rect;
    bool m_view_invert;
    QString m_colormap;
    bool m_new_colormap;
    QSGTexture* m_texture_cmap;
};


#endif // COLORMAPPEDIMAGE_H

