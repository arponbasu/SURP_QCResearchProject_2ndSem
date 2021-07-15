from singleexpressionconversion import numpyString
import sys
import re

def numpyStringFullFile():
  """
Numpy command generating function, entire file :
numpyStringFullFile():
Description:-
Takes a file name , splits it into its individual expressions and generates a numpy command from each one of them using numpyString (exp).
Parameters:-
None
What the function returns:-
A string representing a program of numpy commands
Exceptions:-
This function calls upon numpyString to convert the expressions. Hence avoid any expressions which would also give an exception in numpyString.
  """
  s = sys.argv[1]
  input = open(s,'r').read().strip().replace('\n','')

  sumorder = []
  sumorder = [item for item in input if item == '+' or item == '-' ]

  input = re.split('[-+]', input)
  input = [x for x in input if x != '']

  if len(sumorder) + 1 == len(input):
        sumorder += ['+']
  elif len(sumorder) != len(input):
        print("There seems to be some discrepancy vis-a-vis your addition and/or subtraction symbols.")

  retval = ''
  c = sumorder[0]
  initial = input[0]
  initsign = sumorder[0]
  if c == '-':
          summand = """(-1)*(""" + numpyString(input[0]) + """)"""
  else :
          summand = numpyString(input[0])
      
  retval += summand + """\n"""
  for i in range(1,len(input)):
      c = sumorder[i]
      if c == '-':
          summand = """(-1)*(""" + numpyString(input[i]) + """)"""
      else :
          summand = numpyString(input[i])

      retval += "# " + c + input[i] + """\n"""     
      retval += """S{} = """.format(i) + summand + """\n"""
  
  return retval, initial, initsign


s = str(sys.argv[1])
s = s[:-4]
f = "GeneratedCodeFor_{}.py".format(s)
file = open(f,"w") 
n = numpyStringFullFile()
file.write("import numpy as np\n")
file.write("# ")
file.write(n[2])
file.write(" ")
file.write(n[1])
file.write("\n")
file.write("S = ") 
file.write(n[0]) 

file.close()
