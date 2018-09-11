#include <cstdlib>
#include<iostream>
#include "group.h"

using namespace std;

extern double user_time();
extern double system_time();

int main(int argc, char** argv) {

        /*GroupOfNumbers digits(2);
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

        cout << "" << endl;

        cout << "NOW TESTING PROJECT 2a" << endl;
        cout << "" << endl;

        cout << "TESTING INSERTIONSORT" << endl;

        GroupOfNumbers digits5(0,12,12);
        digits5.displayGroupOfNumbers();
        digits5.insertion_sort();
        digits5.displayGroupOfNumbers();

        cout << "\nTESTING MERGESORT" << endl;

        GroupOfNumbers digits4(0, 9, 10);
        digits4.displayGroupOfNumbers();
        digits4.mergesort();
        digits4.displayGroupOfNumbers();

        cout << "" << endl;

        cout << "TESTING HEAPSORT" << endl;

        GroupOfNumbers digits6(0,12,12);
        digits6.displayGroupOfNumbers();
        digits6.heapsort();
        digits6.displayGroupOfNumbers();

        cout << "" << endl;

        cout << "TESTING QUICKSORT" << endl;

        GroupOfNumbers digits7(0,12,20);
        digits7.displayGroupOfNumbers();
        digits7.quicksort();
        digits7.displayGroupOfNumbers();*/

        /*cout << "NOW TESTING PROJECT 2b" << endl;

        cout << "NOW TESTING QUICK_CHOOSE_FIRST" << endl;

        double start, finish;

        GroupOfNumbers digits8(250000);
        for (int h = 250000; h > 0; h--) {
                digits8.add(1);
        }
        digits8.displayGroupOfNumbers();
        start = user_time() + system_time();
        digits8.quick_choose_last();
        finish = user_time() + system_time();
        digits8.displayGroupOfNumbers();
        double total = finish - start;
        cout << "Time: " << total << endl;

        cout << "NOW TESTING QUICK_CHOOSE_MIDDLE" << endl;

        GroupOfNumbers digits9(0, 100, 250000);
        digits9.displayGroupOfNumbers();
        digits9.quick_choose_middle();
        digits9.displayGroupOfNumbers();

        cout << "NOW TESTING QUICK_CHOOSE_LAST" << endl;

        GroupOfNumbers digits10(0, 100, 250000);
        digits10.displayGroupOfNumbers();
        digits10.quick_choose_last();
        digits10.displayGroupOfNumbers();

	cout << "NOW TESTING PROJECT 3" << endl;

        long b;

        cout << "TESTING SORT_SELECT" << endl;

        GroupOfNumbers digits11(200);
        for (int i = 200; i > 0; i--) {
                digits11.add(i);
        }
        digits11.partial_max_heap_select(200, b);
        cout << b << endl;
        digits11.sort_select(6, b);
        cout << b << endl;
        digits11.partial_sort_select(200, b);
        cout << b << endl;
        digits11.quick_select(200, b);
        cout << b << endl;
        digits11.min_heap_select(200, b);
        cout << b << endl;
        return 0;

	cout << "NOW TESTING PROJECT 4" << endl;

	long c;
	long element = 10000000;
	
	GroupOfNumbers digits12(-10000000,10000000,10000000);
	//for (int i = 250000; i > 0; i--) {
	//	digits12.add(1);
	//}

	double start, finish;

	start = user_time() + system_time();
        digits12.partial_sort_select(element, c);
        finish = user_time() + system_time();
	double total = finish - start;
	cout << "Timex: " << total << endl;
        cout << c << endl;
	cout << "SORT" << endl;
        digits12.sort_select(element, c);
        cout << c << endl;
	cout << "MIN HEAP SELECT" << endl;
        digits12.min_heap_select(element, c);
        cout << c << endl;
	cout << "PARTIAL MAX HEAP:" << endl;
	digits12.partial_max_heap_select(element, c);
        cout << c << endl;
	cout << "QUICK SELECT" << endl;
        digits12.quick_select(element, c);
        cout << c << endl;
	cout << "LINEAR SELECT" << endl;
	digits12.linear_select(element, c);
	cout << c << endl;*/
        
	return 0;
}