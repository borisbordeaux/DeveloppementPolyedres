#ifndef MESH_H
#define MESH_H

#include <QHash>
#include <QString>
#include <vector>

namespace he
{

	class Vertex;

	class HalfEdge;

	class Face;

	class Mesh
	{
	public:
		Mesh() = default;
		~Mesh();

		std::vector<he::Vertex*> const& vertices() const;
		std::vector<he::HalfEdge*> const& halfEdges() const;
		std::vector<he::HalfEdge*> const& halfEdgesNoTwin() const;
		std::vector<he::Face*> const& faces() const;

		void append(he::Vertex* v);
		void append(he::HalfEdge* he, bool completeNotTwin = false);
		void append(he::Face* f);

		/**
         * @brief remove a vertex of the mesh
         * @param v the vertex to remove
         */
		void remove(he::Vertex* v);

		/**
		 * @brief remove a halfedge of the mesh
		 * @param he the halfedge to remove
		 */
		void remove(he::HalfEdge* he);

		/**
		 * @brief remove a face of the mesh
		 * @param f the face to remove
		 */
		void remove(he::Face* f);

		he::HalfEdge* findByName(QString const& name);

		void reset();

		QString toString() const;

	private:
		std::vector<he::Vertex*> m_vertices;
		std::vector<he::HalfEdge*> m_halfEdges;
		std::vector<he::HalfEdge*> m_halfEdgesNotTwin;
		std::vector<he::Face*> m_faces;
	};

} // poly

#endif //MESH_H
