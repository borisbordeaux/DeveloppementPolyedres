#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glview.h"
#include "netcontroler.h"
#include "settingslider.h"
#include "mesh.h"
#include "model.h"

class QSlider;

QT_BEGIN_NAMESPACE

class QGraphicsView;

class QGraphicsScene;
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief Construct the main window of this program
	 * @param parent the parent widget
	 */
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	/**
	 * @brief open a mesh from an OBJ file. Resets the net settings
	 * @param path the path to the OBJ file
	 */
	void openMesh(QString& path);

	/**
	 * @brief open a mesh from a file by calling openMesh() function.
	 * It ask the view to reallocate data to GPU.
	 * Use it only if OpenGL functions has been initialized
	 * @param path the path to the OBJ file
	 */
	void openFile(QString& path);

	// QWidget interface
protected:
	/**
	 * @brief used to quit when 'q' is pressed
	 * @param event the key event
	 */
	void keyReleaseEvent(QKeyEvent* event) override;

	/**
	 * @brief used to close all settings windows when the mainwindow is closed
	 * @param event the close event
	 */
	void closeEvent(QCloseEvent* event) override;

private slots:
	/**
	 * @brief handler when slider position changes
	 */
	void sliderOpening();
	/**
	 * @brief used to animate the opening using a QTimer
	 */
	void animateOpenning();

	//all functions when a menu is triggered in the ui
	void on_actionCube_triggered();
	void on_actionOctahedron_triggered();
	void on_actionDodecahedron_triggered();
	void on_actionPyramid_triggered();
	void on_actionTruncated_dodecahedron_triggered();
	void on_actionDiamond_triggered();
	void on_actionAnimate_triggered();
	void on_actionOther_triggered();
	void on_actionDodecahedron_Triangulated_triggered();
	void on_actionSet_selected_Face_as_root_triggered();
	void on_actionFaces_triggered(bool checked);
	void on_actionEdges_triggered(bool checked);
	void on_actionSet_selected_Face_as_parent_triggered();
	void on_actionTranslate_selected_Face_triggered();
	void on_actionView_Selected_Face_triggered();
	void on_actionExport_PNG_triggered();
	void on_actionNomal_View_triggered();
	void on_actionDisplay_Tabs_triggered(bool checked);
	void on_actionTab_Angle_triggered();
	void on_actionTab_Distance_triggered();
	void on_actionTranslation_Distance_triggered();

private:
	//the ui
	Ui::MainWindow* ui;

	//the slider to control opening
	QSlider* m_sliderOpening;

	//the meshes of the polyhedron and its net
	he::Mesh m_polyhedronMesh;
	he::Mesh m_netMesh;

	//the model of the net that read the
	//mesh and send the data to the view
	Model m_net;

	//net controler that will do the openning
	NetControler m_netControler;

	//OpenGL Widgets that will draw the mesh
	GLView m_netView;

	//timer for the animation
	QTimer m_timerAnimation;
	bool m_isOpenning = true;

	//setting sliders of the net
	SettingSlider m_angleSetting;
	SettingSlider m_distanceSetting;
	SettingSlider m_translationSetting;
    
    //for faces export
    void display();
    QGraphicsView* gv;
    QGraphicsScene* scene;
    float m_sizeSetting = 100.0f;
    float m_distanceFaceSetting = 250.0f;
};
#endif // MAINWINDOW_H
