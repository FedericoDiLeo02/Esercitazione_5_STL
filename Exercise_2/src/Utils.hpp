#ifndef UTILS_HPP
#define UTILS_HPP

#include "PolygonalMesh.hpp"

bool ImportMesh(PolygonalMesh& mesh);
bool ImportCell0Ds(PolygonalMesh& mesh);
bool ImportCell1Ds(PolygonalMesh& mesh);
bool ImportCell2Ds(PolygonalMesh& mesh);
void RunMeshTests(const PolygonalMesh& mesh);

#endif // UTILS_HPP
