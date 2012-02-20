/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  -----------anyone
 *
 * MRcore is licenced under the Common Creative License,
 * Attribution-NonCommercial-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/
#include "../data/laserdata.h"
#include "../data/laserdata3d.h"
#include "../data/odometry.h"
#include "../data/pointcloud.h"
#include "../data/image.h"
#include "../simulation/patrolbotsim.h"
#include "../simulation/pioneer3atsim.h"
#include "../simulation/lms200sim.h"
#include "../world/facesetpart.h"
#include "../simulation/lasersensor3dsim.h"
#include "../simulation/nemolasersensor3dsim.h"
#include "../simulation/powercube70sim.h"
#include "../simulation/adeptonesim.h"
#include "../simulation/lms100sim.h"
namespace mr
{
void mrcoreInit()
{
	LaserData laser;
	Odometry odom;
	PointCloud cloud;
	PatrolbotSim patrol;
	LMS200Sim lasersim;
	LMS100Sim lasersim2;
	FaceSetPart part;
	Pioneer3ATSim pi;
	LaserData3D laserdata3d;
	LaserSensor3DSim laser3dsim;
	PowerCube70Sim powercube;
	NemoLaserSensor3DSim nemolaser3d;
	AdeptOneSim adeptOne;
	Image imag;


} //mr
};