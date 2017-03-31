import sys
import re
from subprocess import call
from os import listdir
from os.path import isfile, join

# Usage: python script.py [options]
opt = sys.argv[1]

dirs = ['./SMTLIB/sat/small','./SMTLIB/sat/big','./SMTLIB/unsat/small','./SMTLIB/unsat/big']

files = []
for i in range(4) :
    files.append([join(dirs[i],f) for f in listdir(dirs[i]) if isfile(join(dirs[i],f))])
        
if opt == '--buildDG' :
    for f in files :
        call('./smt2dg --buildDG --whole %s' %(f),shell=True)
elif opt == '--regex2blif' :
    ePath = './script/regex-blif'
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            sp = sp + '/'
            autFile = sp + 'aut'
            print 'autFile = %s' %(autFile)
            with open(autFile) as infile:
                for line in infile :
                    v = line.split()
                    print v
                    print 'regex=%s , path=%s' %(v[1],sp+v[0])
                    call('./%s \"/%s/\" %s.blif' %(ePath,v[1],sp+v[0]),shell=True)
elif opt == '--blif2vmt' :
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            sp = sp + '/'
            autFile = sp + 'aut'
            print 'autFile = %s' %(autFile)
            with open(autFile) as infile:
                for line in infile :
                    v = line.split()
                    print v
                    print 'regex=%s , path=%s' %(v[1],sp+v[0])
                    call('./smt2dg --blif2vmt %s.blif %s.vmt' %(sp+v[0],sp+v[0]),shell=True)
elif opt == '--readCmd' :
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            cmdFile = sp + '/cmd'
            call('./smt2dg --readCmd %s' %(cmdFile),shell=True)

elif opt == '--exp--cvc4' :
    ePath = ''
    record = open('./experiment/cvc4.csv','w')
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            smtFile = sp + '/sink.smt2'
            call('%s %s > out' %(ePath,smtFile),shell=True)
            with open('out','r') as out:
                out.seek(-120,2)
                lines = out.readlines()
                v1 = lines[-1].split()
                v2 = lines[-2].split()
                if v1[-1] == 'sat' : sat = 1
                else               : sat = 0
                record.write('%s,%s\n' %(sat,v2[-1]))
    record.close()
elif opt == '--exp--norm' :
    ePath = ''
    record = open('./experiment/norm.csv','w')
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            smtFile = sp + '/sink.smt2'
            call('%s %s > out' %(ePath,smtFile),shell=True)
    record.close()
elif opt == '--exp--z3' :
    ePath = ''
    record = open('./experiment/z3.csv','w')
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            smtFile = sp + '/sink.smt2'
            call('%s %s > out' %(ePath,smtFile),shell=True)
            with open('out','r') as out :
                lines = out.readlines()
                v1 = lines[0].split()
                v2 = lines[-1].split()
                if v2[-1][:,-1] == 'sat' : sat = 1
                else                     : sat = 0
                record.write('%s,%s\n' %(sat,v1[0]))
    record.close()
elif opt == '--exp--ic3ia' :
    ePath = ''
    record = open('./experiment/ic3ia.csv','w')
    for f in files :
        dir3 = f[0:f.rfind('.')] + '/'
        subPath = [join(dir3,idx) for idx in listdir(dir3) if isdir(join(dir3,idx))]
        for sp in subPath :
            vmtFile = sp + '/sink.vmt'
            call('%s %s > out' %(ePath,vmtFile),shell=True)
            with open('out','r') as out :
                lines = out.readlines()
                v1 = lines[-1].split()
                v2 = lines[-2].split()
                if v2[2] == 'safe' : safe = 1
                else               : safe = 0
                record.write('%s,%s\n' %(safe,v1[0]))
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

