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
                       "1. ListSequence",
                       "2. ArraySequence"};

const char *MSGS3[] = {"0. Quit",
                       "1. QuickSort",
                       "2. ShellSort",
                       "3. InsertionSort"};

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
    random_device rd;
    mt19937::result_type seed = rd() ^ (
            (mt19937::result_type)
                    chrono::duration_cast<std::chrono::seconds>(
                            chrono::system_clock::now().time_since_epoch()
                    ).count() +
            (mt19937::result_type)
                    chrono::duration_cast<chrono::microseconds>(
                            chrono::high_resolution_clock::now().time_since_epoch()
                    ).count());
    mt19937 rng(seed);
    int choice = 1;
    string tmp;
    unique_ptr<Enumerable < T>>
    seq = make_unique<ArraySequence<T>>();
    while (choice) {
        try {
            choice = Dialog(MSGS1);
            T value1, value2;
            int count;
            switch (choice) {
                case 0:
                    break;
                case 1: {
                    choice = Dialog(MSGS2);
                    if (choice == 1)
                        seq = make_unique<ListSequence<T>>();
                    else if (choice == 2)
                        seq = make_unique<ArraySequence<T>>();
                    choice = 1;
                    break;
                }

                case 2: {
                    seq->Add(InputValue<T>());
                    break;
                }

                case 3:
                    seq->Remove(InputValue<T>());
                    break;

                case 4:
                    cout << "ISequence: " << *seq << endl;
                    break;
                case 5:
                    count = DialogValue("Input items count");
                    for (int i = 0; i < count; ++i) {
                        seq->Add(GenRandom<T>(rng));
                    }
                    break;
                case 6:
                    choice = Dialog(MSGS3);
                    if (choice) {
                        ISort<T> *sort = &QuickSort<T>;
                        if (choice == 2)
                            sort = &ShellSort<T>;
                        else if (choice == 3)
                            sort = &InsertionSort<T>;
                        auto t1 = high_resolution_clock::now();
                        seq->Sort(*sort);
                        auto t2 = high_resolution_clock::now();
                        duration<double, milli> ms_double = t2 - t1;
                        cout << "Result: " << ms_double.count() << "ms" << endl;
                    }
                    choice = 1;
                    break;
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
    StartUI();
    return 0;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(Laba3) {
        emscripten::function("start", &StartUI, emscripten::allow_raw_pointers());
}
#endif
