#ifndef VERTEX_H
#define VERTEX_H

#include <QString>
#include <QVector>
#include <QVector3D>

namespace he
{

	class HalfEdge;

	class Face;

	class Vertex
	{
	public:
		/**
		 * @brief Construct a Vertex with its 3 params
		 * @param x the x value of the vertex
		 * @param y the y value of the vertex
		 * @param z the z value of the vertex
		 */
		Vertex(float x, float y, float z, QString name = "");

		/**
		 * @brief getter
		 * @return the position of this vertex
		 */
		QVector3D pos() const;

		float x() {return m_pos.x();}
		float y() {return m_pos.y();}
		float z() {return m_pos.z();}

		/**
		 * setter
		 * @param pos the new position of the vertex
		 */
		void setPos(QVector3D const& pos);

		/**
		 * @brief getter
		 * @return a half-edge from which this point is its origin
		 */
		he::HalfEdge* halfEdge();

		/**
		 * @brief setter
		 * @param halfEdge a half-edge that have this point as origin
		 */
		void setHalfEdge(he::HalfEdge* halfEdge);

		/**
		 * @brief getter
		 * @return the name of this vertex
		 */
		QString name() const;

		std::size_t degree() const;

		std::vector<he::Face*> getAllFacesAroundVertex(he::Face* f) const;

		void addHalfEdge(he::HalfEdge* halfEdge);
		QVector<he::HalfEdge*>& otherHalfEdges();

		bool equals(he::Vertex* other);


	private:
		//coordinates of this vertex
		QVector3D m_pos;
		//a half-edge from which this point is the origin
		he::HalfEdge* m_halfEdge;
		//all other half-edges from which this point is the origin
		QVector<he::HalfEdge*> m_otherHalfEdges;
		//name of the vertex
		QString m_name;
	};

} // poly

#endif //VERTEX_H
