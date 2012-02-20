#ifndef __MR_GRAPH_H
#define __MR_GRAPH_H

#include <set>
#include <iostream>
#include <fstream>

#include "node.h"
#include "edge.h"

using namespace std;

namespace mr{

template<typename N , typename E>
class UninformedSearch;

template<typename N , typename E>
class DijkstraSearch;

template<typename N , typename E>
ostream& operator << (ostream& os,const Graph<N,E>& graph){

	typedef Node<N,E> NODE;
	typedef Edge<N,E> EDGE;

	typename set<NODE*>::iterator itn;
	typename set<EDGE*>::iterator ite;
	int i=0;

	//serialize the nodes

	//number of nodes
	os << graph.nodes.size()<< " " ;
	//the nodes

	for (itn=graph.nodes.begin() ; itn!=graph.nodes.end() ; itn++, i++){
		NODE* nd = *itn;
		nd->object = new int(i);
		os << *nd;
	}

	//seralize the edges

	// number of edges
	os << graph.edges.size() << " ";
	//the edges
	for (ite=graph.edges.begin(); ite!=graph.edges.end(); ite++){
		EDGE* edg = *ite;
		int nd1 = *( (int*)(edg->getNodes().first->object) );
		int nd2 = *( (int*)(edg->getNodes().second->object) );
		os << nd1 << " " << nd2 << " " << *edg;
	}

	os << endl;

	//delete the created objects
	for (itn=graph.nodes.begin() ; itn!=graph.nodes.end() ; itn++, i++){
		NODE* nd = *itn;
		delete (int*)(nd->object);
	}

	return os;
}


template<typename N , typename E>
class Graph{
	typedef Node<N,E> NODE;
	typedef Edge<N,E> EDGE;

	friend class UninformedSearch<N,E>;
	friend class DijkstraSearch<N,E>;

	//serializer
	friend ostream& operator<< <>(ostream& os,const Graph<N,E>& graph);

public:
	Graph(){}
	virtual ~Graph(){} //!< Do not clear here, we may have graphs pointing to the same nodes.

	/**
	 * Adds a node to the graph
	 * \return The pointer to the node
	 */
	NODE* const addNode(const N& nd){
		NODE* node = new NODE(nd);
		nodes.insert(node);
		return node;
	}
	
	void addNode(NODE* nd){
		if (nodes.find(nd) == nodes.end())
		nodes.insert(nd);
	}

	/**
	 * Add a set of nodes to the graph
	 * \return the the set containing all nodes.
	 */
	const set<NODE*> addNodes(set<N> nds){
		typename set<N>::iterator itn;
		for (itn=nds.begin();itn!=nds.end();itn++){
			addNode(*itn);
		}
		return nodes;
	}


	/**
	 * Adds an edge among two nodes
	 * If any of the nodes is not in the graph, it adds it.
	 * \return Pointer to the edge.
	 */
	EDGE* const addEdge(NODE* node1, NODE* node2, bool d=false){
		//Check if the nodes are in the graph. If not, add them.
		if (nodes.find(node1) == nodes.end() ) nodes.insert(node1);
		if (nodes.find(node2) == nodes.end() ) nodes.insert(node2);
		EDGE** p_edge = new (EDGE*);
		
		//If they are already adjacent, check if edge is directed. If so, make it undirected when corresponds.
		if (isAdjacent(node1,node2,p_edge)){
			if ((*p_edge)->nodes.first == node1){
				EDGE* edg = (*p_edge);
				delete p_edge;
				return edg;
			}else{ //we have an edge from node2 to node1, so it is not directed
				(*p_edge)->isDirected(false);
				EDGE* edg = (*p_edge);
				delete p_edge;
				return edg;
			}
		}

		//else
		EDGE* edge;
		edge = new EDGE(node1,node2,d); //create the edge
		edges.insert(edge); //insert it into the edges list.
		node1->addEdge(edge); //add the edge to the node.
		node2->addEdge(edge);
		return edge;
	}

