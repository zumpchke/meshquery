/* 
 * File:   KdTree.cpp
 * Author: vanush
 * 
 * Created on 27 July 2014, 5:23 PM
 */

#include "KdTree.h"
#include "glm/ext.hpp"

/**
 * Instantiate tree.
 */
KdTree::KdTree() {
    m_ndim = 3;
    DEBUG("Instantiating tree with dimension=" << m_ndim << endl);
    m_best_distance = std::numeric_limits<double>::max();
}

KdTree::KdTree(const KdTree& orig) {
}

KdTree::~KdTree() {
}

/**
 * Get the number of nodes in the tree
 * @return The number of nodes
 */
int KdTree::nodes()
{ 
    return m_nodes.size();
}

/**
 * Inserts a new node into the tree.
 * 
 * Traverse through tree, find the right place to insert the node using tree
 * properties
 * 
 * @param pt The point to insert
 */
void KdTree::insert(Point &pt)
{
    KdTreeDirection q;
    KdTree::node_ptr father, r;
    KdTree::node_ptr node = KdTree::node_ptr(new KdNode(pt));
    // Traverse through tree.
    r = m_root;
    if (m_root == nullptr)
    {

        DEBUG("Adding root node\n");
        // Tree is empty, create new node with given pt.

        m_nodes.push_back(node);
        m_root = node;

        // Start from x
        m_root->disc = 0;

    } else
    {
        // Find the right place to insert the node.
        while (r != nullptr)
        {
            father = r;
            q = compare(node, r, 0);
            KdTree::node_ptr * tmp = son(father, q);
            r = *tmp; //Ugh
        }

        assert (r == nullptr);
        DEBUG("Adding non-root node\n");

        // Add new node from the father
        if (r == nullptr)
        {
            DEBUG("Father is " << glm::to_string(father->pt) << endl);
            DEBUG("I am " << glm::to_string(node->pt) << endl);

            update_discriminator(node, father);
            node_ptr *s = son(father, q);
            *s = node;
            m_nodes.push_back(node);
        }
    }
}

/**
 * Update axis discriminator for splitting plane according to father and
 * the number of dimensions
 * @param node Node to update
 * @param father Node's father
 */
void KdTree::update_discriminator(KdTree::node_ptr node, KdTree::node_ptr father)
{
    node->disc = (father->disc + 1) % m_ndim;
}

/**
 * Determine the pointer-to-the-pointer of a node's son given a direction.
 * @param node Node to get a child branch of
 * @param dir Direction to move to
 * @return Pointer to pointer of a child of a node.
 */
KdTree::node_ptr* KdTree::son(KdTree::node_ptr node, KdTreeDirection dir)
{
    KdTree::node_ptr * ptr;
    DEBUG("Dir is " << (int) dir << endl);
    if (dir == KdTreeDirection::KdTreeDirectionLeft)
    {
        DEBUG("returning left\n");
        ptr = &node->left;
    } else if (dir == KdTreeDirection::KdTreeDirectionRight)
    {
        DEBUG("returning right\n");
        ptr = &node->right;
    }
    return ptr;
}

/**
 * Get the root node
 * @return The root node
 */
KdTree::node_ptr KdTree::root() const
{
    return m_root;
}

/**
 * Recursively traverse the tree to find the nearest neighbor within the maximum
 * search distance, using tree properties to exclude nodes
 * @param query Query point
 * @param currNode current node
 * @param maxDist maximum search distance
 */
void KdTree::nearest(const Point &query, const KdTree::node_ptr &currNode, double maxDist) const
{
    // If current node is null
    if (!currNode)
    {
        return;
    }

    KdTreeDirection ldir, rdir;

    // Calculate distance from current node
    Point dist = query - currNode->pt;
    double squaredDist = glm::dot(dist, dist);
    DEBUG("Squared distance is " << squaredDist << endl);
    if (squaredDist <= maxDist * maxDist && squaredDist < m_best_distance)
    {
        m_best = currNode;
        m_best_distance = squaredDist;
        if (m_best_distance == 0)
        {
            return;
        }
    }

    // Check left and right subtree
    ldir = compare(query, currNode, -maxDist);
    rdir = compare(query, currNode, maxDist);

    if (ldir == KdTreeDirection::KdTreeDirectionRight)
    {
        //nearest(query, currNode->left, maxDist);
        nearest(query, currNode->right, maxDist);
    } else if (rdir == KdTreeDirection::KdTreeDirectionLeft)
    {
        nearest(query, currNode->left, maxDist);
    } else {
        nearest(query, currNode->left, maxDist);
        nearest(query, currNode->right, maxDist);
    }
}

/**
 * Get best node according to nearest search
 * @return Nearest node (can be null)
 */
KdTree::node_ptr KdTree::get_nearest() const
{
    return m_best;
}

/**
 * Get distance associated with nearest node
 * @return distance
 */
double KdTree::get_nearest_distance() const
{
    return m_best_distance;
}

/**
 * Compare a point to a node's point with a delta. Used to exclude branches if
 * a certain bounds criterion is met.
 * @param query Query point
 * @param q Node which contains point to check
 * @param delta Maximum search distance that defines bounds.
 * @return
 */
KdTree::KdTreeDirection KdTree::compare(const Point &query, const KdTree::node_ptr &q, double delta) const
{
    KdTree::node_ptr node = KdTree::node_ptr(new KdNode(query));
    return compare(node, q, delta);
}

/**
 * Compare two nodes in order to determine which direction a child node should move to 
 * @param p Node one
 * @param q Node two
 * @param delta Used if bounds checking is required.
 * @return
 */
KdTree::KdTreeDirection KdTree::compare(const KdTree::node_ptr &p, const KdTree::node_ptr &q, double delta) const
{

    if ( (p->pt[q->disc] + delta) < q->pt[q->disc])
    {
        return KdTreeDirection::KdTreeDirectionLeft;
    } else
    {
        return KdTreeDirection::KdTreeDirectionRight;
    }
}
