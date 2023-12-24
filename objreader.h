#ifndef OBJREADER_H
#define OBJREADER_H

#include <QString>

namespace he
{
	class Mesh;
}

class OBJReader
{
public:
	/**
	 * @brief reads an OBJ file whose fullname is specified
	 * @param filename the full name of the OBJ file to read
	 * @param mesh the mesh that will be filled with the data
	 * of the OBJ file given
	 */
	static void readOBJ(QString const& filename, he::Mesh& mesh);
};

#endif // OBJREADER_H
