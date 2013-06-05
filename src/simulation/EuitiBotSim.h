#pragma once


#include "../world/robotsim.h"




namespace mr
{
		class EuitiBotSim: public RobotSim
		{

				DECLARE_MR_OBJECT(EuitiBotSim)

			public:

				//Serializers
				virtual void writeToStream(Stream& stream);
				virtual void readFromStream(Stream& stream);
				virtual void writeToXML(XMLElement* parent);
				virtual void readFromXML(XMLElement* parent);
				//Constructor
				EuitiBotSim(void);
				~EuitiBotSim(void);

				//Cinematica inversa para EuitiBot
				virtual bool inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf=NULL);

				//Return robot configuration
				virtual bool getConfigurationOf(const vector<double> &_q, unsigned char &_conf);
				bool configuration(unsigned char _conf, double& _s, double& _e, double& _w);
				bool configuration(double _s, double _e, double _w,unsigned char &_conf);

				//Simulate
				void simulate(double delta_t);

				//Aux. methods
				void setFlash();
				bool getCoordinatesOf(vector<double> &_q);// delete?
				bool Pruebas();//delete

			protected:

				//Cinematica inversa para EuitiBot
				//bool EuitiBotinverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf=0x00);
				void goTo(vector<double> _q);


				/***
				Ranges coordiantes:
				joints[0]	->	360º	->[PI/2 ,		-PI/2]
				joints[1]	->	250º	->[25*PI/36 , -25*PI/36]  125º / -125º
				joints[2]	->	250º	->[25*PI/36 , -25*PI/36]
				joints[3]	->	360º	->[PI/2 ,		-PI/2]
				joints[4]	->	250º	->[25*PI/36 , -25*PI/36]
				joints[5]	->	360º	->[PI/2 ,		-PI/2]
				***/


			private:


				unsigned char conf;
				vector<double> q_init;
				vector<double> q_target;

				//cinematic simulation atributes
				vector<double> a0,a1,a2,a3;
				double time;
				double targetTime;



		};

};
