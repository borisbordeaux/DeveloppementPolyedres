#ifndef SETTINGSLIDER_H
#define SETTINGSLIDER_H

#include <QSlider>
#include "netcontroler.h"
#include "model.h"
#include "glview.h"

enum Setting
{
    ANGLE, DISTANCE, TRANSLATION
};

class SettingSlider : public QSlider
{

    Q_OBJECT

public:
    SettingSlider(int min, int max, NetControler *netControler, Model *model, GLView *view, Setting setting);

private slots:
    void changeValue(int value);

private:
    Setting m_setting;
    NetControler *m_netControler;
    Model *m_model;
    GLView *m_view;
};

#endif // SETTINGSLIDER_H
