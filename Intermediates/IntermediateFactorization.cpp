#include "IntermediateFactorization.h"
#include <bits/stdc++.h>
using namespace std;

typedef pair<int, int> intpairs;
typedef pair<int,vector<vector<string>>> pivvs;
typedef vector<pivvs> traverseLog;

int CantorHash (intpairs &ip1){
  int m = ip1.first;
  int n = ip1.second;
  int x = (m + n)*(m + n + 1);
  return (n + (x >> 1));
}

intpairs reverseCantorHash (int n){
  int w = floor(0.5*(sqrt(1+(n<<3))-1));
  int t = (w*w + w) >> 1;
  int y = n - t;
  int x = w - y;
  return make_pair(x,y);
}



void displayAsOV (const intpairs &ip){
  int x = ip.first;
  int y = ip.second;
  cout << "O(O^" << x << "V^" << y << ")";
}

void display (const vector<string> &vs){
  cout << "[";
  int l = vs.size() - 1;
  for(int i = 0; i < l; i++) cout << vs[i] << ", ";
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
    for(auto x : v) cout << "(" << x.first << "," << x.second << ") ";
    cout << '\n';
}

void display (const vector<vector<intpairs>> &v1){
    for(auto x : v1) display(x);
}

string condense (string &s1, string &s2){
  string String = s1 + s2;
  string rhs = "";

  for(long unsigned int i = 0; i < String.length(); i++) {
      int count = 0;
      for(long unsigned int j = 0; j < String.length(); j++){
        if (String[i] == String[j]) count += 1;
        if (count > 1) break;
      }
      if (count == 1) rhs += String[i];

   }
  if(rhs.size() == 0){
    cout << "There appears to be some 2 terms (in atleast one of your tensor contractions) which contract to a scalar. Please don't give such inputs. Exiting gracefully...\n";
    exit(10);//10 is code for segfault caused due to reasons anticipated by me
  }
  sort(rhs.begin(), rhs.end());
  return rhs;
}

vector<intpairs> makePairs (vector<int> &ind){
  vector<intpairs> s;
  long unsigned int leng = ind.size();
  for (long unsigned int i = 0; i < leng - 1; i++) {
		for (long unsigned int j = i + 1; j < leng; j++) {
				intpairs x = make_pair(ind[i], ind[j]);
				s.push_back(x);
		}
	}

return s;
}

vector<intpairs> pairsTilln (int &n){
  vector<int> v;
  for(int i = 0; i < n; i++) v.push_back(i);
  return makePairs(v);
}


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


int cost (const string &s1, const string &s2){
  string string_union;
  string temp1 = s1, temp2 = s2;
  sort(temp1.begin(), temp1.end());
  sort(temp2.begin(), temp2.end());
  set_union(temp1.begin(), temp1.end(), temp2.begin(), temp2.end(), back_inserter(string_union));
  intpairs retval = make_pair(0,0);
  for(long unsigned int i = 0; i < string_union.size(); i++){
    if (string_union[i] < 'g') retval.second += 1;
    else retval.first += 1;
  }

  return CantorHash(retval);
}

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

traverseLog traverse (const vector<string> &ind){
    traverseLog t;
    auto c = cartesianProduct(ind.size());
    for(auto x : c){
        vector<vector<string>> vvs;
        vvs.push_back(ind);
        int price = 0;
        auto tempvec = ind;
        for (auto y : x){
            price = max(price,cost(tempvec[y.first],tempvec[y.second]));
            tempvec = reduction(tempvec,y);
            vvs.push_back(tempvec);
        }
        pivvs p = make_pair(price, vvs);
        t.push_back(p);
    }

    return t;
}

traverseLog keepMinimum (const traverseLog &t){
  auto copy = t;
  sort(copy.begin(),copy.end());
  int iter, c = copy.size(), min = copy[0].first;

  for(iter = 0; iter < c; iter++) if(copy[iter].first > min) break;
  for(int i = copy.size() - 1; i >= iter; i--) copy.pop_back();
  return copy;
}
