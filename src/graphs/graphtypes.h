#ifndef __GRAPH_TYPES_H
#define __GRAPH_TYPES_H

#include "mrcore.h"
#include "graph.h"
#include "searcher.h"
#include "astarsearch.h"
#include "uninformedsearch.h"
#include "dijkstrasearch.h"

namespace mr{

//!< Simple Graphs Typedefs
typedef Graph<int, int> SimpleGraph;
typedef Node<int, int> SimpleNode;
typedef Edge<int,int> SimpleEdge;
typedef Searcher<int, int> SimpleGraphSearcher;
typedef UninformedSearcher<int, int> UninformedSimpleGraphSearcher;
typedef DijkstraSearcher<int, int> DijkstraSimpleGraphSearcher;
typedef AstarSearcher<int, int> AstarSimpleGraphSearcher;


//!< Poses Graphs Typedefs
typedef Graph<Pose,double> PosesGraph;
typedef Node<Pose,double> PoseNode;
typedef Edge<Pose,double> CartesianEdge;
typedef Searcher<Pose, double> PosesGraphSearcher;
typedef UninformedSearcher<Pose,double> UninformedPosesSearcher;
typedef DijkstraSearcher<Pose,double> DijkstraPosesSearcher;
typedef AstarSearcher<Pose,double> AstarPosesSearcher;

}//end namespace

#endif
