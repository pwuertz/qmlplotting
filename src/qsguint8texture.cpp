#include "qsguint8texture.h"


QSGUint8Texture::QSGUint8Texture() :
    QSGTexture(),
    m_id_texture(0),
    m_num_dims(0),
    m_num_components(0),
    m_buffer(),
    m_needs_upload(false)
{
    m_dims[0] = 0;
    m_dims[1] = 0;
}

QSGUint8Texture::~QSGUint8Texture() {
    if (m_id_texture) {
        if (!QOpenGLContext::currentContext()) {
            qWarning("QSGFloatTexture::~QSGFloatTexture needs OpenGL context");
        }
        glDeleteTextures(1, &m_id_texture);
    }
}

int QSGUint8Texture::textureId() const {
    return m_id_texture;
}

QSize QSGUint8Texture::textureSize() const {
    if (m_num_dims == 2) {
        return QSize(m_dims[0], m_dims[1]);
    } else if (m_num_dims == 1) {
        return QSize(m_dims[0], 0);
    } else {
        return QSize();
    }
}

bool QSGUint8Texture::hasAlphaChannel() const {
    return (m_num_components == 4);
}

bool QSGUint8Texture::hasMipmaps() const {
    return false;
}

void QSGUint8Texture::bind() {
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
            internal_format = GL_R;
            format = GL_RED;
            break;
        case 2:
            internal_format = GL_RG;
            format = GL_RG;
            break;
        case 3:
            internal_format = GL_RGB;
            format = GL_RGB;
            break;
        case 4:
            internal_format = GL_RGBA;
            format = GL_RGBA;
            break;
        default:
            return;
        }

        // upload data as 1D or 2D texture
        switch (m_num_dims) {
        case 1:
            glTexImage1D(GL_TEXTURE_1D, 0, internal_format, m_dims[0], 0, format, GL_UNSIGNED_BYTE, m_buffer.data());
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_dims[0], m_dims[1], 0, format, GL_UNSIGNED_BYTE, m_buffer.data());
            break;
        default:
            return;
        }
    }
}

u_int8_t* QSGUint8Texture::allocateData1D(int size, int components)
{
    int num_bytes = size * components * sizeof(u_int8_t);
    if (m_buffer.size() != num_bytes) {
        m_buffer.resize(num_bytes);
    }
    m_num_components = components;
    m_num_dims = 1;
    m_dims[0] = size;
    return (u_int8_t*) m_buffer.data();
}

u_int8_t* QSGUint8Texture::allocateData2D(int width, int height, int components)
{
    int num_bytes = width * height * components * sizeof(u_int8_t);
    if (m_buffer.size() != num_bytes) {
        m_buffer.resize(num_bytes);
    }
    m_num_components = components;
    m_num_dims = 2;
    m_dims[0] = width;
    m_dims[1] = height;
    return (u_int8_t*) m_buffer.data();
}

void QSGUint8Texture::commitData()
{
    m_needs_upload = true;
}
