#include "settingslider.h"

SettingSlider::SettingSlider(int min, int max, NetControler *netControler, Model *model, GLView *view, Setting setting):
    QSlider(Qt::Horizontal),
    m_setting(setting),
    m_netControler(netControler),
    m_model(model),
    m_view(view)
{
    setMinimum(min);
    setMaximum(max);

    QString title;
    switch (m_setting) {
    case DISTANCE:
        title = "Distance : 15";
        setValue(15);
        break;
    case ANGLE:
        title = "Angle : 45";
        setValue(45);
        break;
    case TRANSLATION:
        title = "Translation : 1";
        setValue(1);
    }
    setWindowTitle(title);

    connect(this, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
}

void SettingSlider::changeValue(int value)
{
    switch (m_setting) {
    case DISTANCE:
        m_netControler->setTabDist(float(value)/100.0f);
        break;
    case ANGLE:
        m_netControler->setTabAngle(value);
        break;
    case TRANSLATION:
        m_netControler->setTranslationValue(value);
    }
    m_model->updateData();
    m_view->meshChanged();
    m_view->update();

    QString title;
    switch (m_setting) {
    case DISTANCE:
        title = "Distance : ";
        break;
    case ANGLE:
        title = "Angle : ";
        break;
    case TRANSLATION:
        title = "Translation : ";
    }
    setWindowTitle(title + QString::number(value));
}
