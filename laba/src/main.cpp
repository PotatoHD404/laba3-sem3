#include <iostream>
#include "SquareMatrix.hpp"
#include "SparseMatrix.hpp"
#include "Graph.hpp"
#include "Random.hpp"

using namespace std;


int main() {
    Graph<int> a(10, 20, false, false, true);

    cout << a.GraphvizPrint() << endl;
//
//    Set<size_t *> passed;
//    for (size_t i = 0; i < 200; ++i) {
//        passed.Add(new size_t{i});
//    }
//    passed.Add(new size_t{});
////    for (auto el: passed) {
//////        passed.Add(el);
////        if (!passed.Contains(el))
////            cout << passed.Contains(el) << endl;
////    }
//    int i = 0;
//    size_t *a;
//    for (auto el: passed) {
//        if (i++ != 0) {
//            cout << (a < el) << endl;
//        }
//        a = el;
//    }
////    passed.Add(12);
////    passed.Add(16);
////    passed.Add(0);
////    passed.Add(8);
////    passed.Add(13);
////    passed.Add(34);
//    cout << passed.Count() << endl;
//    cout << passed << endl;

//    SparseMatrix<size_t> a(100);
//    SquareMatrix<size_t> a(3);
//
//    a.Set(1, 0, 20);
//    a.Set(2, 0, 35);
//    a.Set(2, 0, 0);
//
//    a.Set(0, 2, 100);
//    cout << a << endl;
//
//    cout << a.data << endl;
//
//    for (const auto& el: a) {
//        cout << el << endl;
//    }
//
//
//    a.Expand(1);
//    cout << a << endl;
//    cout << a.Size() << endl;
//    a.Get(2, 1) = 1;
//
//    for (size_t i = 0; i < a.data.Count(); ++i) {
//        a.data[i] = i;
//    }

//    a.Expand();
//    cout << a << endl;
//    cout << a.data << endl;
//
//    for (size_t i = 0; i < a.data.Count(); ++i) {
//        a.data[i] = i;
//    }
//    cout << a << endl;
//    cout << a.data << endl;
//    cout << a.Get(0, 4) << endl;


    return 0;
}