from SingleFactorized import generatecppInputFull
import sys
import re

def generatecppInputFullFile():
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


  c = sumorder[0]

  if c == '-':
          retval = generatecppInputFull(input[0],-1)
  else :
          retval = generatecppInputFull(input[0],1)


  for i in range(1,len(input)):
      c = sumorder[i]
      if c == '-':
          retval.extend(generatecppInputFull(input[i],-1))
      else :
          retval.extend(generatecppInputFull(input[i],1))

  return retval



s = str(sys.argv[1])
s = s[:-4]
f = "temp_{}.txt".format(s)

file = open(f,"w")
for x in generatecppInputFullFile():
    file.write(x)
    file.write('\n')

file.close()
