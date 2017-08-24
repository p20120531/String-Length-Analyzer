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
Default_TO              = 20
DPI                     = 400
############################## Benchmark Directory ###########################
benchmark_dir           = {'kaluza'  : ['laut/kaluza'] ,
                           'testing' : ['laut/testing/sat','laut/testing/unsat'],
                           'pisa'    : ['laut/pisa'  ] ,
                           'appscan' : ['laut/appscan']}
benchmark_set           = (['kaluza','testing','pisa','appscan'])
benchmark_abbr_set      = (['--k','--t','--p','--a'])
benchmark_abbr_map      = {'--k' : 'kaluza' ,
                           '--t' : 'testing',
                           '--p' : 'pisa'   ,
                           '--a' : 'appscan'}
############################## Binary Directory ##############################
sla_dir                 = 'bin/sla'
regex2dot_dir           = 'bin/regex2blif/target/regex2blif-0.0.1-SNAPSHOT.jar'
abc70930_dir            = 'bin/abc70930/abc'

abc_dir                 = 'bin/abc/bin/abc'
cvc4_dir                = 'bin/cvc4-2017-06-27/builds/bin/cvc4'
norn_dir                = 'bin/norn/norn'
z3_dir                  = 'bin/z3/build/z3'
s3p_dir                 = 'bin/S3P-bin-220617-1/run.py'
ABC_dir                 = 'abc'
fat_dir                 = 'bin/FAT/FAT'
ic3ia_dir               = 'bin/ic3ia/build/ic3ia'

solver_dir              = {'cvc4':'./' + cvc4_dir,'norn':'./'+norn_dir,'z3':'./'+z3_dir,'s3p':'./'+s3p_dir,
                           'ic3ia':'./'+ic3ia_dir,'abc':'./'+abc_dir,'ABC':ABC_dir,'fat':'./'+fat_dir}
solver_opt              = {'cvc4':'--strings-exp','norn':'','z3':'','s3p':'-f','ic3ia':'-w -v 2','abc':'-f','ABC':'-i','fat':'-model'}
#solver_set              = (['cvc4','z3','s3p','ic3ia'])
solver_set              = (['cvc4','norn','z3','s3p','ic3ia','abc','ABC','fat'])
solver_list             = ['ic3ia','abc','z3','cvc4','norn','ABC','s3p','fat']
exp_solver_list         = ['ic3ia','abc','z3','cvc4','s3p','norn','ABC','fat']
repall_solver_list      = ['ic3ia','abc','ABC','fat','s3p']

extension               = {'cvc4':'smt2','norn':'smt2','z3':'smt2','s3p':'s3','ic3ia':'vmt','abc':'abc','ABC':'smt2','fat':'s3'}

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
    call('mkdir -p dbg tmp', shell=True)
    for benchmark in benchmark_set : 
        call('mkdir -p experiment/%s' %(benchmark), shell=True)

def clean(benchmark) :
    dgIdxList,dgFileList = getSplitMap('experiment/%s/map' %(benchmark))
    for f in dgFileList :
        call('rm %s/*.dot %s/*.blif %s/*.vmt %s/*.pfx %s/*.abc' %(f,f,f,f,f),shell=True)
    

##############################################################################
# [Function Name] getLAutFile
# [ Description ] return length automaton files of the benchmark
# [  Arguments  ] benchmark = ['kaluza','testing','pisa','appscan']
##############################################################################
def getLAutFile(benchmark) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::getLAutFile] invalid benchmark=%s' %(benchmark))
    
    dgFile = []
    dirs   = benchmark_dir[benchmark]
    for d in dirs :
        for sp in listdir(d) : print(sp)
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
# [  Arguments  ] benchmark = ['kaluza','testing','pisa','appscan']
##############################################################################
def getExpResult(benchmark) :
    r_dir   = 'experiment/%s' %(benchmark)
    solvers = [ f[0:f.find('.')] for f in listdir(r_dir) if isfile(join(r_dir,f)) and f[f.rfind('.')+1:]=='csv']
    data,ret_solvers = [],[]
    for solver in solvers :
        #if solver == 'ABC' or solver == 'fat' or solver == 's3p' : continue
        #if solver == 'ABC' or solver == 'fat'  : continue
        data.append(getData(benchmark,'%s/%s.csv' %(r_dir,solver)))
        ret_solvers.append(solver)
    
    return r_dir,data,ret_solvers

