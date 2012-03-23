#ifndef __GRAPH_TYPES_H
#define __GRAPH_TYPES_H

#include "graph.h"
#include "searcher.h"
#include "astarsearch.h"
#include "uninformedsearch.h"
#include "dijkstrasearch.h"
#include "math/transformation2d.h"

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
typedef Graph<Pose2D,double> PosesGraph;
typedef Node<Pose2D,double> PoseNode;
typedef Edge<Pose2D,double> CartesianEdge;
typedef Searcher<Pose2D, double> PosesGraphSearcher;
typedef UninformedSearcher<Pose2D,double> UninformedPosesSearcher;
typedef DijkstraSearcher<Pose2D,double> DijkstraPosesSearcher;
typedef AstarSearcher<Pose2D,double> AstarPosesSearcher;

}//end namespace

#endif
