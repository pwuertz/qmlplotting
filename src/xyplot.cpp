#include "xyplot.h"

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

#define GLSL(ver, src) "#version " #ver "\n" #src


class XYMarkerMaterial : public QSGMaterial
{
public:
    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const;
    int m_markersegments;
    double m_markersize;
    QSizeF m_size;
    QSizeF m_scale;
    QPointF m_offset;
    QColor m_markercolor;
};

class XYMarkerMaterialShader : public QSGMaterialShader
{
public:
    const char *vertexShader() const {
        return GLSL(130,
            attribute highp vec4 vertex;
            uniform highp mat4 matrix;
            uniform highp vec2 size;
            uniform highp vec2 scale;
            uniform highp vec2 offset;

            void main() {
                highp vec2 p = (vertex.xy - offset) * scale * size;
                gl_Position = matrix * vec4(p.x, size.y - p.y, 0., 1.);
            }
        );
    }

    const char *geometryShader() const {
        return GLSL(150 core,
            layout(points) in;
            layout(triangle_strip, max_vertices = 60) out;
            const float PI = 3.1415926;

            uniform int msegments;
            uniform float mradius;

            void main() {
                highp float dphi = PI * 2.0 / float(msegments);
                for (int i = 0; i < msegments; i++) {
                    highp float phi1 = dphi * float(i);
                    highp float phi2 = phi1 + dphi;
                    gl_Position = gl_in[0].gl_Position + mradius*vec4(-sin(phi1)*.9, cos(phi1), 0., 0.);
                    EmitVertex();
                    gl_Position = gl_in[0].gl_Position + mradius*vec4(-sin(phi2)*.9, cos(phi2), 0., 0.);
                    EmitVertex();
                    gl_Position = gl_in[0].gl_Position;
                    EmitVertex();
                    EndPrimitive();
                }
            }
        );
    }

    const char *fragmentShader() const {
        return GLSL(130,
            uniform lowp vec4 mcolor;
            uniform lowp float opacity;
            out vec4 fragColor;

            void main() {
                lowp float o = opacity * mcolor.a;
                fragColor.rgb = mcolor.rgb * o;
                fragColor.a = o;
            }
        );
    }

    char const *const *attributeNames() const
    {
        static char const *const names[] = { "vertex", 0 };
        return names;
    }

    void initialize()
    {
        QSGMaterialShader::initialize();
        QOpenGLShaderProgram* p = program();

        if (QOpenGLShader::hasOpenGLShaders(QOpenGLShader::Geometry)) {
            p->addShaderFromSourceCode(QOpenGLShader::Geometry, geometryShader());
            p->link();
        }
        // TODO: backup solution for OpenGL version < 3.2

        m_id_matrix = p->uniformLocation("matrix");
        m_id_opacity = p->uniformLocation("opacity");
        m_id_size = p->uniformLocation("size");
        m_id_scale = p->uniformLocation("scale");
        m_id_offset = p->uniformLocation("offset");
        m_id_msegments = p->uniformLocation("msegments");
        m_id_mradius = p->uniformLocation("mradius");
        m_id_mcolor = p->uniformLocation("mcolor");
    }

    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *)
    {
        Q_ASSERT(program()->isLinked());
        XYMarkerMaterial* material = static_cast<XYMarkerMaterial*>(newMaterial);
        QOpenGLShaderProgram* p = program();

        if (state.isMatrixDirty())
            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
        if (state.isOpacityDirty())
            program()->setUniformValue(m_id_opacity, state.opacity());

        // bind material parameters
        p->setUniformValue(m_id_size, material->m_size);
        p->setUniformValue(m_id_scale, material->m_scale);
        p->setUniformValue(m_id_offset, material->m_offset);
        p->setUniformValue(m_id_msegments, material->m_markersegments);
        float mradius = .5 * material->m_markersize * state.combinedMatrix()(0, 0);
        p->setUniformValue(m_id_mradius, mradius);
        p->setUniformValue(m_id_mcolor, material->m_markercolor);
    }

private:
    int m_id_matrix;
    int m_id_opacity;
    int m_id_size;
    int m_id_scale;
    int m_id_offset;
    int m_id_msegments;
    int m_id_mradius;
    int m_id_mcolor;
};

inline QSGMaterialShader* XYMarkerMaterial::createShader() const { return new XYMarkerMaterialShader; }


class XYLineMaterial : public QSGMaterial
{
public:
    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const;
    QSizeF m_size;
    QSizeF m_scale;
    QPointF m_offset;
    QColor m_color;
};

class XYLineMaterialShader : public QSGMaterialShader
{
public:
    const char *vertexShader() const {
        return GLSL(130,
            attribute highp vec4 vertex;
            uniform highp mat4 matrix;
            uniform highp vec2 size;
            uniform highp vec2 scale;
            uniform highp vec2 offset;

            void main() {
                highp vec2 p = (vertex.xy - offset) * scale * size;
                gl_Position = matrix * vec4(p.x, size.y - p.y, 0., 1.);
            }
        );
    }

