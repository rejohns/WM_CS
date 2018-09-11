//*****************************************************
// Header file group.h for the GroupOfNumbers class.
// Array-based implementation.
// CS 303 Algorithms
// Project 1, Part A
// Fall 2012
//*****************************************************
#ifndef _GROUP_
#define _GROUP_

#include <cstdlib>  // include for the function exit()
#include <iostream> // include for the input/output members
#include <iomanip>  // include for input/output formatting

using namespace std;

class GroupOfNumbers {

 public:
  // constructors
  // ============
  GroupOfNumbers(long defaultLimit = default_LIMIT);
  // The default constructor.  If the user gives no default
  // limit on the maximum possible size of the group of numbers,
  // default_LIMIT is assumed.  But the limit on the total
  // number of entries that can be represented using the
  // GroupOfNumbers class object should be the maximum of
  // default_Limit and whatever limit the user provides.
  // Error handling:
  //  exits with 1 if there is insufficient memory available to
  //  create a GroupOfNumbers object with defaultLimit entries.

  GroupOfNumbers(const GroupOfNumbers & G);
  // The default copy constructor (deep copy).
  // The deep copy should be identical in every respect.
  // Error handling:
  //  exits with 1 if there is insufficient memory available to
  //  create a GroupOfNumbers object identical to G.

  // destructor
  // ==========
  ~GroupOfNumbers();

  // accessors
  // =========
  bool isEmpty() const;
  // Returns true if the group is empty;
  // otherwise, returns false.
  long total() const;
  // Returns the total number of entries in the group.
  void displayGroupOfNumbers() const;
  // Displays the numbers in the group, separated by one blank
  // space, with a carriage return after every 10 numbers.

  // mutators
  // ========
  bool add(const long& newNumber);
  // If possible, adds newNumber to the group---duplicates
  // allowed; returns true if newNumber can be added to
  // the group and false if the addition of newNumber
  // to the group is not possible because the array is full.
  bool remove(const long& Number);
  // If Number is contained in the group, remove one instance of
  // Number from the group; i.e., if the group contains more than
  // one entry with the same value as Number (duplicates are
  // allowed), only one instance is removed. Returns false if
  // Number is not contained in the group; otherwise, returns true.
  GroupOfNumbers & operator=(const GroupOfNumbers & G);
  // The assignment should be identical in every respect.
  // Error handling:
  //  exits with 1 if there is insufficient memory available to
  //  assign an identical copy of G to the attached object.

 private:
  static const long default_LIMIT = 10; // default max size of the array
  // All GroupOfNumbers objects are guaranteed to be able to
  // represent at least default_Limit numbers in the group.

  long  _LIMIT; // signifies the limit on the size of the actual
                // array (which must be >= default_LIMIT)
  long  _total; // signifies the number of entries
                // currently active in the array
  long* _group; // pointer to the array of numbers

}; // end class GroupOfNumbers
#endif
