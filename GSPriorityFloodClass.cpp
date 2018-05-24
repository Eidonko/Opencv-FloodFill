/*************************************************************************************************
 * Priority-Flood Algorithm No.2
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

#ifndef  __GSFloodFill_CLASS__
#define  __GSFloodFill_CLASS__

typedef bool Boolean;

using namespace std;

template <typename T>
class GSFloodFill {
	public:

		GSFloodFill(T** dem, int r, int c);
		~GSFloodFill(void); // throw();
		Boolean Transform(void);


		typedef std::multimap<T, XY_t> PrioQ_t;
		typedef typename PrioQ_t::iterator PrIterator_t;
		int verbose;
		void setVerbose(int v) { verbose = v; }

	private:

		T** dem;
		Boolean **Closed;
		int rows, cols;

		// Let Open be a priority queue
		PrioQ_t Open;

		// Let Pit be a plain queue
		Q_t Pit;

		Boolean isWithin(XY_t xy);
		Boolean PrioPopHighest(PrioQ_t& queue, XY_t& xy);
		Boolean PrioPop(PrioQ_t& queue, XY_t& xy);
		Boolean isClosed(XY_t xy);
		Boolean neighborsOf(XY_t xy, vector<XY_t>& v);
		XY_t miNeighbors(vector<XY_t> neighbors);
		//template <typename T>
		void printHelp(void);
};

template <typename T>
Boolean GSFloodFill<T>::PrioPopHighest(PrioQ_t& queue, XY_t& xy) {
	if (queue.size() == 0) return false;
	PrIterator_t it = queue.end();
	it--;
	xy = it->second;
	queue.erase(it);
	return true;
}

template <typename T>
Boolean GSFloodFill<T>::PrioPop(PrioQ_t& queue, XY_t& xy) {
	if (queue.size() == 0) return false;
	PrIterator_t it = queue.begin();
	xy = it->second;
	queue.erase(it);
	return true;
}

template <typename T>
inline Boolean GSFloodFill<T>::isWithin(XY_t xy) {
	if (xy.first < 0 || xy.first >= rows)	return false;
	if (xy.second < 0 || xy.second >= cols)	return false;
	return true;
}

template <typename T>
inline Boolean GSFloodFill<T>::isClosed(XY_t xy) {
	return Closed[xy.first][xy.second];
}

template <typename T>
Boolean GSFloodFill<T>::neighborsOf(XY_t xy, vector<XY_t>& v) {
	if (!isWithin(xy)) return false;
	
	XY_t tXY;

	v.clear();
	
	
	// X 0 0
	// X . 0
	// X 0 0

	// 0 0 X
	// 0 . X
	// 0 0 X
	for (int i = -1; i<=1; i++) {
		tXY.first = xy.first + i;
		tXY.second = xy.second - 1;
		if (isWithin(tXY)) {
			v.push_back(tXY);
		}
		
		tXY.second = xy.second + 1;
		if (isWithin(tXY)) {
			v.push_back(tXY);
		}
	}

	// 0 X 0
	// 0 . 0
	// 0 X 0
	tXY.first = xy.first - 1;
	tXY.second = xy.second;
	if (isWithin(tXY)) {
		v.push_back(tXY);
	}
	tXY.first = xy.first + 1;
	if (isWithin(tXY)) {
		v.push_back(tXY);
	}

	return true;
}

template <typename T>
XY_t GSFloodFill<T>::miNeighbors(vector<XY_t> neighbors) {
	Real mindem = std::numeric_limits<T>::max();
	XY_t minxy = pair<int,int>(-1,-1);
	
	for (vector<XY_t>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
		if (dem[it->first][it->second] < mindem) {
			mindem = dem[it->first][it->second];
			minxy = *it;
		}
	}
	return minxy;
}

//
// Destructor
//
template <typename T>
GSFloodFill<T>::~GSFloodFill() {
	try {
		for (int i=0; i<rows; i++)
			delete Closed[i];
		delete Closed;
		Open.clear();
		//Pit.clear();
	} catch (std::exception& e) {
		std::cerr << "Exception caught in the destructor of a GSFloodFill object: "
			<< e.what() << std::endl;
	}
}

//
// Constructor
//
template <typename T>
GSFloodFill<T>::GSFloodFill(T** dempar, int r, int c) {
	rows = r, cols = c;
	dem = dempar;
	verbose = 0;
}


//
// Main function (Flood-fill transform)
//
template <typename T>
Boolean GSFloodFill<T>::Transform() {
	int i, j;
	XY_t xy, c;
	PrIterator_t it;
	vector<XY_t> neighbors;

	 /////////////// 
	// Algorithm 2 //
	 ///////////////

	// Let Closed have the same dimensions as DEM
	// Let Closed be initialized to false
	Closed = new Boolean* [rows];
	for (i=0; i<rows; i++) {
		Closed[i] = new Boolean [cols];
		for (j=0; j<cols; j++) {
			Closed[i][j] = false;
		}
	}

	if (rows == 1) { // monodimensional case
		xy = pair<int,int>(0, 0);
		Open.insert(pair<T,XY_t>(dem[0][0], xy));
		Closed[0][0] = true;
		xy = pair<int,int>(0, cols-1);
		Open.insert(pair<T,XY_t>(dem[0][cols-1], xy));
		Closed[0][cols-1] = true;
	} else {		// bidimensional case
		// for all edges of DEM do
		for (j=0; j<cols; j++) {
			xy = pair<int,int>(0, j);
			Open.insert(pair<T,XY_t>(dem[0][j], xy));
			Closed[0][j] = true;
			xy = pair<int,int>(rows-1, j);
			Open.insert(pair<T,XY_t>(dem[rows-1][j], xy));
			Closed[rows-1][j] = true;
		}
		for (i=1; i<rows-1; i++) {
			xy = pair<int,int>(i, 0);
			Open.insert(pair<T,XY_t>(dem[i][0], xy));
			Closed[i][0] = true;
			xy = pair<int,int>(i,cols-1);
			Open.insert(pair<T,XY_t>(dem[i][cols-1], xy));
			Closed[i][cols-1] = true;
		}
	}

	if (verbose) {
		std::cout << "Number of edges: " << Open.size() << std::endl;
		for (it=Open.begin(), i=1; it != Open.end(); it++, i++)
				std::cout << "Edge " << i << ": (" << it->first << ") => (" << it->second.first
					<< ", " << it->second.second << ")\n";
	}



	// while either Open or Pit is not empty do
	while ( ! Open.empty() || ! Pit.empty() ) {
		if ( ! Pit.empty() ) {
			c=Pit.front();
			Pit.pop();
		} else {
			PrioPop(Open,c);
		}

		// An edge was found on either Open or Pit
		if (verbose)
			std::cerr << "Processing cell (" << c.first << ',' << c.second << "), elevation= " << (Real) dem[c.first][c.second]  << std::endl;
		
		if (! isWithin(c)) {
			if (verbose)
				std::cerr << "\tIgnoring the cell, as it is not within the DEM." << std::endl;
			continue;
		}

		// The edge is within the DEM
		if (verbose)
			std::cerr << "\tCell is within the DEM." << std::endl;

		// neighbors lists the neighbors of c
		if (! neighborsOf(c, neighbors)) {
			if (verbose)
				std::cerr << "\tCell has no neighbors." << std::endl;
			continue;
		}
		// Here, neighbors contains the neighbors of c
		if (verbose)
			std::cerr << "\tCell has " << neighbors.size() << " neighbors." << std::endl;
		

		// miNeighbor is the coordinates of the cell with minimal elevation
		XY_t miNeighbor = miNeighbors(neighbors);
		if (verbose)
			std::cerr << "\tNeighbor with minimal elevation is (" << miNeighbor.first << ',' << miNeighbor.second
				 << "), whose elevation is " << dem[miNeighbor.first][miNeighbor.second] << "." << std::endl;

		
		if (verbose)
			std::cerr << "\tFor all neighbors, do:" << std::endl;



		for (vector<XY_t>::iterator nit = neighbors.begin(); nit != neighbors.end(); nit++) {
			XY_t n = *nit;

			if (isClosed(n)) {
				if (verbose)
					std::cerr << "\t\tNeighbor (" << n.first << ',' << n.second << ") has been already "
					<< "encountered -- ignoring it." << std::endl;

				continue;
			}

			if (verbose) {
				std::cerr << "\t\tProcessing neighbor (" << n.first << ',' << n.second << "):" << std::endl;

				std::cerr << "\t\t\tCalculating max(current neighbor, popped cell), i.e. max(dem[" 
					<< n.first << "][" << n.second << "], dem[" << c.first << "][" << c.second << "]) = "
					<< "max(" << dem[n.first][n.second] << ", " << dem[c.first][c.second] << ") = "
					<< std::max(dem[n.first][n.second], dem[c.first][c.second]) << std::endl;

				std::cerr << "\t\t\tElevating cell (" << n.first << ',' << n.second << ") from " << dem[n.first][n.second]
					<< " to " << std::max(dem[n.first][n.second], dem[c.first][c.second]) << std::endl;
			}

			Closed[n.first][n.second] = true;

			if (dem[n.first][n.second] <= dem[c.first][c.second]) {
				dem[n.first][n.second] = dem[c.first][c.second];

				if (verbose)
					std::cerr << "\t\t\tPush cell (" << n.first << ',' << n.second << ") onto queue Pit" << std::endl;

				Pit.push(n);
			} else {
				if (verbose)
					std::cerr << "\t\t\tPush cell (" << n.first << ',' << n.second << ") onto stack Open with priority "
						<< dem[n.first][n.second] << std::endl;

				// Push n onto Open with priority DEM(n)
				Open.insert(pair<Real,XY_t>(dem[nit->first][nit->second], *nit));
			}
		}
	}

	return true;
}
#endif
