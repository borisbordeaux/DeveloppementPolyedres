#ifndef OBJREADER_H
#define OBJREADER_H

#include <QString>
#include "mesh.h"

class OBJReader
{
public:
    /**
     * @brief reads an OBJ file whose fullname is specified
     * @param filename the full name of the OBJ file ot read
     * @return a pointer to a Mesh based on the OBJ file
     */
    static Mesh *readOBJ(QString filename);
};

#endif // OBJREADER_H
