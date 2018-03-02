#include "qsgdatatexture.h"
#include <QOpenGLContext>
#include <cstdint>
#include <QtGlobal>

template <typename T>
struct GlMap {
    static constexpr GLint internalFormat(int nColors) {
        return internalFormats[nColors];
    }
    static constexpr GLenum dataFormat(int nColors) {
        constexpr GLenum dataFormats[5] = {0, GL_RED, GL_RG, GL_RGB, GL_BGRA};
        return dataFormats[nColors];
    }
    static const GLint internalFormats[5];
    static const GLenum dataType;
};

template<> const GLint GlMap<float>::internalFormats[5] = {0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F};
template<> const GLenum GlMap<float>::dataType = GL_FLOAT;

template<> const GLint GlMap<uint8_t>::internalFormats[5] = {0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
template<> const GLenum GlMap<uint8_t>::dataType = GL_UNSIGNED_BYTE;


template<typename T>
QSGDataTexture<T>::QSGDataTexture() :
    QSGDynamicTexture(), QOpenGLFunctions_2_0()
{
    if (!initializeOpenGLFunctions()) {
        qFatal("QSGDataTexture: OpenGL 2.0 required");
    }
}

template<typename T>
QSGDataTexture<T>::~QSGDataTexture() {
    if (m_id_texture) {
        if (QOpenGLContext::currentContext() == nullptr) {
            qWarning("QSGDataTexture destroyed without OpenGL context");
        }
        glDeleteTextures(1, &m_id_texture);
    }
}

template<typename T>
int QSGDataTexture<T>::textureId() const {
    return static_cast<int>(m_id_texture);
}

template<typename T>
QSize QSGDataTexture<T>::textureSize() const {
    if (m_num_dims == 2) {
        return {m_dims[0], m_dims[1]};
    }
    if (m_num_dims == 1) {
        return {m_dims[0], 0};
    }
    return QSize();
}

template<typename T>
bool QSGDataTexture<T>::hasAlphaChannel() const {
    return (m_num_components == 4);
}

template<typename T>
bool QSGDataTexture<T>::hasMipmaps() const {
    return false;
}

template<typename T>
void QSGDataTexture<T>::bind() {

    if (m_id_texture == 0u) {
        glGenTextures(1, &m_id_texture);
    }
    GLint filter = (filtering() == Linear) ? GL_LINEAR : GL_NEAREST;

    switch (m_num_dims) {
    case 1:
        glBindTexture(GL_TEXTURE_1D, m_id_texture);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        break;
    case 2:
        glBindTexture(GL_TEXTURE_2D, m_id_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;
    case 3:
        glBindTexture(GL_TEXTURE_3D, m_id_texture);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
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
        const GLint internal_format = GlMap<T>::internalFormat(m_num_components);
        const GLenum format = GlMap<T>::dataFormat(m_num_components);
        const GLenum type = GlMap<T>::dataType;

        // upload data as 1D or 2D texture
        switch (m_num_dims) {
        case 1:
            glTexImage1D(GL_TEXTURE_1D, 0, internal_format, m_dims[0], 0, format, type, m_buffer.data());
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, internal_format, m_dims[0], m_dims[1], 0, format, type, m_buffer.data());
            break;
        case 3:
            glTexImage3D(GL_TEXTURE_3D, 0, internal_format, m_dims[0], m_dims[1], m_dims[2], 0, format, type, m_buffer.data());
            break;
        default:
            return;
        }
    }
}

template<typename T>
T *QSGDataTexture<T>::allocateData(const int *dims, int num_dims, int num_components)
{
    if (num_components < 1 || num_components > 4) {
        return nullptr;
    }
    if (num_dims < 1 || num_dims > 3) {
        return nullptr;
    }

    // calculate total number of elements
    int num_elements = num_components;
    for (int i = 0; i < num_dims; ++i) {
        m_dims[i] = dims[i];
        num_elements *= dims[i];
    }

    // calculate total number of bytes, resize buffer if required
    int num_bytes = num_elements * static_cast<int>(sizeof(T));
    if (m_buffer.size() != num_bytes) {
        m_buffer.resize(num_bytes);
    }

    m_num_dims = num_dims;
    m_num_components = num_components;
    return reinterpret_cast<T*>(m_buffer.data());
}

template<typename T>
T* QSGDataTexture<T>::allocateData1D(int size, int num_components)
{
    return reinterpret_cast<T*>(allocateData(&size, 1, num_components));
}

template<typename T>
T* QSGDataTexture<T>::allocateData2D(int width, int height, int num_components)
{
    int dims[] = {width, height};
    return reinterpret_cast<T*>(allocateData(dims, 2, num_components));
}

template<typename T>
T* QSGDataTexture<T>::allocateData3D(int width, int height, int depth, int num_components)
{
    int dims[] = {width, height, depth};
    return reinterpret_cast<T*>(allocateData(dims, 3, num_components));
}

template<typename T>
void QSGDataTexture<T>::commitData()
{
    m_needs_upload = true;
}

template<typename T>
int QSGDataTexture<T>::getDim(int dim)
{
    return m_dims[dim];
}

template<typename T>
bool QSGDataTexture<T>::updateTexture()
{
    return false;
}


// Explicitly instantiate data texture classes in this unit
template class QSGDataTexture<uint8_t>;
template class QSGDataTexture<float>;
