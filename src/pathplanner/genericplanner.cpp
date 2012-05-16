


#include "genericplanner.h"

bool PathPlanner::setStartAndGoalStates(RobotState *start_, RobotState *goal_)
	{
		delete start;
		delete goal;
		delete path;
		path=0;
		start=goal=0;
		if(start_==0)return false;
		if(goal_==0)return false;
		if(start_->isValid()==false)return false;
		if(goal_->isValid()==false)return false;
		start=start_->clone();
		goal=goal_->clone();
		solved=false;
		return true;
	}
bool  SBPathPlanner::setStartAndGoalStates(RobotState *start_, RobotState *goal_)
	{
		
		if(sampler)sampler->initialize();
		return PathPlanner::setStartAndGoalStates(start_,goal_);
	}
void RobotPath::drawGL()
{
	if(path.size()<2)return;

	glLineWidth(3);
	glColor3f(1,0.2F,0.2F);			
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_STRIP);
	for(unsigned int i=0;i<path.size();i++)
		{
			vector<double> v;
			v=path[i]->getSample();
			if(v.size()<2)break;
			if(v.size()==2)glVertex3f(v[0],v[1],0.01);
			else glVertex3f(v[0],v[1],v[2]+0.01);		
		}

	glEnd();
	glEnable(GL_LIGHTING);
			
	

}

void RobotPath::filterLoops()
{
	if(path.size()<2)return;
	RobotState *init;

	vector<RobotState *> auxPath;
	//primero busco si hay dos estados equivalentes (lazos)
	int i=0;
	while(i<(int)path.size()-1)
	{
		init=path[i];
		for(int j=i+1;j<(int)path.size();j++)
		{
			if(init->isEqual(path[j]))
			{
				path.erase(path.begin()+i,path.begin()+j);
				i--;
				break;
			}
		}
		i++;
	}
}
//filtro a lo bruto
//TODO: ojo, los nodos creados por el filtro, no se
//estan agregando a ningun sitio, por lo que 
//cuando se elimine (no se debe llamar a eraseStates
//porque pueden hay nodos pertenecientes a los grafos
// pero sin embargo no se liberan: memory Leak que corregir
void RobotPath::filter()
{
int i=0;
  while(i<(int)path.size()-2)
  {
	for(int j=path.size()-1;j>i+1;j--)
	{
		//si se puede llegar y en menos nodos
		bool success;
		RobotPath *auxPath=RobotPath::createPath(path[i],path[j],success);
		if(success){
			//lo ha conseguido y es mas corto
			if(auxPath->size()<j-i-1){
				//hago la sustitucion y rompo el for
				path.erase(path.begin()+i+1,path.begin()+j-1);
				for(int k=0;k<auxPath->size();k++)
					path.insert(path.begin()+i+k,(*auxPath)[k]);
				delete auxPath;
				break;
			}
	
		}
		//si success y mas corto se salta la eliminación
		auxPath->eraseStates();
		delete auxPath;
		
	}
	i++;
  }
}
/*

while(i<path.size()-1)
{
	init=path[i];
	double estdist=-1;
	for(int j=i+2;j<path.size();j++)
	{ 
		if(estdist==-1)estdist=init->distanceTo(path[i+1]);
		double val=init->distanceTo(path[j]);
		if(val<estdist)
		{
			//intento una trayectoria directa
			//si tiene exito y tiene menos nodos
			//sutiuyo el tramo y reseteo el for
			//sino continuo normal
			fin=path[j];
			init->placeRobotTowards(fin);
			RobotState *newState=init,*prevState=init;
			double dist, antdist;
			dist=fin->distanceTo(newState);
			auxPath.clear();
			while(fin->moveRobotCloser(RESOLUTION))
			{
				newState=newState->createStateFromCurrentRobotState();
				if(newState){
					prevState=newState;
					newState->placeRobotTowards(fin);
					auxPath.push_back(newState);
					antdist=dist;
					dist=dist=fin->distanceTo(newState);
					if(antdist<dist)break; //condicion de finalización
				}else {
					newState=prevState; 

					break;
				}
			}//intento de movimiento
			if(newState==init){
				auxPath.clear();
			}
			else{
				//se ha conseguido unir:
				if((auxPath.size()<j-i)&&(newState->isEqual(fin)))//y es mas corto
				{
					path.erase(path.begin()+i,path.begin()+j);
					for(int k=0;k<auxPath.size();k++)
						path.insert(path.begin()+i+k,auxPath[k]);
					auxPath.clear();
					i--;
					break;//rompo el for de j
				}
				else{//limpio si no se usa
					for(int k=0;k<auxPath.size();k++)delete auxPath[k];
				}
			}

		}//caso prometedor

		estdist=val;
	}//for j
	i++;
}//while
}
*/

//the initial node and end node are not included. the contained nodes are all created
RobotPath *RobotPath::createPath(RobotState *init, RobotState *end, bool &success)
{
	RobotPath *newPath=new RobotPath;
	init->placeRobotTowards(end);
	
	RobotState *newState=init,*prevState=init;
	double dist, antdist;
	success=false; //solo se pone a true is se logra la solucion
	dist=end->distanceTo(newState);
	while(end->moveRobotCloser(RESOLUTION))
	{
		newState=newState->createStateFromCurrentRobotState();
		if(newState)
		{
			if(newState->isEqual(end))//success!!!
			{
				success=true;
				//no lo agrego y finalizo
				return newPath;
			}
			antdist=dist;
			dist=end->distanceTo(newState);
			//si la distancia no ha disminuido finalizo
			if(antdist<dist)return newPath;
			//si ha mejorado, agrego el punto y me vuelvo a mover
			newState->placeRobotTowards(end);
			newPath->path.push_back(newState);
		}
		else return newPath;
	}
	return newPath;
}
