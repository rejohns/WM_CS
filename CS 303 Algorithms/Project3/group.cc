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

bool GroupOfNumbers::sort_select(long k, long & kth_smallest) const {
	if (k < 1 || k > _total) {
		return false;
	}
	GroupOfNumbers temp_group(*this);
	temp_group.quicksort();

	kth_smallest = temp_group.select_element(k - 1);
	return true;
}

bool GroupOfNumbers::partial_sort_select(long k, long & kth_smallest) const {
	if(k < 1 || k > _total) {
		return false;
	}
	GroupOfNumbers temp_group(*this);
	temp_group.quicksort(0, k);
	long i, temp1, temp2, c = 0, small = 0;
	for (i = k; i < _total; i++) {
		if (temp_group.select_element(i) < temp_group.select_element(k - 1)) {
			temp1 = temp_group.select_element(i);
			for (int j = 0; j < k + c; j++) {
				if(temp1 < temp_group.select_element(j)) {
					temp2 = temp_group.select_element(j);
					temp_group.set_element(j, temp1);
					temp1 = temp2;
				}
				small = j + 1;
			}
			temp_group.set_element(small, temp1);
		}
		c++;
	}
	kth_smallest = temp_group.select_element(k - 1);
	return true;
}

bool GroupOfNumbers::min_heap_select(long k, long & kth_smallest) const {
	if (k < 1 || k > _total) {
		return false;
	}

	GroupOfNumbers temp_group(*this);
	long currentSize = temp_group.total();

	for (int i = _total / 2; i >= 0; i--) {
		temp_group.percolateDown2(i, currentSize);
	}

	long temp;

	for (int i = 0; i < k; i++) {
		temp = temp_group.select_element(1);
		temp_group.set_element(1, temp_group.select_element(currentSize--));
		temp_group.percolateDown2(1, currentSize);
	}

	kth_smallest = temp;
	return true;
}

bool GroupOfNumbers::partial_max_heap_select(long k, long & kth_smallest) const {
	if (k < 1 || k > _total) {
		return false;
	}
	GroupOfNumbers temp_group(*this);
	int x;
	for (int i = k / 2; i >= 0; i--) {
		temp_group.percolate_down(i, k);
	}
	for (x = k; x < total(); x++) {
		if (temp_group.select_element(x) < temp_group.select_element(0)) {
			long hold = temp_group.select_element(x);
			temp_group.set_element(x, temp_group.select_element(0));
			temp_group.set_element(0, hold);
			temp_group.percolate_down(0, k);
		}
	}
	
	kth_smallest = temp_group.select_element(0);
	return true;
}

bool GroupOfNumbers::quick_select(long k, long & kth_smallest) const {
	if (k < 1 || k > _total) {
		return false;
	}
	GroupOfNumbers temp_group(*this);
	long left = 0, right = temp_group.total() - 1;

	temp_group.quick_select(left, right, k);

	kth_smallest = temp_group.select_element(k - 1);
	return true;
		
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
			while(i < right && _group[++i] < pivot){}
			while(j > left && pivot < _group[--j]){}
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
		swap(_group[mid],_group[right]);
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
		insertion_sort(left, right);
}

void GroupOfNumbers::quick_choose_last(long left, long right){
	if (left + CutOff <= right) {
		long pivot = _group[right];
		long i = left - 1, j = right;
		for(;;) {
			while(i < right && _group[++i] < pivot){}
			while(j > left && pivot < _group[--j]){}
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
		insertion_sort(left, right);
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

//NEW STUFF

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

long GroupOfNumbers::select_element(long k) const {
	long element;
	if (k >= 0 && k < _total) {
		element = _group[k];
		return element;
	}
	return -696969;
}

void GroupOfNumbers::set_element(long pos, long k) {
	if (pos >= 0 && k < _total) {
		_group[pos] = k;
	}
}

void GroupOfNumbers::quick_select(long left, long right, long k) {
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

		if (k <= i) {
			quick_select(left, i - 1, k);
		}
		else if (k > i + 1) {
			quick_select(i + 1, right, k);
		}
	}
	else
		insertion_sort(left, right);
}

void GroupOfNumbers::percolateDown2(long hole, long currentSize) const {
	long child;
	long temp = _group[hole];

	for(; hole * 2 <= currentSize; hole = child) {
		child = hole * 2;
		if (child != currentSize && _group[child + 1] < _group[child]) {
			child++;
		}
		if (_group[child] < temp) {
			_group[hole] = _group[child];
		}
		else
			break;
	}
	_group[hole] = temp;
}
//end protected methods