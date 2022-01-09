#include <iostream>
//#include "SquareMatrix.hpp"
//#include "SparseMatrix.hpp"
#include "Graph.hpp"
#include "Random.hpp"
#include "WebAssembly.hpp"

using namespace std;

const char *MSGS1[] = {"0. Quit",
                       "1. Init graph",
                       "2. Add node to graph",
                       "3. Remove node from graph",
                       "4. Add edge to graph",
                       "5. Remove edge from graph",
                       "6. Topological sort",
                       "7. Dijkstra",
                       "8. Colorize",
                       "9. Print graph"};

const char *MSGS2[] = {"0. Quit",
                       "1. Yes",
                       "2. No"};

template<int N>
int Dialog(const char *(&msgs)[N]) {
    string error;
    int choice = -1;
    do {
        cout << error;
        error = "You're wrong. Try again!\n";
        for (int i = 0; i < N; ++i) {
            cout << msgs[i] << endl;
        }
        cout << "Make your choice: \n";
        stringstream ss(readline());
        ss >> choice;
    } while (choice < 0 || choice >= N);
    return choice;
}

template<typename T>
T InputValue() {

    bool success = false;
    T value;
    while (!success) {
        cout << "Input value:\n";
        stringstream ss(readline());
        try {
            ss >> value;
            success = true;
        }
        catch (exception &e) {
            cout << "An error has occurred: " << e.what() << "\nTry again!\n";
        }
    }
    return value;
}

int DialogValue(const string &msg) {
    string error;
    int choice = -1;
    do {
        cout << error;
        error = "You're wrong. Try again!\n";
        cout << msg << endl;
        cout << "Make your choice: " << endl;
        stringstream ss(readline());
        ss >> choice;
    } while (choice < 0);
    return choice;
}

void StartUI() {

    int choice = 1;
    string tmp;
    auto graph = new Graph<size_t>();
//    cout << graph->ToString() << endl;
//    delete graph;
    while (choice) {
        try {
            choice = Dialog(MSGS1);
            size_t value1, value2;
            bool directed, weighted;
            ListSequence<size_t> path;
            switch (choice) {
                case 0: {
                    break;
                }
                case 1: {
                    value1 = DialogValue("Input nodes count");
                    value2 = DialogValue("Input edges count");
                    cout << "Will this graph be directed?" << endl;
                    choice = Dialog(MSGS2);
                    if (choice == 1)
                        directed = true;
                    else if (choice == 2)
                        directed = false;
                    else {
                        choice = 1;
                        break;
                    }
                    cout << "Will this graph be edge weighted?" << endl;
                    choice = Dialog(MSGS2);
                    if (choice == 1)
                        weighted = true;
                    else if (choice == 2)
                        weighted = false;
                    else {
                        choice = 1;
                        break;
                    }
                    delete graph;
                    graph = new Graph<size_t>{value1, value2, directed,
                                              false, weighted, false};
                    break;
                }

                case 2: {
                    graph->AddNode();
                    break;
                }

                case 3: {
                    graph->RemoveNode(DialogValue("Input node index"));
                    break;
                }

                case 4: {
                    value1 = DialogValue("Input first node index");
                    value2 = DialogValue("Input second node index");
                    graph->AddEdge(value1, value2);
                    break;
                }
                case 5: {
                    value1 = DialogValue("Input first node index");
                    value2 = DialogValue("Input second node index");
                    graph->RemoveEdge(value1, value2);
                    break;
                }
                case 6: {
                    try {
                        path = graph->TopologicalSort();
                        cout << "Topological order is " << path << endl;
                    }
                    catch (const std::exception &e) {
                        cout << "Topological order is " << e.what() << endl;
                    }

                    break;
                }
                case 7: {
                    value1 = DialogValue("Input first node index");
                    value2 = DialogValue("Input second node index");
                    path = graph->Dijkstra(value1, value2);
                    if (path.Count() != 0)
                        cout << "Path is = " << path << endl;
                    else
                        cout << "The node is unreachable" << endl;
                    cout << graph->ToString(false, path) << endl;
                    break;
                }
                case 8: {
                    cout << "Chromatic num = " << graph->Colorize() << endl;
                    cout << graph->ToString(true) << endl;
                    break;
                }
                case 9: {
                    cout << graph->ToString() << endl;
                    break;
                }
                default: {
                    cout << "How did you end up here?" << endl;
                    break;
                }
            }
        }
        catch (exception &e) {
            cout << "An error has occurred: " << e.what() << "\nTry again!\n";
        }
    }
    delete graph;
}


int main() {
//    Graph<int> a(2, 4, true, false, false, true);
//    a.AddNode();
//    a.AddNode();
//    cout << a.ToString()<< endl;
//
//    a.RemoveEdge(0, 0);
////    auto res = a.Dijkstra(0,0);
//    cout << a.ToString()<< endl;

//    cout << a.ToString(false) << endl;
//    Graph<int> graph{0, 0, true};
//////
//////
//    graph.AddEdge(2, 4, 1);
//    graph.AddEdge(2, 3, 5);
//    graph.AddEdge(4, 3, 3);
//    graph.AddEdge(2, 5, 10);
//    graph.AddEdge(3, 5, 10);
//    graph.AddEdge(5, 0, 1);
//    graph.RemoveEdge(5, 0);
//    cout << graph.ToString() << endl;
//    auto path = graph.Dijkstra(0, 5);
//    cout << graph.ToString() << endl;

//    cout << res << endl;

    StartUI();
    return 0;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Laba3) {
        emscripten::function("start", &StartUI, emscripten::allow_raw_pointers());
}
#endif
