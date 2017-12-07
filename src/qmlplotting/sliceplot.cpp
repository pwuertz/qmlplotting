#include "sliceplot.h"

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

#define GLSL(ver, src) "#version " #ver "\n" #src

class SlicePlotMaterial : public QSGMaterial
{
public:
    SlicePlotMaterial(bool filled) : QSGMaterial(), m_filled(filled) {}
    virtual ~SlicePlotMaterial() {}
    QSGTexture* m_texture_data;
    double m_width;
    double m_height;
    double m_amplitude;
    double m_offset;
    QPointF m_p1;
    QPointF m_p2;
    QColor m_color;
    const bool m_filled;
};

class SliceLinePlotMaterial : public SlicePlotMaterial
{
public:
    SliceLinePlotMaterial() : SlicePlotMaterial(false) {}
    virtual ~SliceLinePlotMaterial() {}
    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const;
};

class SliceFillPlotMaterial : public SlicePlotMaterial
{
public:
    SliceFillPlotMaterial() : SlicePlotMaterial(true) {
        setFlag(QSGMaterial::Blending);
    }
    virtual ~SliceFillPlotMaterial() {}
    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const;
};

class SlicePlotShader : public QSGMaterialShader
{
public:
    SlicePlotShader() : QSGMaterialShader() {}
    virtual ~SlicePlotShader() {}

    char const *const *attributeNames() const
    {
        static char const *const names[] = { "vertex", 0 };
        return names;
    }

    void initialize()
    {
        QSGMaterialShader::initialize();
        m_id_matrix = program()->uniformLocation("matrix");
        m_id_opacity = program()->uniformLocation("opacity");
        m_id_color = program()->uniformLocation("color");
        m_id_data = program()->uniformLocation("image");
        m_id_width = program()->uniformLocation("width");
        m_id_height = program()->uniformLocation("height");
        m_id_amplitude = program()->uniformLocation("amplitude");
        m_id_offset = program()->uniformLocation("offset");
        m_id_p1 = program()->uniformLocation("p1");
        m_id_p2 = program()->uniformLocation("p2");
    }

    void activate() {
    }

    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *)
    {
        Q_ASSERT(program()->isLinked());
        SliceLinePlotMaterial* material = static_cast<SliceLinePlotMaterial*>(newMaterial);

        if (state.isMatrixDirty())
            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
        if (state.isOpacityDirty())
            program()->setUniformValue(m_id_opacity, state.opacity());

        // bind material parameters
        program()->setUniformValue(m_id_width, float(material->m_width));
        program()->setUniformValue(m_id_height, float(material->m_height));
        program()->setUniformValue(m_id_amplitude, float(material->m_amplitude));
        program()->setUniformValue(m_id_offset, float(material->m_offset));
        program()->setUniformValue(m_id_p1, material->m_p1);
        program()->setUniformValue(m_id_p2, material->m_p2);
        program()->setUniformValue(m_id_color, material->m_color);

        // bind the material data texture
        program()->setUniformValue(m_id_data, 0);
        material->m_texture_data->bind();
    }

    void deactivate() {
    }

protected:
    int m_id_matrix;
    int m_id_opacity;
    int m_id_color;
    int m_id_width;
    int m_id_height;
    int m_id_data;
    int m_id_amplitude;
    int m_id_offset;
    int m_id_p1;
    int m_id_p2;
};

class SliceLinePlotShader : public SlicePlotShader
{
public:
    SliceLinePlotShader() : SlicePlotShader() {}
    virtual ~SliceLinePlotShader() {}

    const char *vertexShader() const {
        return GLSL(130,
            in highp vec2 vertex;
            uniform sampler2D data;
            uniform highp float width;
            uniform highp float height;
            uniform highp float amplitude;
            uniform highp float offset;
            uniform highp vec2 p1;
            uniform highp vec2 p2;
            uniform highp mat4 matrix;

            void main() {
                highp vec2 pos = (1.-vertex.x)*p1 + vertex.x*p2;
                highp float val = texture(data, pos).r;
                highp float yval = amplitude * (val+offset);
                gl_Position = matrix * vec4(width * vertex.x, height * (1.-yval), 0., 1.);
            }
        );
    }

