#ifndef POLYGONALMESH_HPP
#define POLYGONALMESH_HPP

#include "Eigen/Eigen"
#include <vector>
#include <map>
#include <list>

using namespace std;
using namespace Eigen;

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0;
    std::vector<unsigned int> Cell0DId = {};
    std::vector<Vector2d> Cell0DCoordinates = {};
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

    unsigned int NumberCell1D = 0;
    std::vector<unsigned int> Cell1DId = {};
    std::vector<Vector2i> Cell1DVertices = {};
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

    unsigned int NumberCell2D = 0;
    std::vector<unsigned int> Cell2DId = {};
    std::vector<vector<unsigned int>> Cell2DVertices = {};
    std::vector<vector<unsigned int>> Cell2DEdges = {};
    std::map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
};

#endif // POLYGONALMESH_HPP
