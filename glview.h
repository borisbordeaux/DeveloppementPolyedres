#ifndef GLVIEW_H
#define GLVIEW_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QtOpenGL/QOpenGLVertexArrayObject>
#include <QtOpenGL/QOpenGLBuffer>
#include <QMatrix4x4>
#include <QTimer>

class QOpenGLShaderProgram;
class Model;
class NetControler;

namespace he
{
	class Face;
}

//to switch between face selection or edges selection
enum SelectionMode
{
	FACES, EDGES
};

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	/**
	 * @brief Construct an OpenGL widget that will draw a Model
	 * @param model the model that has to be drawn
	 * @param netControler the net controler used to control the net
	 * @param parent the parent of this widget
	 */
	GLView(Model* model, NetControler* netControler, QWidget* parent = 0);
	~GLView();

	/**
	 * @brief setter, the angle has to be twice its
	 * value in degree because of precision when
	 * using the mouse
	 * @param angle the angle in degree about its X axis
	 */
	void setXRotation(int angle);

	/**
	 * @brief setter, the angle has to be twice its
	 * value in degree because of precision when
	 * using the mouse
	 * @param angle the angle in degree about its Y axis
	 */
	void setYRotation(int angle);

	/**
	 * @brief setter, the angle has to be twice its
	 * value in degree because of precision when
	 * using the mouse
	 * @param angle the angle in degree about its Z axis
	 */
	void setZRotation(int angle);

	/**
	 * @brief setter, set the selection mode for item picking
	 * @param mode the mode to set
	 */
	void setSelectionMode(SelectionMode mode);

	/**
	 * @brief indicates that the mesh changed, so we have
	 * to reallocate memory and resend data to the GPU
	 */
	void meshChanged();

	/**
	 * @brief set a face that has to be viewed by the camera
	 * @param f the face to see
	 */
	void setViewFace(he::Face* f);

	/**
	 * @brief indicates that an image has to be exported
	 * @param path the path to the exported image
	 */
	void setExportNet(QString path);

protected:
	// QOpenGLWidget interface
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	// QWidget interface
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	//compute matrices for the rendering
	void computeMVMatrices();
	//manage face selection
	void clickFaceManagement();
	//manage edge selection
	void clickEdgeManagement();
	//manage exportation
	void exportNet();

	//the controler for modification of the model
	NetControler* m_netControler;

	//rotation of the model
	int m_xRot = 0;
	int m_yRot = 0;
	int m_zRot = 0;

	//the last position of the mouse
	//used for rotation
	QPoint m_lastPos;

	//OpenGL stuff for rendering
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLBuffer m_vboEdge;
	QOpenGLShaderProgram* m_program = nullptr;
	QOpenGLShaderProgram* m_programEdge = nullptr;

	//location of the differents variables in the GPU
	int m_projMatrixLoc = 0;
	int m_mvMatrixLoc = 0;
	int m_normalMatrixLoc = 0;
	int m_lightPosLoc = 0;
	int m_cameraPosLoc = 0;
	int m_modelMatrixLoc = 0;
	int m_isPickingLoc = 0;

	//for edges
	int m_projMatrixLocEdge = 0;
	int m_mvMatrixLocEdge = 0;
	int m_isPickingLocEdge = 0;

	//matrices for rendering
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;

	//camera position
	QVector3D m_cameraPos;
	QVector3D m_cameraLookAt;
	float m_cameraDistance = 6.0;

	//the model that will be displayed
	Model* m_model;

	//useful for item selection
	bool m_clicked = false;
	QPoint m_clickPos;
	QSize m_screenSize;

	//used for selection
	SelectionMode m_selectionMode = FACES;

	//the face the camera has to look
	he::Face* m_viewFace = nullptr;

	//indicates that an exportation has to be done
	bool m_exportNet = false;
	//path to the image exported
	QString m_exportPath;
};

#endif // GLVIEW_H
