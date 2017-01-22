import sys
import re
from subprocess import call
from os import listdir
from os.path import isfile, join


numS = set(['0','1','2','3','4','5','6','7','8','9'])
def isUntracked(w) :
    if w[0] == '\"' and w[-1] == '\"':
        return False
    isAllNum = True
    for c in w :
        if c not in numS :
            isAllNum = False
            break
    if isAllNum == True :
        return False
    return True

sup     = ['str.in.re','str.len','str.++','str.replace','str.to.re','re.++','re.union','re.inter','re.*','re.+']
unsup   = ['str.contains','str.prefixof','str.suffixof','str.indexof','str.to.int','str.to.u16','str.to.u32','str.substr','int.to.str','u16.to.str','u32.to.str','str.at','re.opt']
boolop  = ['=','and','or','ite','not']
intop   = ['-','+','>=','<=','>','<','div']
reserve = ['set-logic','QF_S','check-sat','assert','true','false']

supS    = set(sup)
unsupS  = set(unsup)
boolopS = set(boolop)
intopS  = set(intop)
reserveS= set(reserve)

supA    = [0] * len(sup)
unsupA  = [0] * len(unsup)
boolopA = [0] * len(boolop)
intopA  = [0] * len(intop)
reserveA= [0] * len(reserve)

supD    = { sup[i]:i     for i in range(len(sup))    }
unsupD  = { unsup[i]:i   for i in range(len(unsup))  }
boolopD = { boolop[i]:i  for i in range(len(boolop)) }
intopD  = { intop[i]:i   for i in range(len(intop))  }
reserveD= { reserve[i]:i for i in range(len(reserve))}

path  = sys.argv[1]
files = [join(path,f) for f in listdir(path) if isfile(join(path, f))]
#files = ['SMTLIB/sat/small/1001.corecstrs.readable.smt2']
eqClass = {'!=':0, 'or*':0, 'and*':0, '<*':0, '>*':0, '<=*':0, '>=*':0}
untracked = set()
fileWithUntracked = set()
shitHappends = False
check,str_constEQcnt,const_strEQcnt = 0,0,0
for fName in files :
    var  = set()
    svar = set()
    with open(fName) as f:
        for line in f :
            #print 'l =' ,line
            w = re.split('[\s\(\)]',line)
            #print 'w = ',w
            v = []
            for word in w :
                if word != '' :
                    v.append(word)
            #print 'v = ', v
            if v and v[0] == 'declare-fun' :
                var.add(v[1])
                if v[2] == 'String' :
                    svar.add(v[1])
                continue
            for i in range(len(v)) :
                w = v[i]
                if w in supS :
                    supA[supD[w]] = 1
                elif w in unsupS :
                    unsupA[unsupD[w]] = 1
                elif w in boolopS :
                    boolopA[boolopD[w]] = 1
                    if w == 'not':
                        if   v[i+1] == '=':
                            eqClass['!=']   += 1
                            if v[i+2] in svar and v[i+3] in svar :
                                shitHappends = True
                                print 'at file %s' %(fName)
                            elif v[i+2] in svar and v[i+3] == 'str.++':
                                shitHappends = True
                                print 'at file %s' %(fName)
                            elif v[i+3] in svar and v[i+2] == 'str.++':
                                shitHappends = True
                                print 'at file %s' %(fName)
                            elif v[i+2] in svar and v[i+3][0] == '\"' :
                                str_constEQcnt += 1
                            elif v[i+3] in svar and v[i+2][0] == '\"' :
                                #print '=(%s %s at file %s' %(v[i+2],v[i+3],fName)
                                str_constEQcnt += 1
                                const_strEQcnt += 1
                            if v[i+2] in svar or v[i+3] in svar:
                                check += 1
                        elif v[i+1] == 'and':
                            eqClass['or*']  += 1
                        elif v[i+1] == 'or':
                            eqClass['and*'] += 1
                        elif v[i+1] == '<' :
                            eqClass['>=*']  += 1
                        elif v[i+1] == '<=':
                            eqClass['>*']   += 1
                        elif v[i+1] == '>' :
                            eqClass['<=*']  += 1
                        elif v[i+1] == '>=':
                            eqClass['<*']   += 1
                elif w in intopS :
                    intopA[intopD[w]] = 1
                elif w in reserveS:
                    reserveA[reserveD[w]] = 1
                else :
                    if w not in var :
                        if isUntracked(w) == True:
                            print 'untracked=%s at %s' %(w,fName)
                            untracked.add(w)
                            fileWithUntracked.add(fName)

print '%-30s appears? 1:0' %('suppoerted string operator')
for i in range(len(sup)) :
    if supA[i] == 1 :
        print '%-30s     1' %(sup[i])
    else :
        print '%-30s     0' %(sup[i])
print '\n%-30s appears? 1:0' %('unsuppoerted string operator')
for i in range(len(unsup)) :
    if unsupA[i] == 1 :
        print '%-30s     1' %(unsup[i])
    else :
        print '%-30s     0' %(unsup[i])
print '\n%-30s appears? 1:0' %('bool operator')
for i in range(len(boolop)) :
    if boolopA[i] == 1 :
        print '%-30s     1' %(boolop[i])
    else :
        print '%-30s     0' %(boolop[i])
for op in eqClass :
    print '%-30s     %d' %(op,eqClass[op])

print '\n%-30s appears? 1:0' %('int operator')
for i in range(len(intop)) :
    if intopA[i] == 1 :
        print '%-30s     1' %(intop[i])
    else :
        print '%-30s     0' %(intop[i])
print '\n%-30s appears? 1:0' %('other reserve words')
for i in range(len(reserve)) :
    if reserveA[i] == 1 :
        print '%-30s     1' %(reserve[i])
    else :
        print '%-30s     0' %(reserve[i])
if shitHappends == True :
    print '!= VAR_STRING VAR_STRING exist'
else :
    print '!= VAR_STRING VAR_STRING inexist'
print 'const_strEQcnt=%d , str_constEQcnt=%d , check=%d' %(const_strEQcnt,str_constEQcnt,check)
    
print '\nuntracked (variable names, string literals and numbers have been ruled out)'
numS = set(['0','1','2','3','4','5','6','7','8','9'])
for w in list(untracked) :
    print w
for f in list(fileWithUntracked) :
    print 'rm %s' %(f)
    call('rm %s' %(f),shell=True)
