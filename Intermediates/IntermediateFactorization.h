#ifndef __FACTORIZATION_H__
#define __FACTORIZATION_H__

/**
    Generates the cheapest contraction(s) for a given index sequence along with their respective
    costs in the big -O notation.
    @file Factorization.cpp
    @author Arpon Basu
*/


#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> intpairs;
typedef pair<int,vector<vector<string>>> pivvs;
typedef vector<pivvs> traverseLog;


/**
    A bijective hash function from the set of pairs of whole numbers to the set of whole numbers.
    Reference :- https://en.wikipedia.org/wiki/Pairing_function
    @param pair of whole numbers
    @return a whole number
*/
int CantorHash (intpairs &ip1);


/**
    A bijective hash function from the set of whole numbers to the set of a pair of whole numbers.
    Reference :- https://en.wikipedia.org/wiki/Pairing_function
    @param a whole number
    @return pair of whole numbers
*/
intpairs reverseCantorHash (int n);


/**
    For displaying the cost intpairs (pair of integers) as a big-O notation.
    @param pair of whole numbers
    @return void
*/
void displayAsOV (const intpairs &ip);

/**
    A display function for displaying vectors of strings. The function is overloaded
    for 5 different data types below.
    @param a certain data type to be inferred by the compiler based on the function call.
    @return void
*/
void display (const vector<string> &vs);
void display (const vector<vector<string>> &vvs);
void display (const traverseLog &t);
void display (const vector<intpairs> &v);
void display (const vector<vector<intpairs>> &v1);

/**
    For condensing two strings by concatenating, removing all elements common b/w them
    and then sorting it.
    @param two strings
    @return their condensate string
*/
string condense (string &s1, string &s2);

/**
    For converting a vector of integers into a vector of their pairs.
    @param a vector of integers
    @return a vector of pairs of integers
*/
vector<intpairs> makePairs (vector<int> &ind);

/**
    For generating the vector of all pairs of integers from 0 to n - 1.
    @param an integer > 1
    @return a vector of pairs of whole numbers
*/
vector<intpairs> pairsTilln (int &n);

/**
    For generating the cartesian products of all the vectors pairsTilln(n), ..., pairsTilln(2).
    Reference :- https://stackoverflow.com/questions/5279051/how-can-i-create-cartesian-product-of-vector-of-vectors
    @param an integer > 1
    @return a vector of vectors of pairs of whole numbers
*/
vector<vector<intpairs>> cartesianProduct (int n);

/**
    For evaluating the cost of contracting two index strings and then Cantor-hashing
    to reduce it to a unique whole number cost.
    @param 2 strings
    @return whole number
*/
int cost (const string &s1, const string &s2);

/**
    For replacing the two elements in a vector whose indices are dictated by ip
    with their condensate.
    @param a vector of strings and a pair of integers
    @return a vector of strings
*/
vector<string> reduction (vector<string> &vec, intpairs &ip);

/**
    For generating all possible contractions of a given index list and pairing them up
    with their respective costs which are the maximum of all the costs that were
    incurred while contracting them to a single expression.
    @param a vector of strings
    @return a vector<pair<int,vector<vector<string>>>>
*/
traverseLog traverse (const vector<string> &ind);

/**
    For keeping only the cheapest contraction sequences from the traversal of an index
    list by the function defined above.
    @param a vector<pair<int,vector<vector<string>>>>
    @return a vector<pair<int,vector<vector<string>>>>
*/
traverseLog keepMinimum (const traverseLog &t);



#endif
