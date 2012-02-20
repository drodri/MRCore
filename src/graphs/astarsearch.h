#ifndef __ASTAR_SEARCH_H
#define __ASTAR_SEARCH_H

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "searcher.h"

#include <vector>
#include <set>

namespace mr{

template<typename N , typename E>
class AstarSearcher: public Searcher<N,E>{

	enum {NON_VISITED=0, VISITED, DEATH};

	typedef Node<N,E> NODE;
	typedef Edge<N,E> EDGE;
	typedef Graph<N,E> GRAPH;

	class AuxObject{
		public:
			AuxObject(int s, E costTC, E costTG, NODE* comF=NULL){status=s; costToCome=costTC; minCostToGo=costTG; comingFrom=comF; optimalCost=costToCome+minCostToGo;}
			NODE* comingFrom;
			E costToCome;
			E minCostToGo;
			E optimalCost;
			int status;
	};

	public:
		AstarSearcher(const E& z, const E& i):ZERO_COST(z),INFINITE_COST(i){this->graph=NULL;}
		AstarSearcher(GRAPH* g, const E& z, const E& i):ZERO_COST(z),INFINITE_COST(i){this->graph=g;}
		virtual ~AstarSearcher(){}

		bool findPath(NODE* initNode, NODE* goalNode){
			E cost;
			findPath(initNode,goalNode,cost);
		}
		/**
		 * Finds Path using the Dijkstra Algorithm between init Node and Goal Node
		 */
		bool findPath(NODE* initNode, NODE* goalNode, E& costToCome){
			//clean();
			initialize();
			//cout << "end init" << endl;
			vector<NODE*> searchQueue;
			searchQueue.clear();
			NODE* currentNode;
			NODE* nextNode;

			E minCostToGo = computeMinCostToGo(initNode,goalNode);
			AuxObject* obj= (AuxObject*)initNode->object;
			if (obj==NULL){
				cout << "Error, obj = NULL" << endl;
				return false;
			}
			obj->status=VISITED;
			obj->costToCome=ZERO_COST;
			obj->comingFrom=NULL;
			obj->minCostToGo=minCostToGo;
			obj->optimalCost = obj->costToCome + obj->minCostToGo;
			searchQueue.push_back(initNode);
			
			
			while(!searchQueue.empty()){

				//cout << "Queue size " << searchQueue.size() << endl;
				currentNode=searchQueue.front(); //start searching with the first node -> FIFO
				((AuxObject*)currentNode->object)->status=DEATH; //once we take it we know it is optimal cost to come
				searchQueue.erase(searchQueue.begin()); //and erase it form the searching queue

				if (currentNode==goalNode){
					tracePath(initNode,goalNode);
					AuxObject* obj = (AuxObject*)currentNode->object;
					costToCome = obj->costToCome;
					clean();
					return true; // YEHAA!! goal found
				}

				//else

				if (currentNode==NULL){
					cout << "AstarSearch: CurrentNode = NULL, this should never happen" << endl;
					continue;
				}
				set < pair < NODE* , EDGE* > > outNeig = currentNode->getOutgoing();
				//cout << "outN size " << outNeig.size() << endl;
				typename set< pair < NODE* , EDGE* > >::iterator out_it;
				//cout <<"1" << endl;
				for (out_it=outNeig.begin(); out_it!=outNeig.end(); out_it++){
					nextNode = (*out_it).first;
					EDGE* edg = (*out_it).second;

					//new nodes not yet visited
					AuxObject* obj = (AuxObject*)nextNode->object;
					//cout << "2" << endl;


					if ( (obj != NULL) && (obj->status == NON_VISITED)){
						//cout << "non visited node" << endl;
						obj->status=VISITED; // set as VISITED
						obj->comingFrom=currentNode; // for reconstructing this->path
						obj->costToCome=((AuxObject*)currentNode->object)->costToCome  + edg->getWeight();
						obj->minCostToGo=computeMinCostToGo(nextNode,goalNode);
						obj->optimalCost = obj->costToCome + obj->minCostToGo;
						//cout << "Inserting New Node: " << *nextNode;
						djInsert(searchQueue,nextNode); // we add it, as it was NON_VISITED
					}

					//cout << "3" << endl;

					//nodes already visited but not yet death
					if ( (obj != NULL) && (obj->status == VISITED)){
						//if the cost to come throught this this->path is smaller
						E thisCostToCome = ((AuxObject*)currentNode->object)->costToCome  + edg->getWeight();
						if ( thisCostToCome < obj->costToCome ){
							obj->comingFrom=currentNode; // for reconstructing this->path
							obj->costToCome=thisCostToCome;
							//do not add it to the search queue because it was already there
							//cout << "Changing node pose " << endl;
							djSwap(searchQueue,nextNode); //and short it as it has changed the cost to come
						}
					}
				}
			}

			clean();
			costToCome=INFINITY;
			return false; //OOPS! final Node not found
		}

