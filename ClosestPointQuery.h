/* 
 * File:   ClosestPointQuery.h
 * Author: vanush
 *
 * Created on 28 July 2014, 2:28 AM
 */

#ifndef CLOSESTPOINTQUERY_H
#define	CLOSESTPOINTQUERY_H

#include "KdTree.h"

typedef KdTree Mesh;

class ClosestPointQuery
{
public:
    ClosestPointQuery(const Mesh& m);

    /**
     * Return the closest point to the mesh within specified search distance
     * @param queryPoint query point
     * @param maxDist maximum distance
     * @return Point if found
     */
    Point operator() (const Point& queryPoint, double maxDist) const;
private:
    Mesh m_mesh;
};

#endif	/* CLOSESTPOINTQUERY_H */

