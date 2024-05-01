#include "Utils.hpp"

using namespace std;

int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        return 1;
    }

    RunMeshTests(mesh);

    return 0;
}
