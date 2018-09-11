#include "group.h"
#include "rng.h"

using namespace std;

//constructors
GroupOfNumbers::GroupOfNumbers(long defaultLimit):
_LIMIT(defaultLimit), _total(0) {
	if (defaultLimit < default_LIMIT) {
		_LIMIT = default_LIMIT;
	}
	try {
		_group = new long[_LIMIT];
	}
	catch(bad_alloc) {
		cerr << "GroupOfNumbers: bad alloc" << endl;
		cerr << "GroupOfNumbers: allocation of size " << _LIMIT << " failed" << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 1 << endl;
		exit(1);
	}
}

GroupOfNumbers::GroupOfNumbers(const GroupOfNumbers& G):
_LIMIT(default_LIMIT), _total(0), _group(NULL){
    *this = G;
}

GroupOfNumbers::GroupOfNumbers(long lower, long upper, long total_requested):
_LIMIT(total_requested), _total(0) {
	if (lower > upper) {
		cerr << "GroupOfNumbers: incorrect values for the arguments lower and upper: (" << lower << ", " << upper << ")" << endl;
		cerr << "GroupOfNumbers: lower is required to be less than or equal to upper" << endl;
		cerr << "GroupOfNumbers: line: " << __LINE__ << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers:: exit: " << 2 << endl;
		exit(2);
	}
	if (total_requested < default_LIMIT) {
		_LIMIT = default_LIMIT;
	}
	else {
		_LIMIT = total_requested;
	}
	try {
		_group = new long[_LIMIT];
	}
	catch(bad_alloc) {
		cerr << "GroupOfNumbers: bad alloc" << endl;
		cerr << "GroupOfNumbers: allocation of size " << _LIMIT << " failed" << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 1 << endl;
		exit(1);
	}
	long i;
	for (i = 0; i < total_requested; i++) {
		add(Equilikely(lower, upper));
	}
}

GroupOfNumbers::GroupOfNumbers(const char* fname, long total_requested):
_LIMIT(total_requested), _total(0) {
	if (total_requested < default_LIMIT) {
		_LIMIT = default_LIMIT;
	}
	else {
		_LIMIT = total_requested;
	}
	try {
		_group = new long[_LIMIT];
	}
	catch(bad_alloc) {
		cerr << "GroupOfNumbers: bad alloc" << endl;
		cerr << "GroupOfNumbers: allocation of size " << _LIMIT << " failed" << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 1 << endl;
		exit(1);
	}
	ifstream myfile;
	myfile.open(fname);
	if (!myfile) {
		cerr << "GroupOfNumbers: " << fname << ": No such file" << endl;
		cerr << "GroupOfNumbers: line: " << __LINE__ << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 3 << endl;
		exit(3);
	}
	string str;
	getline(myfile, str);
	long i = atol(str.c_str());
	if (i < total_requested) {
		cerr << "GroupOfNumbers: Insufficient number of entries in file: " << fname << endl;
		cerr << "GroupOfNumbers: Number requested from file: " << total_requested << endl;
		cerr << "GroupOfNumbers: Number reputedly available: " << i << endl;
		cerr << "GroupOfNumbers: line: " << __LINE__ - 4 << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 4 << endl;
		exit(4);
	}
	long y;
	long count = 0;
	/*------------------comments by grader (-1 pt)
        while(myfile >> y && count < total_requested) {
        --------------------------*/
	while(myfile >> y && count <= total_requested) {
		add(y);
		count++;
		//cout << "y = " << y << "; count = " << count << endl;
	}
	//cout << "count: " << count << endl;
	/*------------------comments by grader (-2 pts)
        if(count<total_requested)
        --------------------------*/
	if (count < i) {
		cerr << "GroupOfNumbers: Insufficient number of entries in file: " << fname << endl;
		cerr << "GroupOfNumbers: Number requested from file: " << total_requested << endl;
		cerr << "GroupOfNumbers: Number reputedly available: " << i << endl;
		cerr << "GroupOfNumbers: Number actually available: " << count << endl;
		cerr << "GroupOfNumbers: line: " << __LINE__ - 5 << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 5 << endl;
		exit(5);
	}
	myfile.close();
}
//end constructors

//destructor
GroupOfNumbers::~GroupOfNumbers() {
	delete [] _group;
}
//end destructor

//accessors
bool GroupOfNumbers::isEmpty() const {
	return(_total == 0);
}

long GroupOfNumbers::total() const {
	return _total;
}

void GroupOfNumbers::displayGroupOfNumbers() const {
	long i;
	cout.setf(ios::right);
	for (i = 0; i < _total; i++) {
		if ((i%10) == 0) {
			cout << '\n' << setw(8) << _group[i];
		}
		else {
			cout << setw(8) << _group[i];
		}
	}
	cout << endl;
}

long GroupOfNumbers::Equilikely(long lower, long upper) {
	return (lower + (long) ((upper - lower + 1) * Random()) );
}

void GroupOfNumbers::writeGroupOfNumbersTo(const char* fname) const {
	ofstream myfile;
	myfile.open(fname);
	long i;
	myfile.setf(ios::right);
	myfile << _total;
	for (i = 0; i < _total; i++) {
		if ((i%10) == 0) {
			myfile << " " << '\n' << _group[i];
		}
		else {
			myfile << " " << _group[i];
		}
	}
	myfile.close();
}
//end accessors

//mutators
bool GroupOfNumbers::add(const long& newNumber) {
	if (_total < _LIMIT) {
		_group[_total] = newNumber;
		_total++;
		return true;
	}
	else {
		return false;
	}
}

bool GroupOfNumbers::remove(const long & number) {
	bool success;
	long i;
	for (i = 0; (i < _total) && (_group[i] != number); i++) {}
	success = (i < _total);
	if (success) {
		_group[i] = _group[--_total];
	}
	return success;
}

GroupOfNumbers& GroupOfNumbers::operator=(const GroupOfNumbers& G) {
	if (this != &G) {
		delete[] _group;
		try{
			_group = new long[G._LIMIT];
		}
		catch (bad_alloc) {
			cerr << "GroupOfNumbers: bad_alloc" << endl;
			cerr << "GroupOfNumbers: allocation of size " << _LIMIT << " failed" << endl;
			cerr << "GroupOfNumbers: line " << (__LINE__ -5) << endl;
			cerr << "GroupOfNumbers: file " << __FILE__ << endl;
			cerr << "GroupOfNumbers: exit: " << 1 << endl;
			exit(1);
		}
		_LIMIT = G._LIMIT;
		long i;
		for (i = 0; i < G._total; i++) {
			_group[i] = G._group[i];
		}
		_total = G._total;
	}
	return *this;
}
//end mutators

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
note: close all files you opened before each exit.

Score: 37/40

@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
