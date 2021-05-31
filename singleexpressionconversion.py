def fracDeal (exp):
      exp = exp.strip()
      prefact = 1
      if 'frac' in exp:
          prefact = int(exp[-2])
          exp = exp[6:-4]
      return exp, prefact

def generalDeal (exp):
      prefactor = fracDeal(exp)[1]
      exp = fracDeal(exp)[0]
     
      retval = []
      if exp[-5] == 'P':
            c1 = exp[-3]
            c2 = exp[-2]
            c = '0' 
            newExp = exp.replace(c1,c).replace(c2,c1).replace(c,c2)
            exp = exp.replace('}_{','')
            exp = exp[:-6]
            #retval[exp] = prefactor
            retval.append([exp,prefactor])
            newExp = newExp[:-6]
            newExp = newExp.replace('}_{','')
            #retval[newExp] = prefactor
            retval.append([newExp,prefactor])
            
      else :
            exp = exp.replace('}_{','')
            retval.append([exp,prefactor])
      
      return retval

def chop (string, index):
    return (string[:(index-1)] + string[(index+5):])

def biPermdeal (exp):
     
      indexList = [i for i, c in enumerate(exp) if c == 'P']
      count = len(indexList)
      if count <= 1 :
          return generalDeal(exp)
      else :
          permlist = []
          retval = []
          for i in indexList:
              #print(chop(exp,i), end = '\t')
              permlist.append(chop(exp,i))
          #print(permlist, end = '\t')
          for term in permlist:
              for x in generalDeal(term):
                  retval.append(x)
          return retval
'''
expression1 = ' \\frac{t^{c}_{i} t^{ab}_{kl} v^{kl}_{jc} P(ij)}{2} '
expression2 = 'f^{k}_{c} t^{c}_{i} t^{ab}_{jk} P(ij) '
expression3 = ' \\frac{t^{ab}_{kl} v^{kl}_{ij}}{2} '
expression4 = ' t^{a}_{k} t^{b}_{l} v^{kl}_{ij} '
expression5 = ' t^{c}_{i} t^{ad}_{jk} v^{bk}_{cd} P(ab) P(ij) '

print(biPermdeal(expression1))
print(biPermdeal(expression2))
print(biPermdeal(expression3))
print(biPermdeal(expression4))
print(biPermdeal(expression5))
'''
def generateEinsumString (indlist):
  rev = ''   
  for c in indlist[0]:  
      rev = c + rev 
  indlist[0] = rev
  sep1 = ','
  sep2 = ''
  retval = sep1.join(indlist)
  retval = retval + '->'
  String = sep2.join(indlist)
  for i in String:
    count = 0
    for j in String:
        if i == j:
            count += 1
        if count > 1:
            break
    if count == 1:
        retval = retval + i


  return str(retval)
#print(generateEinsumString(['bi', 'acjk', 'jkbc']))

from collections import Counter

def nameTensors (tens):
  
  counts = {k:v for k,v in Counter(tens).items() if v > 1}
  retval = tens[:]
  l = len(tens)
  for i in reversed(range(l)):
      item = tens[i]
      if item in counts and counts[item]:
          retval[i] += str(counts[item])
          counts[item] -= 1
  
  return retval

def nameTensors (tens, indlist):
    l = len(tens)
    for j in range(0,l):
        h = len(indlist[j])
        if tens[j] == 't' and h == 2:
                tens[j] = 't1'
        elif tens[j] == 't' and h == 4:
                tens[j] = 't2'
    return tens



def numpyStringBasic (exp, prf):
    
    exp = biPermdeal(exp)[0][0]
    exp = exp.replace('}_{','')
    List = exp.split()
    indices = []
    tensors = []
    for individual in List:
        indices.append(individual[individual.find('{') + 1:individual.rfind('}')])
        tensors.append(individual[:individual.find('^')])
    
    retval = """np.einsum('"""
    retval = retval + generateEinsumString(indices) + """',"""
    t = nameTensors(tensors,indices)
    l = len(t)-1
    for i in range(0,l):
          retval = retval + t[i] + ""","""
    retval = retval + t[l] + """)"""
    
    if prf != 1:
      coeff = str(1.0/prf)
      retval = coeff + """*""" + retval
    
    return retval

def numpyString (exp):
  prefix = fracDeal(exp)[1]
 
  l = len(biPermdeal(exp))
  if l >= 2 :
    retval = numpyStringBasic(exp, prefix) + """ + """
    for i in range(1,l):
        retval = retval + numpyStringBasic(biPermdeal(exp)[i][0], prefix) + """ + """
    return retval[:-3]
  else :
    return numpyStringBasic(exp, prefix)

'''
expression1 = ' \\frac{t^{c}_{i} t^{ab}_{kl} v^{kl}_{jc} P(ij)}{2} '
expression2 = 'f^{k}_{c} t^{c}_{i} t^{ab}_{jk} P(ij) '
expression3 = ' \\frac{t^{ab}_{kl} v^{kl}_{ij}}{2} '
expression4 = ' t^{a}_{k} t^{b}_{l} v^{kl}_{ij} '
expression5 = ' t^{c}_{i} t^{ad}_{jk} v^{bk}_{cd} P(ab) P(ij) '

print(numpyString(expression1))

print(numpyString(expression2))

print(numpyString(expression3))

print(numpyString(expression4))

print(numpyString(expression5))
'''
