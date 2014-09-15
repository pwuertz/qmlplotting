#include "qsgfloattexture.h"
#include <QOpenGLContext>

QSGFloatTexture::QSGFloatTexture() : QSGTexture(), m_id_texture(0), m_width(0), m_height(0) {
    glGenTextures(1, &m_id_texture);
}

QSGFloatTexture::~QSGFloatTexture() {
    if (!QOpenGLContext::currentContext()) {
        qWarning("QSGFloatTexture::~QSGFloatTexture needs OpenGL context");
    }
    if (m_id_texture) {
        glDeleteTextures(1, &m_id_texture);
    }
}

int QSGFloatTexture::textureId() const {
    return m_id_texture;
}

QSize QSGFloatTexture::textureSize() const {
    return QSize(m_width, m_height);
}

bool QSGFloatTexture::hasAlphaChannel() const {
    return false;
}

bool QSGFloatTexture::hasMipmaps() const {
    return false;
}

void QSGFloatTexture::updateTextureData(float *data, int w, int h, int num_components) {
    m_width = w;
    m_height = h;
    // bind texture object and upload data
    bind();
    switch (num_components) {
    case 1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0, GL_RED, GL_FLOAT, data);
        break;
    case 2:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, w, h, 0, GL_RG, GL_FLOAT, data);
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_FLOAT, data);
        break;
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, data);
        break;
    default:
        break;
    }
}

void QSGFloatTexture::bind() {
    glBindTexture(GL_TEXTURE_2D, m_id_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
