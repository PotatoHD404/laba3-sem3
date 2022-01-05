#include <iostream>
//#include "SquareMatrix.hpp"
//#include "SparseMatrix.hpp"
#include "Graph.hpp"
#include "Random.hpp"

using namespace std;


int main() {
    Graph<int> a(5, 5, true, false, false, false);
//    auto res = a.Dijkstra(0,0);
    cout << a.TopologicalSort() << endl;

//    cout << a.ToString(false) << endl;

    return 0;
}
