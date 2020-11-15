#ifndef NETVIEW_H
#define NETVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class NetView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    NetView(QWidget *parent);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void paintGL() override;
};

#endif // NETVIEW_H
