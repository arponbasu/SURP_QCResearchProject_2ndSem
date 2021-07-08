def fracDeal (exp):
      exp = exp.strip()
      prefact = 1.0
      if 'frac' in exp:
          prefact = float(exp[-2]) 
          exp = exp[6:-4]
      return exp, prefact

def uniPerm (exp):
      u = fracDeal(exp)
      prefactor = u[1]
      exp = u[0]
     
      retval = []
      if exp[-5] == 'P':
            
            c1 = exp[-3]
            c2 = exp[-2]
            c = '0' 
            newExp = exp.replace(c1,c).replace(c2,c1).replace(c,c2)
            exp = exp.replace('}_{','')
            exp = exp[:-6]
            
            retval.append([exp,prefactor])
            newExp = newExp[:-6]
            newExp = newExp.replace('}_{','')
            
            retval.append([newExp,(-1)*prefactor])
            
      else :
            exp = exp.replace('}_{','')
            retval.append([exp,prefactor])
      
      return retval

def chop (string, index):
    return (string[:(index-1)] + string[(index+5):])

def biPerm (exp):
  indexList = [i for i, c in enumerate(exp) if c == 'P']
  count = len(indexList)
  if count <= 1:
    return uniPerm(exp)
  elif count == 2:
    retval = [item for item in uniPerm(chop(exp,indexList[1]))]
    u = uniPerm(chop(exp,indexList[0]))
    u[0][1] *= (-1)
    u[1][1] *= (-1)
    retval.append(list(u[0]))
    retval.append(list(u[1]))
    return retval

def generateEinsumString (indlist):
  retval = ','.join(indlist)
  retval = retval + '->'
  String = ''.join(indlist)
  rhs = ''
  for i in String:
    count = 0
    for j in String:
        if i == j:
            count += 1
        if count > 1:
            break
    if count == 1:
        rhs = rhs + i
  rhs = ''.join(sorted(rhs))
  retval = retval + rhs

  return str(retval)

def transcribe (ind, mode):
    retval = ''
    for ite in ind:
        if ite < 'g':
            if mode == 'f':
                retval += 'a'
            elif mode == 'v':
                retval += 'v'
        else:
            if mode == 'f':
                retval += 'i'
            elif mode == 'v':
                retval += 'o'
    return retval


def nameTensors (tens, indlist):
    for j in range(len(tens)):
        h = len(indlist[j])
        if tens[j] == 't':
            if h == 2:
               tens[j] = 't1'
            elif h == 4: 
               tens[j] = 't2'
        elif tens[j] == 'r':
            if h == 1:
               tens[j] = 'r1'
            elif h == 3: 
               tens[j] = 'r2'
        elif tens[j] == 'v':
            tens[j] = transcribe(indlist[j],'v')
        elif tens[j] == 'f':
            tens[j] = 'f' + transcribe(indlist[j],'f')

    return tens

def numpyStringBasic (exp, prf):
    exp = uniPerm(exp)[0][0]
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
    for i in range(l):
          retval = retval + t[i] + ""","""
    retval = retval + t[l] + """)"""
    
    if prf != 1 and prf != -1:
      c = str(prf)
      retval = retval + """/(""" + c + """)"""
    elif prf == -1:
      retval = """(-1)*""" + retval

    
    return retval

def numpyString (exp):

  
  prefix = fracDeal(exp)[1]
  l = len(biPerm(exp))
  if l >= 2 :
    initial = biPerm(exp)[0]
    retval = numpyStringBasic(initial[0], initial[1]) + """ + """
    for i in range(1,l):
        b = biPerm(exp)[i]
        retval = retval + numpyStringBasic(b[0], b[1]) + """ + """
    return retval[:-3]
  else :
    return numpyStringBasic(exp, prefix)
