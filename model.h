#ifndef MODEL_H
#define MODEL_H

#include <qopengl.h>
#include <QVector>

class Model
{
public:
    Model();

    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; };

protected:
    QVector<GLfloat> m_data;
    int m_count = 0;
};

#endif // MODEL_H
