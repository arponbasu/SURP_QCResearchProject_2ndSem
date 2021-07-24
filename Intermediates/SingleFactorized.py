def fracDeal (exp):
      exp = exp.strip()
      prefact = 1.0
      if 'frac' in exp:
          prefact = float(exp[-2])
          exp = exp[6:-4]
      return exp, prefact

def chop (string, index):
  return (string[:(index-1)] + string[(index+5):])

def cutOut (string, index):
  return string[(index-1):(index+5)]

def switch (s,i1,i2):
  dummy = '0'
  return s.replace(i1,dummy).replace(i2,i1).replace(dummy,i2)

def uniPerm (exp):
  u = fracDeal(exp)
  prefactor = u[1]
  exp = u[0]
  retval = []
  if exp[-5] == 'P':

            c1 = exp[-3]
            c2 = exp[-2]
            newExp = switch(exp,c1,c2)
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


def biPerm (exp):
  indexList = [i for i, c in enumerate(exp) if c == 'P']
  count = len(indexList)
  if count <= 1:
    return uniPerm(exp)
  elif count == 2:
    retval = [item for item in uniPerm(chop(exp,indexList[1]))]
    u = uniPerm(chop(exp,indexList[1]))
    c = cutOut(exp,indexList[1])
    u[0][0] = switch(u[0][0],c[-3],c[-2])
    u[1][0] = switch(u[1][0],c[-3],c[-2])
    u[0][1] *= (-1)
    u[1][1] *= (-1)
    retval.append(u[0])
    retval.append(u[1])
    return retval

def collateList (List):
    retval = ""
    for x in List:
        retval += (x + " ")
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
        rhs += i
  rhs = ''.join(sorted(rhs))
  retval += rhs

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

def charHash (c):
    if c == 'f':
        return 1
    elif c == 'v':
        return 2
    elif c == 't':
        return 3
    elif c == 'r':
        return 4

def reversecharHash (n):
    if n == 1:
        return 'f'
    elif n == 2:
        return 'v'
    elif n == 3:
        return 't'
    elif n == 4:
        return 'r'



def sortShuffle (tens, indlist):
    temp = [charHash(c) for c in tens]
    lz = list(zip(*sorted(zip(temp, indlist))))
    rt1 = [reversecharHash(n) for n in lz[0]]
    return rt1, lz[1]


def nameTensors (tens, indlist):
    for j in range(len(tens)):
        h = len(indlist[j])
        if tens[j] == 't':
            if h == 2:
               tens[j] = 't1'
            elif h == 4:
               tens[j] = 't2'
            elif h == 6:
               tens[j] = 't3'
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

def generatecppInput (exp,prf):
    indices = []
    tensors = []
    for individual in ((uniPerm(exp)[0][0]).replace('}_{','').split()):
        indices.append(individual[individual.find('{') + 1:individual.rfind('}')])
        tensors.append(individual[:individual.find('^')])
    ss = sortShuffle (tensors, indices)
    tensors = ss[0]
    indices = ss[1]  
    return (collateList(indices) + "|" + collateList(nameTensors(tensors,indices)) + "|" + str(prf))


def generatecppInputFull (exp, sgn):
    prefix = sgn*fracDeal(exp)[1]
    retval = []
    l = len(biPerm(exp))
    if l >= 2 :
      initial = biPerm(exp)[0]
      retval.append(generatecppInput(initial[0], sgn*initial[1]))
      for i in range(1,l):
          b = biPerm(exp)[i]
          retval.append(generatecppInput(b[0], sgn*b[1]))
      return retval
    else :
      return [generatecppInput(exp, prefix)]
