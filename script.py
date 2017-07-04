from __future__ import print_function
import sys
import time
import re
import csv
import numpy as np
from subprocess32 import call
from subprocess32 import TimeoutExpired
from os import listdir
from os.path import isdir, isfile, join
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
benchmark_abbr_set      = (['--k','--ks','--t','--p','--a'])
############################## Binary Directory ##############################
sla_dir                 = 'bin/sla2'
regex2dot_dir           = 'bin/regex2blif/target/regex2blif-0.0.1-SNAPSHOT.jar'
abc70930_dir            = 'bin/abc70930/abc'

abc_dir                 = 'bin/abc/bin/abc'
cvc4_dir                = 'bin/cvc4-2017-06-27/builds/bin/cvc4'
norn_dir                = 'bin/norn/norn'
z3_dir                  = 'bin/z3/build/z3'
s3p_dir                 = 'bin/S3P-bin-220617-1/run.py'
ABC_dir                 = 'abc'
fat_dir                 = 'bin/'
ic3ia_dir               = 'bin/ic3ia/build/ic3ia'

solver_dir              = {'cvc4':cvc4_dir,'norn':norn_dir,'z3':z3_dir,'s3p':s3p_dir,'ic3ia':ic3ia_dir,'abc':abc_dir,'ABC':ABC_dir}
solver_opt              = {'cvc4':'--strings-exp','norn':'','z3':'','s3p':'-f','ic3ia':'-w -v 2','abc':'-f','ABC':'-i'}
#solver_set              = (['cvc4','z3','s3p','ic3ia'])
solver_set              = (['cvc4','norn','z3','s3p','ic3ia','abc','ABC'])
solver_list             = ['ic3ia','abc','z3','cvc4','ABC','s3p','norn']

extention               = {'cvc4':'smt2','norn':'smt2','z3':'smt2','s3p':'s3','ic3ia':'vmt','abc':'abc','ABC':'smt2'}
##############################################################################
# [Function Name] file2lines
# [ Description ] parse file into list of lines
# [  Arguments  ] fileName : name of the file
##############################################################################
def file2lines(fileName) :
    out = open(fileName)
    lines = out.read().splitlines()
    out.close()
    return lines

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
    call('mkdir -p tmp dbg', shell=True)

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
# [Function Name] getExpResult
# [ Description ] return record directory and experimental results
# [  Arguments  ] benchmark = ['Kaluza','testing','pisa','appscan']
#                 scope     = ['all','strlen','sample']
##############################################################################
def getExpResult(benchmark,scope) :
    r_dir   = 'experiment/%s/result/%s' %(benchmark,scope)
    solvers = [ f[0:f.find('.')] for f in listdir(r_dir) if isfile(join(r_dir,f)) and f[f.rfind('.')+1:]=='csv']
    data = []
    for solver in solvers :
        data.append(getData('%s/%s.csv' %(r_dir,solver)))
    return r_dir,data,solvers

