#ifndef SETTINGSLIDER_H
#define SETTINGSLIDER_H

#include <QSlider>

class Model;
class GLView;
class NetControler;

//indicates which setting this slider controls
enum Setting
{
	ANGLE, LENGTH, TRANSLATION
};

class SettingSlider : public QSlider
{

	Q_OBJECT

public:
	/**
	 * @brief constructor
	 * @param min the min value of the setting
	 * @param max the max value of the setting
	 * @param netControler the net controler the settings will be applied
	 * @param model the model used to update the data
	 * @param view the view that will be updated
	 * @param setting the type of setting
	 */
	SettingSlider(int min, int max, NetControler* netControler, Model* model, GLView* view, Setting setting);

private slots:
	/**
	 * @brief action taht will be done when the slider value will be changed
	 * @param value the new value
	 */
	void changeValue(int value);

private:
	//the setting type
	Setting m_setting;
	//the net controler that will use this setting
	NetControler* m_netControler;

	//model and view used to update the data and the view
	Model* m_model;
	GLView* m_view;
};

#endif // SETTINGSLIDER_H
