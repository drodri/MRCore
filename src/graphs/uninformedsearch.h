#ifndef __UNINFORMED_SEARCH_H
#define __UNINFORMED_SEARCH_H

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "searcher.h"

#include <vector>
#include <set>

namespace mr{

template<typename N , typename E>
class UninformedSearcher: public Searcher<N,E>{

	enum {NON_VISITED=0, VISITED};
	enum {DEPTH_FIRST=0, BREADTH_FIRST};

	typedef Node<N,E> NODE;
	typedef Edge<N,E> EDGE;
	typedef Graph<N,E> GRAPH;

	class AuxObject{
		public:
			AuxObject(){status=VISITED;}
			AuxObject(int s):status(s){};
		NODE* comingFrom;
		int status;
	};

	public:
		UninformedSearcher(){this->graph=NULL;alg=BREADTH_FIRST;}
		UninformedSearcher(GRAPH* g){this->graph=g; alg=BREADTH_FIRST;}
		virtual ~UninformedSearcher(){}

		/**
		 * Search Algorithm: DEPTH_FIRST, BREADTH_FIRST
		 */
		void setAlgorithm(int a){alg=a;}

		bool findPath(NODE* initNode, NODE* goalNode){

			initialize();

			vector<NODE*> searchQueue;
			searchQueue.clear();
			NODE* currentNode;
			NODE* nextNode;

			AuxObject* obj= (AuxObject*)initNode->object;
			obj->status=VISITED;
			obj->comingFrom=NULL; // for reconstructing path
			searchQueue.push_back(initNode);


			while(!searchQueue.empty()){
				//cout << "Queue size " << searchQueue.size() << endl;
				switch(alg){
					case BREADTH_FIRST:
						//cout <<"1" << endl;
						currentNode=searchQueue.front(); //start searching with the first node -> FIFO
						//cout <<"2" << endl;
						searchQueue.erase(searchQueue.begin()); //and erase it form the searching queue
						//cout <<"3" << endl;
						break;
					case DEPTH_FIRST:
						currentNode=searchQueue.back(); //start searching with the last node -> LIFO
						searchQueue.pop_back(); //and erase it form the searching queue
						break;
				}
				//cout <<"4" << endl;
				if (currentNode==goalNode){
					tracePath(initNode,goalNode);
					clean();
					return true; // YEHAA!! goal found
				}

				//else

				//cout <<"5" << endl;
				if (currentNode==NULL){
					cout << "UninformedSearch: CurrentNode = NULL, this should never happen" << endl;
					continue;
				}
				//cout <<"6" << endl;
				//cout << currentNode << endl;
				//cout << "Node " << *currentNode << endl;
				set<NODE*> outNeig = currentNode->getOutgoingNodes();
				//cout << "outN size " << outNeig.size() << endl;
				typename set<NODE*>::iterator out_it;
				//cout <<"7" << endl;
				for (out_it=outNeig.begin(); out_it!=outNeig.end(); out_it++){
					nextNode=*out_it;
					//we want only non visited nodes
					AuxObject* obj = (AuxObject*)nextNode->object;
					if (obj->status == NON_VISITED){ //if it would have been visited it should be NULL (graph.clean())
						obj->status=VISITED; //for not looping anymore
						obj->comingFrom=currentNode; // for reconstructing path
						searchQueue.push_back(nextNode);
						//cout << "Adding to queue"<< endl;
					}
				}
			}
			clean();
			return false; //OOPS! final Node not found
		}


	private:
		int alg;
		//vector<NODE*> searchQueue;



		/**
		 * Traces the path from goalNode to initNode.
		 * The search must be computed befofrehand
		 */
		void tracePath(NODE* initNode, NODE* goalNode){
			this->path.clear();
			if(goalNode->object==NULL){
				cout << "Error tracing path" << endl;
				return;
			}

			NODE* currNode;

			AuxObject* obj = (AuxObject*)(goalNode->object);
			this->path.push_back(goalNode); //begin with the final node

			while (obj->comingFrom!=initNode){ //until we arrive to the initNode
				currNode=obj->comingFrom; //previous step
				this->path.push_back(currNode); //add it to the path
				if(currNode->object==NULL){ //this should not happen
					cout << "Error tracing path" << endl;
					return;
				}
				obj=(AuxObject*)currNode->object;
			}

			this->path.push_back(initNode); //finally add the initNode
		}


		/**
		 * Initialize the graph for performing a search
		 */
		void initialize()
		{
			typename set<NODE*>::iterator itn;
			for (itn=this->graph->nodes.begin();itn!=this->graph->nodes.end();itn++){
				if ( (*itn)!=NULL ){
					(*itn)->object = new AuxObject(NON_VISITED); //NOT VISITED
				}
			}
		}

		/**
		 * Cleans the graph after performing a search
		 */
		void clean()
		{
			typename set<NODE*>::iterator itn;
			for (itn=this->graph->nodes.begin();itn!=this->graph->nodes.end();itn++){
				if ( (*itn) != NULL ){
					delete (AuxObject*)((*itn)->object);
				}
			}
		}


};

} //end namespace

#endif
