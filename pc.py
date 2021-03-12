#!/usr/bin/env python3
from os import system as cmd
from os import getenv as env
from subprocess import check_output as cmd_out
from datetime import datetime as dt
import os
import sys

CL_SIZE = 64 
MAX_ITS = 1024 * 1024

cmd('make pc')
stride = int(env('stride',64))
max_region_size = int(env('max_region_size',32*1024*1024))
it_count = int(env('it_count',1024))

if stride < CL_SIZE: 
	print("Stride lower than cacheline size",file=sys.stderr)
	exit(1)


#Performing experiment
max_node_count = max_region_size/stride
i = 1
EXP_BASE = 2
EXP_INC = 0.25
exp = 0
node_count = pow(EXP_BASE,exp)
result = []
while node_count <= max_node_count:
	if (it_count * node_count) > (MAX_ITS): it_count //=2
	if (it_count ) == 1: it_count = 2
	command = []
	command.append('./pc') #TODO: make this not hardcoded
	command.append(str(stride))
	command.append(str(node_count))
	command.append(str(it_count))
	print(command,end='\r')
	latency = int(cmd_out(command).strip().split()[0].decode('utf-8'))
	latency //= node_count * it_count
	result.append((node_count*CL_SIZE,latency))
	exp+= EXP_INC
	node_count = int(pow(EXP_BASE,exp))


#Storing results
res_dir = os.path.join("result",str(stride))
if not os.path.exists(res_dir):
	 os.makedirs(res_dir)

dt_string = dt.now().strftime("%d-%m-%H-%M")
out_file_name = res_dir+'/'+dt_string
print("Writing to "+out_file_name)
out_file = open(out_file_name+'.csv','w')
out_file.write("Accessed_size,Latency\n")
for r in result:
	out_file.write(str(r[0])+','+str(r[1])+'\n')
out_file.close()

#Plotting results
import matplotlib.pyplot as plt 
from plot import plot
print("Plotting...") #TODO
print(result)
plot(result,figure_name=out_file_name+'.png',label=str(stride))
#x_axis = [t[0] for t in result]
#y_axis = [t[1] for t in result]
#plt.plot(x_axis, y_axis, label = str(stride))

#plt.xlabel('Accessed size (!= Arraysize)')
#plt.xscale('log')
#plt.ylabel('Latency (rdtsc ticks)')
#plt.title('Pointer-chasing microbenchmark')
#plt.legend()
#plt.savefig(out_file_name+'.png', dpi=300)
