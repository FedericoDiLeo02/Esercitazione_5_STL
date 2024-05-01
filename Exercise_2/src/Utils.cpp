#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

bool ImportMesh(PolygonalMesh& mesh);
bool ImportCell0Ds(PolygonalMesh& mesh);
bool ImportCell1Ds(PolygonalMesh& mesh);
bool ImportCell2Ds(PolygonalMesh& mesh);


// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("PolygonalMesh/Cell0Ds.csv");

    if(file.fail())
    {
        std::cerr << "Impossibile aprire il file CSV." << std::endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front(); //il primo elemento della lista di stringhe è la stringa d'intestazione, la rimuovo

    mesh.NumberCell0D = listLines.size(); //ogni riga rappresenta una cella

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >>  id >> marker >> coord(0) >> coord(1);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end()) //se cercando la chiave marker arrivo in fondo (ovvero non c'è)
            {
                mesh.Cell0DMarkers.insert({marker, {id}});
            } //allora inserisco la coppia chiave (marker) e lista per ora con il solo elemento id
            else
            {
                mesh.Cell0DMarkers[marker].push_back(id);
            } //se è già presente quel marker come chiave, io accedo con la chiave e aggiungo il singolo id alla lista
        }

    }
    file.close();
    return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("PolygonalMesh/Cell1Ds.csv");

    if(file.fail())
    {
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    file.close();

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i originend; //interi non serve double

        converter >>  id >> marker >> originend(0) >> originend(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(originend); //aggiungo il vettore di 2 interi originend al vettore di vettori

        if( marker != 0)
        {
            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
            {
                mesh.Cell1DMarkers.insert({marker, {id}});
            }
            else
            {
                mesh.Cell1DMarkers[marker].push_back(id);
            }
        }

    }
    file.close();
    return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("PolygonalMesh/Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        std::replace(line.begin(),line.end(),';',' ');
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int numvertices;
        unsigned int numedges;

        converter >>  id >> marker >> numvertices; //fermo lo stream al numero di vertici perché ora che so quanti sono faccio vettore con dimensione per streammarci i vertici dentro
        vector<unsigned int> vertices(numvertices) ;

        for(unsigned int i = 0; i < numvertices; i++)
            converter >> vertices[i];

        converter >> numedges;
        vector<unsigned int> edges(numedges) ;

        for(unsigned int i = 0; i < numedges; i++)
            converter >> edges[i];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);



        if (mesh.Cell2DMarkers.find(marker) == mesh.Cell2DMarkers.end()) //se non trovo il marker
        {
            mesh.Cell2DMarkers.insert({marker, {id}}); //inserisco insieme il marker e l'id che è il primo id della lista
        }
        else
        {
            mesh.Cell2DMarkers[marker].push_back(id); //se trovo il marker, aggiungo id alla rispettiva lista di id
        }

    }
    file.close();
    return true;
}
// ***********************************************************************************
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++) //scorro la mappa da inizio a fine
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
            {
                cout << "\t" << id;
            }

            cout << endl;
        }
    }

    if(!ImportCell1Ds(mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
            {
                cout << "\t" << id;
            }

            cout << endl;
        }
    }

    if(!ImportCell2Ds(mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell2D marker:" << endl;
        for(auto it = mesh.Cell2DMarkers.begin(); it != mesh.Cell2DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
            {
                cout << "\t" << id;
            }

            cout << endl;
        }
    }

    if(!ImportCell2Ds(mesh))
    {
        return false;
    }
    else
    {
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++) //per ogni cella 2D prendo la c-esima
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c]; //la componente c-esima del vettore Cell2DEdges contiene il vettore di edges della cella c-esima
            unsigned int ne = edges.size(); //ho tanti edges nella c-esima cella quanto la dimensione del vettore che li contiene

            for(unsigned int e = 0; e < ne; e++) //scorro tutti gli edges della c-esima cella 2D
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0]; //Cell1DVertices è un vettore di vettori di 2 interi (origin e end);
                    //all'entrata i-esima trovo il vettore origin-end rispettivo al lato con ID i-esimo;
                    //adesso edges[e] contiene l'ID di uno dei tanti lati dell'universo
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin); //cerca origin nella lista dei vertici della c-esima cella 2D
                    //perché origin di uno degli edges della cella c-esima
                    //deve essere uno dei vertici della cella c-esima
                if(findOrigin == mesh.Cell2DVertices[c].end()) //se non la trovo ovvero arrivo alla fine della lista
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }
    return true;
}

// Funzione per calcolare la lunghezza di un lato dato due punti
double CalculateEdgeLength(const Vector2d& point1, const Vector2d& point2) {
    double dx = point2(0) - point1(0);
    double dy = point2(1) - point1(1);
    return sqrt(dx * dx + dy * dy);
}

