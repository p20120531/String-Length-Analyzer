import sys
import re
from subprocess import call
from os import listdir
from os.path import isdir, join

#ePath = '/media/DATA/StrangerProject/Workspace/PatcherWorker/script/regex-blif'
ePath = './script/regex-blif'
fPath = './1001.corecstrs.readable/'
subPath = [join(fPath,f) for f in listdir(fPath) if isdir(join(fPath, f))]
for sp in subPath :
    print sp
for sp in subPath :
    sp = sp + '/'
    autFile = sp + 'aut'
    print 'autFile = %s' %(autFile)
    with open(autFile) as infile:
        bfList,nList = [],[]
        for line in infile :
            v = line.split()
            bfList.append(v[0])
            nList.append(v[1])
            print 'regex=%s , path=%s' %(v[2],sp+v[0])
            call('./%s \"/%s/\" %s.blif' %(ePath,v[2],sp+v[0]),shell=True)
        for i in range(len(bfList)) :
            print 'bpath=%s , vpath=%s' %(sp+bfList[i]+'.blif',sp+bfList[i]+'.vmt')
            call('python ./blif2vmt.py %s.blif %s.vmt %s' %(sp+bfList[i],sp+bfList[i],nList[i]),shell=True)
