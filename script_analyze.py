from subprocess import call
call('mkdir -p result',shell=True)
call('python analyze.py SMTLIB/sat/small > result/sat.small',shell=True)
call('python analyze.py SMTLIB/sat/big > result/sat.big',shell=True)
call('python analyze.py SMTLIB/unsat/small > result/unsat.small',shell=True)
call('python analyze.py SMTLIB/unsat/big > result/unsat.big',shell=True)
