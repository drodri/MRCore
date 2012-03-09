#ifndef __MR_NODE_H
#define __MR_NODE_H

#include <set>
#include <iostream>
#include <cmath>

using namespace std;

namespace mr{

template<typename N , typename E>
class Graph;

template<typename N , typename E>
class Edge;

template<typename N , typename E>
class UninformedSearcher;

template<typename N , typename E>
class DijkstraSearcher;


template<typename N , typename E>
class Node{
	friend class Graph<N,E>;
	friend class Edge<N,E>;
	friend class UninformedSearcher<N,E>;
	friend class DijkstraSearcher<N,E>;

	friend ostream& operator<< (ostream&os, const Node<N,E> & node){
		os << node.value << " " ;
		return os;
	}
	
	typedef Edge<N,E> EDGE;
	
public:
	Node(const N& n){
		value=n;
		object=NULL;
	}
	virtual ~Node(){}

	N getValue() const {return value;}

	void setValue(const N& v){value=v;}

	set< Node<N,E>* > getOutgoingNodes(){
		if (neighUpdate) updateNeighbors();
		return outgoingNodes;
	}
	
	set< Node<N,E>* > getIncomingNodes(){
		if (neighUpdate) updateNeighbors();
		return incomingNodes;
	}

	/**
	 * \return a pair (Node,Edge) with the incoming Nodes and their corresponding edge.
	 */
	set < pair < Node<N,E>* , EDGE* > > getIncoming(){
		if (neighUpdate) updateNeighbors();
		return incoming;
	}

	/**
	 * \return a pair (Node,Edge) with the outgoing Nodes and their corresponding edge.
	 */
	set < pair < Node<N,E>* , EDGE* > > getOutgoing(){
		if (neighUpdate) updateNeighbors();
		return outgoing;
	}

	/**
	 * This function must be defined for each particular node type
	 */
	E costTo(Node<N,E>*);

	void* object; //!< void pointer to store whatever we may need for nodes
protected:

	N value;
	set<EDGE*> edges;
	set < Node<N,E>* > incomingNodes; //!< Nodes FROM which there is an edge
	set < Node<N,E>* > outgoingNodes; //!< Nodes TO which there is an edge

	set < pair < Node<N,E>* , EDGE* > > incoming;
	set < pair < Node<N,E>* , EDGE * > > outgoing;

	/**
	 * Adds an Edge to the Node.
	 * It makes a copy of the passed edge.
	 * \return Pointer to the created Edge.
	 */
	/*EDGE* addEdge(EDGE edg){
		EDGE* e = new EDGE(edg);
		edges.insert(e);
		neighUpdate=true; //!< Required to update neighbors next time they are consulted
		return e;
	}*/

	/**
	 * Adds an Edge to the Node.
	 * It makes a copy of the passed edge.
	 * \return Pointer to the created Edge.
	 */
	/* 
	EDGE* addEdge(E w){
		EDGE* edg = new EDGE(w);
		edges.insert(edg);
		neighUpdate=true; //!< Required to update neighbors next time they are consulted
		return edg;
	}*/

	/**
	 * Adds an Edge to the Node.
	 * It takes control over the pointer.
	 */
	void addEdge(EDGE* edg){
		if (edges.find(edg)==edges.end()) edges.insert(edg);
		neighUpdate=true; //!< Required to update neighbors next time they are consulted
	}

	void removeEdge(EDGE* edg){
		edges.erase(edges.find(edg));
		neighUpdate=true;
	}


	bool hasNeighbor(Node<N,E>* neig, EDGE** p_edge){
		//cout << 1 << endl;
		typename set<EDGE*>::iterator it;
		for (it=edges.begin(); it!=edges.end(); it++)
		{
			if ( (*it)->hasNode(neig) ){
				//cout << 2 << endl;
				(*p_edge)=(*it);
				return true;
			}
		}
		//cout << 3 << endl;
		(*p_edge)=NULL;
		//cout << 4 << endl;
		return false;
	}


private:
	bool neighUpdate; //!< True when edges have changed and neighbors list update required

	/**
	 * Updates the incoming and outgoing nodes list
	 */
	void updateNeighbors(){
		
		//clear the lists, we will make them new
		incomingNodes.clear(); 
		outgoingNodes.clear();

		incoming.clear();
		outgoing.clear();

		//cout << "Cleared inc and out" << endl;

		//Iterate on the edges
		typename set<EDGE*>::iterator it;
		
		for ( it=edges.begin() ; it != edges.end(); it++ ){
			pair<Node<N,E>*,Node<N,E>*> nodes = (*it)->getNodes();
			//if directed
			//cout << "Edge nodes" << endl;
			if ((*it)->isDirected() ){
				//cout << "Directed" << endl;
				//if we are the first -> outgoing
				if (nodes.first == this ){
					//cout << " insert out " << endl;
					outgoingNodes.insert(nodes.second);
					pair < Node<N,E>*,EDGE* > pr = make_pair(nodes.second, (*it) );
					outgoing.insert(pr);
				}
				//if we are the second -> incoming
				if (nodes.second == this ){
					//cout << "insert it " << endl;
					incomingNodes.insert(nodes.first);
					pair < Node<N,E>*,EDGE* > pr = make_pair(nodes.first, *it);
					incoming.insert(pr);
				}
			}else{ //not directed
				//cout << "Nod directed" << endl;
				if (nodes.first == this ){
					//cout << "ND, insert second" << endl;
					
					outgoingNodes.insert(nodes.second);
					incomingNodes.insert(nodes.second);
					pair < Node<N,E>*,EDGE* > pr = make_pair(nodes.second, *it);
					outgoing.insert(pr);
					incoming.insert(pr);
				}
				
				if (nodes.second == this ){
					//cout << "ND, insert first" << endl;
					outgoingNodes.insert(nodes.first);
					incomingNodes.insert(nodes.first);
					pair < Node<N,E>*,EDGE* > pr = make_pair(nodes.first, *it);
					outgoing.insert(pr);
					incoming.insert(pr);
				}
			}//end else(directed)
		}//end for
	}
};


}//end namespace

#endif