##############################################################################
# [Function Name] getData
# [ Description ] return experimental results
# [  Arguments  ] fileName : csv file
##############################################################################
def getData(fileName) :
    isIc3ia = False
    solver = fileName[ fileName.rfind('/') + 1 : fileName.rfind('.') ]
    if solver not in solver_set : sys.exit('[ERROR::getData] invalid solver=%s' %(solver))
    if solver == 'ic3ia' : isIc3ia = True
    idx,sat,time,step = [],[],[],[]
    with open(fileName,'r') as  csvfile :
        csvreader = csv.reader(csvfile)
        csvreader.next()
        for row in csvreader :
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
                call('./%s --buildDG %s %s' %(exePath,smtFile,DGdir),shell=True)
    elif benchmark == 'single' :
        if len(argv) != 2 : 
            sys.exit('[ERROR::buildDG] argv size not match for buildDG single')
        call('./%s --buildDG %s %s' %(exePath,argv[0],argv[1]), shell=True)
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
            for f in dgFile :
                dgCnt += 1
                predFile = open(join(f,'pred'))
                lines = predFile.read().splitlines()
                predFile.close()
                hasStrlen = False
                firstcol  = False
                for line in lines :
                    if line[-1] != ')' and firstcol == True :
                        hasStrlen = True
                    if line[-1] == ';' :
                        firstcol = True
                if hasStrlen :
                    mapFile.write('\n%d,%s' %(dgCnt,f))
        elif scope == 'sample' :
            sampleSize= 20000
            strlenMapFile = open('experiment/Kaluza/strlen')
            lines = strlenMapFile.read().splitlines()
            strlenMapFile.close()
            samples = np.random.choice(len(lines)-1,sampleSize,replace=False)
            samples.sort()
            splitNum = sampleSize / 5000
            for i in range(splitNum) :
                splitFile = open('experiment/Kaluza/sample_%s' %(i),'w')
                splitFile.write('id,name')
                for j in range(i*5000,(i+1)*5000) :
                    splitFile.write('\n%s' %(lines[samples[j]+1]))
                    mapFile.write('\n%s' %(lines[samples[j]+1]))
                splitFile.close()
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
    tmp = 'tmp/garbage'
    dbg = 'dbg'
    cnt = 0
    for f in dgFileList :
        print ('fileCnt=%d'%(cnt))
        lines = file2lines( join(f,'aut') )
        for line in lines :
            name = line[ 0 : line.find(' ') ]
            drkt = join( f , name )
            fchr = line[ line.find(' ') + 1 ]
            if   fchr == '\"' : regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            elif fchr == "\'" : regex = line[ line.find("\'")  : line.rfind("\'") + 1 ]
            else              : regex = line[ line.find('~')   : line.rfind(')') + 1  ]
            print ('aut_name = %-4s regex = %s' %(name,regex))
            ret = call('java -jar %s -r %s -d %s.dot -o %s -l FATAL' 
	    %(regex2dot_dir,regex,drkt,tmp),stdout=open('%s/r2d.log' %(dbg),'w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::regex2dot] fails file=%s' %(drkt))
        cnt += 1
    print ('[INFO::regex2dot] %d cases pass' %(cnt))

##############################################################################
# [Function Name] dot2blif
# [ Description ] convert dot file to blif file
# [  Arguments  ] dgFileList : list of dependency graph files
##############################################################################
def dot2blif(dgFileList) :
    tmp = 'tmp/abc_cmd'
    cnt = 0
    for f in dgFileList :
        lines = file2lines( join(f,'aut') )
        for line in lines :
            name = line[ 0 : line.find(' ') ]
            drkt = join( f , name )
            fchr = line[ line.find(' ') + 1 ]
            if   fchr == '\"' : regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            elif fchr == "\'" : regex = line[ line.find("\'")  : line.rfind("\'") + 1 ]
            else              : regex = line[ line.find('~')   : line.rfind(')') + 1  ]
            
            print ('aut_name = %-4s regex = %s' %(name,regex))
            
            ret1 = call('./%s --dot2blif %s.dot %s.blif' %(sla_dir,drkt,drkt),stdout=open('dbg/d2b.log','w'),shell=True)
            if ret1 != 0 : sys.exit('[ERROR::dot2blif] dot2blif fails file=%s' %(drkt))
            if regex == '".*"' or regex == '"~\\(".*"\\)"' or regex == '""' or regex == '~\\(""\\)' : continue
            abcCmdFile = open('%s' %(tmp),'w')
            abcCmdFile.write('read %s.blif' %(drkt))
            abcCmdFile.write('\nespresso')
            abcCmdFile.write('\nwrite %s.blif' %(drkt))
            abcCmdFile.close()
            ret2 = call('./%s -f %s' %(abc70930_dir,tmp),stdout=open('dbg/abc.log','w'),shell=True)
            if ret2 != 0 : sys.exit('[ERROR::dot2blif] abc minimization fails file=%s' %(regex,drkt))
        cnt += 1
    print ('[INFO::dot2blif] %d cases pass' %(cnt))

##############################################################################
# [Function Name] blif2vmt
# [ Description ] convert blif file to vmt file
# [  Arguments  ] dgFileList : list of dependency graph files
##############################################################################
def blif2vmt(dgFileList) :
    cnt = 0
    exePath = sla_dir
    for f in dgFileList :
        lines = file2lines( join(f,'aut') )
        for line in lines :
            drkt = join( f , line[ 0 : line.find(' ')] )
            ret  = call('./%s --blif2vmt %s.blif %s.vmt' %(exePath,drkt,drkt),stdout=open('dbg/b2v.log','w'),shell=True)
            if ret != 0 : sys.exit('[ERROR::blif2vmt] blif2vmt fails file=%s' %(drkt))
        cnt += 1
    print ('[INFO::blif2vmt] %d cases pass' %(cnt))

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
        ret = call('./%s --readCmd %s' %(exePath,cmdFile),stdout=open('dbg/readcmd.log','w'),shell=True)
        if ret != 0 : sys.exit('[ERROR::readCmd] readCmd fails file=%s' %(cmdFile))
        cnt += 1
    print ('[INFO::readCmd] %d cases pass' %(cnt))

##############################################################################
# [Function Name] exp
# [ Description ] experiment on different solvers
# [  Arguments  ] benchmark  = ['Kaluza','testing','pisa','appscan']
#                 scope      = ['all','strlen','sample']
#                 dgIdxList  : idx list of dependency graph files
#                 dgFileList : file list of dependency graph files
#                 solver     = ['cvc4','norn','z3','ic3ia']
#                 1 = sat ; 0 = unsat ; x = error ; t = timeout 
##############################################################################
def exp(benchmark,scope,dgIdxList,dgFileList,solver,TO) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::exp] invalid benchmark=%s' %(benchmark))
    recordName,header = expParam(benchmark,scope,solver)
    record = open(recordName,'w')
    record.write('%s' %(header))
    for i in range(len(dgIdxList)) :
        print ('solver = %-6s id = %-4s file = %s' %(solver,dgIdxList[i],dgFileList[i]))
        expRecord(solver,dgIdxList[i],dgFileList[i],record,TO)
    record.close()

