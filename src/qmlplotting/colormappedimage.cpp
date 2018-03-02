#include "colormappedimage.h"

#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGMaterialShader>
#include <QSGTexture>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QStringList>
#include "qsgdatatexture.h"
#include "colormaps.h"

#define GLSL(ver, src) "#version " #ver "\n" #src

// ----------------------------------------------------------------------------

class QSQColormapMaterial : public QSGMaterial
{
public:
    QSGMaterialType *type() const override { static QSGMaterialType type; return &type; }
    QSGMaterialShader *createShader() const override;
    QSGTexture* m_texture_image;
    QSGDataTexture<float> m_texture_cmap;
    double m_amplitude;
    double m_offset;
    QSGTexture::Filtering m_filter;
};

class QSQColormapShader : public QSGMaterialShader
{
public:
    const char *vertexShader() const override {
        return GLSL(130,
            in highp vec4 vertex;
            in highp vec2 texcoord;
            uniform highp mat4 matrix;
            out highp vec2 coord;

            void main() {
                coord = texcoord;
                gl_Position = matrix * vertex;
            }
        );
    }

    const char *fragmentShader() const override {
        return GLSL(130,
            uniform sampler2D cmap;
            uniform sampler2D image;
            uniform highp float amplitude;
            uniform highp float offset;
            uniform lowp float opacity;
            in highp vec2 coord;
            out vec4 fragColor;

            void main() {
                bool inside = coord.s > 0. && coord.s < 1. && coord.t > 0. && coord.t < 1.;
                highp float val = texture(image, coord.st).r;
                val = amplitude * (val + offset);
                vec4 color = texture(cmap, vec2(val, 0.));
                lowp float o = opacity * color.a * float(inside);
                fragColor.rgb = color.rgb * o;
                fragColor.a = o;
            }
        );
    }

    char const *const *attributeNames() const override
    {
        static char const *const names[] = { "vertex", "texcoord", nullptr };
        return names;
    }

    void initialize() override
    {
        QSGMaterialShader::initialize();
        m_id_matrix = program()->uniformLocation("matrix");
        m_id_opacity = program()->uniformLocation("opacity");
        m_id_image = program()->uniformLocation("image");
        m_id_cmap = program()->uniformLocation("cmap");
        m_id_amplitude = program()->uniformLocation("amplitude");
        m_id_offset = program()->uniformLocation("offset");
    }

    void activate() override {
    }

    void updateState(const RenderState& state, QSGMaterial* newMaterial, QSGMaterial*) override
    {
        Q_ASSERT(program()->isLinked());
        auto* material = static_cast<QSQColormapMaterial*>(newMaterial);
        QOpenGLFunctions* functions = state.context()->functions();

        if (state.isMatrixDirty()) {
            program()->setUniformValue(m_id_matrix, state.combinedMatrix());
        }
        if (state.isOpacityDirty()) {
            program()->setUniformValue(m_id_opacity, state.opacity());
        }

        // Bind material parameters
        program()->setUniformValue(m_id_amplitude, float(material->m_amplitude));
        program()->setUniformValue(m_id_offset, float(material->m_offset));

        // Bind the material textures (image and colormap)
        functions->glActiveTexture(GL_TEXTURE1);
        program()->setUniformValue(m_id_cmap, 1);
        material->m_texture_cmap.bind();
        functions->glActiveTexture(GL_TEXTURE0);
        program()->setUniformValue(m_id_image, 0);
        material->m_texture_image->setFiltering(material->m_filter);
        material->m_texture_image->bind();
    }

