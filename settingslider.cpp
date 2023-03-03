#include "settingslider.h"

#include "glview.h"
#include "model.h"
#include "netcontroler.h"

SettingSlider::SettingSlider(int min, int max, NetControler* netControler, Model* model, GLView* view, Setting setting):
	QSlider(Qt::Horizontal),
	m_setting(setting),
	m_netControler(netControler),
	m_model(model),
	m_view(view)
{
	//set bound values
	setMinimum(min);
	setMaximum(max);

	//set right title and values depending on the setting type
	QString title;

	switch (m_setting)
	{
		case LENGTH:
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
	//the new window title
	QString title;

	//apply setting depending on the setting types
	switch (m_setting)
	{
		case LENGTH:
			m_netControler->setTabLength(float(value) / 100.0f);
			title = "Distance : ";
			break;

		case ANGLE:
			m_netControler->setTabAngle(value);
			title = "Angle : ";
			break;

		case TRANSLATION:
			m_netControler->setTranslationValue(value);
			title = "Translation : ";
			break;
	}

	//update data and view
	m_model->updateData();
	m_view->meshChanged();
	m_view->update();

	//update the title
	setWindowTitle(title + QString::number(value));
}
