/*************************************************************************************************
 * class Phase class
 *
 * This code allows the user to declare "phases" in his/her code
 * A phase is a string with a time of occurrence, expressed via  chrono::system_clock()
 * An object of class Phase registers those strings and their times of registration.
 * A summary of the time spent by the code in each phase is returned when the object is
 * destructed. The summary is printed either on std::cout or into a file, if so specified.
 *
 * By Eidon (eidon@tutanota.be), 2016-10-27.
 *
 * Version: 1.0, Thu, Oct 27, 2016  1:45:27 PM
 *
 *************************************************************************************************/


#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>		// std::pair
#include <iomanip>		// std::setprecision
#include <limits>		// std::numeric_limits
#include <vector>
#include <algorithm>	// std::max
#include <queue>
#include <cstring>		// strlen


#include <time.h> 		// clock()
#include <chrono>		// std::chrono::steady_clock::now()
#include "deco.h"



class Phase {
	typedef struct { std::chrono::time_point<std::chrono::system_clock> t; string s; } phase_t;

	private:
		int sp;
		vector<phase_t> v;
		map<string,int> m;
		string outputFile;
		size_t maxstring;
		phase_t ph;

	public:
		Phase() { sp = 0; maxstring = 0; outputFile.clear(); }
		~Phase();
		void Set(string phase) {
			ph.t = std::chrono::system_clock::now();
			ph.s = phase;
			v.push_back(ph);
			m[phase] = sp;
			//if (phase.length() > maxstring) maxstring = phase.length();
			maxstring = std::max(maxstring, phase.length());
			sp++;
		}
		phase_t Get(string phase) {
			return v[ m[ phase ] ];
		}
		phase_t Get(int phase) {
			return v[ phase ];
		}
		int GetIndex(string phase) {
			return m[ phase ];
		}
		std::chrono::time_point<std::chrono::system_clock> GetTime(int i) {
			return v[ i ].t;
		}
		std::chrono::time_point<std::chrono::system_clock> GetTime(string s) {
			return v[ m[ s ] ].t;
		}
		string GetString(int i) {
			return v[ i ].s;
		}
		string SetFilename(string s) {
			if (s != "-")
				outputFile = s;
			else
				outputFile.clear();
			return outputFile;
		}
		int GetMaxLen(void) { return maxstring; }
};


Phase::~Phase() {
	FILE *f;
	Deco deco;
	int mid = -1;
	int mmx = -1;

	std::chrono::duration<double> tall = GetTime(sp-1) - GetTime(0);
	std::chrono::duration<double> tnow;

	f = (outputFile.empty())? stdout : fopen(outputFile.c_str(), "w");
	char format[256];
	sprintf(format,
	"From phase %%%dd (%%%ds) to phase %%%dd (%%%ds), %%s%%10lf%%ss elapsed (%%s%%lf%%%%%%s)\n",
			to_string(sp).length(), maxstring, to_string(sp).length(), maxstring);
	sprintf(format,
	"From phase %%%dd (%%%ds) to phase %%%dd (%%%ds), %%10lfs elapsed (%%lf%%%%)\n",
			to_string(sp).length(), maxstring, to_string(sp).length(), maxstring);

	std::cout << "Format = " << format << std::endl;

	auto mall = std::chrono::duration_cast<std::chrono::milliseconds>(tall).count();
	//std::cout << "Turn around time = " << mall/1000.0 << "s" << std::endl;
	auto mnow = std::chrono::duration_cast<std::chrono::milliseconds>(tnow).count();

	std::cout << deco.Rx();

	for (int i=0; i<sp-1; i++) {
		tnow = GetTime(i+1)-GetTime(i);
		mnow = std::chrono::duration_cast<std::chrono::milliseconds>(tnow).count();
		if (mnow > mmx) { mmx = mnow, mid = i; }
		fprintf(f, format, i, GetString(i).c_str(), i+1, GetString(i+1).c_str()

			//, (f!=stdout)? "" : deco.Lxc(YELLOW,UNDERLINED)
			, (double) mnow/1000.0
			//, (f!=stdout)? "" : deco.Rxc()

			//, (f!=stdout)? "" : deco.Lxc(BLUE,NORMAL)
			, (double) mnow / mall * 100.0
			//, (f!=stdout)? "" : deco.Rxc()

		);
	}

	tnow = GetTime(mid+1)-GetTime(mid);
	mnow = std::chrono::duration_cast<std::chrono::milliseconds>(tnow).count();
	std::cout << "\nThe longest lasting phase was phase " << mid << " (\"" 
			<< GetString(mid) << "\"), which took "
			<< mnow / 1000.0 << "s out of a total of " << mall / 1000.0 << "s, "
			<< "or 1 / " << (double) mall / mnow << "th of the total time." << std::endl;
	if (f != stdout) fclose(f);
}


