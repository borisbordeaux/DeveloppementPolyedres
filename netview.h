#ifndef NETVIEW_H
#define NETVIEW_H

#include "myopenglwidget.h"

class NetView : public MyOpenGLWidget
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
