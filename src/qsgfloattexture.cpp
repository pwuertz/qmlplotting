#include "qsgfloattexture.h"


QSGFloatTexture::QSGFloatTexture() :
    QSGDynamicTexture(),
    m_id_texture(0),
    m_num_dims(0),
    m_num_components(0),
    m_buffer(nullptr),
    m_buffer_size(0),
    m_needs_upload(false)
{
    m_dims[0] = 0;
    m_dims[1] = 0;
    m_dims[2] = 0;
}

QSGFloatTexture::~QSGFloatTexture() {
    if (m_buffer) {
        delete[] m_buffer;
    }

    if (m_id_texture) {
        if (!QOpenGLContext::currentContext()) {
            qWarning("QSGFloatTexture::~QSGFloatTexture needs OpenGL context");
        }
        glDeleteTextures(1, &m_id_texture);
    }
}

int QSGFloatTexture::textureId() const {
    return m_id_texture;
}

QSize QSGFloatTexture::textureSize() const {
    if (m_num_dims == 2) {
        return QSize(m_dims[0], m_dims[1]);
    } else {
        return QSize();
    }
}

bool QSGFloatTexture::hasAlphaChannel() const {
    return (m_num_components == 4);
}

bool QSGFloatTexture::hasMipmaps() const {
    return false;
}

void QSGFloatTexture::bind() {
    if (!m_id_texture) glGenTextures(1, &m_id_texture);

    switch (m_num_dims) {
    case 1:
        glBindTexture(GL_TEXTURE_1D, m_id_texture);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        break;
    case 2:
        glBindTexture(GL_TEXTURE_2D, m_id_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case 3:
        glBindTexture(GL_TEXTURE_3D, m_id_texture);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        break;
    default:
        break;
    }

    if (m_needs_upload) {
        m_needs_upload = false;
        // determine color format
        GLint internal_format;
        GLenum format;
        switch (m_num_components) {
        case 1:
            internal_format = GL_R32F;
            format = GL_RED;
            break;
        case 2:
            internal_format = GL_RG32F;
            format = GL_RG;
            break;
        case 3:
            internal_format = GL_RGB32F;
            format = GL_RGB;
            break;
        case 4:
            internal_format = GL_RGBA32F;
            format = GL_RGBA;
            break;
        default:
            return;
        }

        // upload data as 1D, 2D or 3D texture
        switch (m_num_dims) {
        case 1:
            glTexImage1D(GL_TEXTURE_1D, 0, internal_format, m_dims[0], 0, format, GL_FLOAT, m_buffer);
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_dims[0], m_dims[1], 0, format, GL_FLOAT, m_buffer);
            break;
        case 3:
            glTexImage3D(GL_TEXTURE_3D, 0, internal_format, m_dims[0], m_dims[1], m_dims[2], 0, format, GL_FLOAT, m_buffer);
            break;
        default:
            return;
        }
    }
}

void QSGFloatTexture::setData(const double *data, const int *dims, int num_dims, int num_components)
{
    if (num_dims <= 0 || num_dims > 3) return;
    m_num_dims = num_dims;
    m_num_components = num_components;

    // copy dims and calculate number of elements
    int num_elements = num_components;
    for (int i = 0; i < num_dims; ++i) {
        m_dims[i] = dims[i];
        num_elements *= dims[i];
    }
    // allocate new buffer if necessary
    if (m_buffer_size < num_elements) {
        if (m_buffer) delete[] m_buffer;
        m_buffer = new GLfloat[num_elements];
        m_buffer_size = num_elements;
    }
    // copy data to float buffer for texture upload later
    for (int i = 0; i < num_elements; ++i) {
        m_buffer[i] = data[i];
    }
    m_needs_upload = true;
}

void QSGFloatTexture::setData1D(const double *data, int size, int num_components)
{
    setData(data, &size, 1, num_components);
}

void QSGFloatTexture::setData2D(const double *data, int width, int height, int num_components)
{
    int dims[] = {width, height};
    setData(data, dims, 2, num_components);
}

void QSGFloatTexture::setData3D(const double *data, int width, int height, int depth, int num_components)
{
    int dims[] = {width, height, depth};
    setData(data, dims, 3, num_components);
}

bool QSGFloatTexture::updateTexture() {
    return false;
}