    const char *fragmentShader() const {
        return GLSL(130,
            uniform lowp vec4 color;
            uniform lowp float opacity;
            out vec4 fragColor;

            void main() {
                fragColor = vec4(color.rgb*color.a, color.a) * opacity;
            }
        );
    }

    char const *const *attributeNames() const
    {
        static char const *const names[] = {"vertex", 0};
        return names;
    }

    void initialize()
    {
        QSGMaterialShader::initialize();
        QOpenGLShaderProgram* p = program();

        m_id_matrix = p->uniformLocation("matrix");
        m_id_opacity = p->uniformLocation("opacity");
        m_id_size = p->uniformLocation("size");
        m_id_scale = p->uniformLocation("scale");
        m_id_offset = p->uniformLocation("offset");
        m_id_color = p->uniformLocation("color");
    }

    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *)
    {
        Q_ASSERT(program()->isLinked());
        XYLineMaterial* material = static_cast<XYLineMaterial*>(newMaterial);
        QOpenGLShaderProgram* p = program();

        if (state.isMatrixDirty())
            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
        if (state.isOpacityDirty())
            program()->setUniformValue(m_id_opacity, state.opacity());

        // bind material parameters
        p->setUniformValue(m_id_size, material->m_size);
        p->setUniformValue(m_id_scale, material->m_scale);
        p->setUniformValue(m_id_offset, material->m_offset);
        p->setUniformValue(m_id_color, material->m_color);
    }

private:
    int m_id_matrix;
    int m_id_opacity;
    int m_id_size;
    int m_id_scale;
    int m_id_offset;
    int m_id_color;
};

inline QSGMaterialShader* XYLineMaterial::createShader() const { return new XYLineMaterialShader; }


XYPlot::XYPlot(QQuickItem *parent) :
    DataClient(parent),
    m_xmin(0.), m_xmax(1.),
    m_ymin(0.), m_ymax(1.),
    m_line(true),
    m_linewidth(1.),
    m_linecolor(0., 0., 0.),
    m_marker(true),
    m_markersegments(8),
    m_markersize(5.),
    m_markercolor(0., 0., 0.)
{
    setFlag(QQuickItem::ItemHasContents);
    setClip(true);
}

void XYPlot::setXMin(double value)
{
    if (m_xmin == value) return;
    m_xmin = value;
    emit xMinChanged(m_xmin);
    update();
}

void XYPlot::setXMax(double value)
{
    if (m_xmax == value) return;
    m_xmax = value;
    emit xMaxChanged(m_xmax);
    update();
}

void XYPlot::setYMin(double value)
{
    if (m_ymin == value) return;
    m_ymin = value;
    emit yMinChanged(m_ymin);
    update();
}

void XYPlot::setYMax(double value)
{
    if (m_ymax == value) return;
    m_ymax = value;
    emit yMaxChanged(m_ymax);
    update();
}

void XYPlot::setLineEnabled(bool enabled)
{
    if (m_line == enabled) return;
    m_line = enabled;
    emit lineEnabledChanged(m_line);
    update();
}

void XYPlot::setLineWidth(double width)
{
    if (m_linewidth == width) return;
    m_linewidth = width;
    emit lineWidthChanged(m_linewidth);
    update();
}

void XYPlot::setLineColor(const QColor &color)
{
    if (m_linecolor == color) return;
    m_linecolor = color;
    emit lineColorChanged(m_linecolor);
    update();
}

void XYPlot::setMarkerEnabled(bool enabled)
{
    if (m_marker == enabled) return;
    m_marker = enabled;
    emit lineEnabledChanged(m_marker);
    update();
}

void XYPlot::setMarkerSegments(int n)
{
    if (m_markersegments == n) return;
    m_markersegments = n;
    emit markerSegmentsChanged(m_markersegments);
    update();
}

void XYPlot::setMarkerSize(double size)
{
    if (m_markersize == size) return;
    m_markersize = size;
    emit markerSizeChanged(m_markersize);
    update();
}

void XYPlot::setMarkerColor(const QColor &color)
{
    if (m_markercolor == color) return;
    m_markercolor = color;
    emit markerColorChanged(m_markercolor);
    update();
}


class LineNode : public QSGGeometryNode
{
public:
    LineNode() : QSGGeometryNode() {
        QSGGeometry* geometry;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(GL_LINE_STRIP);
        QSGMaterial* material;
        material = new XYLineMaterial;
        setGeometry(geometry);
        setFlag(QSGNode::OwnsGeometry);
        setMaterial(material);
        setFlag(QSGNode::OwnsMaterial);
    }
    virtual ~LineNode() {}
    virtual bool isSubtreeBlocked() const {return m_blocked;}
    bool m_blocked;
};


