#include "graphtypes.h"

namespace mr{


/********************************************
 ***********  SIMPLE GRAPH FUNCS ************
 ********************************************/

//!< Constructor for simple graphs. Weight is always one
template<> SimpleEdge::Edge(SimpleNode* node1, SimpleNode* node2, bool d){
		nodes = make_pair(node1,node2);
		directed=d;
		weight=1;
		//ZERO_COST=0;
		//INFINITE_COST=99999999;
}

//! dist among two int nodes
template<> int SimpleNode::costTo(SimpleNode* dest){
		//this crazy thing is because windows does not like when I use fabs
	int aux = value-dest->value;
	if (aux < 0) return -aux;
	else return aux;
}

//! serializers
/*
ostream& operator << (ostream& os,const SimpleNode& node){
	os << node.getValue() << " ";
	return os;
}

ostream& operator << (ostream& os,const SimpleEdge& edge){
	os << edge.getWeight() << " ";
	return os;
}
*/


/********************************************
 ***********  POSES GRAPH FUNCS ************
 ********************************************/

//! dist among two Pose nodes
template<> double PoseNode::costTo(PoseNode* dest){
	return distance(value,dest->value);
}





} // end namespace
