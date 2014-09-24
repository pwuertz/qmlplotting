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


XYPlot::XYPlot(QQuickItem *parent) :
    DataClient(parent),
    m_xmin(0.), m_xmax(1.),
    m_ymin(0.), m_ymax(1.),
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

QSGNode *XYPlot::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode* n = static_cast<QSGGeometryNode*>(node);
    QSGGeometry *geometry;
    XYMarkerMaterial *material;

    if (n && !m_source) {
        // remove the node if there is no data source
        delete n;
        return nullptr;
    }

    if  (!n && m_source) {
        // create a node if there is a data source
        n = new QSGGeometryNode();
        // inintialize geometry & material
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        geometry->setDrawingMode(GL_POINTS);
        material = new XYMarkerMaterial;
        n->setGeometry(geometry);
        n->setFlag(QSGNode::OwnsGeometry);
        n->setMaterial(material);
        n->setFlag(QSGNode::OwnsMaterial);
    }

    if (!n) {
        // return null if there is nothing to be drawn
        return nullptr;
    }

    // ** graph node and data source can be considered valid from here on **

    geometry = n->geometry();
    material = static_cast<XYMarkerMaterial*>(n->material());
    QSGNode::DirtyState dirty_state = QSGNode::DirtyMaterial;

    // update material parameters
    material->m_size.setWidth(width());
    material->m_size.setHeight(height());
    material->m_scale.setWidth(1. / (m_xmax - m_xmin));
    material->m_scale.setHeight(1. / (m_ymax - m_ymin));
    material->m_offset.setX(m_xmin);
    material->m_offset.setY(m_ymin);
    material->m_markersegments = m_markersegments;
    material->m_markercolor = m_markercolor;
    material->m_markersize = m_markersize;
    material->setFlag(QSGMaterial::Blending, m_markercolor.alphaF() != 1.);

    // reallocate geometry if number of points changed
    int num_points = m_source->dataWidth() / 2;
    if (geometry->vertexCount() != num_points) {
        geometry->allocate(num_points);
        m_new_data = true;
    }

    // update geometry
    if (m_new_source || m_new_data) {
        m_new_source = false;
        m_new_data = false;
        double* src = (double*) m_source->data();
        float* dst = static_cast<float*>(geometry->vertexData());
        for (int i = 0; i < num_points*2; ++i) {
            dst[i] = src[i];
        }
        dirty_state |= QSGNode::DirtyGeometry;
    }

    n->markDirty(dirty_state);
    return n;
}