    const char *fragmentShader() const {
        return GLSL(130,
            uniform sampler2D data;
            uniform lowp float opacity;
            uniform mediump vec4 color;
            out vec4 fragColor;

            void main() {
                lowp float o = opacity * color.a;
                fragColor.rgb = color.rgb * o;
                fragColor.a = o;
            }
        );
    }
};

class SliceFillPlotShader : public SlicePlotShader
{
public:
    SliceFillPlotShader() : SlicePlotShader() {}
    virtual ~SliceFillPlotShader() {}

    const char *vertexShader() const {
        return GLSL(130,
            in highp vec2 vertex;
            uniform sampler2D data;
            uniform highp float width;
            uniform highp float height;
            uniform highp vec2 p1;
            uniform highp vec2 p2;
            uniform highp mat4 matrix;
            out highp vec2 pos;
            out highp vec2 coord;

            void main() {
                coord = (1.-vertex.x)*p1 + vertex.x*p2;
                pos = vec2(vertex.x, 1.-vertex.y);
                gl_Position = matrix * vec4(width * vertex.x, height * vertex.y, 0., 1.);
            }
        );
    }

    const char *fragmentShader() const {
        return GLSL(130,
            uniform sampler2D data;
            uniform lowp float opacity;
            uniform highp float amplitude;
            uniform highp float offset;
            uniform mediump vec4 color;
            in highp vec2 pos;
            in highp vec2 coord;
            out vec4 fragColor;

            void main() {
                highp float pos_y = pos.y - amplitude*offset;
                highp float y_val = amplitude * texture(data, coord).r;
                highp float dist_above_curve = sign(y_val) * (pos_y-y_val);
                float below_curve = float(dist_above_curve < 0.);
                float above_zero = float(sign(y_val) * pos_y >= 0.);
                float fill = below_curve * above_zero;
                lowp float o = opacity * color.a * fill;
                fragColor.rgb = color.rgb * o;
                fragColor.a = o;
            }
        );
    }
};

inline QSGMaterialShader* SliceLinePlotMaterial::createShader() const { return new SliceLinePlotShader; }
inline QSGMaterialShader* SliceFillPlotMaterial::createShader() const { return new SliceFillPlotShader; }

// ----------------------------------------------------------------------------


SlicePlot::SlicePlot(QQuickItem *parent) :
    DataClient(parent),
    m_min_value(0.), m_max_value(1.), m_num_segments(20),
    m_p1(0., 0.), m_p2(1., 1.),
    m_color(Qt::red), m_filled(false)
{
    setFlag(QQuickItem::ItemHasContents);
    setClip(true);
}

SlicePlot::~SlicePlot()
{

}

void SlicePlot::setMinimumValue(double value)
{
    if (value == m_min_value) return;
    m_min_value = value;
    emit minimumValueChanged(value);
    update();
}

void SlicePlot::setMaximumValue(double value)
{
    if (value == m_max_value) return;
    m_max_value = value;
    emit maximumValueChanged(value);
    update();
}

void SlicePlot::setNumSegments(int n)
{
    if (n == m_num_segments) return;
    m_num_segments = n;
    m_new_geometry = true;
    emit numSegmentsChanged(n);
    update();
}

void SlicePlot::setP1(const QPointF &p)
{
    if (m_p1 == p) return;
    m_p1 = p;
    emit p1Changed(m_p1);
    update();
}

void SlicePlot::setP2(const QPointF &p)
{
    if (m_p2 == p) return;
    m_p2 = p;
    emit p2Changed(m_p2);
    update();
}

void SlicePlot::setColor(const QColor &color)
{
    if (m_color == color) return;
    m_color = color;
    emit colorChanged(m_color);
    update();
}

void SlicePlot::setFilled(bool filled)
{
    if (m_filled == filled) return;
    m_filled = filled;
    emit filledChanged(m_filled);
    update();
}