##############################################################################
# [Function Name] getData
# [ Description ] return experimental results
# [  Arguments  ] fileName : csv file
##############################################################################
def getData(benchmark,fileName) :
    #pisa_mute_set    = (['1','2','3','4','5','6'])
    #appscan_mute_set = (['43','45','47','52','65','66','72'])
    recStep = False
    solver = fileName[ fileName.rfind('/') + 1 : fileName.rfind('.') ]
    if solver not in solver_set : sys.exit('[ERROR::getData] invalid solver=%s' %(solver))
    if solver == 'ic3ia' or solver == 'abc': recStep = True
    idx,sat,time,step = [],[],[],[]
    with open(fileName,'r') as  csvfile :
        csvreader = csv.reader(csvfile)
        csvreader.next()
        csvreader.next()
        for row in csvreader :
            #if benchmark == 'pisa' :
                #if row[0] in pisa_mute_set : continue
            #elif benchmark == 'appscan' :
                #if row[0] in appscan_mute_set : continue
            idx.append(row[0])
            sat.append(row[1])
            time.append(row[2])
            if recStep : step.append(row[3])
    if recStep :
        return idx,sat,time,step
    else :
        return idx,sat,time

##############################################################################
# [Function Name] buildMap
# [ Description ] build id to name map of files
# [  Arguments  ] benchmark = ['kaluza','testing','pisa','appscan']
##############################################################################
def buildMap(benchmark) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::buildMap] invalid benchmark=%s' %(benchmark))
    mapFile = open('experiment/%s/map' %(benchmark),'w')
    mapFile.write('id,name')
    dgFile,dgCnt = getLAutFile(benchmark),0
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
    cnt = 0
    for f in dgFileList :
        print ('fileCnt = %-6d fileName = %s'%(cnt,f))
        lines = file2lines( join(f,'laut') )
        for line in lines :
            if line == ';' : break
            name = line[ 0 : line.find(' ') ]
            drkt = join( f , name )
            fchr = line[ line.find(' ') + 1 ]
            if   fchr == '\"' : regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            elif fchr == "\'" : regex = line[ line.find("\'")  : line.rfind("\'") + 1 ]
            else              : regex = line[ line.find('~')   : line.rfind(')') + 1  ]
            print ('aut_name = %-4s regex = %s' %(name,regex))
            ret = call('java -jar %s -r %s -d %s.dot -o %s -l FATAL' 
	    %(regex2dot_dir,regex,drkt,tmp),stdout=open('dbg/r2d.log','w'),shell=True)
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
        print ('fileCnt = %-6d fileName = %s'%(cnt,f))
        lines = file2lines( join(f,'laut') )
        for line in lines :
            if line == ';' : break
            name = line[ 0 : line.find(' ') ]
            drkt = join( f , name )
            fchr = line[ line.find(' ') + 1 ]
            if   fchr == '\"' : regex = line[ line.find('\"')  : line.rfind('\"') + 1 ]
            elif fchr == "\'" : regex = line[ line.find("\'")  : line.rfind("\'") + 1 ]
            else              : regex = line[ line.find('~')   : line.rfind(')') + 1  ]
            
            #print ('    aut_name = %-10s regex = %s' %(name,regex))
            
            ret1 = call('./%s --dot2blif %s.dot %s.blif' %(sla_dir,drkt,drkt),stdout=open('dbg/d2b.log','w'),shell=True)
            if ret1 != 0 : sys.exit('[ERROR::dot2blif] dot2blif fails file=%s' %(drkt))
            if regex == '".*"' or regex == '"~\\(".*"\\)"' or regex == '""' or regex == '~\\(""\\)' : continue
            abcCmdFile = open('%s' %(tmp),'w')
            abcCmdFile.write('read %s.blif' %(drkt))
            abcCmdFile.write('\nespresso')
            abcCmdFile.write('\nwrite %s.blif' %(drkt))
            abcCmdFile.close()
            ret2 = call('./%s -f %s' %(abc70930_dir,tmp),stdout=open('dbg/abc.log','w'),shell=True)
            if ret2 != 0 : sys.exit('[ERROR::dot2blif] abc minimization fails file=%s' %(drkt))
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
        print ('fileCnt = %-6d fileName = %s'%(cnt,f))
        lines = file2lines( join(f,'laut') )
        for line in lines :
            if line == ';' : break
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
        print ('fileCnt = %-6d fileName = %s'%(cnt,f))
        cmdFile = join(f,'laut')
        ret = call('./%s --readCmd %s' %(exePath,cmdFile),stdout=open('dbg/readcmd.log','w'),shell=True)
        if ret != 0 : sys.exit('[ERROR::readCmd] readCmd fails file=%s' %(cmdFile))
        cnt += 1
    print ('[INFO::readCmd] %d cases pass' %(cnt))

