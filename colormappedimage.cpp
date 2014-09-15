#include "colormappedimage.h"

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGMaterialShader>
#include <QSGTexture>
#include <QOpenGLContext>
#include "qsgfloattexture.h"

#include "math.h"

class QSQColormapMaterial : public QSGMaterial
{
public:
    QSGMaterialType *type() const { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const;
    QSGTexture* m_texture_image;
    QSGTexture* m_texture_cmap;
    double m_amplitude;
    double m_offset;
};

class QSQColormapShader : public QSGMaterialShader
{
public:
    const char *vertexShader() const {
        return
        "attribute highp vec4 vertex;          \n"
        "attribute highp vec2 texcoord;        \n"
        "uniform highp mat4 matrix;            \n"
        "varying highp vec2 coord;             \n"
        "                                      \n"
        "void main() {                         \n"
        "    coord = texcoord;                 \n"
        "    gl_Position = matrix * vertex;    \n"
        "}";
    }

    const char *fragmentShader() const {
        return
        "uniform sampler2D image;                                \n"
        "uniform sampler2D cmap;                                 \n"
        "uniform highp float amplitude;                          \n"
        "uniform highp float offset;                             \n"
        "varying highp vec2 coord;                               \n"
        "uniform lowp float opacity;                             \n"
        "void main() {                                           \n"
        "    highp float val = texture2D(image, coord.st).r;     \n"
        "    val = amplitude * (val + offset);                   \n"
        "    vec4 color = texture2D(cmap, vec2(val, 1.));        \n"
        "    gl_FragColor = color * opacity;                     \n"
        "}";
    }

    char const *const *attributeNames() const
    {
        static char const *const names[] = { "vertex", "texcoord", 0 };
        return names;
    }

    void initialize()
    {
        QSGMaterialShader::initialize();
        m_id_matrix = program()->uniformLocation("matrix");
        m_id_opacity = program()->uniformLocation("opacity");
        m_id_image = program()->uniformLocation("image");
        m_id_cmap = program()->uniformLocation("cmap");
        m_id_amplitude = program()->uniformLocation("amplitude");
        m_id_offset = program()->uniformLocation("offset");
    }

    void activate() {
    }

    void updateState(const RenderState &state, QSGMaterial *newMaterial, QSGMaterial *)
    {
        Q_ASSERT(program()->isLinked());
        QSQColormapMaterial* material = static_cast<QSQColormapMaterial*>(newMaterial);

        if (state.isMatrixDirty())
            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
        if (state.isOpacityDirty())
            program()->setUniformValue(m_id_opacity, state.opacity());

        // bind material parameters
        program()->setUniformValue(m_id_amplitude, float(material->m_amplitude));
        program()->setUniformValue(m_id_offset, float(material->m_offset));

        // bind the material textures (image and colormap)
        glActiveTexture(GL_TEXTURE1);
        program()->setUniformValue(m_id_cmap, 1);
        material->m_texture_cmap->bind();
        glActiveTexture(GL_TEXTURE0);
        program()->setUniformValue(m_id_image, 0);
        material->m_texture_image->bind();
    }

    void deactivate() {
        // unbind all textures
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    int m_id_matrix;
    int m_id_opacity;
    int m_id_image;
    int m_id_cmap;
    int m_id_amplitude;
    int m_id_offset;
};


inline QSGMaterialShader* QSQColormapMaterial::createShader() const { return new QSQColormapShader; }

// ----------------------------------------------------------------------------


ColormappedImage::ColormappedImage(QQuickItem *parent) :
    DataClient(parent),
    m_min_value(0.), m_max_value(1.), m_texture_cmap(nullptr)
{
    setFlag(QQuickItem::ItemHasContents);
}

ColormappedImage::~ColormappedImage()
{
    if (m_texture_cmap) {
        m_texture_cmap->deleteLater();
    }
}

void ColormappedImage::setMinimumValue(double value)
{
    if (value == m_min_value) return;
    m_min_value = value;
    emit minimumValueChanged(value);
    update();
}

void ColormappedImage::setMaximumValue(double value)
{
    if (value == m_max_value) return;
    m_max_value = value;
    emit maximumValueChanged(value);
    update();
}

QSGNode *ColormappedImage::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode* n = static_cast<QSGGeometryNode*>(node);
    QSGGeometry *geometry;
    QSQColormapMaterial *material;

    if (n && !m_datacontainer) {
        // remove the node if there is no data source
        delete n;
        return nullptr;
    }

    if  (!n && m_datacontainer) {
        // create a node if there is a data source
        n = new QSGGeometryNode();
        // inintialize geometry
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
        geometry->setDrawingMode(GL_TRIANGLE_STRIP);
        n->setGeometry(geometry);
        n->setFlag(QSGNode::OwnsGeometry);
        m_new_geometry = true;
        // initialize material
        material = new QSQColormapMaterial;
        n->setMaterial(material);
        n->setFlag(QSGNode::OwnsMaterial);
        // initialize colormap
        GLfloat data_cmap[] = {
            .35, .35, .95,
            .95, .95, .95,
            .95, .35, .35
        };
        if (!m_texture_cmap) {
            m_texture_cmap = new QSGFloatTexture();
            m_texture_cmap->updateTextureData(data_cmap, 3, 1, 3);
        }
        material->m_texture_cmap = m_texture_cmap;
        material->m_texture_image = m_datacontainer->textureProvider()->texture();
    }

    if (!n) {
        // return null if there is nothing to be drawn
        return nullptr;
    }

    // ** graph node and data source can be considered valid from here on **

    geometry = n->geometry();
    material = static_cast<QSQColormapMaterial*>(n->material());
    QSGNode::DirtyState dirty_state = QSGNode::DirtyMaterial;

    // update geometry in case the width and height changed
    if (m_new_geometry) {
        double w = width();
        double h = height();
        geometry->vertexDataAsTexturedPoint2D()[0].set(0, 0, 0, 0);
        geometry->vertexDataAsTexturedPoint2D()[1].set(0, h, 0, 1);
        geometry->vertexDataAsTexturedPoint2D()[2].set(w, 0, 1, 0);
        geometry->vertexDataAsTexturedPoint2D()[3].set(w, h, 1, 1);
        dirty_state |= QSGNode::DirtyGeometry;
        m_new_geometry = false;
    }

    // update material parameters
    material->m_amplitude = 1. / (m_max_value - m_min_value);
    material->m_offset = -m_min_value;

    // check for data source change
    if (m_new_container) {
        material->m_texture_image = m_datacontainer->textureProvider()->texture();
        m_new_container = false;
    }

    // check if the image texture should be updated
    if (m_new_data) {
        static_cast<DataTexture*>(material->m_texture_image)->updateTexture();
        m_new_data = false;
    }

    n->markDirty(dirty_state);
    return n;
}
