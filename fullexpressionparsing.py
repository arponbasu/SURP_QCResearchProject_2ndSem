from singleexpressionconversion import numpyString
import sys
import re

def numpyStringFullFile():
  s = sys.argv[1]
  input = open(s,'r').read().strip().replace('\n','')

  sumorder = []
  sumorder = [item for item in input if item == '+' or item == '-' ]

  input = re.split('[-+]', input)
  input = [x for x in input if x != '']

  if len(sumorder) + 1 == len(input):
        sumorder = ['+'] + sumorder
  elif len(sumorder) != len(input):
        print("There seems to be some discrepancy vis-a-vis your addition and/or subtraction symbols.")

  retval = ''
  c = sumorder[0]
  if c == '-':
          summand = """(-1)*(""" + numpyString(input[0]) + """)"""
  else :
          summand = numpyString(input[0])
      
  retval = retval + summand + """\n"""
  for i in range(1,len(input)):
      c = sumorder[i]
      if c == '-':
          summand = """(-1)*(""" + numpyString(input[i]) + """)"""
      else :
          summand = numpyString(input[i])
      
      retval = retval + """S += """ + summand + """\n"""
  
  return retval


s = str(sys.argv[1])
s = s[:-4]
#print(s)
f = "GeneratedCodeFor_{}.py".format(s)
file = open(f,"w") 
 
file.write("import numpy as np\n")
file.write("S = ") 
file.write(numpyStringFullFile()) 

file.close()
