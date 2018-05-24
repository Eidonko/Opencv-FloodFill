/*************************************************************************************************
 * Priority-Flood Algorithm
 *
 * This code implements the priority-flood flood-fill Algorithm 2 described in article:
 * Barnes, Lehman, Mulla. "Priority-Flood: An Optimal Depression-Filling and Watershed-Labeling
 * Algorithm for Digital Elevation Models". Computers & Geosciences. Vol 62, Jan 2014, pp 117–127,
 * doi: 10.1016/j.cageo.2013.04.024.
 *
 * By Eidon (eidon@tutanota.be), 2016-10-22.
 *
 * Version: 1.0, Sam Okt 22 21:07:24 CEST 2016
 *
 *************************************************************************************************/
#ifndef   __PRIOFLOOD_H__
#define   __PRIOFLOOD_H__

#include <string.h>
#include <iostream>
#include <map>
#include <utility>		// std::pair
#include <iomanip>		// std::setprecision
#include <limits>		// std::numeric_limits
#include <vector>
#include <algorithm>	// std::max
#include <queue>

using namespace std;

typedef double Real;
typedef bool Boolean;
typedef pair<int,int> XY_t;
typedef queue<XY_t> Q_t;

#include "GSPriorityFloodClass.cpp"

#endif /* __PRIOFLOOD_H__ */
