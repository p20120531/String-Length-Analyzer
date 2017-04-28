import sys
import time
import re
from subprocess import call
from os import listdir
from os.path import isdir, isfile, join

# Usage: python script.py [options]
opt = sys.argv[1]

#d1 = ['./SMTLIB/sat/small','./SMTLIB/sat/big','./SMTLIB/unsat/small','./SMTLIB/unsat/big']
#d2 = ['./DG/sat/small','./DG/sat/big','./DG/unsat/small','./DG/unsat/big']
d1 = ['./testing']
d2 = d1
files1 = []
for i in range(len(d1)) :
    files1 += [join(d1[i],f) for f in listdir(d1[i]) if isfile(join(d1[i],f))]
        
files2 = []
for i in range(len(d2)) :
    files2 += [join(d2[i],f) for f in listdir(d2[i]) if isdir(join(d2[i],f))]
#print files

validSet = set(['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'])

if opt == '--analyze' :
    for f in files1:
        call('./smt2dg --analyze %s' %(f),shell=True) 

elif opt == '--buildDG' :
    if len(sys.argv) == 1 :
        for f in files1 :
            call('./smt2dg --buildDG --whole %s' %(f),shell=True)
    else :
        call('./smt2dg --buildDG --single %s' %(sys.argv[2]),shell=True)

elif opt == '--filter' :
    frecord = open('./experiment/filter','w')
    for f in files2 :
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            sp = sp + '/'
            autFile = sp + 'aut'
            isValid = True
            print 'autFile = %s' %(autFile)
            with open(autFile) as infile:
                for line in infile :
                    v = line.split()
                    print 'path=%s , regex=%s' %(sp+v[0],v[1])
                    regex = v[1]
                    if v[1] == '\"/.*/\"' or v[1] == '\"//\"' : continue
                    if v[1][0] == '~' : 
                        assert v[1][1]  == '('
                        assert v[1][-1] == ')'
                        regex = v[1][1:-1]
                    for c in regex :
                        if c not in validSet :
                            isValid = False
                            break
            if isValid == True :
                frecord.write('%s' %(sp))
    frecord.close()

elif opt == '--regex2blif' :
    ePath = './script/regex-blif'
    for f in files2 :
        #dirs2 = f[0:f.rfind('.')] + '/'
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            sp = sp + '/'
            autFile = sp + 'aut'
            print 'autFile = %s' %(autFile)
            with open(autFile) as infile:
                for line in infile :
                    v = line.split()
                    print 'path=%s , regex=%s' %(sp+v[0],v[1])
                    if v[1] == '\"/.*/\"' or v[1] == '\"//\"' : continue
                    call('./%s %s %s.blif' %(ePath,v[1],sp+v[0]),shell=True)

elif opt == '--blif2vmt' :
    epsilonPath = './special_regex/epsilon.vmt'
    sigmaStarPath = './special_regex/sigmastar.vmt'
    sigmaWithoutEpsilonStarPath = './special_regex/sigmawoepsilonstar.vmt'
    for f in files2 :
        #dirs2 = f[0:f.rfind('.')] + '/'
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            sp = sp + '/'
            autFile = sp + 'aut'
            print 'autFile = %s' %(autFile)
            with open(autFile) as infile:
                for line in infile :
                    v = line.split()
                    print 'path=%s , regex=%s' %(sp+v[0],v[1])
                    if v[1] == '\"/.*/\"' :
                        call('cp %s %s.blif' %(sigmaStarPath,sp+v[0]),shell=True)
                    elif v[1] == '\"//\"' :
                        call('cp %s %s.blif' %(epsilonPath,sp+v[0]),shell=True)
                    else :
                        call('./smt2dg --blif2vmt %s.blif %s.vmt' %(sp+v[0],sp+v[0]),shell=True)
                        call('./smt2dg --intersect %s %s.vmt' %(sigmaWithoutEpsilonStarPath,sp+v[0]),shell=True)
elif opt == '--readCmd' :
    for f in files2 :
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            cmdFile = sp + '/cmd'
            call('./smt2dg --readCmd %s' %(cmdFile),shell=True)