##############################################################################
# [Function Name] exp
# [ Description ] experiment on different solvers
# [  Arguments  ] benchmark  = ['kaluza','testing','pisa','appscan']
#                 dgIdxList  : idx list of dependency graph files
#                 dgFileList : file list of dependency graph files
#                 solver     = ['cvc4','norn','z3','ic3ia']
#                 1 = sat ; 0 = unsat ; x = error ; t = timeout 
##############################################################################
def exp(benchmark,dgIdxList,dgFileList,solver,TO) :
    if benchmark not in benchmark_set :
        sys.exit('[ERROR::exp] invalid benchmark=%s' %(benchmark))
    recordName,header = expParam(benchmark,solver)
    record = open(recordName,'w')
    record.write('benchmark=%s,TO=%.6f' %(benchmark,TO))
    record.write('\n%s' %(header))
    for i in range(len(dgIdxList)) :
        print ('solver = %-6s id = %-4s file = %s' %(solver,dgIdxList[i],dgFileList[i]))
        expRecord(solver,dgIdxList[i],dgFileList[i],record,TO)
    record.close()

def expParam(benchmark,solver) :
    recordName = 'experiment/%s/%s.csv' %(benchmark,solver)
    header     = 'id,sat,time'
    if solver == 'ic3ia' or solver == 'abc': header ='id,sat,time,step'
    return recordName,header

def expRecord(solver,idx,dirName,record,TO) :
    if solver not in solver_set :
        sys.exit('[ERROR::expRecord] invalid solver name=%s' %(solver))
    f       = join(dirName,'sink.%s' %(extension[solver]))
    exePath = solver_dir[solver]
    option  = solver_opt[solver]
    tmpPath = 'tmp/exp'
    try :
        if solver == 'ABC' :
            tmpPath = 'tmp/abc.INFO'
            ts  = time.time()
            call('%s %s %s --log-dir tmp' %(exePath,option,f),timeout=TO,shell=True)
            te  = time.time()
        else :
            ts  = time.time()
            call('%s %s %s' %(exePath,option,f),timeout=TO,stdout=open(tmpPath,'w'),shell=True)
            te  = time.time()
    except TimeoutExpired :
        if   solver == 'ic3ia' or solver == 'abc' : record.write('\n%s,t,0.0,0' %(idx))
        else                  : record.write('\n%s,t,0.0'   %(idx))
    else :
        lines = file2lines(tmpPath)
        # handle core dump / segmentation fault condition
        if not lines :
            if solver == 'ic3ia' or solver == 'abc': 
                record.write('\n%s,x,%.6f,0' %(idx,0.0))
            else : 
                record.write('\n%s,x,%.6f' %(idx,0.0))
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
    else                     : sat = 'x';\
                               dt  = 0.0
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_norn(idx,dt,lines,record) :
    if   lines[0] == 'sat'   : sat = '1'
    elif lines[0] == 'unsat' : sat = '0'
    else                     : sat = 'x';\
                               dt  = 0.0
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_z3(idx,dt,lines,record) :
    sat = 'x'
    for line in lines :
        if   line == 'sat'   : sat = '1'
        elif line == 'unsat' : sat = '0'
    if sat == 'x' : dt = 0.0
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
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_ABC(idx,dt,lines,record) :
    sat = 'x'
    for line in lines :
        if   line.find('is_sat: SAT'  ) != -1 : sat = '1'
        elif line.find('is_sat: UNSAT') != -1 : sat = '0'
    if sat == 'x' : dt = 0.0
    record.write('\n%s,%s,%.6f' %(idx,sat,dt))

