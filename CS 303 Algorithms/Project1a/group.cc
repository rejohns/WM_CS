#include "group.h"

using namespace std;

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

GroupOfNumbers::~GroupOfNumbers() {
	delete [] _group;
}

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
	for (i = 0; (_group[i] != number) && (i < _total); i++) {}
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