	/**
	 * Adds an edge among two nodes
	 * If any of the nodes is not in the graph, it adds it.
	 * \return Pointer to the edge.
	 */
	EDGE* const addEdge(NODE* node1, NODE* node2, E value, bool d=false){

		//Check if the nodes are in the graph. If not, add them.

		if (nodes.find(node1) == nodes.end() ) nodes.insert(node1);
		if (nodes.find(node2) == nodes.end() ) nodes.insert(node2);

		
		EDGE** p_edge;
		if (isAdjacent(node1,node2,p_edge)) return *p_edge;

		//else
		EDGE* edge;
		edge = new EDGE(node1,node2,value, d); //create the edge
		edges.insert(edge); //insert it into the edges list.
		node1->addEdge(edge); //add the edge to the node.
		node2->addEdge(edge);

		return edge;
	}

	/**
	 * Computes the weight of the edges
	 * If the edges had already a weight it does not change it
	 */
	void computeEdgesWeight(){
		typename set<EDGE*>::iterator it;
		for (it=edges.begin();it!=edges.end();it++){
			(*it)->computeWeightIfNotSet();
		}
	}

	/**
	 * Computes all the weight of the edges even if they were previously set
	 */
	void recomputeEdgesWeight(){
		typename set<EDGE*>::iterator it;
		for (it=edges.begin();it!=edges.end();it++){
			(*it)->computeWeight();
		}
	}

	/**
	 * \return a const set of pointers to nodes
	 */
	const set<NODE*> getNodes() const {return nodes;}

	/**
	 * \return a const set of pointers to edges
	 */
	const set<EDGE*> getEdges() const {return edges;}

	/**
	 * Deletes an edge among two nodes
	 */
	 void removeEdge(NODE* node1, NODE* node2){
		 EDGE** edg;
		 if (isAdjacent(node1,node2,edg)){
			 node1->removeEdge(*edg);
			 node2->removeEdge(*edg);
			 edges.erase(edges.find(*edg));
			 delete (*edg);
		 }
	 }

	 /**
	 * Deletes an edge among two nodes
	 */

	void removeEdge(EDGE* edg){
		NODE* node1 = edg->nodes.first;
		NODE* node2 = edg->nodes.second;
		node1->removeEdge(edg);
		node2->removeEdge(edg);
		edges.erase(edges.find(edg));
		delete edg;
	}

	void removeNode(NODE* nd){
		set<EDGE*> nodeEdgs = nd->edges;
		typename set<EDGE*>::iterator it;

		//remove the edges of nodes connected to this node
		for (it=nodeEdgs.begin();it!=nodeEdgs.end();it++){
			if ( (*it)->nodes.first != nd ) ((*it)->nodes.first)->removeEdge(*it);
			if ( (*it)->nodes. second != nd ) ((*it)->nodes.second)->removeEdge(*it);
		}

		nodes.erase(nodes.find(nd));
		delete nd;
	}

	int nodesSize(){
		return nodes.size();
	}

	int edgesSize(){
		return edges.size();
	}

	void clear(){
		typename set<NODE*>::iterator itn;
		for (itn=nodes.begin();itn!=nodes.end();itn++){
			if (*itn != NULL) delete (*itn);
		}
		nodes.clear();

		typename set<EDGE*>::iterator ite;
		for (ite=edges.begin();ite!=edges.end();ite++){
			if (*ite != NULL) delete (*ite);
		}
		edges.clear();
	}

//protected:
	set <NODE*> nodes;
	set <EDGE*> edges;

	/**
	 * Checks if two nodes are connected.
	 * Returns a constant pointer to the edge connecting them (if any). Otherwise, NULL
	 */
	bool isAdjacent(NODE* node1, NODE* node2, EDGE** p_edge){
		typename set<NODE*>::iterator ndIt;
		//edge=NULL;
		ndIt=nodes.find(node1);
		if(  (ndIt == nodes.end()) || (nodes.find(node2) == nodes.end()) ){
			cerr << "Node does not belong to the graph" << endl;
			return false;
		}
		//else
		if ((*ndIt)->hasNeighbor(node2,p_edge)){
			return true;
		}
		return false;
	}

	//saves graph to file
	void saveToFile(string fileName){
		ofstream file;
		file.open(fileName.c_str());
		if(file.is_open()){
			file << *(this) << endl;
			file.close();
		}else{
			cerr << "Cannot open file " << fileName << endl;
		}
	}

	//loads graph from file
	void loadFromFile(string fileName){
	}

};

} //end namespace

#endif
