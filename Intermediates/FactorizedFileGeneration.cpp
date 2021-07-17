#include "IntermediateFactorization.h"
#include <boost/algorithm/string.hpp>
//Install boost as sudo apt-get install libboost-all-dev before using this

typedef vector<vector<vector<string>>> reducedtraverseLog;
typedef vector<reducedtraverseLog> vrtl;


vector<vector<string>> obtainIndlist (const vector<string> &v){
  vector<string> vec;
  vector<vector<string>> retval;
  for(auto x : v){
    boost::split(vec,x,boost::is_any_of(" "));
    vec.pop_back();
    retval.push_back(vec);
  }
  return retval;
}

reducedtraverseLog reduceTL (const traverseLog &t){
  reducedtraverseLog rtl;
  for(auto x : t) rtl.push_back(x.second);
  return rtl;
}


vrtl collateReducedTraverseLogs (const vector<string> &ind){
  vrtl retval;
  for (auto x : obtainIndlist(ind)) {
        if(x.size() >= 3) retval.push_back(reduceTL(keepMinimum(traverse(x))));
        else if (x.size() == 2){
        vector<string> vs;
        vs.push_back(condense(x[0],x[1]));
        vector<vector<vector<string>>> vvvs;
        vector<vector<string>> vvs;
        vvs.push_back(x);
        vvs.push_back(vs);
        vvvs.push_back(vvs);
        retval.push_back(vvvs);
        }
        else{
        vector<vector<vector<string>>> vvvs;
        vector<vector<string>> vvs;
        vvs.push_back(x);
        vvvs.push_back(vvs);
        retval.push_back(vvvs);
        }

    }
  return retval;
}

vrtl collateReducedTraverseLogsTest (const vector<string> &ind){
  vrtl retval;
  for (auto x : obtainIndlist(ind)) {
        if(x.size() >= 3){
        auto first = keepMinimum(traverse(x))[0];
        traverseLog trvl;
        trvl.push_back(first);
        retval.push_back(reduceTL(trvl));
        }
        else if (x.size() == 2){
        vector<string> vs;
        vs.push_back(condense(x[0],x[1]));
        vector<vector<vector<string>>> vvvs;
        vector<vector<string>> vvs;
        vvs.push_back(x);
        vvs.push_back(vs);
        vvvs.push_back(vvs);
        retval.push_back(vvvs);
        }
        else{
        vector<vector<vector<string>>> vvvs;
        vector<vector<string>> vvs;
        vvs.push_back(x);
        vvvs.push_back(vvs);
        retval.push_back(vvvs);
        }

    }
  return retval;
}


vrtl allPossibleContractions (const vrtl &v){
  vrtl s = {{}};
  for (const auto& u : v) {
      vrtl r;
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

pair<int,int> missingIndices (const vector<string> &v1, const vector<string> &v2){
vector<int> v;
for(long unsigned int j = 0; j < v1.size(); j++){
    int i;
    int leng = v2.size() - 1;
    for(i = 0; i < leng; i++) if(v1[j] == v2[i]) break;
    if(i == leng) v.push_back(j);
}

return make_pair(v[0],v[1]);
}

long long fcounter = -1;
int cnt = 1;
string writeCode (vector<vector<string>> &vvs, const vector<string> &tens, const string &coeff){
fcounter++;
if(tens.size() > 1){
    string retval;
    vector<string> temp = tens;

    long unsigned int i;
    for(i = 0; i < vvs.size() - 2; i++){
    auto vvsi = vvs[i], vvsi1 = vvs[i+1];
    auto m = missingIndices(vvsi,vvsi1);
    auto fir = m.first, sec = m.second;
    auto tscn = to_string(cnt);

    retval += "I" + tscn + " = " + "np.einsum('" + vvsi[fir] + "," + vvsi[sec] + "->" +
    vvsi1[vvsi1.size()-1] + "'," + temp[fir] + "," + temp[sec] + ")\n";

    temp[fir] = "";
    temp[sec] = "";
    temp.push_back("I" + tscn);
    temp.erase(remove(temp.begin(), temp.end(), ""), temp.end());


    cnt++;

    }
    auto vvsi = vvs[i], vvsi1 = vvs[i+1];
    auto m = missingIndices(vvsi,vvsi1);
    auto fir = m.first, sec = m.second;
    auto tsc = to_string(fcounter);
    retval += "F" + tsc + " = np.einsum('" + vvsi[fir] + "," + vvsi[sec] + "->" +
    vvsi1[vvsi1.size()-1] + "'," + temp[fir] + "," + temp[sec] + ")\n";
    if(coeff != "1.0"){
      if(coeff == "-1.0") retval += "F" + tsc + " *= (-1)\n";
      else retval += "F" + tsc + " /= (" + coeff + ")\n";
    }

    return retval;
    }
    else{
        string retval;
        auto tsc = to_string(fcounter);
        retval += "F" + tsc + " = np.einsum('" + vvs[0][0] + "->" +
        vvs[0][0] + "'," + tens[0] + ")\n";
        if(coeff != "1.0"){
          if(coeff == "-1.0") retval += "F" + tsc + " *= (-1)\n";
          else retval += "F" + tsc + " /= (" + coeff + ")\n";
        }
        return retval;
    }


}
string writeCodeFullFile (vector<string> &ind, vector<string> &tens, vector<string> &coeff, int num = 0){
auto tensors = obtainIndlist(tens);
auto apc = allPossibleContractions(collateReducedTraverseLogsTest(ind));//apc is vrtl.
string retval = "import numpy as np\n";
auto x = apc[num];
long unsigned int xs = x.size();
for(long unsigned int i = 0; i < xs; i++) retval += writeCode(x[i],tensors[i],coeff[i]);
retval += "S = ";
long unsigned int j;
for(j = 0; j < xs - 1; j++) retval += "F" + to_string(j) + " + ";
retval += "F" + to_string(j);
return retval;
}


int main(int argc, const char** argv) {


  string s(argv[1]);
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  ifstream infile(s);
  string line;
  vector<string> ind, tens, coeff;
  while (getline(infile, line)) {
    vector<string> strs;
    boost::split(strs, line, boost::is_any_of("|"));
    string s0 = strs[0], s1 = strs[1], s2 = strs[2];
    ind.push_back(s0);
    tens.push_back(s1);
    coeff.push_back(s2);
  }

  for(int i = 0; i < 4; i++) s.pop_back();
  size_t position = s.find("_");
  s.erase(s.begin(), s.begin()+position+1);
  s = "code_" + s + ".py";

  ofstream fs(s);

  if(!fs){
        cerr << "Cannot open the output file.\n";
        return 1;
  }

  fs << writeCodeFullFile(ind,tens,coeff);
  fs.close();
  return 0;
}
