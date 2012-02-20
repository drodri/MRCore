#ifndef __SEARCHER_H
#define __SEARCHER_H

#include "graph.h"
#include "node.h"
#include "edge.h"

#include <vector>
#include <set>

namespace mr{

template<typename N , typename E>
class Searcher{

	typedef Node<N,E> NODE;
	typedef Edge<N,E> EDGE;
	typedef Graph<N,E> GRAPH;

	public:
		Searcher(){graph=NULL;}
		Searcher(GRAPH* g){graph=g;}
		virtual ~Searcher(){}

		/**
		 * Sets pointer to graph
		 */
		void setGraph(GRAPH* g){graph=g;}

		virtual bool findPath(NODE* initNode, NODE* goalNode) = 0 ;
		/**
		 * returns vector of nodes constituting the last computed path
		 */
		const vector<NODE*> getPath() const {return path;}

	protected:
		vector<NODE*> path;
		GRAPH* graph;
	private:


		/**
		 * Traces the path from goalNode to initNode.
		 * The search must be computed befofrehand
		 */
		virtual void tracePath(NODE* initNode, NODE* goalNode) = 0;


		/**
		 * Initialize the graph for performing a search
		 */
		virtual void initialize() = 0;
		
		/**
		 * Cleans the graph after performing a search
		 */
		virtual void clean() = 0;
};

} //end namespace

#endif
