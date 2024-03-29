#include "IntermediateFactorization.h"
#include <boost/algorithm/string.hpp>
// Install boost as sudo apt-get install libboost-all-dev before using this

typedef vector<vector<vector<string>>> reducedtraverseLog;
typedef vector<reducedtraverseLog> vrtl;

vector<vector<string>> obtainIndlist(const vector<string> &v)
{
  vector<string> vec;
  vector<vector<string>> retval;
  for (auto x : v)
  {
    boost::split(vec, x, boost::is_any_of(" "));
    vec.pop_back();
    retval.push_back(vec);
  }
  return retval;
}

reducedtraverseLog reduceTL(const traverseLog &t)
{
  reducedtraverseLog rtl;
  for (auto x : t)
    rtl.push_back(x.second);
  return rtl;
}

vrtl collateReducedTraverseLogs(const vector<string> &ind)
{
  vrtl retval;
  for (auto x : obtainIndlist(ind))
  {
    if (x.size() >= 3)
      retval.push_back(reduceTL(keepMinimum(traverse(x))));
    else if (x.size() == 2)
    {
      vector<string> vs;
      vs.push_back(condense(x[0], x[1]));
      vector<vector<vector<string>>> vvvs;
      vector<vector<string>> vvs;
      vvs.push_back(x);
      vvs.push_back(vs);
      vvvs.push_back(vvs);
      retval.push_back(vvvs);
    }
    else
    {
      vector<vector<vector<string>>> vvvs;
      vector<vector<string>> vvs;
      vvs.push_back(x);
      vvvs.push_back(vvs);
      retval.push_back(vvvs);
    }
  }
  return retval;
}

vrtl collateReducedTraverseLogsTest(const vector<string> &ind)
{
  vrtl retval;
  for (auto x : obtainIndlist(ind))
  {
    if (x.size() >= 3)
    {
      auto first = keepMinimum(traverse(x))[0];
      traverseLog trvl;
      trvl.push_back(first);
      retval.push_back(reduceTL(trvl));
    }
    else if (x.size() == 2)
    {
      vector<string> vs;
      vs.push_back(condense(x[0], x[1]));
      vector<vector<vector<string>>> vvvs;
      vector<vector<string>> vvs;
      vvs.push_back(x);
      vvs.push_back(vs);
      vvvs.push_back(vvs);
      retval.push_back(vvvs);
    }
    else
    {
      vector<vector<vector<string>>> vvvs;
      vector<vector<string>> vvs;
      vvs.push_back(x);
      vvvs.push_back(vvs);
      retval.push_back(vvvs);
    }
  }
  return retval;
}

vrtl allPossibleContractions(const vrtl &v)
{
  vrtl s = {{}};
  for (const auto &u : v)
  {
    vrtl r;
    for (const auto &x : s)
    {
      for (const auto y : u)
      {
        r.push_back(x);
        r.back().push_back(y);
      }
    }
    s = move(r);
  }

  return s;
}

pair<int, int> missingIndices(const vector<string> &v1, const vector<string> &v2)
{
  vector<int> v;
  for (long unsigned int j = 0; j < v1.size(); j++)
  {
    int i;
    int leng = v2.size() - 1;
    for (i = 0; i < leng; i++)
      if (v1[j] == v2[i])
        break;
    if (i == leng)
      v.push_back(j);
  }

  return make_pair(v[0], v[1]);
}

string explode_string(string &s)
{
  string retval = "";
  for (int i = 0; i < s.size() - 1; ++i)
  {
    retval += s[i];
    retval += ",";
  }
  retval += s[s.size() - 1];
  return retval;
}