def expParam(benchmark,scope,solver) :
    recordName = 'experiment/%s/result/%s/%s.csv' %(benchmark,scope,solver)
    header     = 'id,sat,time'
    if solver == 'ic3ia' or solver == 'abc': header ='id,sat,time,step'
    return recordName,header

def expSingle(dgFileDir,TO=10) :
    if dgFileDir[-1] == '/' : dgFileDir = dgFileDir[0:dgFileDir.rfind('/')]
    fileName = dgFileDir[dgFileDir.rfind('/') + 1:]
    call('mkdir -p experiment/single/%s' %(fileName),shell=True)
    record = open('experiment/single/%s/all_solver.csv' %(fileName),'w')
    record.write('file : %s TO=%ss' %(fileName,TO))
    record.write('\nidx,sat,time[,step]')
    for solver in solver_set :
        record.write('\n%s' %(solver))
        expRecord(solver,'1',dgFileDir,record,TO)

def expRecord(solver,idx,dirName,record,TO) :
    if solver not in solver_set :
        sys.exit('[ERROR::expRecord] invalid solver name=%s' %(solver))
    f       = join(dirName,'sink.%s' %(extention[solver]))
    exePath = solver_dir[solver]
    option  = solver_opt[solver]
    tmpPath = 'tmp/exp'
    try : 
        ts  = time.time()
        call('./%s %s %s' %(exePath,option,f),timeout=TO,stdout=open(tmpPath,'w'),shell=True)
        te  = time.time()
    except TimeoutExpired :
        if   solver == 'ic3ia': record.write('\n%s,t,0.0,0' %(idx))
        else                  : record.write('\n%s,t,0.0'   %(idx))
    else :
        lines = file2lines(tmpPath)
        # handle core dump / segmentation fault condition
        if not lines :
            if solver == 'ic3ia' or solver == 'abc': 
                record.write('\n%s,x,0.0,0' %(idx))
            else : 
                record.write('\n%s,x,0.0' %(idx))
            return  
        if   solver == 'cvc4' : exp_cvc4 (idx,te-ts,lines,record)
        elif solver == 'norn' : exp_norn (idx,te-ts,lines,record)
        elif solver == 'z3'   : exp_z3   (idx,te-ts,lines,record)
        elif solver == 's3p'  : exp_s3p  (idx,te-ts,lines,record)
        elif solver == 'ABC'  : exp_ABC  (idx,te-ts,lines,record)
        elif solver == 'fat'  : exp_fat  (idx,te-ts,lines,record)
        elif solver == 'abc'  : exp_abc  (idx,te-ts,lines,record)
        elif solver == 'ic3ia': exp_ic3ia(idx,te-ts,lines,record)

