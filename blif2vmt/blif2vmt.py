import sys

def writeSingleCube( outFile , cube , curOutputName , varList ) :
    outFile.write('\n(define-fun %s () Bool (and' %(curOutputName))
    for i in range(len(cube)) :
        if cube[i] == '1' :
            outFile.write(' %s' %(varList[i]))
        elif cube[i] == '0':
            outFile.write(' (not %s)' %(varList[i]))
        else :
            print '%s is DC' %(varList[i])
    outFile.write('))')
    
def writeMultipleCube( outFile , cube , intermediateCnt , varList ) :
    outFile.write('\n(define-fun g%d () Bool (and' %(intermediateCnt))
    for i in range(len(cube)) :
        if cube[i] == '1' :
            outFile.write(' %s' %(varList[i]))
        elif cube[i] == '0':
            outFile.write(' (not %s)' %(varList[i]))
        else :
            print '%s is DC' %(varList[i])
    outFile.write('))')

blifFile,vmtFile,lcvar = sys.argv[1:4]

stateCnt,intermediateCnt,isFirst,inputCnt = 0,0,True,0
varList,cubeList = [],[]
curOutputName = ''

outFile = open(vmtFile,'wb')
outFile.write('(set-info :source |printed by MathSAT|)')
with open(blifFile,'rb') as infile:
    for line in infile :
        print 'line = ' ,line
        v = line.split()
        print 'v = ' ,v
        if len(v) == 0 : continue
        if v[0] == '.inputs' :
            for i in range(1,len(v)) :
                if v[i][0] == 'i' :
                    inputNum = int(v[i][1:])
                    inputNum += 1
                    outFile.write('\n(declare-fun x%d () Bool)' %(inputNum)
                    #outFile.write('\n(declare-fun x%s () Bool)' %(v[i][1:]))
                elif v[i][0] == 's':
                    stateCnt += 1
                    outFile.write('\n(declare-fun %s () Bool)' %(v[i]))
                elif v[i][0] == 'n':
                    outFile.write('\n(declare-fun s%s.next () Bool)' %(v[i][1:]))
                else:
                    print 'WARNING: inesist char appears!!'
        elif v[0] == '.outputs':
            outFile.write('\n(declare-fun %s () Int)' %(lcvar))
            outFile.write('\n(declare-fun %s.next () Int)' %(lcvar))
            for i in range(stateCnt) :
                intermediateCnt += 1
                outFile.write('\n(define-fun g%d () Bool (! s%d :next s%d.next))' %(intermediateCnt,i+1,i+1))
            outFile.write('\n(define-fun lc () Bool (! %s :next %s.next))' %(lcvar,lcvar))
        elif v[0] == '.names' or v[0] == '.end':
            if isFirst == False :
                if len(cubeList) == 1 :
                    writeSingleCube(outFile,cubeList[0],curOutputName,varList)
                else :
                    begin = intermediateCnt + 1
                    for cube in cubeList :
                        intermediateCnt += 1
                        writeMultipleCube(outFile,cube,intermediateCnt,varList)
                    outFile.write('\n(define-fun %s () Bool (or' %(curOutputName))
                    for i in range(begin,intermediateCnt+1) :
                        outFile.write(' g%d' %(i))
                    outFile.write('))')
            
            if isFirst == True : isFirst = False
            varList = []
            cubeList = []
            curOutputName = v[-1].upper()
            for i in range(1,len(v)-1) :
                if v[i][0] == 'n' :
                    s = 's%s.next' %(v[i][1:])
                    varList.append(s)
                elif v[i][0] == 'i' :
                    s = 'x%s' %(v[i][1:])
                    varList.append(s)
                else :
                    varList.append(v[i])
            print varList

        elif v[0][0] == '0' or v[0][0] == '1' or v[0][0] == '-' :
            if len(v) != 2 :
                print 'WARNING: cube line should be length 2'
            if len(v[0]) != len(varList) :
                print 'WARNING: #varialbe should be matched'
            cubeList.append(v[0])
            
outFile.write('\n(define-fun d0 () Bool (! T :trans true))')
outFile.write('\n(define-fun d1 () Bool (! I :init true))')
outFile.write('\n(define-fun d2 () Bool (! (not O) :invar-property 0))')
