#ifndef __COL_OR_H___
#define __COL_OR_H___

#include <string>
#include <iostream>

using namespace std;

#define BLACK   30
#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define MAGENTA 35
#define CYAN    36
#define WHITE   37

#define NORMAL     0
#define BOLD       1
#define FAINT      2
#define ITALIC     3
#define UNDERLINED 4
#define REVERSED  26
#define FRAMED    51

class Deco {
	private:
		string s;
	public:
		Deco() { s.clear(); }
		string Set(int color, int decoration, int n) {
			return Set(color, decoration, to_string(n));
		}
		string Set(int color, int decoration, string str) {
			s = "\033[" + to_string(decoration) + ";" + to_string(color) + "m" + str + "\033[0m";
			return s;
		}
		const char *Rxc() { return Rx().c_str(); }
		string Rx() {
			s = "\033[m";
			return s;
		}
		string Lx(int color, int decoration) {
			return "\033[" + to_string(decoration) + ";" + to_string(color) + "m";
		}
		const char *Lxc(int c, int d) { return Lx(c,d).c_str(); }
		string Get() { return s; }
		void Get(string& str) { str = s; }
};

#endif
