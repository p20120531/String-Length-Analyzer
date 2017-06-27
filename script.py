import sys
import time
import re
import csv
import numpy as np
from subprocess32 import call
from subprocess32 import TimeoutExpired
from os import listdir
from os.path import isdir, isfile, join
'''
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpat
from matplotlib.font_manager import FontProperties
'''
############################## Global Variable ###############################
TO                      = 10
DPI                     = 200
############################## Benchmark Directory ###########################
benchmark_kaluza_dir    = ['benchmark/Kaluza/SMTLIB/sat/small'  ,
                           'benchmark/Kaluza/SMTLIB/sat/big'    ,
                           'benchmark/Kaluza/SMTLIB/unsat/small',
                           'benchmark/Kaluza/SMTLIB/unsat/big'  ]
#dg_kaluza_dir           = ['DG/Kaluza/sat/small']
dg_kaluza_dir           = ['DG/Kaluza/sat/small'  ,'DG/Kaluza/sat/big',
                           'DG/Kaluza/unsat/small','DG/Kaluza/unsat/big']
dg_testing_dir          = ['DG/testing/sat', 'DG/testing/unsat']
dg_pisa_dir             = ['DG/pisa']
dg_appscan_dir          = ['DG/appscan']
benchmark_dir           = {'Kaluza' : dg_kaluza_dir,'testing' : dg_testing_dir,'pisa' : dg_pisa_dir,'appscan' : dg_appscan_dir}
benchmark_set           = (['Kaluza','testing','pisa','appscan'])
############################## Binary Directory ##############################
sla_dir                 = 'bin/sla'
regex2dot_dir           = 'bin/regex2blif/target/regex2blif-0.0.1-SNAPSHOT.jar'
abc70930_dir            = 'bin/abc70930'
cvc4_dir                = 'bin/cvc4-2017-06-27/builds/bin/cvc4'
norn_dir                = 'bin/norn/norn'
z3_dir                  = 'bin/z3/build/z3'
ic3ia_dir               = 'bin/ic3ia/build/ic3ia'

##############################################################################
# [Function Name] init
# [ Description ] create directories for experiments
# [  Arguments  ]
##############################################################################
def init() :
    d1 = ['experiment/Kaluza/result']
    d2 = ['all','strlen','sample']
    for i in d1 :
        for j in d2 :
            call('mkdir -p %s/%s' %(i,j),shell=True)
    d3 = ['DG/Kaluza']
    d4 = ['sat','unsat']
    d5 = ['small','big']
    for i in d3 :
        for j in d4 :
            for k in d5 :
                call('mkdir -p %s/%s/%s' %(i,j,k),shell=True)
    call('mkdir -p experiment/testing/result/all',shell=True)
    call('mkdir -p experiment/appscan/result/all',shell=True)
    call('mkdir -p experiment/pisa/result/all'   ,shell=True)

##############################################################################
# [Function Name] getDGFile
# [ Description ] return dependency graph files of the benchmark
# [  Arguments  ] benchmark = ['Kaluza','testing','pisa','appscan']
##############################################################################
def getDGFile(benchmark) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::getDGFile] invalid benchmark=%s' %(benchmark))
    
    dgFile = []
    dirs   = benchmark_dir[benchmark]
    
    if benchmark == 'Kaluza' :
        for d in dirs :
            subpath = [join(d,sp) for sp in listdir(d) if isdir(join(d,sp))]
            for sp in subpath :
                dgFile += [join(sp,idx) for idx in listdir(sp) if isdir(join(sp,idx))]
        return dgFile
    else :
        for d in dirs :
            dgFile += [join(d,sp) for sp in listdir(d) if isdir(join(d,sp))]
        return dgFile

##############################################################################
# [Function Name] getSplitMap
# [ Description ] return idx list and file list from a id to name map
# [  Arguments  ] mapFileName = name of the id to name map file
##############################################################################
def getSplitMap(mapFileName) :
    idxList,fileList = [],[]
    with open(mapFileName) as f :
        lines = f.read().splitlines()
        for i in range(1,len(lines)) :
            v = lines[i].split(',')
            idxList.append(v[0])
            fileList.append(v[1])
    return idxList,fileList

