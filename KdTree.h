/* 
 * File:   KdTree.h
 * Author: vanush
 *
 * Created on 27 July 2014, 5:23 PM
 */

#ifndef KDTREE_H
#define	KDTREE_H
#include <memory>
#include <vector>
#include "glm/vec3.hpp"
#include <iostream>
#include <limits>

using namespace std;

//#define DEBUG(x) do { std::cerr << x; } while (0)
#define DEBUG(x) 0

typedef glm::vec3 Point;

class KdTree {
public:
    KdTree();
    KdTree(const KdTree& orig);
    virtual ~KdTree();
    int nodes();

    void insert(Point &pt);

    enum KdTreeDirection {
        KdTreeDirectionLeft,
        KdTreeDirectionRight
    };

    struct KdNode {
        shared_ptr<KdNode> left, right;
        Point pt;
        int disc;
        KdNode(const Point p) : pt(p) {}
    };

    typedef shared_ptr<KdNode> node_ptr;

    void nearest(const Point &query, const KdTree::node_ptr &currNode, double maxDist) const;
    node_ptr get_nearest() const;
    double get_nearest_distance() const;
    node_ptr root() const;

private:
    vector<node_ptr> m_nodes;
    KdTree::node_ptr mutable m_best;
    double mutable m_best_distance;
    int m_ndim;

    KdTreeDirection compare(const KdTree::node_ptr &p, const KdTree::node_ptr &q, double delta = 0) const;
    KdTreeDirection compare(const Point &query, const KdTree::node_ptr &q, double delta) const;
    shared_ptr<KdNode> m_root;

    KdTree::node_ptr* son(KdTree::node_ptr node, KdTreeDirection dir);
    void update_discriminator(KdTree::node_ptr node, KdTree::node_ptr father);

};

#endif	/* KDTREE_H */

