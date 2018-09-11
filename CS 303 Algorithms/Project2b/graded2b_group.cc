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
		myfile.close();
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
		myfile.close();
		exit(4);
	}
	long y;
	long count = 0;
	while(myfile >> y && count < total_requested) {
		add(y);
		count++;
	}
	if (count < total_requested) {
		cerr << "GroupOfNumbers: Insufficient number of entries in file: " << fname << endl;
		cerr << "GroupOfNumbers: Number requested from file: " << total_requested << endl;
		cerr << "GroupOfNumbers: Number reputedly available: " << i << endl;
		cerr << "GroupOfNumbers: Number actually available: " << count << endl;
		cerr << "GroupOfNumbers: line: " << __LINE__ - 5 << endl;
		cerr << "GroupOfNumbers: file: " << __FILE__ << endl;
		cerr << "GroupOfNumbers: exit: " << 5 << endl;
		myfile.close();
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

void GroupOfNumbers::insertion_sort(){
	insertion_sort(0, total() - 1);
}

void GroupOfNumbers::heapsort(){
	for (int i = total() / 2; i >= 0; i--)
		percolate_down(i, total());
	for (int j = total() - 1; j > 0; j--){
		long temp = _group[0];
		_group[0] = _group[j];
		_group[j] = temp;
		percolate_down(0, j);
	}	
}

void GroupOfNumbers::mergesort()
{
	if(_total>0)
	{
		try
		{
			long *temp_group = new long[_total];
			mergesort(temp_group, 0, _total - 1);
			delete[] temp_group;
		}
		catch(bad_alloc)
		{
			exit(1);
		}
	}
}

void GroupOfNumbers::quicksort(){
	quicksort(0, total() - 1);
}

void GroupOfNumbers::quick_choose_first() {
	quick_choose_first(0, total() - 1);
}

void GroupOfNumbers::quick_choose_middle(){
	quick_choose_middle(0, total() - 1);
}

void GroupOfNumbers::quick_choose_last(){
	quick_choose_last(0, total() - 1);
}
//end mutators

//protected methods
void GroupOfNumbers::quick_choose_first(long left, long right) {
	if (left + CutOff <= right) {
		long pivot = _group[left];
		long i = left, j = right + 1;
		for(;;) {
			/*----------------comments by grader: (-1)
			i<=right is not an efficient bounder for the index i, when i==right, we will access _group[right+1].
			For the same reson, when h==left, we will access _group[left-1]
			The same problem also lies in quick_choose_last().
			-----------------*/
			while(i <= right && _group[++i] < pivot){}
			while(j >= left && pivot < _group[--j]){}
			if(i < j) {
				swap(_group[i], _group[j]);
			}
			else
				break;
		}

		swap(_group[left], _group[j]);

		quick_choose_first(left, j - 1);
		quick_choose_first(j + 1, right); 
	}
	else
		insertion_sort(left, right);
}

void GroupOfNumbers::quick_choose_middle(long left, long right){
	if (left + CutOff <= right) {
		long mid = (left + right) / 2;
		long pivot = _group[mid];
		/*------------------comments by grader: (-1)
		store pivot to _group[right]:
		swap(_group[mid],_group[right]);
		--------------------*/
		long i = left - 1, j = right;
		for(;;) {
			while(_group[++i] < pivot){}
			while(j > left && pivot < _group[--j]){}
			if(i < j)
				swap(_group[i], _group[j]);
			else
				break;
		}

		swap(_group[i], _group[right]);

		quick_choose_middle(left, i - 1);
		quick_choose_middle(i + 1, right);
	}
	else
		/*------------------comments by grader: (-1)
                insertion_sort(left, right);
                ---------------------*/
		insertion_sort(left, right + 1);
}

void GroupOfNumbers::quick_choose_last(long left, long right){
	if (left + CutOff <= right) {
		long pivot = _group[right];
		/*-----------------comments by grader: (-1)
		start with i=left-1 and j=right;
		-------------------*/
		long i = left, j = right - 1;
		for(;;) {
			while(i <= right && _group[++i] < pivot){}
			while(j >= left && pivot < _group[--j]){}
			if(i < j)
				swap(_group[i], _group[j]);
			else
				break;
		}

		swap(_group[i], _group[right]);

		quick_choose_last(left, i - 1);
		quick_choose_last(i + 1, right);
	}
	else
		/*------------------comments by grader: (-1)
                insertion_sort(left, right);
                ---------------------*/
		insertion_sort(left, right + 1);
}

long GroupOfNumbers::Equilikely(long lower, long upper) {
	return (lower + (long) ((upper - lower + 1) * Random()) );
}

//--for insertion sort
void GroupOfNumbers::insertion_sort(long left, long right){
	for (int i = left; i <= right; i++) {
		long tmp = _group[i];
		int j;
		/*------------------comments by grader: (-1)
                Index j should be in the range of [left, right].
                ---------------------*/
		for (j = i; j > 0 && tmp < _group[j - 1]; j--)
			_group[j] = _group[j - 1];
		_group[j] = tmp;
	}	
}

//--for heapsort
void GroupOfNumbers::percolate_down(long i, long n){
	int child;
	long tmp;

	for(tmp = _group[i]; left_child(i) < n; i = child)
	{
		child = left_child(i);
		if (child != n - 1 && _group[child] < _group[child + 1]) {
			child++;
		}
		if (tmp < _group[child]){
			_group[i] = _group[child];
		}
		else
			break;
	}
	_group[i] = tmp;
}

//--for mergesort
void GroupOfNumbers::mergesort(long *temp_group, long left, long right){
	if(left < right) {
		long center = (left + right)/2;
		mergesort(temp_group, left, center);
		mergesort(temp_group, center + 1, right);
		merge(temp_group, left, center + 1, right);
	}
}

void GroupOfNumbers::merge(long *temp_group, long leftPos, long rightPos, long rightEnd){
	long leftEnd = rightPos - 1;
	long tmpPos = leftPos;
	long numElements = rightEnd - leftPos + 1;
	while(leftPos <= leftEnd && rightPos <= rightEnd)
		if (_group[leftPos] <= _group[rightPos])
			temp_group[tmpPos++] = _group[leftPos++];		
		else
			temp_group[tmpPos++] = _group[rightPos++];

	while(leftPos <= leftEnd)
		temp_group[tmpPos++] = _group[leftPos++];
	
	while(rightPos <= rightEnd)
		temp_group[tmpPos++] = _group[rightPos++];

	for (long i = 0; i < numElements; i++, rightEnd--)
		_group[rightEnd] = temp_group[rightEnd];
}


//--for quicksort
void GroupOfNumbers::quicksort(long left, long right){
	if (left + CutOff <= right) {
		long pivot = median_of_3(left, right);
		long i = left, j = right - 1;
		for(;;) {
			while(_group[++i] < pivot){}
			while(pivot < _group[--j]){}
			if(i < j)
				swap(_group[i], _group[j]);
			else
				break;
		}

		swap(_group[i], _group[right - 1]);

		quicksort(left, i - 1);
		quicksort(i + 1, right);
	}
	else
		insertion_sort(left, right);
}

long GroupOfNumbers::median_of_3(long left, long right){
	int center = (left + right)/2;
	if (_group[center] < _group[left]) {
		swap(_group[left], _group[center]);
	}
	if (_group[right] < _group[left]) {
		swap(_group[left], _group[right]);
	}
	if (_group[right] < _group[center]) {
		swap(_group[center], _group[right]);
	}
	swap(_group[center], _group[right - 1]);
	return _group[right - 1];
}
//end protected methods

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

Score: 54/60

@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