def exp_fat(idx,dt,lines,record) :
    sat = 'x'
    splitlinefound = False
    for i in range(1,len(lines)+1) :
        if lines[-i] == '================================================' :
            splitlinefound = True
        if splitlinefound :
            if   lines[-i].find('>> SAT')   != -1 : sat = '1';\
                                                    break
            elif lines[-i].find('>> UNSAT') != -1 : sat = '0';\
                                                    break
    if sat == 'x' : dt = 0.0
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
        sat  = 'x'
        step = 'x'
        dt   = 0.0
    record.write('\n%s,%s,%.6f,%s' %(idx,sat,dt,step))

def exp_ic3ia(idx,dt,lines,record) :
    if   lines[-1] == 'safe'   : sat = '0'
    elif lines[-1] == 'unsafe' : sat = '1'
    else                       : sat = 'x';\
                                 dt  = 0.0
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
##############################################################################
def ConsistencyChecking(benchmark) :
    r_dir,data,solvers = getExpResult(benchmark)
    
    # Case Number Checking
    logFile = open('%s/log' %(r_dir),'w')
    for i in range(1,len(data)) :
        if len(data[0][0]) != len(data[i][0]) :
            logFile.write('number of cases not match\n')
            logFile.close()
            sys.exit("[ERROR::CC] number of cases not match")
    logFile.write('number of cases match\n')
    
    if benchmark == 'replace_involved' :
        for i in range(len(solvers)) :
            if solvers[i] == 'ic3ia' :
                z3Idx = i
                break
    else :
        for i in range(len(solvers)) :
            if solvers[i] == 'z3' :
                z3Idx = i
                break

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
    # SAT/UNSAT Checking
    satCnt,unsatCnt,icCnt = [],[],[]
    for i in range(solverNum) :
        satn,unsatn,icn = 0,0,0
        for j in range(caseNum) :
            if data[i][1][j] == '1' :
                if data[z3Idx][1][j] == '0' :
                    icn += 1
                else :
                    satn += 1
            elif data[i][1][j] == '0' :
                if data[z3Idx][1][j] == '1' :
                    icn += 1
                else :
                    unsatn += 1
        satCnt.append(satn)
        unsatCnt.append(unsatn)
        icCnt.append(icn)

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
    
    # Record Cumulative Time
    crt = []
    for i in range(solverNum) :
        time = 0
        for j in range(caseNum) :
            if data[i][1][j] != 't' and data[i][1][j] != 'x' :
                if   data[i][1][j] == '1' :
                    if data[z3Idx][1][j] == '0' : continue
                elif data[i][1][j] == '0' :
                    if data[z3Idx][1][j] == '1' : continue
                time += float(data[i][2][j])
        crt.append(time)

    # Consistency Checking
    icset = set()
    allConsistent = True
    for i in range(solverNum) :
        for j in range(caseNum) :
            if ( data[i][1][j] != data[z3Idx][1][j] and 
                 data[i][1][j] != 'x' and data[i][1][j] != 't' and
                 data[z3Idx][1][j] != 'x' and data[z3Idx][1][j] != 't' ) :
                allConsistent = False
                icset.add(data[i][0][j])
    
    # Write Statistics
    statsFile = open('%s/stats' %(r_dir),'w')
    statsFile.write('benchmark                    : %s\n' %(benchmark))
    statsFile.write('timeout                      : %.6f (s)\n' %(Default_TO))
    statsFile.write('number of all cases          : %d\n' %(caseNum))
    statsFile.write('number of inconsistent cases : %d\n\n' %(len(icset)))
    statsFile.write('%-8s %-10s %-10s %-10s %-10s %-15s %-10s %-10s\n' %('solver','Abort','Timeout','SAT','UNSAT','Inconsistent','Exclusive','Cumulative Time'))
    for i in range(len(data)) :
        statsFile.write('%-8s %-10s %-10s %-10s %-10s %-15s %-10s %-10s\n' %(solvers[i],errCnt[i],toCnt[i],satCnt[i],unsatCnt[i],icCnt[i],exCase[i],crt[i]))
    statsFile.close()

    for i in sorted(map(int,list(icset))) :
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
##############################################################################
def plot(benchmark) :
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    import matplotlib.patches as mpat
    from matplotlib.font_manager import FontProperties
    
    r_dir,data,solvers = getExpResult(benchmark)
    
    for i in range(len(solvers)) :
        if solvers[i] == 'z3' :
            z3Idx = i
            break
    
    solverNum = len(solvers)
    caseNum   = len(data[0][0])
    
    # trim and sort
    trim = []
    for i in range(solverNum) :
        idx,sat,time = [],[],[]
        for j in range(caseNum) :
            if data[i][1][j] != 't' and data[i][1][j] != 'x' :
                if   data[i][1][j] == '1' :
                    if data[z3Idx][1][j] == '0' : continue
                elif data[i][1][j] == '0' :
                    if data[z3Idx][1][j] == '1' : continue
                idx.append(data[i][0][j])
                sat.append(data[i][1][j])
                time.append(float(data[i][2][j]))
        time.sort()
        trim.append(time)
    
    plotCumTime(benchmark,r_dir,trim,solvers,plt,mpat)
    
    for i in range(len(solvers)) :
        if solvers[i] == 'ic3ia' or solvers[i] == 'abc' :
            plotScatter(benchmark,r_dir,data[i],solvers[i],plt,mpat)

