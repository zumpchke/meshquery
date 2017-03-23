/*
 * File:   ClosestPointQuery.cpp
 * Author: vanush
 *
 * Created on 28 July 2014, 2:28 AM
 */

#include "ClosestPointQuery.h"
#include "glm/vec3.hpp"
#include "glm/ext.hpp"
#include <exception>
#include <stdexcept>

ClosestPointQuery::ClosestPointQuery(const Mesh& m) {
    m_mesh = m;
}

Point ClosestPointQuery::operator() (const Point& queryPoint, double maxDist) const
{
    if (maxDist <= 0)
    {
        throw std::logic_error("Maximum search distance must be positive");
    }

    m_mesh.nearest(queryPoint, m_mesh.root(), maxDist);
    Mesh::node_ptr near_node = m_mesh.get_nearest();
    if (near_node)
    {
       DEBUG("Nearest point to query point is " << glm::to_string(near_node->pt)
              << endl << "with squared distance " << m_mesh.get_nearest_distance() << endl);

    } else
    {
        throw std::logic_error("No point found within search distance");
    }
    // Copy point
    Point ret_pt(near_node->pt);
    return ret_pt;
}