		virtual E computeMinCostToGo(NODE *currentNode, NODE *targetNode){
			return currentNode->costTo(targetNode);
		}
			

		/**
		 * Initializes all AuxObjects from this->graph in order to perform a search
		 */
		void initialize()
		{
			typename set<NODE*>::iterator itn;
			for (itn=this->graph->nodes.begin();itn!=this->graph->nodes.end();itn++){
				if (*itn != NULL) {
					//cout << "Initialize" << endl;
					(*itn)->object = new AuxObject(NON_VISITED, INFINITE_COST, INFINITE_COST, NULL); //NOT VISITED, costToCome=INIFINITE_COST, coming_from=NULL
				}
			}

			this->graph->computeEdgesWeight();
		}

		/**
		 * Cleans the this->graph after performing a search
		 */
		void clean()
		{
			typename set<NODE*>::iterator itn;
			for (itn=this->graph->nodes.begin();itn!=this->graph->nodes.end();itn++){
				if ( (*itn) != NULL ){
					if ((AuxObject*)((*itn)->object) != NULL){
						delete (AuxObject*)((*itn)->object);
						(*itn)->object=NULL;
					}
				}
			}
		}

	protected:

	private:
		const E ZERO_COST; //!<Zero Cost of the Edge
		const E INFINITE_COST; //!<Infinite Cost of the Edge


		/**
		 * Inserts a Node in the Queue on its proper position
		 */
		void djInsert(vector<NODE*> & queue, NODE* nd){
			
			AuxObject* obj =  (AuxObject*)nd->object; // the node we want to insert

			/* check if it must be placed at the beginning or at the end*/
			if (queue.size()>0){
				//at the beginning
				AuxObject* objinit= (AuxObject*)(queue[0])->object;
				if (obj->optimalCost <= objinit->optimalCost){
					queue.insert(queue.begin(), nd);
					return;
				}
				// at the end
				AuxObject* objend = (AuxObject*)queue.back()->object;
				if (obj->optimalCost >= objend->optimalCost){
					queue.push_back(nd);
					return;
				}
			}else{  //the queue is empty
				queue.push_back(nd);
			}

			/* Insert in the middle*/
			typename vector<NODE*>::iterator it;
			
			for (it=queue.begin(); it!= queue.end(); it++){
				AuxObject* current = (AuxObject*)(*it)->object;
				//if current is smaller, insert there
				if(obj->optimalCost <= current->optimalCost){
					it = queue.insert(it,nd);
					return;
				}
			}
			
			queue.push_back(nd);
			return;
		}

	/**
	* Finds a Node in the queue and returns the iterator to it
	*/
	typename vector<NODE*>::iterator findNode(vector<NODE*> & queue, NODE* nd){	
			unsigned int init = 0;
			unsigned int end = queue.size()-1;
			unsigned int middle = (end + init) / 2;

			AuxObject* obj = (AuxObject*)nd->object;

			while ( nd != queue[middle]){

				AuxObject* middleObj1 = (AuxObject*)queue[middle]->object;
				AuxObject* middleObj2 = (AuxObject*)queue[middle+1]->object;

				//stop condition
				if (middleObj1->optimalCost == obj->optimalCost || middleObj2->optimalCost == obj->optimalCost ) break; //end while
				
				if ( middleObj1->optimalCost > obj->optimalCost ){ // it is before
					end = middle;
					middle = (end + init) / 2;
				} else{ //it is after
					init = middle;
					middle = (end + init) / 2;
				}
			}
			
			/*it must be just before or after */

			int aux=0;
			for (int i=0; i<queue.size(); i++)
			{
				aux = middle + i;
				if ( queue[aux] == nd ){
					return (queue.begin()+aux);
				}

				aux = middle - i;
				if ( queue[aux] == nd ){
					return (queue.begin()+aux);
				}

			}
		}



		/**
		 * Moves the position of a Node in the Queue to its proper position
		 */
		void djSwap(vector<NODE*> & queue, NODE* nd){
			//find and erase it
			queue.erase(findNode(queue,nd));
			//insert it in the new and proper position
			djInsert(queue,nd);
		}

		/**
		 * Traces the this->path from goalNode to initNode.
		 * The search must be computed befofrehand
		 */
		void tracePath(NODE* initNode, NODE* goalNode){
			this->path.clear();
			if(goalNode->object==NULL){
				cout << "Error tracing this->path" << endl;
				return;
			}

			NODE* currNode;

			AuxObject* obj = (AuxObject*)(goalNode->object);
			this->path.push_back(goalNode); //begin with the final node

			while (obj->comingFrom!=initNode){ //until we arrive to the initNode
				currNode=obj->comingFrom; //previous step
				this->path.push_back(currNode); //add it to the this->path
				if(currNode->object==NULL){ //this should not happen
					cout << "Error tracing this->path" << endl;
					return;
				}
				obj=(AuxObject*)currNode->object;
			}

			this->path.push_back(initNode); //finally add the initNode
		}




};

} //end namespace

#endif
