#ifndef XYPLOT_H
#define XYPLOT_H

#include "dataclient.h"

class XYPlot : public DataClient
{
    Q_OBJECT
    Q_PROPERTY(double xMin MEMBER m_xmin WRITE setXMin NOTIFY xMinChanged)
    Q_PROPERTY(double xMax MEMBER m_xmax WRITE setXMax NOTIFY xMaxChanged)
    Q_PROPERTY(double yMin MEMBER m_ymin WRITE setYMin NOTIFY yMinChanged)
    Q_PROPERTY(double yMax MEMBER m_ymax WRITE setYMax NOTIFY yMaxChanged)
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

public:
    explicit XYPlot(QQuickItem *parent = 0);
    virtual ~XYPlot() {}

    void setXMin(double value);
    void setXMax(double value);
    void setYMin(double value);
    void setYMax(double value);
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

signals:
    void xMinChanged(double);
    void xMaxChanged(double);
    void yMinChanged(double);
    void yMaxChanged(double);
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

protected:
    virtual QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    double m_xmin;
    double m_xmax;
    double m_ymin;
    double m_ymax;
    bool m_fill;
    QColor m_fillcolor;
    bool m_line;
    double m_linewidth;
    QColor m_linecolor;
    bool m_marker;
    double m_markersegments;
    double m_markersize;
    QColor m_markercolor;
    bool m_markerborder;
};

#endif // XYPLOT_H
