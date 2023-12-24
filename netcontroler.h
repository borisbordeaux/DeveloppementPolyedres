#ifndef NETCONTROLER_H
#define NETCONTROLER_H

#include <QMatrix4x4>

namespace he
{
	class Mesh;
	class Face;
	class HalfEdge;
	class Vertex;
}
class Model;

class NetControler
{
public:
	/**
	 * @brief constructor
	 * @param model the model this controler will use
	 */
	NetControler(Model* model);
	~NetControler();

	/**
	 * @brief create a net of the given mesh
	 * @param mesh the mesh the net will be based on
	 * @param net the mesh of the net that will be filled
	 */
	void createNet(he::Mesh& mesh, he::Mesh& net);

	/**
	 * @brief open the net by a given percent
	 * @param percent the percent used to open the mesh.
	 * It can be negative to close the mesh
	 */
	void open(int percent);

	/**
	 * @brief set the selected face as root face
	 */
	void updateRootFace();

	/**
	 * @brief translate the given face
	 * @param f the face to translate
	 */
	void translateFace(he::Face* f);

	/**
	 * @brief add a local root by setting the given face as parent to all adjacent faces of f
	 * @param f the face that will be the local root
	 */
	void setFaceAsLocalRoot(he::Face* f);

	/**
	 * @brief getter
	 * @return the percentage of opening
	 */
	int getPercentOpening();

	/**
	 * @brief remove tabs of the net
	 */
	void removeTabs();

	/**
	 * @brief create automatically tabs
	 */
	void createTabs();

	/**
	 * @brief setter
	 * @param display has to be true if tabs has to be shown, false otherwise
	 */
	void setDisplayTabs(bool display);

	/**
	 * @brief setter
	 * @param angle the angle to use for tabs
	 */
	void setTabAngle(float angle);

	/**
	 * @brief setter
	 * @param length the length of the tabs
	 */
	void setTabLength(float length);

	/**
	 * @brief setter
	 * @param value the value of the translation
	 */
	void setTranslationValue(float value);

private:
	/**
	 * @brief automatically add tabs on the net
	 */
	void automaticallyAddTabs();

	/**
	 * @brief add the given halfedge to the list of halfedges that will have a tab
	 * @param he the halfedge that will have a tab
	 */
	void addTabToHalfEdge(he::HalfEdge* he);

	/**
	 * @brief add a tab to the given halfedge
	 * @param he the halfedge the tab is attached to
	 */
	void createTab(he::HalfEdge* he);

	/**
	 * @brief add a local root by setting the given face as parent to all adjacent faces of f
	 * @param f the face that will be the local root
	 * @param rewriteParent has to be false for the creation of the net, but true for the modification
	 */
	void setFaceAsLocalRoot(he::Face* f, bool rewriteParent);

	/**
	 * @brief find if there will be a loop if child face had parent face as parent
	 * @param child the potential child
	 * @param parent the potential parent
	 * @return true if there is a loop, false otherwise
	 */
	bool findLoop(he::Face* child, he::Face* parent) const;

	/**
	 * @brief create a tree representing the faces and their links between themselves
	 */
	void createTree();

	/**
	 * @brief transforms a face by applying its parent trasformation and
	 * its proper transformation (including translation)
	 * @param f the face to transform
	 * @param percent the percent of opening
	 * @param parentTransform the f's parent transformation
	 * @return the transformation of the face
	 */
	QMatrix4x4 transform(he::Face* f, int percent, QMatrix4x4 parentTransform);

	/**
	 * @brief compute the angle between 2 faces
	 * @param f1 the first face
	 * @param f2 the second face
	 * @param rotationAxis the rotation axis between the faces
	 * @return the oriented angle of the faces
	 */
	float angleBetweenFaces(he::Face* f1, he::Face* f2, he::HalfEdge* rotationAxis);

	/**
	 * @brief simplify the net by merging all adjacent faces that has the same normal
	 */
	void simplifyNet();

	/**
	 * @brief compute if the given faces have the same normal
	 * @param f1 the first face
	 * @param f2 the second face
	 * @return true if the faces has the same normal, false otherwise
	 */
	bool haveSameNormal(he::Face* f1, he::Face* f2) const;

	/**
	 * @brief merge faces by modifying f1 and deleting f2
	 * @param f1 the face that will be updated
	 * @param f2 the facethat will be merged to f1
	 * @param he the common halfedge between f1 and f2
	 */
	void mergeFaces(he::Face* f1, he::Face* f2, he::HalfEdge* he);

	//a map to get for each face their rotation axis
	QMap<he::Face*, he::HalfEdge*> m_baseRotation;

	//a map to get for each face their default angle
	QMap<he::Face*, float> m_angles;

	//a map to get for each face their parent
	QMap<he::Face*, he::Face*> m_parent;

	//a map to get for each face their transformation for opening
	QMap<he::Face*, QMatrix4x4> m_translationFaceOpening;

	//a map to get for each face their transformation for closing
	QMap<he::Face*, QMatrix4x4> m_translationFaceClosing;

	//the translation value
	float m_translationValue = 1.0f;

	//the root face
	he::Face* m_rootFace;

	//the net that will be manipulated
	he::Mesh* m_net;

	//the model of the net (to update data)
	Model* m_model;

	//the percent of opening
	int m_percentOpening = 0;

	//indicate if the previous percent opening was 100
	bool m_wasFullyOpened = false;

	//indicates if tabs has to be added or not
	bool m_displayTabs = true;

	//the angle of the tabs
	float m_tabAngle = 45.0f;

	//the length of the tabs
	float m_tabLength = 0.15f;

	//list that will contain all halfedges that will have a tab
	QVector<he::HalfEdge*> m_heWithTab;

	//data for all tabs, will be deleted if net is not opened at 100%
	QVector<he::Face*> m_tabFaces;
	QVector<he::HalfEdge*> m_tabHalfEdges;
	QVector<he::Vertex*> m_tabVertices;
};

#endif // NETCONTROLER_H