QSGNode *SlicePlot::updatePaintNode(QSGNode *n, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode* n_geom;
    QSGGeometry *geometry;
    SlicePlotMaterial *material;

    if (!n) {
        n = new QSGNode;
    }

    if (!m_source) {
        // remove child node if there is no data source
        if (n->firstChild()) {
            n_geom = static_cast<QSGGeometryNode*>(n->firstChild());
            n->removeAllChildNodes();
            delete n_geom;
        }
        // return empty node
        return n;
    }

    if (!n->firstChild()) {
        // create child node if there is a data source
        n_geom = new QSGGeometryNode();
        n_geom->setFlag(QSGNode::OwnedByParent);
        // inintialize geometry & material
        if (m_filled) {
            geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            geometry->setDrawingMode(GL_TRIANGLE_STRIP);
            material = new SliceFillPlotMaterial;
            material->m_texture_data = m_source->textureProvider()->texture();
        } else {
            geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
            geometry->setDrawingMode(GL_LINE_STRIP);
            geometry->setLineWidth(1);
            material = new SliceLinePlotMaterial;
            material->m_texture_data = m_source->textureProvider()->texture();
        }
        n_geom->setGeometry(geometry);
        n_geom->setFlag(QSGNode::OwnsGeometry);
        n_geom->setMaterial(material);
        n_geom->setFlag(QSGNode::OwnsMaterial);
        //
        n->appendChildNode(n_geom);
    }

    // ** graph node and data source can be considered valid from here on **

    n_geom = static_cast<QSGGeometryNode*>(n->firstChild());
    geometry = n_geom->geometry();
    material = static_cast<SlicePlotMaterial*>(n_geom->material());
    QSGNode::DirtyState dirty_state = QSGNode::DirtyMaterial;

    // check if the filled parameter was changed, switch material if necessary
    if (m_filled != material->m_filled) {
        if (m_filled) {
            geometry->allocate(0);
            geometry->setDrawingMode(GL_TRIANGLE_STRIP);
            material = new SliceFillPlotMaterial;
            material->m_texture_data = m_source->textureProvider()->texture();
        } else {
            geometry->allocate(0);
            geometry->setDrawingMode(GL_LINE_STRIP);
            geometry->setLineWidth(1.);
            material = new SliceLinePlotMaterial;
            material->m_texture_data = m_source->textureProvider()->texture();
        }
        n_geom->setMaterial(material);
    }

    // update material parameters
    material->m_width = width();
    material->m_height = height();

    //material->m_amplitude = - scale_val;
    //material->m_offset = off_val - 1./scale_val;
    material->m_amplitude = 1. / (m_max_value - m_min_value);
    material->m_offset = -m_min_value;

    material->m_p1 = m_p1;
    material->m_p2 = m_p2;
    material->m_color = m_color;

    // update geometry if the number of segments changed
    if (m_filled) {
        if (geometry->vertexCount() != 4) {
            geometry->allocate(4);
            geometry->vertexDataAsPoint2D()[0].set(0, 0);
            geometry->vertexDataAsPoint2D()[1].set(0, 1);
            geometry->vertexDataAsPoint2D()[2].set(1, 0);
            geometry->vertexDataAsPoint2D()[3].set(1, 1);
            dirty_state |= QSGNode::DirtyGeometry;
        }
    } else {
        if (geometry->vertexCount() != (m_num_segments+1)) {
            geometry->allocate(m_num_segments+1);
            float* data = static_cast<float*>(geometry->vertexData());
            for (int i = 0; i <= m_num_segments; ++i) {
                double f = i * (1./m_num_segments);
                data[2*i] = f;
            }
            dirty_state |= QSGNode::DirtyGeometry;
        }
    }

    // check for data source change
    if (m_new_source) {
        material->m_texture_data = m_source->textureProvider()->texture();
        m_new_source = false;
    }

    // check if the image texture should be updated
    if (m_new_data) {
        static_cast<QSGDynamicTexture*>(material->m_texture_data)->updateTexture();
        m_new_data = false;
    }

    n->markDirty(dirty_state);
    n_geom->markDirty(dirty_state);
    return n;
}
