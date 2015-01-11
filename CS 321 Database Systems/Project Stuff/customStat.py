#!/usr/bin/python
from math import *
from string import *


opList = ["(", ")", "/", "+", "-", "*", "^"]
operands = []
tempList = []

stringFormula = raw_input("Please enter a formula: ")

opBool = True
strBool = True
count = 0

# Iterate through the inputted formula and extract the operands e.g. "runs",
for char in stringFormula:

    if char in opList:
        
        if count > 0 and opBool == False:
            operand = ''.join(tempList)
            operands.append(operand)
            tempList = []
        opBool = True
        
    elif char not in opList:
        tempList.append(char)
        opBool = False

        
    count += 1

print operands

# Here is where we need to query the database for individual statistics in the 
# operands list.
# grab the values from database with SQL commands
# if it is an int, keep it as is. --> string.isdigit(). or try float(a) except ValueError

# This was just used to test that it works. It will be replaced by SQL
count = 0
for item in operands:
    operands[count] = count
    count += 1

print operands

# This creates ordered list of operators
formulaList = []
for char in stringFormula:
    if char in opList:
        formulaList.append(char)

print formulaList

count = 0
opIndex = 0

# This creates the math expression with actual queried integer values
for char in formulaList:
    if char == ")" and lastChar == "(":
        formulaList.insert(count, str(operands[opIndex]))
        opIndex += 1
        
    lastChar = char
    count += 1
    
print formulaList

formula = ''.join(formulaList)

print formula

# This evaluates the expression
result = eval(formula)

print result

#cur.execute("Select ...
