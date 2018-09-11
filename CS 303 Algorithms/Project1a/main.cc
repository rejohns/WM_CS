#include <cstdlib>
#include<iostream>
#include "group.h"

using namespace std;

int main(int argc, char** argv) {
	GroupOfNumbers digits(2);
	cout << "Empty: " << digits.isEmpty() << endl;
	cout << "Total: " << digits.total() << endl;
	digits.add(1);
	digits.displayGroupOfNumbers();
	cout << "Empty: " << digits.isEmpty() << endl;
	cout << "Total: " << digits.total() << endl;
	digits.add(2);
	digits.displayGroupOfNumbers();
	digits.add(3);
	digits.displayGroupOfNumbers();
	digits.add(4);
	digits.displayGroupOfNumbers();
	cout << "Empty: " << digits.isEmpty() << endl;
	cout << "Total: " << digits.total() << endl;
	int y;
	for (y = 4; y > 0; y--) {
		digits.remove(y);    
		cout << "Empty: " << digits.isEmpty() << endl;
		cout << "Total: " << digits.total() << endl;
		digits.displayGroupOfNumbers();
	}
	int x;	
	for (x = 0; x < 12; x++) {
		digits.add(x);    
		cout << "Empty: " << digits.isEmpty() << endl;
		cout << "Total: " << digits.total() << endl;
		digits.displayGroupOfNumbers();
	}
	cerr << "END TEST WITH GROUP 1" << endl;
	GroupOfNumbers digits3(10);
	GroupOfNumbers digits2(12);
	for (x = 0; x < 12; x++) {
		digits2.add(x);    
	}
	digits2.displayGroupOfNumbers();
	for (x = 0; x < 10; x++) {
		digits3.add(x * 2);    
	}
	cout << "Digit 2 total: " << digits2.total() << endl;	
	cout << "Digit 3 total: " << digits3.total() << endl;
	digits3.displayGroupOfNumbers();
	digits3 = digits2;
	cerr << "WE HAVE NOW USED THE EQUALS OPERATOR" << endl;
	digits2.displayGroupOfNumbers();
	digits3.displayGroupOfNumbers();
	
	return 0;
}