def exp_cvc4(idx,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x'
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_norn(idx,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x'
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_z3(idx,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x'
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_s3p(idx,dt,lines,record) :
    sat = 'x'
    vok = False
    for line in lines :
        if line == '* v-ok' : 
            vok = True
        if line == '>> SAT' :
            if vok == True :
                sat = '1'
                break
        if line == '>> UNSAT':
            sat = '0'
            break
    
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_ABC(idx,dt,lines,record) :
    if   lines[1].find('is_sat: SAT'  ) != -1 : sat = '1'
    elif lines[1].find('is_sat: UNSAT') != -1 : sat = '0'
    else                                      : sat = 'x'
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_fat(idx,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x'
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_abc(idx,dt,lines,record) :
    if   lines[-1].find('Output 0 of miter \"sink\" was asserted in frame') != -1 :
        sat = '1'
        bpos = lines[-1].find('frame') + 6
        step = lines[-1][bpos:]
        step = step[:step.find('.')]
    elif lines[-1].find('Property proved.') != -1 :
        sat = '0'
        for line in lines :
            v = line.split()
            if not v : continue
            if v[0] == 'Invariant' :
                bpos = v[1].find('[') + 1
                epos = v[1].rfind(']')
                step = v[1][bpos:epos]
    else : 
        sat = 'x'
        step = 'x'
    record.write('\n%s,%s,%.6f,%s' %(idx,sat,dt,step))

def exp_ic3ia(idx,dt,lines,record) :
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

##############################################################################
# [Function Name] ConsistencyChecking
# [ Description ] consistency checking
# [  Arguments  ] benchmark  = ['Kaluza','testing','pisa','appscan']
#                 scope      = ['all','strlen','sample']
##############################################################################
def ConsistencyChecking(benchmark,scope) :
    r_dir,data,solvers = getExpResult(benchmark,scope)
    
    # Case Number Checking
    logFile = open('%s/log' %(r_dir),'w')
    for i in range(1,len(data)) :
        if len(data[0][0]) != len(data[i][0]) :
            logFile.write('number of cases not match\n')
            logFile.close()
            sys.exit("[ERROR::CC] number of cases not match")
    logFile.write('number of cases match\n')
    
    solverNum = len(solvers)
    caseNum   = len(data[0][0])

    # Error Checking
    errCnt  = []
    errFree = True
    for i in range(solverNum) :
        cnt = 0
        for j in range(caseNum) :
            if data[i][1][j] == 'x' :
                errFree = False
                cnt += 1
                logFile.write('case:%-6s ERROR   solver=%s\n' %(data[i][0][j],solvers[i]))
        errCnt.append(cnt)

    # Timeout Checking
    toCnt  = []
    toFree = True
    for i in range(solverNum) :
        cnt = 0
        for j in range(caseNum) :
            if data[i][1][j] == 't' :
                toFree = False
                cnt   += 1
                logFile.write('case:%-6s TIMEOUT solver=%s\n' %(data[i][0][j],solvers[i]))
        toCnt.append(cnt)
    '''
    if not errFree :
        logFile.close()
        sys.exit('[ERROR::CC] some cases have ERROR')
    '''
    # Exclusive Checking
    exCase = [0] * len(data)
    for i in range(caseNum) :
        zeroCnt = 0
        oneCnt  = 0
        for j in range(solverNum) :
            if data[j][1][i] == '1' :
                oneCnt += 1
            elif data[j][1][i] == '0' :
                zeroCnt += 1
        if oneCnt == 1 and zeroCnt == 0 :
            for j in range(solverNum) :
                if data[j][1][i] == '1' :
                    exCase[j] += 1
        elif oneCnt == 0 and zeroCnt ==1 :
            for j in range(solverNum) :
                if data[j][1][i] == '0' :
                    exCase[j] += 1
    
    statsFile = open('%s/stats' %(r_dir),'w')
    statsFile.write('%-8s %-10s %-10s %-10s %-10s\n' %(benchmark,'Error','Timeout','Exclusive','Solved'))
    for i in range(len(data)) :
        tmp = len(data[0][0]) - (errCnt[i]+toCnt[i])
        statsFile.write('%-8s %-10s %-10s %-10s %-10s\n' %(solvers[i],errCnt[i],toCnt[i],exCase[i],tmp))
    statsFile.close()

    # Consistency Checking
    icset = set()
    allConsistent = True
    for i in range(1,solverNum) :
        for j in range(caseNum) :
            if data[0][1][j] != data[i][1][j] and data[0][1][j] != 't' and data[i][1][j] != 't' and data[0][1][j] != 'x' and data[i][1][j] != 'x' :
                allConsistent = False
                icset.add(data[i][0][j])

    for i in icset :
        logFile.write('case:%-6s inconsistent\n' %(i))

    if allConsistent :
        logFile.write('all cases consistent')
    else :
        logFile.close()
        #sys.exit("[ERROR::CCandPlot] some cases inconsistent")
    
    logFile.close()

##############################################################################
# [Function Name] plot
# [ Description ] plot
# [  Arguments  ] benchmark = ['Kaluza','testing','pisa','appscan']
#                 scope     = ['all','strlen','sample']
##############################################################################
def plot(benchmark,scope) :
    
    r_dir,data,solvers = getExpResult(benchmark,scope)
    
    # trim and sort
    trim = []
    for i in range(len(data)) :
        idx,sat,time = [],[],[]
        for j in range(len(data[i][1])) :
            if data[i][1][j] != 't' and data[i][1][j] != 'x' :
                idx.append(data[i][0][j])
                sat.append(data[i][1][j])
                time.append(float(data[i][2][j]))
        time.sort()
        print ('solver=%s' %(solvers[i]))
        for t in time :
            print ('%.4f' %(t))
        trim.append(time)
    
    plotCumTime(benchmark,r_dir,trim,solvers)

    # TODO plot time vs step scatter for ic3ia unsafe case
    '''
    unsafeStep,unsafeTime = [],[]
    for i in range(len(ic3[3])) :
        if ic3[1][i] == '0' :
            if ic3[3][i] == 'x' :
                sys.exit('[ERROR::CCandPlot] ic3ia : idx=%s unsafe case must have step != \'x\'' %(ic3[0][i]))
            unsafeStep.append(ic3[3][i])
            unsafeTime.append(ic3[2][i])
    print (unsafeStep)
    print (unsafeTime)
    '''
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
    '''
    plt.scatter(unsafeStep,unsafeTime,c='b')
    plt.title('Time vs Step Scatter Plot (# of case = %d)' %(len(unsafeStep)))
    plt.xlabel('Step Count')
    plt.ylabel('Run Time (s)')
    plt.savefig('%s/step_ic3ia_unsafe.jpg' %(rstr),dpi=DPI)
    plt.cla()
    plt.clf()
    '''

def plotCumTime(benchmark,r_dir,data,solvers) :
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    import matplotlib.patches as mpat
    from matplotlib.font_manager import FontProperties
    # plot cumulative time vs case index
    
    colorMap = {'ic3ia' : (0.0,1.0,0.0), 
                'cvc4'  : (1.0,0.0,0.0),
                'z3'    : (0.0,0.0,1.0),
                'norn'  : (0.8,0.7,0.0),
                's3p'   : (0.9,0.5,0.1),
                'ABC'   : (1.0,0.0,0.0),
                'fat'   : (0.0,1.0,0.0),
                'abc'   : (0.0,0.0,1.0)}

    csum = []
    for i in range(len(data)) :
        print ('solver=%s' %(solvers[i]))
        d = np.array(data[i],dtype=float)
        print ('origin')
        for j in d : print (j)
        np.cumsum(d,out=d)
        print ('cumsum')
        for j in d : print (j)
        csum.append(d)

    fig = plt.figure()
    ax  = plt.subplot(111)
    for i in range(len(csum)) :
        ax.plot(np.arange(len(csum[i])),csum[i],color=colorMap[solvers[i]],label=solvers[i])
    box = ax.get_position()
    ax.set_position([box.x0,box.y0,box.width*0.8,box.height])
    ax.legend(loc='center left',bbox_to_anchor=(1,0.5))
    plt.title('%s Sorted Cumulative Run Time' %(benchmark.title()))
    plt.ylabel('Time (s)')
    plt.xlabel('Number of Solved Instance')
    plt.savefig('%s/crt.jpg' %(r_dir),dpi=DPI)
    plt.cla()
    plt.clf()

##############################################################################

def parse(argv) :
    if   len(argv) == 1 : opt1(argv)
    elif len(argv) == 2 : opt2(argv)
    elif len(argv) <= 4 : opt3(argv)
    else                :
        sys.exit('[ERROR::parse] invalid argc=%d' %(len(argv)))

def benchmark_scope(opt) :
    if opt not in benchmark_abbr_set :
        sys.exit('[ERROR::benchmark_scope] invalid opt=%s' %(opt))
    if   opt == '--k'  : return 'Kaluza' , 'strlen'
    elif opt == '--ks' : return 'Kaluza' , 'sample'
    elif opt == '--t'  : return 'testing', 'all'
    elif opt == '--p'  : return 'pisa'   , 'all'
    elif opt == '--a'  : return 'appscan', 'all'

def opt1(argv) :
    if   argv[0] == '--buildDG'  : 
        buildDG('Kaluza')
        buildMap('Kaluza','all')
    elif argv[0] == '--resample' :
        call('rm experiment/Kaluza/sample', shell=True)
        call('rm -rf experiment/Kaluza/result/sample', shell=True)
        buildMap('Kaluza','sample')
    elif argv[0] == '--h' or argv[0] == '-h' :
        print ('''
    --buildDG    ( for Kaluza )
    --resample   ( for Kaluza )
    --build      < --k | --ks | --t | --p | --a >
    --clear      < --k | --ks | --t | --p | --a > ( only clear map and experimental results )
    --reset      < --k | --ks | --t | --p | --a > ( remove *.dot *.blif *.vmt )
    --cc         < --k | --ks | --t | --p | --a >
    --plot       < --k | --ks | --t | --p | --a >
    --execmd     < --k | --ks | --t | --p | --a > < --r2d   | --d2b  | --b2v | --cmd | --all >
    --solve      < --k | --ks | --t | --p | --a > 
                 < --ic3ia | --cvc4 | --z3 | --s3p | --abc | --ABC | --norn | --all >
                 [ time out (default=10s) ]
    --single     <    dgFileName    > < --r2d | --d2b | --b2v | --cmd | --all | --solve [ time out (default=10s) ] | --reset >
    --mapFile    <    mapFileName   > < --r2d | --d2b | --b2v | --cmd | --all | --solve [ time out (default=10s) ] | --reset >
              ''')
    else :
        sys.exit('[ERROR::opt1] invalid opt=%s' %(argv[0]))

def opt2(argv) :
    if argv[1] not in benchmark_abbr_set :
        sys.exit('[ERROR::opt2] invalid opt=%s' %(argv[1]))
    t = benchmark_scope(argv[1])
    if   argv[0] == '--build'   : buildMap(t[0],t[1])
    elif argv[0] == '--clear'   : call('rm experiment/%s/%s' %(t[0],t[1]),shell=True);\
                                  call('rm -rf experiment/%s/result/%s' %(t[0],t[1]),shell=True)
    elif argv[0] == '--reset'   : opt_reset(t[0],t[1])
    elif argv[0] == '--cc'      : ConsistencyChecking(t[0],t[1])
    elif argv[0] == '--plot'    : plot(t[0],t[1])
    else                        : sys.exit('[ERROR::opt2] invalid opt=%s' %(argv[0]))

def opt3(argv) :
    if   argv[0] == '--execmd' : t = benchmark_scope(argv[1]);\
                                 dgIdx,dgFiles = getSplitMap('experiment/%s/%s' %(t[0],t[1]));\
                                 opt_execmd(argv[2],dgFiles)
    elif argv[0] == '--solve'  : 
        t = benchmark_scope(argv[1])
        if len(argv) == 3 : opt_solve(t[0],t[1],argv[2],10)
        else              : opt_solve(t[0],t[1],argv[2],argv[3])
    elif argv[0] == '--single' :
        if argv[2] == '--reset' :
            call('rm %s/*.dot %s/*.blif %s/*.vmt' %(argv[1],argv[1],argv[1]), shell=True)
        elif argv[2] == '--solve' :
            if len(argv) == 4 : TO = float(argv[3])
            expSingle(argv[1],TO)
        else :
            dgFileList = [argv[1]]
            opt_execmd(argv[2],dgFileList)
    elif argv[0] == '--mapFile' :
        scope = argv[1][argv[1].rfind('/')+1:]
        dgIdxList,dgFileList = getSplitMap('%s' %(argv[1]))
        if argv[2] == '--reset' :
            for dg in dgFileList : 
                call('rm %s/*.dot %s/*.blif %s/*.vmt' %(dg,dg,dg), shell=True)
        elif argv[2] == '--solve' :
            call('mkdir -p experiment/Kaluza/result/%s' %(scope))
            if len(argv) == 4 : TO = float(argv[3])
            for solver in solver_list :
                exp('Kaluza',scope,dgIdxList,dgFileList,solver,TO)
        else :
            opt_execmd(argv[2],dgFileList)
        
    else : sys.exit('[ERROR::opt3] invalid opt=%s' %(argv[0]))

def opt_execmd(opt,dgFileList) :
    if   opt == '--r2d' : regex2dot(dgFileList)
    elif opt == '--d2b' : dot2blif(dgFileList)
    elif opt == '--b2v' : blif2vmt(dgFileList)
    elif opt == '--cmd' : readCmd(dgFileList)
    elif opt == '--all' : regex2dot(dgFileList);\
                          dot2blif(dgFileList); \
                          blif2vmt(dgFileList); \
                          readCmd(dgFileList)
    else : sys.exit('[ERROR:opt_execmd] invalid opt=%s' %(opt))

def opt_reset(benchmark,scope) :
    dgIdxList, dgFileList = getSplitMap('experiment/%s/%s' %(benchmark,scope))
    for dgFile in dgFileList :
        call('rm %s/*.dot %s/*.blif %s/*.vmt' %(dgFile,dgFile,dgFile), shell=True)

def opt_solve(benchmark,scope,opt,TO=10) :
    if opt == '--all' : solvers = solver_list
    else              : solvers = [ opt[ opt.rfind('-') + 1 : ] ]
    dgIdxList, dgFileList = getSplitMap('experiment/%s/%s' %(benchmark,scope))
    for solver in solvers :
        exp(benchmark,scope,dgIdxList,dgFileList,solver,TO)

if __name__ == '__main__' :
    init()
    parse(sys.argv[1:])
    '''
    parse( ['--execmd','--p','--all'] )
    parse( ['--solve','--p','--all'])
    parse( ['--cc','--p'] )
    parse( ['--plot','--p'] )
    #parse( ['--execmd','--a','--all'] )
    parse( ['--solve','--a','--all'])
    parse( ['--cc','--a'] )
    parse( ['--plot','--a'] )
    '''
    '''
    exePath = cvc4_dir
    f = 'DG/pisa/pisa-008-1/sink.smt2'
    ts = time.time()
    call('%s --strings-exp %s' %(exePath,f),stdout=open('out','w'),shell=True)
    te = time.time()
    print 'RT=%f' %(te-ts)
    '''
