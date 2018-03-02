#ifndef XYPLOT_H
#define XYPLOT_H

#include "dataclient.h"

class XYPlot : public DataClient
{
    Q_OBJECT
    Q_PROPERTY(QRectF viewRect MEMBER m_view_rect WRITE setViewRect NOTIFY viewRectChanged)
    Q_PROPERTY(bool fillEnabled MEMBER m_fill WRITE setFillEnabled NOTIFY fillEnabledChanged)
    Q_PROPERTY(QColor fillColor MEMBER m_fillcolor WRITE setFillColor NOTIFY fillColorChanged)
    Q_PROPERTY(bool lineEnabled MEMBER m_line WRITE setLineEnabled NOTIFY lineEnabledChanged)
    Q_PROPERTY(double lineWidth MEMBER m_linewidth WRITE setLineWidth NOTIFY lineWidthChanged)
    Q_PROPERTY(QColor lineColor MEMBER m_linecolor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(bool markerEnabled MEMBER m_marker WRITE setMarkerEnabled NOTIFY markerEnabledChanged)
    Q_PROPERTY(int markerSegments MEMBER m_markersegments WRITE setMarkerSegments NOTIFY markerSegmentsChanged)
    Q_PROPERTY(double markerSize MEMBER m_markersize WRITE setMarkerSize NOTIFY markerSizeChanged)
    Q_PROPERTY(QColor markerColor MEMBER m_markercolor WRITE setMarkerColor NOTIFY markerColorChanged)
    Q_PROPERTY(bool markerBorder MEMBER m_markerborder WRITE setMarkerBorder NOTIFY markerBorderChanged)
    Q_PROPERTY(bool logY MEMBER m_logy WRITE setLogY NOTIFY logYChanged)

public:
    explicit XYPlot(QQuickItem *parent = nullptr);
    ~XYPlot() override = default;

    void setViewRect(const QRectF& viewrect);
    void setFillEnabled(bool enabled);
    void setFillColor(const QColor& color);
    void setLineEnabled(bool enabled);
    void setLineWidth(double width);
    void setLineColor(const QColor& color);
    void setMarkerEnabled(bool enabled);
    void setMarkerSegments(int n);
    void setMarkerSize(double size);
    void setMarkerColor(const QColor& color);
    void setMarkerBorder(bool enabled);
    void setLogY(bool enabled);

signals:
    void viewRectChanged(const QRectF& viewrect);
    void fillEnabledChanged(bool);
    void fillColorChanged(const QColor&);
    void lineEnabledChanged(bool);
    void lineWidthChanged(double);
    void lineColorChanged(const QColor&);
    void markerEnabledChanged(bool);
    void markerSegmentsChanged(int);
    void markerSizeChanged(double);
    void markerColorChanged(const QColor&);
    void markerBorderChanged(bool);
    void logYChanged(bool);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

private:
    QRectF m_view_rect = {0, 0, 1, 1};
    bool m_fill = false;
    QColor m_fillcolor = {0, 0, 0};
    bool m_line = true;
    double m_linewidth = 1.;
    QColor m_linecolor = {0, 0, 0};
    bool m_marker = true;
    double m_markersegments = 0;
    double m_markersize = 5.;
    QColor m_markercolor = {0, 0, 0};
    bool m_markerborder = false;
    bool m_logy = false;
};

#endif // XYPLOT_H