def plotScatter(benchmark,r_dir,data,solver,plt,mpat) :
    unsat,sat = [ [] for x in range(2) ] , [ [] for x in range(2) ]
    for i in range(len(data[1])) :
        if   data[1][i] == '0' :
            unsat[0].append(data[3][i])
            unsat[1].append(data[2][i])
        elif data[1][i] == '1' :
            sat[0].append(data[3][i])
            sat[1].append(data[2][i])

    plt.scatter(sat[0],sat[1],c='b')
    plt.title('%s (SAT Cases) %s : Time vs Step Scatter Plot (# of case = %d)' %(benchmark.title(),solver.title(),len(sat[0])))
    plt.xlabel('Step Count')
    plt.ylabel('Run Time (s)')
    plt.savefig('%s/sat_step_%s.jpg' %(r_dir,solver),dpi=DPI)
    plt.cla()
    plt.clf()
    
    plt.scatter(unsat[0],unsat[1],c='b')
    plt.title('%s (UNSAT Cases) %s : Time vs Frame Scatter Plot (# of case = %d)' %(benchmark.title(),solver.title(),len(unsat[0])))
    plt.xlabel('Frame Index')
    plt.ylabel('Run Time (s)')
    plt.savefig('%s/unsat_frame_%s.jpg' %(r_dir,solver),dpi=DPI)
    plt.cla()
    plt.clf()

