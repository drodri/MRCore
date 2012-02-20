/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Diego Rodríguez-Losada
 *			Alberto Valero
 *			Paloma de la Puente
 *			Miguel Hernando
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
#ifndef __MRCORE__MRCORE_INCLUDED_H
#define __MRCORE__MRCORE_INCLUDED_H

#pragma once

#include "./hw/hw.h"

#include "./base/base.h"

#include "./data/pointcloud.h"
#include "./data/laserdata.h"
#include "./data/laserdata3d.h"
#include "./data/quadrotordata.h"
#include "./data/image.h"
#include "./data/sonardata.h"

#include "./math/math.h"

#include "./world/world.h"
#include "./world/positionableentity.h"
#include "./world/referencesystem.h"
#include "./world/simplejoint.h"
#include "./world/facesetpart.h"
#include "./world/composedentity.h"
#include "./world/meshpart.h"
#include "./world/prismaticpart.h"
#include "./world/cylindricalpart.h"
#include "./world/spherepart.h"
#include "./world/tcp.h"

#include "./gl/gltools.h"
#include "./gl/glscene.h"

#include "./system/serialport.h"
#include "./system/time.h"

#include "navigation2d/gridmap.h"
#include "navigation2d/labeledgridmap.h"
#include "navigation2d/occupancygridmap255.h"
#include "navigation2d/path2d.h"

#include "graphs/astarsearch.h"
#include "graphs/edge.h"
#include "graphs/node.h"
#include "graphs/uninformedsearch.h"
#include "graphs/dijkstrasearch.h"
#include "graphs/graph.h"
#include "graphs/graphtypes.h"
#include "graphs/searcher.h"

#include "simulation/simulation.h"

#endif
