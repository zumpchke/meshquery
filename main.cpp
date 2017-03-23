/**
@file main.cpp
*/
/*
 * File:   main.cpp
 * Author: vanush
 *
 * Created on 27 July 2014, 5:22 PM
 */

#include <cstdlib>
#include <ctime>
#include "glm/vec3.hpp"
#include "glm/ext.hpp"
#include "KdTree.h"
#include "ClosestPointQuery.h"
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>

using namespace std;

/** 2D test 1. Assert that no point is found
 *  within the specified distance
 */
void test_2d_1()
{
    Point a(10.0f, 5.0f, 0.0f);
    Mesh tree;
    tree.insert(a);

    ClosestPointQuery query(tree);
    try
    {
        query(Point(0.0f, 0.0f, 0.0f), 1.0f);
    } catch (exception &e)
    {
        assert (typeid(e) == typeid(std::logic_error));
    }
}
/**
 * 2D test 2. Assert a point is found within the specified distance.
 */
void test_2d_2()
{
    Point a(55.0f, 10.2f, 0.0f);
    Mesh tree;
    tree.insert(a);

    ClosestPointQuery query(tree);
    Point ret_pt = query(Point(55.0f, 10.2f, 0.0f), 1.0f);
    assert(ret_pt == a);
}

/**
 * 2D test 3. Example from Samet book
 */
void test_2d_3()
{
    Point a(35.0f, 40.0f, 0.0f);
    Point b(5.0f, 45.0f, 0.0f);
    Point c(50.0f, 10.0f, 0.0f);
    Point d(25.0f, 35.0f, 0.0f);
    Point e(90.0f, 5.0f, 0.0f);
    Point f(80.0f, 65.0f, 0.0f);
    Point g(60.0f, 75.0f, 0.0f);
    Point h(85.0f, 15.0f, 0.0f);

    Mesh tree;
    tree.insert(a); tree.insert(b);
    tree.insert(c); tree.insert(d);
    tree.insert(e); tree.insert(f);
    tree.insert(g); tree.insert(h);

    ClosestPointQuery query(tree);
    Point ret_pt = query(Point(88.0f, 6.0f, 0.0f), 3.0f);
    Point f_pt(90.0f, 5.0f, 0.0f);
    assert (ret_pt == f_pt);
}

/**
 * 2D test 4. Constructed example.
 */
void test_2d_4()
{
    Mesh tree;
    Point a(62.0, 34.0, 0.0);
    Point b(65.0, 31.0, 0.0);
    Point c(70.0, 62.0, 0.0);
    Point d(64.0, 16.0, 0.0);
    Point e(85.0, 62.0, 0.0);
    Point f(52.0, 53.1, 2.0);

    tree.insert(a);
    tree.insert(b);
    tree.insert(c);
    tree.insert(d);
    tree.insert(e);
    tree.insert(f);

    ClosestPointQuery query(tree);

    Point ret_pt = query(Point(52.0, 53.1f, 2.0f), 10.0f);
    assert (ret_pt == f);
}

/**
 * 3D test 1. Generate 1000 random points. Add known point, then do a query with
 * the known point and assert result.
 */
void test_3d_1()
{
    int x, y, z;
    Mesh tree;
    for (int i =0; i < 2000; i++)
    {
        x = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/100));
        y = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/100));
        z = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX/100));
        Point a(x, y, z);
        tree.insert(a);
    }
    assert(tree.nodes() == 2000);
    Point b(52.0f, 53.1f, 0.0f);

    tree.insert(b);
    ClosestPointQuery query(tree);

    Point ret_pt = query(Point(51.0f, 53.1f, 0.0f), 10.0f);
    cout << "Closest point is " << glm::to_string(ret_pt) << endl;
    assert (ret_pt == b);
}

/**
 * Application entry point. Runs tests.
 * @return
 */
int main(int argc, char** argv) {
    // Seed for rand())
    srand (static_cast <unsigned> (time(0)));

    test_2d_1();
    test_2d_2();
    test_2d_3();
    test_2d_4();

    test_3d_1();
    cout << "All tests passed" << endl;
    return 0;
}