def plotCumTime(benchmark,r_dir,data,solvers,plt,mpat) :
    # plot cumulative time vs case index
    from matplotlib.lines import Line2D
    import colorsys
    import math
    
    markers = []
    for m in Line2D.markers :
        try:
            if len(m) == 1 and m != ' ':
                markers.append(m)
        except TypeError :
            pass

    N = 8
    #HSV_tuples = [(x*1.0/N, 0.5, x*(1.0-1.0/N)) for x in range(N)]
    HSV_tuples = [(abs(math.sin(x*math.pi/N)), 0.5, abs(math.cos(x*math.pi/N))) for x in range(N)]
    rgb = map(lambda x: colorsys.hsv_to_rgb(*x), HSV_tuples)
    k = 256.0
    colorMap = {'Slender-u' : (96/k,104/k,128/k), 
                'Slender-b'   : (220/k,20/k,60/k),
                'Z3STR3' : (121/k,128/k,32/k),
                'CVC4'  : (204/k,0/k,255/k),
                'S3P'   : (0/k,255/k,170/k),
                'ABC'   : (255/k,136/k,0/k),
                'TRAU'   : (0/k,204/k,255/k),
                'Norn'  : (34/k,139/k,34/k)}
    '''
    colorMap = {'ic3ia' : rgb[0], 
                'abc'   : rgb[1],
                'z3'    : rgb[2],
                'cvc4'  : rgb[3],
                's3p'   : rgb[4],
                'ABC'   : rgb[5],
                'fat'   : rgb[6],
                'norn'  : rgb[7]}
    '''
    solverIdxMap = { 'ic3ia' : 0,
                     'abc'   : 1,
                     'z3'    : 2,
                     'cvc4'  : 3,
                     'norn'  : 4,
                     's3p'   : 5,
                     'ABC'   : 6,
                     'fat'   : 7}
    tmpdata,tmpsolver = [[] for x in range(8)],[[] for x in range(8)]
    for i in range(8) :
        tmpdata[solverIdxMap[solvers[i]]] = data[i]
        tmpsolver[solverIdxMap[solvers[i]]] = solvers[i]

    data = tmpdata
    solvers = tmpsolver

    # change solver name
    for i in range(len(solvers)) :
        if   solvers[i] == 'ic3ia' : solvers[i] = 'Slender-u'
        elif solvers[i] == 'abc'   : solvers[i] = 'Slender-b'
        elif solvers[i] == 'z3'    : solvers[i] = 'Z3STR3'
        elif solvers[i] == 'norn'  : solvers[i] = 'Norn'
        elif solvers[i] == 's3p'   : solvers[i] = 'S3P'
        elif solvers[i] == 'fat'   : solvers[i] = 'TRAU'
        elif solvers[i] == 'cvc4'  : solvers[i] = 'CVC4'

    csum = []
    for i in range(len(data)) :
        d = np.array(data[i],dtype=float)
        if solvers[i] == 'z3' : tmpsolver
        np.cumsum(d,out=d)
        csum.append(d)

    fig = plt.figure()
    ax  = plt.subplot(111)
    for i in range(len(csum)) :
        ax.plot(np.arange(len(csum[i])),csum[i],color=colorMap[solvers[i]],label=solvers[i])
        #ax.plot(np.arange(len(csum[i])),csum[i],marker=markers[i],markersize=3,linewidth=0.1,color=colorMap[solvers[i]],label=solvers[i])
    box = ax.get_position()
    ax.set_position([box.x0,box.y0,box.width*0.8,box.height])
    ax.legend(loc='center left',bbox_to_anchor=(1,0.5))
    plt.ylabel('Time (s)')
    plt.xlabel('Number of Solved Instances')
    plt.savefig('%s/%s.jpg' %(r_dir,benchmark.lower()),dpi=DPI)
    plt.cla()
    plt.clf()

##############################################################################

def parse(argv) :
    if   argv[0] == '--h' : opt_help(argv)
    elif len(argv) == 2 : opt2(argv)
    elif len(argv) <= 4 : opt3(argv)
    else                :
        sys.exit('[ERROR::parse] invalid argc=%d' %(len(argv)))

def opt_help(argv) :
    print ('''
    --k=kaluza --t=testing --p=pisa --a=appscan
    
    --build      < --k | --t | --p | --a >
    --clean      < --k | --t | --p | --a >
    --cc         < --k | --t | --p | --a >
    --plot       < --k | --t | --p | --a >
    
    --execmd     < --k | --t | --p | --a > < --r2d | --d2b  | --b2v | --cmd | --all >
    
    --solve      < --k | --t | --p | --a > 
                 < --ic3ia | --cvc4 | --z3 | --s3p | --abc | --ABC | --fat | --norn | --all >
                 [ time out (default=20s) ]
    
              ''')

