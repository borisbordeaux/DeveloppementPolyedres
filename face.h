#ifndef FACE_H
#define FACE_H

#include <QString>
#include <QVector3D>

namespace he
{

	class HalfEdge;

	class Face
	{
	public:
		/**
		 * @brief Construct a Face with one half-edge
		 * @param halfEdge the half-edge the face will use
		 */
		explicit Face(QString name = "", he::HalfEdge* halfEdge = nullptr);

		/**
		 * @brief getter
		 * @return the half-edge associated to this Face
		 */
		he::HalfEdge* halfEdge();
		/**
		 * @brief setter
		 * @param halfEdge the half-edge that has to be
		 * associated to this Face
		 */
		void setHalfEdge(he::HalfEdge* halfEdge);

		/**
		 * @brief getter
		 * @return the name of this face
		 */
		QString name() const;

		/**
		 * @brief compute the normal of the face
		 * @return the normalized normal of the face
		 */
		QVector3D computeNormal();

		std::size_t nbEdges() const;

		std::vector<he::HalfEdge*> allHalfEdges() const;

		float area();

		/**
         * @brief compute the center of the face
         * @return the center of the face
         */
		QVector3D getCenter();

	private:
		//the name of the face
		QString m_name;
		//the half-edge of the face
		he::HalfEdge* m_halfEdge;
	};

}

#endif //FACE_H