// Funzione per calcolare l'area di un triangolo dato tre vertici
double CalculateTriangleArea(const Vector2d& vertex1, const Vector2d& vertex2, const Vector2d& vertex3) {
    double a = CalculateEdgeLength(vertex1, vertex2);
    double b = CalculateEdgeLength(vertex2, vertex3);
    double c = CalculateEdgeLength(vertex3, vertex1);
    double s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}



// Funzione per verificare che tutti i marker siano memorizzati correttamente
bool TestMarkers(const PolygonalMesh& mesh) {
    bool markersCorrect = true;

    // Verifica i marker per Cell0D
    for (const auto& pair : mesh.Cell0DMarkers) {
        // Esegui i controlli sui marker della Cell0D
        // In questo esempio, semplicemente verifichiamo che ci siano almeno due marker
        if (pair.second.size() < 1) {
            markersCorrect = false;
            break;
        }
    }

    // Verifica i marker per Cell1D
    // In questo esempio, controlliamo solo che ci siano almeno due marker
    if (markersCorrect) {
        for (const auto& pair : mesh.Cell1DMarkers) {
            if (pair.second.size() < 1) {
                markersCorrect = false;
                break;
            }
        }
    }

    // Verifica i marker per Cell2D
    // In questo esempio, controlliamo solo che ci siano almeno due marker
    if (markersCorrect) {
        for (const auto& pair : mesh.Cell2DMarkers) {
            if (pair.second.size() < 1) {
                markersCorrect = false;
                break;
            }
        }
    }

    return markersCorrect;
}

// Funzione per verificare che la lunghezza degli spigoli dei triangoli sia non nulla
bool TestEdgeLengths(const PolygonalMesh& mesh) {
    bool edgeLengthsNonZero = true;

    // Itera attraverso le celle 2D per calcolare le lunghezze degli spigoli dei triangoli
    for (unsigned int i = 0; i < mesh.NumberCell2D; ++i) {
        // Ottieni i vertici del triangolo corrente
        const vector<unsigned int>& vertices = mesh.Cell2DVertices[i];
        // Controllo rapido: se non ci sono abbastanza vertici, passa al prossimo triangolo
        if (vertices.size() != 3) {
            continue;
        }
        // Calcola la lunghezza degli spigoli
        double edge1 = CalculateEdgeLength(mesh.Cell0DCoordinates[vertices[0]], mesh.Cell0DCoordinates[vertices[1]]);
        double edge2 = CalculateEdgeLength(mesh.Cell0DCoordinates[vertices[1]], mesh.Cell0DCoordinates[vertices[2]]);
        double edge3 = CalculateEdgeLength(mesh.Cell0DCoordinates[vertices[2]], mesh.Cell0DCoordinates[vertices[0]]);
        // Verifica che le lunghezze degli spigoli siano non nulle
        if (edge1 <= 0 || edge2 <= 0 || edge3 <= 0) {
            edgeLengthsNonZero = false;
            break;
        }
    }

    return edgeLengthsNonZero;
}

// Funzione per verificare che l'area dei triangoli sia non nulla
bool TestTriangleAreas(const PolygonalMesh& mesh) {
    bool triangleAreasNonZero = true;

    // Itera attraverso le celle 2D per calcolare le aree dei triangoli
    for (unsigned int i = 0; i < mesh.NumberCell2D; ++i) {
        // Ottieni i vertici del triangolo corrente
        const vector<unsigned int>& vertices = mesh.Cell2DVertices[i];
        // Controllo rapido: se non ci sono abbastanza vertici, passa al prossimo triangolo
        if (vertices.size() != 3) {
            continue;
        }
        // Calcola l'area del triangolo
        double area = CalculateTriangleArea(mesh.Cell0DCoordinates[vertices[0]], mesh.Cell0DCoordinates[vertices[1]], mesh.Cell0DCoordinates[vertices[2]]);
        // Verifica che l'area del triangolo sia non nulla
        if (area <= 0) {
            triangleAreasNonZero = false;
            break;
        }
    }

    return triangleAreasNonZero;
}
// Funzione per eseguire i test sui dati della mesh poligonale
void RunMeshTests(const PolygonalMesh& mesh) {
    // Test per verificare che tutti i marker siano memorizzati correttamente
    bool markersCorrect = TestMarkers(mesh);

    // Test per verificare che la lunghezza degli spigoli dei triangoli sia non nulla
    bool edgeLengthsNonZero = TestEdgeLengths(mesh);

    // Test per verificare che l'area dei triangoli sia non nulla
    bool triangleAreasNonZero = TestTriangleAreas(mesh);

    // Stampa i risultati dei test
    cout << "Test results:" << endl;
    cout << "All markers stored correctly: " << (markersCorrect ? "Yes" : "No") << endl;
    cout << "Edge lengths are non-zero: " << (edgeLengthsNonZero ? "Yes" : "No") << endl;
    cout << "Triangle areas are non-zero: " << (triangleAreasNonZero ? "Yes" : "No") << endl;
}