elif opt == '--exp--cvc4' :
    ePath = './cvc4-2017-03-20-x86_64-linux-opt'
    record = open('./experiment/cvc4.csv','w')
    for f in files2 :
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            smtFile = sp + '/sink.smt2'
            ts = time.time()
            call('%s %s > out' %(ePath,smtFile),shell=True)
            te = time.time()
            with open('out','r') as out:
                out.seek(-120,2)
                lines = out.readlines()
                v1 = lines[-1].split()
                v2 = lines[-2].split()
                if v1[-1] == 'sat' : sat = 1
                else               : sat = 0
                #record.write('%s,%s\n' %(sat,v2[-1]))
                record.write('%s,%.6f\n' %(sat,te-ts))
    record.close()
elif opt == '--exp--norn' :
    ePath = './norn/norn'
    record = open('./experiment/norn.csv','w')
    for f in files :
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            smtFile = sp + '/sink.smt2'
            ts = time.time()
            call('%s %s > out' %(ePath,smtFile),shell=True)
            te = time.time()
            with open('out','r') as out:
                lines = out.readlines()
                v1 = lines[-1]
                if v1[-1] == 'sat' : sat = 1
                else               : sat = 0
                record.write('%s,%.6f\n' %(sat,te-ts))
    record.close()
elif opt == '--exp--z3' :
    ePath = './z3-master/build/z3'
    record = open('./experiment/z3.csv','w')
    for f in files :
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            smtFile = sp + '/sink.smt2'
            ts = time.time()
            call('%s %s > out' %(ePath,smtFile),shell=True)
            te = time.time()
            with open('out','r') as out :
                lines = out.readlines()
                v1 = lines[0].split()
                v2 = lines[-1].split()
                if v2[-1][:,-1] == 'sat' : sat = 1
                else                     : sat = 0
                #record.write('%s,%s\n' %(sat,v1[0]))
                record.write('%s,%.6f\n' %(sat,te-ts))
    record.close()
elif opt == '--exp--ic3ia' :
    ePath = './ic3ia/build/ic3ia'
    record = open('./experiment/ic3ia.csv','w')
    for f in files :
        d = f + '/'
        subPath = [join(d,idx) for idx in listdir(d) if isdir(join(d,idx))]
        for sp in subPath :
            vmtFile = sp + '/sink.vmt'
            ts = time.time()
            call('%s %s > out' %(ePath,vmtFile),shell=True)
            te = time.time()
            with open('out','r') as out :
                lines = out.readlines()
                v1 = lines[-1].split()
                v2 = lines[-2].split()
                if v2[2] == 'safe' : safe = 1
                else               : safe = 0
                #record.write('%s,%s\n' %(safe,v1[0]))
                record.write('%s,%.6f\n' %(safe,te-ts))
    record.close()

elif opt == 'plot' :
    import matplotlib.pyplot as plt
    import numpy as np
    cvc4 = np.genfromtxt('./experiment/cvc4.csv', delimiter=',')
    norn = np.genfromtxt('./experiment/norn.csv', delimiter=',')
    z3   = np.genfromtxt('./experiment/z3.csv', delimiter=',')
    ic3  = np.genfromtxt('./experiment/ic3ia.csv', delimiter=',')
    
    # Consistency Checking
    assert (len(cvc4[:,1]) == len(norn[:,1]))
    assert (len(z3[:,1])   == len(ic3[:,1]))
    assert (len(cvc4[:,1]) == len(ic3[:,1]))
    for i in range(len(cvc4[:,1])) :
        assert (cvc4[i,1] == norn[i,1])
        assert (cvc4[i,1] == z3[i,1])
        assert (cvc4[i,1] != ic3[i,1])

    cvc4 = cvc4[:,0]
    norn = norn[:,0]
    z3   = z3[:,0]
    ic3  = ic3[:,0]
    np.cumsum(cvc4, out=cvc4)
    np.cumsum(norn,out=norn)
    np.cumsum(z3,out=z3)
    np.cumsum(ic3,out=ic3)
    
    plt.plot(cvc4,'r')
    plt.plot(norn,'g')
    plt.plot(z3,'b')
    plt.plot(ic3,'k')
    plt.ylabel('Cumulative Run Time (s)')
    plt.xlabel('Benchmark Index')
    plt.savefig("./experiment/crt.png",dpi=800)
