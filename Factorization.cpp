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
int CantorHash (intpairs &ip1){
  int m = ip1.first;
  int n = ip1.second;
  return (n + (m + n)*(m + n + 1)/2);
}



/**
    A bijective hash function from the set of whole numbers to the set of a pair of whole numbers.
    Reference :- https://en.wikipedia.org/wiki/Pairing_function
    @param a whole number
    @return pair of whole numbers
*/
intpairs reverseCantorHash (int n){
  int w = floor(0.5*(sqrt(8*n+1)-1));
  int t = (w*w + w)/2;
  int y = n - t;
  int x = w - y;
  return make_pair(x,y);
}



/**
    For displaying the cost intpairs (pair of integers) as a big-O notation.
    @param pair of whole numbers
    @return void
*/
void displayAsOV (const intpairs &ip){
  int x = ip.first;
  int y = ip.second;
  cout << "O(O^" << x << "V^" << y << ")";
}

/**
    A display function for displaying vectors of strings. The function is overloaded
    for 5 different data types below.
    @param a certain data type to be inferred by the compiler based on the function call.
    @return void
*/
void display (const vector<string> &vs){
  cout << "[";
  int l = vs.size() - 1;
  for(int i = 0; i < l; i++){
    cout << vs[i] << ", ";
  }
  cout << vs[l] << "]";
}

void display (const vector<vector<string>> &vvs){
  cout << "[";
  int l = vvs.size() - 1;
  for(int i = 0; i < l; i++){
    display(vvs[i]);
    cout << ", ";
  }
  display(vvs[l]);
  cout << "]\n";

}


void display (const traverseLog &t){
  for(auto x : t){
    auto vvs = x.second;
    displayAsOV(reverseCantorHash(x.first));
    cout << " : ";
    display(vvs);
  }
}

void display (const vector<intpairs> &v){
    for(auto x : v){
        cout << "(" << x.first << "," << x.second << ") ";
    }
    cout << '\n';
}

void display (const vector<vector<intpairs>> &v1){
    for(auto x : v1) display(x);
}

/**
    For condensing two strings by concatenating, removing all elements common b/w them
    and then sorting it.
    @param two strings
    @return their condensate string
*/
string condense (string &s1, string &s2){
  string String = s1 + s2;
  string rhs = "";

  for(long unsigned int i = 0; i < String.length(); i++) {
      int count = 0;
      for(long unsigned int j = 0; j < String.length(); j++){
        if (String[i] == String[j]) count += 1;
        if (count > 1) break;
      }
      if (count == 1) rhs = rhs + String[i];

   }
  sort(rhs.begin(), rhs.end());
  return rhs;
}

/**
    For converting a vector of integers into a vector of their pairs.
    @param a vector of integers
    @return a vector of pairs of integers
*/
vector<intpairs> makePairs (vector<int> &ind){
  vector<intpairs> s;
  for (long unsigned int i = 0; i < ind.size() - 1; i++) {
		for (long unsigned int j = i + 1; j < ind.size(); j++) {

				intpairs x = make_pair(ind[i], ind[j]);
				s.push_back(x);

		}
	}

return s;
}

/**
    For generating the vector of all pairs of integers from 0 to n - 1.
    @param an integer > 1
    @return a vector of pairs of whole numbers
*/
vector<intpairs> pairsTilln (int &n){
  vector<int> v;
  for(int i = 0; i < n; i++){
    v.push_back(i);
  }
  return makePairs(v);
}


/**
    For generating the cartesian products of all the vectors pairsTilln(n), ..., pairsTilln(2).
    Reference :- https://stackoverflow.com/questions/5279051/how-can-i-create-cartesian-product-of-vector-of-vectors
    @param an integer > 1
    @return a vector of vectors of pairs of whole numbers
*/
vector<vector<intpairs>> cartesianProduct (int n){

vector<vector<intpairs>> vn;
for(int i = n; i >= 2; i--){
  vn.push_back(pairsTilln(i));
}
vector<vector<intpairs>> s = {{}};
for (const auto& u : vn) {
    vector<vector<intpairs>> r;
    for (const auto& x : s) {
        for (const auto y : u) {
            r.push_back(x);
            r.back().push_back(y);
            }
        }
        s = move(r);
    }


return s;
}


/**
    For evaluating the cost of contracting two index strings and then Cantor-hashing
    to reduce it to a unique whole number cost.
    @param 2 strings
    @return whole number
*/
int cost (const string &s1, const string &s2){
  string string_union;
  set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(string_union));
  intpairs retval = make_pair(0,0);
  for(long unsigned int i = 0; i < string_union.size(); i++){
    if (string_union[i] < 'g') retval.second += 1;
    else retval.first += 1;
  }
  retval.second -= 1;
  return CantorHash(retval);
}

/**
    For replacing the two elements in a vector whose indices are dictated by ip
    with their condensate.
    @param a vector of strings and a pair of integers
    @return a vector of strings
*/
vector<string> reduction (vector<string> &vec, intpairs &ip){
  int f = ip.first;
  int s = ip.second;
  auto s1 = vec[f];
  auto s2 = vec[s];
  vec.push_back(condense(s1,s2));
  vec.erase(remove(vec.begin(), vec.end(), s1), vec.end());
  vec.erase(remove(vec.begin(), vec.end(), s2), vec.end());
  return vec;

}

/**
    For generating all possible contractions of a given index list and pairing them up
    with their respective costs which are the maximum of all the costs that were
    incurred while contracting them to a single expression.
    @param a vector of strings
    @return a vector<pair<int,vector<vector<string>>>>
*/
traverseLog traverse (const vector<string> &ind){
    traverseLog t;
    auto c = cartesianProduct(ind.size());

    //auto x = c[179];
    //display(x);
    for(auto x : c){
        vector<vector<string>> vvs;
        vvs.push_back(ind);
        int price = 0;
        auto tempvec = ind;
        for (auto y : x){
            price = max(price,cost(tempvec[y.first],tempvec[y.second]));
            //cout << price << '\n';
            tempvec = reduction(tempvec,y);
            //display(tempvec);
            //cout << '\n';
            vvs.push_back(tempvec);
        }

        pivvs p = make_pair(price, vvs);
        t.push_back(p);
    }

    return t;
}

/**
    For keeping only the cheapest contraction sequences from the traversal of an index
    list by the function defined above.
    @param a vector<pair<int,vector<vector<string>>>>
    @return a vector<pair<int,vector<vector<string>>>>
*/
traverseLog keepMinimum (const traverseLog &t){
  auto copy = t;
  sort(copy.begin(),copy.end());
  int iter, c = copy.size(), min = copy[0].first;

  for(iter = 0; iter < c; iter++){
    if(copy[iter].first > min) break;
  }

  for(int i = copy.size() - 1; i >= iter; i--) copy.pop_back();
  return copy;
}

/**
    For taking command line inputs and pushing them into a vector
    creating a vector of strings which the input to this entire program.
    @param int argc and char** argv of the main function
    @return a vector of strings
*/
vector<string> processArgument(int argc, const char** argv){
  vector<string> indlist;
  for(int i = 1; i < argc; i++){
    string s(argv[i]);
    indlist.push_back(s);
  }
  return indlist;

}


int main(int argc, const char** argv) {
  //auto start = chrono::high_resolution_clock::now();

  display(keepMinimum(traverse(processArgument(argc,argv))));

  //auto stop = chrono::high_resolution_clock::now();
  //auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
  //cout << duration.count() << endl;


return 0;
}
