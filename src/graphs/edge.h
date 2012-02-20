#ifndef __MR_EDGE_H
#define __MR_EDGE_H

#include <set>
#include <iostream>

using namespace std;

namespace mr{

template<typename N , typename E>
class Graph;

template<typename N , typename E>
class Node;

template<typename N , typename E>
class UninformedSearch;


template<typename N , typename E>
class Edge{
	friend class Graph<N,E>;
	friend class Node<N,E>;
	friend class UninformedSearch<N,E>;

	friend ostream& operator<< (ostream& os,const Edge<N,E>& edge){
		os << edge.weight << " ";
	}

	
	typedef Node<N,E> NODE; 
public:
	Edge(NODE* node1, NODE* node2, bool d=false){
		nodes = make_pair(node1,node2);
		directed=d;
		weightSet=false;
	}
	
	Edge(NODE* node1, NODE* node2, E w, bool d=false){
		nodes = make_pair(node1,node2);
		weight=w;
		weightSet=true;
		directed=d;
	}
	
	virtual ~Edge(){}

	/**
	 * Sets the weight of the edge
	 */
	void setWeight(E w){weight = w; weightSet=true;}

	/**
	 * Computes the weight of the edge according to the nodes
	 */
	void computeWeight(){weight=nodes.first->costTo(nodes.second);}

	/**
	 * Computes the weight of the edge if it was not previously set according to the nodes
	 */
	void computeWeightIfNotSet(){if (!weightSet) computeWeight();}

	/**
	 * \return the weight of the edge
	 */
	E getWeight() const {
		//if (!weightSet) computeWeight();
		return weight;
	}

	/**
	 * \return a pair set pointing to the extremes of the edge
	 */
	pair<NODE*,NODE*> getNodes(){return nodes;}

	/**
	 * \return if is a directed edge. Direction is from node 1 to 2
	 */
	bool isDirected(){return directed;}

	/**
	 * Sets if an edge is directed
	 */
	void isDirected(bool d){directed=d;}


protected:
	E weight;
	bool weightSet;
	bool directed;
	pair<NODE*,NODE*> nodes;

	/**
	 * \return If the edge connects the node passed through parameter.
	 */
	bool hasNode(NODE* nd){
		if ( (nodes.first == nd) || (nodes.second == nd) ) return true;
		else return false;
	}

	/**
	 * \return if the edge joins the nodes passed through parameter
	 */
	bool joinsNodes(NODE* nd1, NODE* nd2){
		return ( hasNode(nd1) && hasNode(nd2) );
	}
	
};

} //end namespace

#endif