##############################################################################
# [Function Name] buildDG
# [ Description ] build dependency graph file(s) from the Kaluza benchmark
# [  Arguments  ] benchmark = ['Kaluza','single']
##############################################################################
def buildDG(benchmark,argv=None) :
    exePath = sla_dir
    if benchmark == 'Kaluza' :
        dirs1 = benchmark_kaluza_dir
        dirs2 = dg_kaluza_dir
        for i in range(len(dirs1)) : 
            files = [f for f in listdir(dirs1[i]) if isfile(join(dirs1[i],f))]
            for f in files :
                smtFile = join(dirs1[i],f)
                DGdir = join(dirs2[i],f[0:f.rfind('.')])
                call('%s --buildDG %s %s' %(exePath,smtFile,DGdir),shell=True)
    elif benchmark == 'single' :
        if len(argv) != 2 : 
            sys.exit('[ERROR::buildDG] argv size not match for buildDG single')
        call('%s --buildDG %s %s' %(exePath,argv[0],argv[1]), shell=True)
    else :
        sys.exit('[ERROR::buildDG] invalid benchmark=%s' %(benchmark))

##############################################################################
# [Function Name] buildMap
# [ Description ] build id to name map of files
# [  Arguments  ] benchmark = ['Kaluza','testing','pisa','appscan']
#                 scope = ['all','strlen','sample']
##############################################################################
def buildMap(benchmark,scope) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::buildMap] invalid benchmark=%s' %(benchmark))
    if scope != 'all' and scope != 'strlen' and scope != 'sample': 
        sys.exit('[ERROR::buildMap] invalid scope=%s' %(scope))
    if benchmark == 'Kaluza' :
        mapFile = open(join('experiment/Kaluza',scope),'w')
        mapFile.write('id,name')
        dgFile,dgCnt = getDGFile('Kaluza'),0
        if scope == 'all' :
            for f in dgFile :
                dgCnt += 1
                mapFile.write('\n%d,%s' %(dgCnt,f))
        elif scope == 'strlen' :
            # block 'div' here
            for f in dgFile :
                dgCnt += 1
                predFile = open(join(f,'pred'))
                lines = predFile.read().splitlines()
                predFile.close()
                hasStrlen = False
                for line in lines :
                    if line[-1] != ')' :
                        hasStrlen = True
                if hasStrlen :
                    mapFile.write('\n%d,%s' %(dgCnt,f))
        elif scope == 'sample' :
            sampleSize,lines = 100,[]
            strlenMapFile = open('experiment/Kaluza/strlen')
            lines = strlenMapFile.read().splitlines()
            strlenMapFile.close()
            samples = np.random.choice(len(lines)-1,sampleSize,replace=False)
            samples.sort()
            for i in samples :
                mapFile.write('\n%s' %(lines[i]))
        mapFile.close()
    else :
        if scope != 'all' : sys.exit('[ERROR::buildMap] benchmark=%s invalid scope=%s' %(benchmark,scope))
        mapFile = open('experiment/%s/all' %(benchmark),'w')
        mapFile.write('id,name')
        dgFile,dgCnt = getDGFile(benchmark),0
        for f in dgFile :
            dgCnt += 1
            mapFile.write('\n%d,%s' %(dgCnt,f))
        mapFile.close()