long long fcounter = -1; // global counter variable
int cnt = 1;             // global counter variable
int timer_switch;        // global switch variable
// string start_messg = "start_time = datetime.datetime.now()\n";
string writeCode(vector<vector<string>> &vvs, const vector<string> &tens, const string &coeff)
{
  fcounter++;
  if (tens.size() > 1)
  {
    string retval;
    vector<string> temp = tens;
    int start = cnt;
    long unsigned int i;
    if (timer_switch)
      retval += "start_time = datetime.datetime.now()\n";
    else
      retval += "# start_time = datetime.datetime.now()\n";
    for (i = 0; i < vvs.size() - 2; i++)
    {
      auto vvsi = vvs[i], vvsi1 = vvs[i + 1];
      auto m = missingIndices(vvsi, vvsi1);
      auto fir = m.first, sec = m.second;
      auto tscn = to_string(cnt);

      if (temp[sec][0] == 't' || temp[sec][0] == 'r' || temp[sec][0] == 'l')
      {
        // retval += "I" + tscn + " = np.einsum('" + vvsi[fir] + "," + vvsi[sec] + "->" +
        //           vvsi1[vvsi1.size() - 1] + "'," + temp[fir] + "," + temp[sec] + ")\n"; // ORIGINAL
        retval += "@tensor I" + tscn + "[" + explode_string(vvsi1[vvsi1.size() - 1]) + "] = " + temp[fir] + "[" + explode_string(vvsi[fir]) + "] * " + temp[sec] + "[" + explode_string(vvsi[sec]) + "]\n";
      }
      else
      {
        // retval += "I" + tscn + " = np.einsum('" + vvsi[sec] + "," + vvsi[fir] + "->" +
        //           vvsi1[vvsi1.size() - 1] + "'," + temp[sec] + "," + temp[fir] + ")\n";
        retval += "@tensor I" + tscn + "[" + explode_string(vvsi1[vvsi1.size() - 1]) + "] = " + temp[sec] + "[" + explode_string(vvsi[sec]) + "] * " + temp[fir] + "[" + explode_string(vvsi[fir]) + "]\n";
      }

      temp[fir] = "";
      temp[sec] = "";
      temp.push_back("I" + tscn);
      temp.erase(remove(temp.begin(), temp.end(), ""), temp.end());

      cnt++;
    }
    int end = cnt;
    auto vvsi = vvs[i], vvsi1 = vvs[i + 1];
    auto m = missingIndices(vvsi, vvsi1);
    auto fir = m.first, sec = m.second;
    auto tsc = to_string(fcounter);

    if (temp[sec][0] == 't' || temp[sec][0] == 'r' || temp[sec][0] == 'l')
    {
      // retval += "F" + tsc + " = np.einsum('" + vvsi[fir] + "," + vvsi[sec] + "->" +
      //           vvsi1[vvsi1.size() - 1] + "'," + temp[fir] + "," + temp[sec] + ")\n";
      retval += "@tensor F" + tsc + "[" + explode_string(vvsi1[vvsi1.size() - 1]) + "] = " + temp[fir] + "[" + explode_string(vvsi[fir]) + "] * " + temp[sec] + "[" + explode_string(vvsi[sec]) + "]\n";
    }
    else
    {
      // retval += "F" + tsc + " = np.einsum('" + vvsi[sec] + "," + vvsi[fir] + "->" +
      //           vvsi1[vvsi1.size() - 1] + "'," + temp[sec] + "," + temp[fir] + ")\n";
      retval += "@tensor F" + tsc + "[" + explode_string(vvsi1[vvsi1.size() - 1]) + "] = " + temp[sec] + "[" + explode_string(vvsi[sec]) + "] * " + temp[fir] + "[" + explode_string(vvsi[fir]) + "]\n";
    }

    if (coeff != "1.0")
    {
      if (coeff == "-1.0")
        retval += "F" + tsc + " *= (-1)\n";
      else
        retval += "F" + tsc + " /= (" + coeff + ")\n";
    }
    if (fcounter == 0)
      retval += "S = F0\n";
    else
      retval += "S += F" + tsc + "\n";
    if (timer_switch)
    {
      retval += "end_time = datetime.datetime.now(); ";
      string message = "timer(\"The time taken to evaluate F" + tsc + " has been \",";
      retval += (message + "int((end_time - start_time).total_seconds()), \" seconds.\")\n");
    }
    else
    {
      retval += "# end_time = datetime.datetime.now(); ";
      string message = "timer(\"The time taken to evaluate F" + tsc + " has been \",";
      retval += (message + "int((end_time - start_time).total_seconds()), \" seconds.\")\n");
    }
    for (int i = start; i < end; i++)
      retval += "I" + to_string(i) + " = nothing; ";
    retval += "F" + tsc + " = nothing;\n";
    return retval;
  }
  else
  {
    string retval;
    auto tsc = to_string(fcounter);
    if (timer_switch)
      retval += "start_time = datetime.datetime.now()\n";
    else
      retval += "# start_time = datetime.datetime.now()\n";
    // retval += "F" + tsc + " = np.einsum('" + vvs[0][0] + "->" +
    //           vvs[0][0] + "'," + tens[0] + ")\n";
    retval += "@tensor F" + tsc + "[" + explode_string(vvs[0][0]) + "] = " + tens[0] + "[" + explode_string(vvs[0][0]) + "]\n";

    if (coeff != "1.0")
    {
      if (coeff == "-1.0")
        retval += "F" + tsc + " *= (-1)\n";
      else
        retval += "F" + tsc + " /= (" + coeff + ")\n";
    }
    if (fcounter == 0)
      retval += "S = F0\n";
    else
      retval += "S += F" + tsc + "\n";
    if (timer_switch)
    {
      retval += "# end_time = datetime.datetime.now(); ";
      string message = "timer('The time taken to evaluate F" + tsc + " has been ',";
      retval += (message + "int((end_time - start_time).total_seconds()), ' seconds.')\n");
    }
    retval += "F" + tsc + " = nothing;\n";
    return retval;
  }
}
string writeCodeFullFile(const vector<string> &ind, const vector<string> &tens, const vector<string> &coeff, const vector<string> exp, int num = 0)
{
  auto tensors = obtainIndlist(tens);
  auto apc = allPossibleContractions(collateReducedTraverseLogsTest(ind)); // apc is vrtl.
  string retval = "using TensorOperations\ndatetime = Pycall.pyimport(\"datetime\")\nfunction timer(s1,x,s2):\n\tif x >= 5:\n\t\tprintln(s1,x,s2)\nend\n";
  auto x = apc[num];
  long unsigned int xs = x.size();
  for (long unsigned int i = 0; i < xs; i++)
  {
    retval += "# The code for the expression ";
    retval += exp[i];
    retval += " is as follows : \n";
    retval += writeCode(x[i], tensors[i], coeff[i]);
  }
  return retval;
}

int main(int argc, const char **argv)
{
  if (argc != 3)
  {
    cout << "You need to supply exactly 2 arguments, first the name of your text file, then your timer switch. Exiting gracefully...\n";
    exit(10); // 10 is code for segfault caused due to reasons anticipated by me
  }
  string s(argv[1]);
  timer_switch = atoi(argv[2]);
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  ifstream infile(s);
  string line;
  vector<string> ind, tens, coeff, exp;
  while (getline(infile, line))
  {
    vector<string> strs;
    boost::split(strs, line, boost::is_any_of("|"));
    string s0 = strs[0], s1 = strs[1], s2 = strs[2], s3 = strs[3];
    ind.push_back(s0);
    tens.push_back(s1);
    coeff.push_back(s2);
    exp.push_back(s3);
  }

  for (int i = 0; i < 4; i++)
    s.pop_back();
  size_t position = s.find("_");
  s.erase(s.begin(), s.begin() + position + 1);
  s = "IntermediatesEvaluation_" + s + ".jl";

  ofstream fs(s);

  if (!fs)
  {
    cerr << "Cannot open the output file.\n";
    return 1;
  }

  fs << writeCodeFullFile(ind, tens, coeff, exp);
  fs.close();
  return 0;
}