    void deactivate() override {
        // unbind all textures
        QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();
        functions->glActiveTexture(GL_TEXTURE1);
        functions->glBindTexture(GL_TEXTURE_1D, 0);
        functions->glActiveTexture(GL_TEXTURE0);
        functions->glBindTexture(GL_TEXTURE_2D, 0);
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


ColormappedImage::ColormappedImage(QQuickItem *parent) : DataClient(parent)
{
    setFlag(QQuickItem::ItemHasContents);
}

ColormappedImage::~ColormappedImage()
{
    if (m_texture_cmap != nullptr) {
        m_texture_cmap->deleteLater();
    }
}

void ColormappedImage::setMinimumValue(double value)
{
    if (value != m_min_value) {
        m_min_value = value;
        emit minimumValueChanged(value);
        update();
    }
}

void ColormappedImage::setMaximumValue(double value)
{
    if (value != m_max_value) {
        m_max_value = value;
        emit maximumValueChanged(value);
        update();
    }
}

void ColormappedImage::setViewRect(const QRectF& viewrect)
{
    if (viewrect != m_view_rect) {
        m_view_rect = viewrect;
        m_new_geometry = true;
        emit viewRectChanged(m_view_rect);
        update();
    }
}

void ColormappedImage::setExtent(const QVector4D& extent)
{
    if (extent != m_extent) {
        m_extent = extent;
        m_new_geometry = true;
        emit extentChanged(extent);
        update();
    }
}

void ColormappedImage::setColormap(const QString &colormap)
{
    if (colormap != m_colormap) {
        m_colormap = colormap;
        m_new_colormap = true;
        emit colormapChanged(m_colormap);
        update();
    }
}

void ColormappedImage::setFilter(const QString &filter)
{
    QSGTexture::Filtering new_filter = (filter == QStringLiteral("linear")) ? QSGTexture::Linear : QSGTexture::Nearest;
    if (new_filter != m_filter) {
        m_filter = new_filter;
        emit filterChanged(getFilter());
        update();
    }
}

QString ColormappedImage::getFilter() const
{
    return (m_filter == QSGTexture::Linear) ? QStringLiteral("linear") : QStringLiteral("nearest");
}

static void updateColormapTexture(QSGDataTexture<float>& texture, const QString& colormap) {
    texture.setFiltering(QSGTexture::Linear);

    // Get colormap data pointer and number of samples by name
    double* data;
    int numpoints;
    if (colormap == QStringLiteral("wjet")) {
        data = const_cast<double*>(cmap_wjet);
        numpoints = sizeof(cmap_wjet) / (3*sizeof(double));
    } else if (colormap == QStringLiteral("jet")) {
        data = const_cast<double*>(cmap_jet);
        numpoints = sizeof(cmap_jet) / (3*sizeof(double));
    } else if (colormap == QStringLiteral("hot")) {
        data = const_cast<double*>(cmap_hot);
        numpoints = sizeof(cmap_hot) / (3*sizeof(double));
    } else if (colormap == QStringLiteral("bwr")) {
        data = const_cast<double*>(cmap_bwr);
        numpoints = sizeof(cmap_bwr) / (3*sizeof(double));
    } else if (colormap == QStringLiteral("viridis")) {
        data = const_cast<double*>(cmap_viridis);
        numpoints = sizeof(cmap_viridis) / (3*sizeof(double));
    } else if (colormap == QStringLiteral("ferrugineus")) {
        data = const_cast<double*>(cmap_ferrugineus);
        numpoints = sizeof(cmap_ferrugineus) / (3*sizeof(double));
    } else {
        data = const_cast<double*>(cmap_gray);
        numpoints = sizeof(cmap_gray) / (3*sizeof(double));
    }

    // Copy colormap to data texture
    float* textureBuffer = texture.allocateData2D(numpoints, 1, 3);
    for (int i = 0; i < numpoints*3; ++i) {
        textureBuffer[i] = static_cast<float>(data[i]);
    }
    texture.commitData();
}

QSGNode* ColormappedImage::updatePaintNode(QSGNode* n, QQuickItem::UpdatePaintNodeData*)
{
    QSGGeometryNode* n_geom;
    QSGGeometry* geometry;
    QSQColormapMaterial* material;

    if (n == nullptr) {
        n = new QSGNode;
    }

    if (m_source == nullptr) {
        // Remove child node if there is no data source
        if (n->firstChild() != nullptr) {
            n_geom = static_cast<QSGGeometryNode*>(n->firstChild());
            n->removeAllChildNodes();
            delete n_geom;
        }
        // Return empty node
        return n;
    }

    if (n->firstChild() == nullptr) {
        // Create child node if there is a data source
        n_geom = new QSGGeometryNode();
        n_geom->setFlag(QSGNode::OwnedByParent);
        // Inintialize geometry
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
        geometry->setDrawingMode(GL_TRIANGLE_STRIP);
        n_geom->setGeometry(geometry);
        n_geom->setFlag(QSGNode::OwnsGeometry);
        m_new_geometry = true;
        // Initialize material
        material = new QSQColormapMaterial;
        material->m_texture_image = m_source->textureProvider()->texture();
        n_geom->setMaterial(material);
        n_geom->setFlag(QSGNode::OwnsMaterial);
        // Force colormap initialization
        m_new_colormap = true;
        //
        n->appendChildNode(n_geom);
    }
    // Graph node and data source can be considered valid from here on

    n_geom = static_cast<QSGGeometryNode*>(n->firstChild());
    geometry = n_geom->geometry();
    material = static_cast<QSQColormapMaterial*>(n_geom->material());
    QSGNode::DirtyState dirty_state = QSGNode::DirtyMaterial;

    // Check for geometry changes
    if (m_new_geometry) {
        // Map function for view/extent to texture coordinates (single dimension)
        constexpr auto toTexCoords = [](const float v1, const float v_delta, const float e1, const float e2) -> std::tuple<float, float> {
            const float v_center = v1 + .5f * v_delta;
            const float e_center = .5f * (e1 + e2);
            const float e_delta = e2 - e1;
            const float x_delta = v_delta / e_delta;
            const float tex_center = (v_center - e_center) / e_delta + .5f;
            return {tex_center - .5f * x_delta, tex_center + .5f * x_delta};
        };
        const auto xCoords = toTexCoords(m_view_rect.x(), m_view_rect.width(), m_extent[0], m_extent[1]);
        const auto yCoords = toTexCoords(m_view_rect.y(), m_view_rect.height(), m_extent[2], m_extent[3]);
        // Enable blending if any texture coordinate is outside [0, 1]
        const bool blend = (std::get<0>(xCoords) < 0) || (std::get<0>(xCoords) > 1) || (std::get<1>(xCoords) < 0) || (std::get<1>(xCoords) > 1)
                || (std::get<0>(yCoords) < 0) || (std::get<0>(yCoords) > 1) || (std::get<1>(yCoords) < 0) || (std::get<1>(yCoords) > 1);
        material->setFlag(QSGMaterial::Blending, blend);
        // Update geometry and texture coordinates
        const float w = static_cast<float>(width());
        const float h = static_cast<float>(height());
        geometry->vertexDataAsTexturedPoint2D()[0] = {0, 0, std::get<0>(xCoords), std::get<1>(yCoords)};
        geometry->vertexDataAsTexturedPoint2D()[1] = {0, h, std::get<0>(xCoords), std::get<0>(yCoords)};
        geometry->vertexDataAsTexturedPoint2D()[2] = {w, 0, std::get<1>(xCoords), std::get<1>(yCoords)};
        geometry->vertexDataAsTexturedPoint2D()[3] = {w, h, std::get<1>(xCoords), std::get<0>(yCoords)};
        dirty_state |= QSGNode::DirtyGeometry;
        m_new_geometry = false;
    }

    // Check for data source change
    if (m_new_source) {
        material->m_texture_image = m_source->textureProvider()->texture();
        m_new_source = false;
    }

    // Check if the image texture should be updated
    if (m_new_data) {
        static_cast<QSGDynamicTexture*>(material->m_texture_image)->updateTexture();
        m_new_data = false;
    }

    // Check if the colormap should be updated
    if (m_new_colormap) {
        updateColormapTexture(material->m_texture_cmap, m_colormap);
        m_new_colormap = false;
    }

    // Update material parameters
    double cmap_margin = .5 / material->m_texture_cmap.getDim(0);
    material->m_amplitude = (1. - 2.*cmap_margin) / (m_max_value - m_min_value);
    material->m_offset = (cmap_margin / material->m_amplitude) - m_min_value;
    material->m_filter = m_filter;

    n->markDirty(dirty_state);
    n_geom->markDirty(dirty_state);
    return n;
}