##############################################################################
# [Function Name] regex2dot
# [ Description ] convert regex to dot file
# [  Arguments  ] dgFileList : list of dependency graph files
##############################################################################
def regex2dot(dgFileList) :
    cnt = 0
    for f in dgFileList :
        autFile = open(join(f,'aut'))
        lines = autFile.read().splitlines()
        autFile.close()
        for line in lines :
            name = join( f , line[ 0 : line.find(' ')] )
            if line[ line.find(' ') + 1 ] == '\"' :
                regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            else :
                regex = line[ line.find('~') : line.rfind(')') + 1]
            print regex
            ret = call('java -jar %s -r %s -d %s.dot -o garbage -l FATAL' %(regex2dot_dir,regex,name),stdout=open('debug/r2d.log','w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::regex2dot] fails file=%s' %(name))
        cnt += 1
    print '[INFO::regex2dot] %d cases pass' %(cnt)

##############################################################################
# [Function Name] dot2blif
# [ Description ] convert dot file to blif file
# [  Arguments  ] dgFileList : list of dependency graph files
##############################################################################
def dot2blif(dgFileList) :
    cnt = 0
    for f in dgFileList :
        autFile = open(join(f,'aut'))
        lines = autFile.read().splitlines()
        autFile.close()
        for line in lines :
            name = join( f , line[ 0 : line.find(' ')] )
            if line[ line.find(' ') + 1 ] == '\"' :
                regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            else :
                regex = line[ line.find('~') : line.rfind(')') + 1]
            print regex
            ret1 = call('%s --dot2blif %s.dot %s.blif' %(sla_dir,name,name),stdout=open('debug/d2b.log','w'),shell=True)
            if ret1 != 0 : sys.exit('[ERROR::dot2blif] dot2blif fails file=%s' %(name))
            if regex == '".*"' or regex == '"~\\(".*"\\)"' or regex == '""' or regex == '~\\(""\\)' : continue
            abcCmdFile = open('abc_cmd','w')
            abcCmdFile.write('read %s.blif' %(name))
            abcCmdFile.write('\nespresso')
            abcCmdFile.write('\nwrite %s.blif' %(name))
            abcCmdFile.close()
            ret2 = call('%s -f abc_cmd' %(abc70930_dir),stdout=open('debug/abc.log','w'),shell=True)
            if ret2 != 0 : sys.exit('[ERROR::dot2blif] abc minimization fails file=%s' %(regex,name))
        cnt += 1
    print '[INFO::dot2blif] %d cases pass' %(cnt)

##############################################################################
# [Function Name] blif2vmt
# [ Description ] convert blif file to vmt file
# [  Arguments  ] dgFileList : list of dependency graph files
##############################################################################
def blif2vmt(dgFileList) :
    cnt = 0
    exePath = sla_dir
    for f in dgFileList :
        autFile = open(join(f,'aut'))
        lines = autFile.read().splitlines()
        autFile.close()
        for line in lines :
            name = join( f , line[ 0 : line.find(' ')] )
            if line[ line.find(' ') + 1 ] == '\"' :
                regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            else :
                regex = line[ line.find('~') : line.rfind(')') + 1]
            ret = call('%s --blif2vmt %s.blif %s.vmt' %(exePath,name,name),stdout=open('debug/b2v.log','w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::blif2vmt] blif2vmt fails file=%s' %(name))
        cnt += 1
    print '[INFO::blif2vmt  ] %d cases pass' %(cnt)

##############################################################################
# [Function Name] readCmd
# [ Description ] execute cmd file of the file list
# [  Arguments  ] dgFileList : list of dependency graph files
##############################################################################
def readCmd(dgFileList) :
    cnt = 0
    exePath = sla_dir
    for f in dgFileList :
        cmdFile = join(f,'cmd')
        ret = call('%s --readCmd %s' %(exePath,cmdFile),stdout=open('debug/readcmd.log','w'),shell=True)
        if ret != 0 : sys.exit('[ERROR::readCmd] readCmd fails file=%s' %(cmdFile))
        cnt += 1
    print '[INFO::readCmd   ] %d cases pass' %(cnt)

##############################################################################
# [Function Name] exp
# [ Description ] experiment on different solvers
# [  Arguments  ] benchmark  = ['Kaluza','testing','pisa','appscan']
#                 scope      = ['all','strlen','sample']
#                 dgIdxList  : idx list of dependency graph files
#                 dgFileList : file list of dependency graph files
#                 solverName = ['cvc4','norn','z3','ic3ia']
#                 1 = sat ; 0 = unsat ; x = error ; t = timeout 
##############################################################################
def exp(benchmark,scope,dgIdxList,dgFileList,solverName) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::exp] invalid benchmark=%s' %(benchmark))
    recordName,header = expParam(benchmark,scope,solverName)
    record = open(recordName,'w')
    record.write('%s' %(header))
    for i in range(len(dgIdxList)) :
        print 'solver = %-6s id = %-4s file = %s' %(solverName,dgIdxList[i],dgFileList[i])
        expRecord(solverName,dgIdxList[i],dgFileList[i],record)
    record.close()

def expParam(benchmark,scope,solverName) :
    recordName = 'experiment/%s/result/%s' %(benchmark,scope)
    header = 'id,sat,time'
    recordName += '/%s.csv' %(solverName)
    if solverName == 'ic3ia':
        header ='id,sat,time,step'
    return recordName,header

def expRecord(solverName,idx,dirName,record) :
    f = join(dirName,'sink.smt2')
    if  solverName == 'cvc4' :
        exePath     = cvc4_dir
        try :
            ts = time.time()
            call('%s --strings-exp %s' %(exePath,f),timeout=TO,stdout=open('out','w'),shell=True)
            te = time.time()
        except TimeoutExpired :
            dt  = 0
            sat = 't'
        else :
            dt  = te-ts
            out = open('out')
            lines = out.read().splitlines()
            out.close()
            if   lines[0] == 'sat'   : sat = '1'
            elif lines[0] == 'unsat' : sat = '0'
            else                     : sat = 'x'
        record.write('\n%s,%s,%.6f' %(idx,sat,dt))
    elif solverName == 'norn' :
        exePath     =  norn_dir
        try :
            ts = time.time()
            call('%s %s' %(exePath,f),timeout=TO,stdout=open('out','w'),shell=True)
            te = time.time()
        except TimeoutExpired :
            dt  = 0
            sat = 't'
        else :
            dt  = te-ts
            out = open('out')
            lines = out.read().splitlines()
            out.close()
            if   lines[0] == 'sat'   : sat = '1'
            elif lines[0] == 'unsat' : sat = '0'
            else                     : sat = 'x'
        record.write('\n%s,%s,%.6f' %(idx,sat,dt))
    elif solverName == 'z3'   :
        exePath     =  z3_dir
        try :
            ts = time.time()
            call('%s %s' %(exePath,f),timeout=TO,stdout=open('out','w'),shell=True)
            te = time.time()
        except TimeoutExpired :
            dt  = 0
            sat = 't'
        else :
            dt  = te-ts
            out = open('out')
            lines = out.read().splitlines()
            out.close()
            if   lines[0] == 'sat'   : sat = '1'
            elif lines[0] == 'unsat' : sat = '0'
            else                     : sat = 'x'
        record.write('\n%s,%s,%.6f' %(idx,sat,dt))
    elif solverName == 'ic3ia':
        exePath     =  ic3ia_dir
        f = join(dirName,'sink.vmt')
        try :
            ts = time.time()
            call('%s -w -v 2 %s' %(exePath,f),stdout=open('out','w'),timeout=TO,shell=True)
            te = time.time()
        except TimeoutExpired :
            print 'timeout'
            dt  = 0
            sat = 't'
        else :
            dt  = te-ts
            out = open('out')
            lines = out.read().splitlines()
            out.close()
            print 'file=%s %s' %(dirName,lines[len(lines)-1])
            if   lines[-1] == 'safe'   : sat = '0'
            elif lines[-1] == 'unsafe' : sat = '1'
            else                       : sat = 'x'
        #TODO safe <-> frame , unsafe <-> step
        step = 'x'
        if sat == '1' :
            for i in range(1,len(lines)+1) :
                if lines[-i][0:2] == ';;' :
                    v = lines[-i].split()
                    if v[1] != 'step' :
                        sys.exit('[ERROR::expRecord] \';;\' not followed by \'step\'')
                    else :
                        step = v[2]
                    break
        elif sat == '0' :
            for i in range(1,len(lines)+1) :
                if (lines[-i][0:8] == 'fixpoint') :
                    v = lines[-i].split()
                    step = v[4]
                    break
        record.write('\n%s,%s,%.6f,%s' %(idx,sat,dt,step))
    else :
        sys.exit('[ERROR::expRecord] invalid solver name=%s' %(solverName))

##############################################################################
# [Function Name] ConsistencyChecking
# [ Description ] consistency checking
# [  Arguments  ] benchmark  = ['Kaluza','testing','pisa','appscan']
#                 scope      = ['all','strlen','sample']
#                 solverList : list of solvers
##############################################################################
def ConsistencyChecking(benchmark,scope,solverList) :
    rstr = 'experiment/%s/result/%s' %(benchmark,scope)
    data = []
    for solver in solverList :
        isIc3ia = False
        if solver == 'ic3ia' : isIc3ia = True
        data.append(getData('%s/%s.csv' %(rstr,solver),isIc3ia))

    # Case Number Checking
    logFile = open('%s/log' %(rstr),'w')
    for i in range(1,len(data)) :
        if len(data[0][0]) != len(data[i][0]) :
            logFile.write('number of cases not match\n')
            logFile.close()
            sys.exit("[ERROR::CC] number of cases not match")
    logFile.write('number of cases match\n')
    
    # Error Checking
    errFree = True
    for i in range(len(data)) :
        for j in range(len(data[i][1])) :
            if data[i][1][j] == 'x' :
                errFree = False
                logFile.write('case:%-6s ERROR solver=%s\n' %(data[i][0][j],solverList[i]))
    
    # Timeout Checking
    toFree = True
    for i in range(len(data)) :
        for j in range(len(data[i][1])) :
            if data[i][1][j] == 't' :
                toFree = False
                logFile.write('case:%-6s TO    solver=%s\n' %(data[i][0][j],solverList[i]))
    '''
    if not errFree :
        logFile.close()
        sys.exit('[ERROR::CC] some cases have ERROR')
    '''
    # Consistency Checking
    icset = set()
    allConsistent = True
    for i in range(1,len(data)) :
        for j in range(len(data[i][1])) :
            if data[0][1][j] != data[i][1][j] and data[0][1][j] != 't' and data[i][1][j] != 't' :
                allConsistent = False
                icset.union(data[i][0][j])

    for i in icset :
        logFile.write('case:%-6s inconsistent\n' %(data[i][0][j]))

    if allConsistent :
        logFile.write('all cases consistent')
    else :
        logFile.close()
        sys.exit("[ERROR::CCandPlot] some cases inconsistent")
    
    logFile.close()

##############################################################################
# [Function Name] CCandPlot
# [ Description ] consistency checking and plot
# [  Arguments  ] benchmark = ['Kaluza','testing']
#                 scope     = ['all','strlen','sample']
##############################################################################
def CCandPlot(benchmark,scope) :
    rstr = 'experiment/%s/result/%s' %(benchmark,scope)
    cvc4 = getData('%s/cvc4.csv' %(rstr))
    norn = getData('%s/norn.csv' %(rstr))
    z3   = getData('%s/z3.csv' %(rstr))
    ic3  = getData('%s/ic3ia.csv' %(rstr),True)

    # Consistency Checking
    logFile = open('%s/log' %(rstr),'w')
    if  (len(cvc4[0]) != len(norn[0])) or (len(z3[0]) != len(ic3[0])) or (len(cvc4[0]) != len(ic3[0])) : 
        logFile.write('number of cases not match\n')
        logFile.close()
        sys.exit("[ERROR::CCandPlot] number of cases not match")
    else :
        logFile.write('number of cases match\n')
    errFree,allConsistent = True,True
    for i in range(len(cvc4[1])) :
        if cvc4[1][i] == 'x' or norn[1][i] == 'x' or z3[1][i] == 'x' or ic3[1][i] == 'x' :
            errFree = False
            logFile.write('case:%-6s ERROR\n' %(cvc4[0][i]))
        else :
            if (cvc4[1][i] != norn[1][i]) or (cvc4[1][i] != z3[1][i]) or (cvc4[1][i] == ic3[1][i]) :
                allConsistent = False
                logFile.write('case:%-6s inconsistent\n' %(cvc4[0][i]))
    if not errFree :
        logFile.close()
        sys.exit('[ERROR::CCandPlot] some cases have ERROR')
    if allConsistent :
        logFile.write('all cases consistent')
    else :
        logFile.write('some cases inconsistent')
        logFile.close()
        sys.exit("[ERROR::CCandPlot] some cases inconsistent")
    logFile.close()
    
    # plot time vs step scatter for ic3ia unsafe case
    unsafeStep,unsafeTime = [],[]
    for i in range(len(ic3[3])) :
        if ic3[1][i] == '0' :
            if ic3[3][i] == 'x' :
                sys.exit('[ERROR::CCandPlot] ic3ia : idx=%s unsafe case must have step != \'x\'' %(ic3[0][i]))
            unsafeStep.append(ic3[3][i])
            unsafeTime.append(ic3[2][i])
    print unsafeStep
    print unsafeTime
    '''
    unsafeStep = np.array(unsafeStep).astype(float)
    unsafeTime = np.array(unsafeTime).astype(float)
    print unsafeStep
    print unsafeTime
    coeff = np.polyfit(unsafeStep, unsafeTime, 1)
    yp    = np.zeros(unsafeStep.shape[0])
    prod  = np.ones(unsafeStep.shape[0])
    for i in range(1,len(coeff)+1) :
        yp += coeff[-i] * prod
        prod *= x
    plt.plot(unsafeStep,yp,'g')
    '''
    plt.scatter(unsafeStep,unsafeTime,c='b')
    plt.title('Time vs Step Scatter Plot (# of case = %d)' %(len(unsafeStep)))
    plt.xlabel('Step Count')
    plt.ylabel('Run Time (s)')
    plt.savefig('%s/step_ic3ia_unsafe.jpg' %(rstr),dpi=DPI)
    plt.cla()
    plt.clf()
    
    # cvc4 , norn , z3 , ic3
    #ALL   = [cvc4[2],norn[2],z3[2],ic3[2]]
    ALL   = [ [] for x in range(4)]
    SAT   = [ [] for x in range(4)]
    UNSAT = [ [] for x in range(4)]
    for i in range(len(cvc4[0])) :
        if ic3[1][i] == 'x' : continue
        ALL[0].append(cvc4[2][i])
        ALL[1].append(norn[2][i])
        ALL[2].append(z3[2][i])
        ALL[3].append(ic3[2][i])
        if cvc4[1][i] == '1' : SAT[0].append(cvc4[2][i])
        else                 : UNSAT[0].append(cvc4[2][i])
        if norn[1][i] == '1' : SAT[1].append(norn[2][i])
        else                 : UNSAT[1].append(norn[2][i])
        if z3[1][i] == '1'   : SAT[2].append(z3[2][i])
        else                 : UNSAT[2].append(z3[2][i])
        if ic3[1][i] == '1'  : UNSAT[3].append(ic3[2][i])
        else                 : SAT[3].append(ic3[2][i])
    
    plotCumTime(rstr,ALL,'all')
    plotCumTime(rstr,SAT,'sat')
    plotCumTime(rstr,UNSAT,'unsat')

def plotCumTime(rstr,data,mode) :
    # plot cumulative time vs case index
    name = ''
    if   mode == 'all'  : name = 'crt_all'
    elif mode == 'sat'  : name = 'crt_sat'
    elif mode == 'unsat': name = 'crt_unsat'
    cvc4 = np.array(data[0],dtype=float)
    norn = np.array(data[1],dtype=float)
    z3   = np.array(data[2],dtype=float)
    ic3  = np.array(data[3],dtype=float)
    np.cumsum(cvc4, out=cvc4)
    np.cumsum(norn,out=norn)
    np.cumsum(z3,out=z3)
    np.cumsum(ic3,out=ic3)

    fig = plt.figure()
    ax  = plt.subplot(111)
    ax.plot(cvc4,'r',label='cvc4')
    ax.plot(norn,'g',label='norn')
    ax.plot(z3,'b',label='z3')
    ax.plot(ic3,'k',label='ic3ia')
    box = ax.get_position()
    ax.set_position([box.x0,box.y0,box.width*0.8,box.height])
    ax.legend(loc='center left',bbox_to_anchor=(1,0.5))
    plt.title('Cumulative Run Time')
    plt.ylabel('Cumulative Run Time (s)')
    plt.xlabel('Benchmark Index')
    plt.savefig('%s/%s_wnorn.jpg' %(rstr,name),dpi=DPI)
    plt.cla()
    plt.clf()

    fig = plt.figure()
    ax  = plt.subplot(111)
    ax.plot(cvc4,'r',label='cvc4')
    ax.plot(z3,'b',label='z3')
    ax.plot(ic3,'k',label='ic3ia')
    box = ax.get_position()
    ax.set_position([box.x0,box.y0,box.width*0.8,box.height])
    ax.legend(loc='center left',bbox_to_anchor=(1,0.5))
    plt.title('Cumulative Run Time')
    plt.ylabel('Cumulative Run Time (s)')
    plt.xlabel('Benchmark Index')
    plt.savefig('%s/%s_wonorn.jpg' %(rstr,name),dpi=DPI)

def getData(filename,isIc3ia=False) :
    idx,sat,time,step = [],[],[],[]
    with open(filename,'r') as  csvfile :
        csvreader = csv.reader(csvfile)
        csvreader.next()
        for row in  csvreader :
            #print row
            idx.append(row[0])
            sat.append(row[1])
            time.append(row[2])
            if isIc3ia : step.append(row[3])
    if isIc3ia :
        return idx,sat,time,step
    else :
        return idx,sat,time
##############################################################################

def parse(argv) :
    if   len(argv) == 1 : opt1(argv)
    elif len(argv) == 2 : opt2(argv)
    elif len(argv) == 3 : opt3(argv)
    else                :
        sys.exit('[ERROR::parse] invalid argc=%d' %(len(argv)))

def opt1(argv) :
    if   argv[0] == '--buildDG'  : 
        buildDG('Kaluza')
        buildMap('Kaluza','all')
    elif argv[0] == '--resample' :
        call('rm experiment/Kaluza/sample', shell=True)
        call('rm -rf experiment/Kaluza/result/sample', shell=True)
        buildMap('Kaluza','sample')
    elif argv[0] == '--h' or argv[0] == '-h' :
        print '''
    --buildDG    ( for Kaluza )
    --resample   ( for Kaluza )
    --build      < --k | --ks | --t | --p | --a >
    --clear      < --k | --ks | --t | --p | --a > ( only clear map and experimental results )
    --reset      < --k | --ks | --t | --p | --a > ( remove *.dot *.blif *.vmt )
    --solve      < --k | --ks | --t | --p | --a >
    --solve-n    < --k | --ks | --t | --p | --a >
    --plot       < --k | --ks | --t | --p | --a >
    --execmd     < --k | --ks | --t | --p | --a > < --r2d | --d2b | --b2v | --cmd | --all >
    --single     <    dgFileName    > < --r2d | --d2b | --b2v | --cmd | --all | --reset >
              '''
    else :
        sys.exit('[ERROR::opt1] invalid opt=%s' %(argv[0]))

def opt2(argv) :
    if   argv[0] == '--build'   : opt_build(argv[1])
    elif argv[0] == '--clear'   : opt_clear(argv[1])
    elif argv[0] == '--reset'   : opt_reset(argv[1])
    elif argv[0] == '--solve'   : opt_solve(['ic3ia'],argv[1])
    elif argv[0] == '--solve-n' : opt_solve(['norn'],argv[1])
    else : sys.exit('[ERROR::opt2] invalid opt=%s' %(argv[0]))

def opt_build(opt) :
    if   opt == '--k'  : buildMap('Kaluza','strlen')
    elif opt == '--ks' : buildMap('Kaluza','sample')
    elif opt == '--t'  : buildMap('testing','all')
    elif opt == '--p'  : buildMap('pisa','all')
    elif opt == '--a'  : buildMap('appscan','all')
    else : sys.exit('[ERROR_opt_build] invalid opt=%s' %(opt))

def opt_clear(opt) :
    if   opt == '--k'  : 
        call('rm experiment/Kaluza/strlen',shell=True)
        call('rm -rf experiment/Kaluza/result/strlen',shell=True)
    elif opt == '--ks' :
        call('rm experiment/Kaluza/sample',shell=True)
        call('rm -rf experiment/Kaluza/result/sample',shell=True)
    elif opt == '--t'  :
        call('rm experiment/testing/all',shell=True)
        call('rm -rf experiment/testing/result/all',shell=True)
    elif opt == '--p'  :
        call('rm experiment/pisa/all',shell=True)
        call('rm -rf experiment/pisa/result/all',shell=True)
    elif opt == '--a'  :
        call('rm experiment/appscan/all',shell=True)
        call('rm -rf experiment/appscan/result/all',shell=True)
    else :
        sys.exit('[ERROR_opt_build] invalid opt=%s' %(opt))

def opt_reset(opt) :
    dgFileList = opt_scope(opt)
    for dgFile in dgFileList :
        call('rm %s/*.dot %s/*.blif %s/*.vmt' %(dgFile,dgFile,dgFile), shell=True)

def opt_solve(solvers,opt) :
    if   opt == '--k'  : 
        f = 'experiment/Kaluza/strlen'
        benchmark = 'Kaluza'
        scope     = 'strlen'
    elif opt == '--ks' : 
        f = 'experiment/Kaluza/sample'
        benchmark = 'Kaluza'
        scope     = 'sample'
    elif opt == '--t'  : 
        f = 'experiment/testing/all'
        benchmark = 'testing'
        scope     = 'all'
    elif opt == '--p'  : 
        f = 'experiment/pisa/all'
        benchmark = 'pisa'
        scope     = 'all'
    elif opt == '--a'  : 
        f = 'experiment/appscan/all'
        benchmark = 'appscan'
        scope     = 'all'
    else : sys.exit('[ERROR::opt_solve] invalid opt=%s' %(opt))
    dgIdxList,dgFileList = getSplitMap(f)
    for solver in solvers :
        exp(benchmark,scope,dgIdxList,dgFileList,solver)
    ConsistencyChecking(benchmark,scope,solvers)

def opt3(argv) :
    if   argv[0] == '--execmd' : 
        dgFileList = opt_scope(argv[1])
        opt_execmd(argv[2],dgFileList)
    elif argv[0] == '--single' :
        if argv[2] == '--reset' :
            call('rm %s/*.dot %s/*.blif %s/*.vmt' %(argv[1],argv[1],argv[1]), shell=True)
        else :
            dgFileList = [argv[1]]
            opt_execmd(argv[2],dgFileList)
    else : sys.exit('[ERROR::opt3] invalid opt=%s' %(argv[0]))

def opt_scope(opt) :
    if   opt == '--k'  : f = 'experiment/Kaluza/strlen'
    elif opt == '--ks' : f = 'experiment/Kaluza/sample'
    elif opt == '--t'  : f = 'experiment/testing/all'
    elif opt == '--p'  : f = 'experiment/pisa/all'
    elif opt == '--a'  : f = 'experiment/appscan/all'
    else : sys.exit('[ERROR::opt_scope] invalid opt=%s' %(opt))
    dgIdxList, dgFileList = getSplitMap(f)
    return dgFileList

def opt_execmd(opt,dgFileList) :
    if   opt == '--r2d' : regex2dot(dgFileList)
    elif opt == '--d2b' : dot2blif(dgFileList)
    elif opt == '--b2v' : blif2vmt(dgFileList)
    elif opt == '--cmd' : readCmd(dgFileList)
    elif opt == '--all' : 
        regex2dot(dgFileList)
        dot2blif(dgFileList)
        blif2vmt(dgFileList)
        readCmd(dgFileList)
    else : sys.exit('[ERROR:opt_execmd] invalid opt=%s' %(opt))

if __name__ == '__main__' :
    init()
    parse(sys.argv[1:])
