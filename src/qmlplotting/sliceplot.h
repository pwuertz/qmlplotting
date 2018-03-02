#ifndef LINEPLOT_H
#define LINEPLOT_H

#include <QColor>
#include "dataclient.h"

class SlicePlot : public DataClient
{
    Q_OBJECT
    Q_PROPERTY(double minimumValue READ minimumValue WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(double maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(int numSegments READ numSegments WRITE setNumSegments NOTIFY numSegmentsChanged)
    Q_PROPERTY(QPointF p1 READ p1 WRITE setP1 NOTIFY p1Changed)
    Q_PROPERTY(QPointF p2 READ p2 WRITE setP2 NOTIFY p2Changed)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool filled MEMBER m_filled WRITE setFilled NOTIFY filledChanged)

public:
    explicit SlicePlot(QQuickItem *parent = nullptr);
    ~SlicePlot() override;

    double minimumValue() const {return m_min_value;}
    double maximumValue() const {return m_max_value;}
    int numSegments() const {return m_num_segments;}
    const QPointF& p1() const {return m_p1;}
    const QPointF& p2() const {return m_p2;}
    const QColor& color() const {return m_color;}

    void setMinimumValue(double value);
    void setMaximumValue(double value);
    void setNumSegments(int n);
    void setP1(const QPointF& p);
    void setP2(const QPointF& p);
    void setColor(const QColor& color);
    void setFilled(bool filled);

signals:
    void minimumValueChanged(double);
    void maximumValueChanged(double);
    void numSegmentsChanged(int);
    void p1Changed(const QPointF&);
    void p2Changed(const QPointF&);
    void colorChanged(const QColor& color);
    void filledChanged(bool);

protected:
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* updatePaintNodeData) override;

private:
    double m_min_value;
    double m_max_value;
    int m_num_segments;
    QPointF m_p1;
    QPointF m_p2;
    QColor m_color;
    bool m_filled;
};

#endif // LINEPLOT_H