def opt2(argv) :
    if argv[1] not in benchmark_abbr_set :
        sys.exit('[ERROR::opt2] invalid opt=%s' %(argv[1]))
    t = benchmark_abbr_map[argv[1]]
    if   argv[0] == '--build'   : buildMap(t)
    elif argv[0] == '--clean'   : clean(t)
    elif argv[0] == '--cc'      : ConsistencyChecking(t)
    elif argv[0] == '--plot'    : plot(t)
    else                        : sys.exit('[ERROR::opt2] invalid opt=%s' %(argv[0]))

def opt3(argv) :
    if   argv[0] == '--execmd' : dgIdx,dgFiles = getSplitMap('experiment/%s/map' %(benchmark_abbr_map[argv[1]]));\
                                 opt_execmd(argv[2],dgFiles)
    elif argv[0] == '--solve'  : 
        t = benchmark_abbr_map[argv[1]]
        if len(argv) == 3 : opt_solve(t,argv[2],Default_TO)
        else              : opt_solve(t,argv[2],float(argv[3]))
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

def opt_solve(benchmark,opt,TO) :
    if   opt == '--all' : solvers = exp_solver_list
    else                : solvers = [ opt[ opt.rfind('-') + 1 : ] ]
    dgIdxList, dgFileList = getSplitMap('experiment/%s/map' %(benchmark))
    for solver in solvers :
        exp(benchmark,dgIdxList,dgFileList,solver,TO)

def mergeFile(mapFile,kaluza=0) :
    lines = file2lines(mapFile)
    cnt = 0
    for i in range(1,len(lines)) :
        cnt += 1
        path = (lines[i].split(','))[1]
        print ('path=%s' %(path))
        
        if kaluza == 1 :
            call('mkdir -p laut/kaluza/%d' %(cnt),shell=True)
            call('cp %s/sink.smt2 laut/kaluza/%d' %(path,cnt), shell=True)
            call('cp %s/sink.s3   laut/kaluza/%d' %(path,cnt), shell=True)
            laut = open('laut/kaluza/%d/laut' %(cnt),'w')
        else :
            laut = open('%s/laut' %(path),'w')

        aut  = file2lines('%s/aut'  %(path))
        pred = file2lines('%s/pred' %(path))
        cmd  = file2lines('%s/cmd'  %(path))
        
        for l in aut : laut.write('%s\n' %(l))
        laut.write(';\n')

        section = 0
        for l in pred :
            if l == ';' :
                section = 1
                laut.write(';\n')
                continue
            if section == 0 :
                v = l.split()
                laut.write('%s %s\n' %(v[1],v[3][0:-1]))
            else :
                if l[-1] == ')' :
                    laut.write('%s\n' %(l[8:-1]))
                else :
                    v = l.split()
                    if   v[3] == '(str.len' : laut.write('(trklen %s %s)\n' %(v[2],v[7]))
                    elif v[2] == '(str.len' : laut.write('(trklen %s %s)\n' %(v[4][0:-1],v[7]))
                    elif v[3] == '(str.indexof' : laut.write('(trkidx %s %s %s %s)\n' %(v[6][0:-3],v[2],v[9],v[10]))
                    elif v[2] == '(str.indexof' : laut.write('(trkidx %s %s %s %s)\n' %(v[5][0:-1],v[6][0:-2],v[9],v[10]))
                    elif v[1] == '(str.substr'  : laut.write('(substr %s %s %s %s)\n' %(v[3],v[4][0:-2],v[7],v[8]))

        laut.write(';\n')

        for l in cmd :
            if l == 'write beforeaddpred' : continue
            if l == 'addpred' or l == 'addpred pred' :
                laut.write('addpred\nwrite sink\nisempty sink')
                break
            v = l.split()
            if v[0] == 'addlen' : v[0] = 'trklen'
            elif v[0] == 'indexof' : v[0] = 'trkidx'
            laut.write('%s' %(v[0]))
            for j in range(1,len(v)) : laut.write(' %s' %(v[j]))
            laut.write('\n')
        laut.close()

if __name__ == '__main__' :
    #init()
    #parse(sys.argv[1:])
    mergeFile(sys.argv[1],1)
