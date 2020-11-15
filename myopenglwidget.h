#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QResizeEvent>

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    MyOpenGLWidget(QWidget *parent);

    // QWidget interface
protected:
    void resizeGL(int w, int h) override;
    void initializeGL() override;

private:
    void perspective(double fov, double aspect, double near, double far);
};

#endif // MYOPENGLWIDGET_H