class MarkerNode : public QSGGeometryNode
{
public:
    MarkerNode() : QSGGeometryNode() {
        QSGGeometry* geometry;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(GL_POINTS);
        QSGMaterial* material;
        material = new XYMarkerMaterial;
        setGeometry(geometry);
        setFlag(QSGNode::OwnsGeometry);
        setMaterial(material);
        setFlag(QSGNode::OwnsMaterial);
    }
    virtual ~MarkerNode() {}
    virtual bool isSubtreeBlocked() const {return m_blocked;}
    bool m_blocked;
};


QSGNode *XYPlot::updatePaintNode(QSGNode *n, QQuickItem::UpdatePaintNodeData *)
{
    LineNode* n_line;
    MarkerNode* n_marker;
    QSGGeometry *mgeometry;
    XYMarkerMaterial *mmaterial;
    QSGGeometry *lgeometry;
    XYLineMaterial *lmaterial;

    if (n && !m_source) {
        // remove the node if there is no data source
        delete n;
        return nullptr;
    }

    if  (!n && m_source) {
        // create a base node if there is a data source
        n = new QSGNode;
        // append child nodes for line and markers
        n_line = new LineNode;
        n_marker = new MarkerNode;
        n_line->setFlag(QSGNode::OwnedByParent);
        n_marker->setFlag(QSGNode::OwnedByParent);
        n->appendChildNode(n_line);
        n->appendChildNode(n_marker);
    }

    if (!n) {
        // return null if there is nothing to be drawn
        return nullptr;
    }

    // ** graph node and data source can be considered valid from here on **

    n_line = static_cast<LineNode*>(n->firstChild());
    n_marker = static_cast<MarkerNode*>(n->lastChild());
    lgeometry = n_line->geometry();
    mgeometry = n_marker->geometry();
    lmaterial = static_cast<XYLineMaterial*>(n_line->material());
    mmaterial = static_cast<XYMarkerMaterial*>(n_marker->material());

    QSGNode::DirtyState dirty_state = QSGNode::DirtyMaterial;

    n_line->m_blocked = !m_line;
    n_marker->m_blocked = !m_marker;
    int num_data_points = m_source->dataWidth() / 2;

    if (m_line) {
        // update line material parameters
        lmaterial->m_size.setWidth(width());
        lmaterial->m_size.setHeight(height());
        lmaterial->m_scale.setWidth(1. / (m_xmax - m_xmin));
        lmaterial->m_scale.setHeight(1. / (m_ymax - m_ymin));
        lmaterial->m_offset.setX(m_xmin);
        lmaterial->m_offset.setY(m_ymin);
        lmaterial->m_color = m_linecolor;
        lmaterial->setFlag(QSGMaterial::Blending, m_linecolor.alphaF() != 1.);
        lgeometry->setLineWidth(m_linewidth);

        // reallocate geometry if number of points changed
        if (lgeometry->vertexCount() != num_data_points) {
            lgeometry->allocate(num_data_points);
            m_new_data = true;
        }
    }

    if (m_marker) {
        // update marker material parameters
        mmaterial->m_size.setWidth(width());
        mmaterial->m_size.setHeight(height());
        mmaterial->m_scale.setWidth(1. / (m_xmax - m_xmin));
        mmaterial->m_scale.setHeight(1. / (m_ymax - m_ymin));
        mmaterial->m_offset.setX(m_xmin);
        mmaterial->m_offset.setY(m_ymin);
        mmaterial->m_markersegments = m_markersegments;
        mmaterial->m_markercolor = m_markercolor;
        mmaterial->m_markersize = m_markersize;
        mmaterial->setFlag(QSGMaterial::Blending, m_markercolor.alphaF() != 1.);

        // reallocate geometry if number of points changed
        if (mgeometry->vertexCount() != num_data_points) {
            mgeometry->allocate(num_data_points);
            m_new_data = true;
        }
    }

    // update geometry
    if (m_new_source || m_new_data) {
        m_new_source = false;
        m_new_data = false;
        double* src = (double*) m_source->data();
        float* ldst = static_cast<float*>(lgeometry->vertexData());
        float* mdst = static_cast<float*>(mgeometry->vertexData());
        if (m_line) {
            for (int i = 0; i < num_data_points*2; ++i) ldst[i] = src[i];
        }
        if (m_marker) {
            for (int i = 0; i < num_data_points*2; ++i) mdst[i] = src[i];
        }
        dirty_state |= QSGNode::DirtyGeometry;
    }

    n->markDirty(dirty_state);
    n_line->markDirty(dirty_state);
    n_marker->markDirty(dirty_state);
    return n;
}
