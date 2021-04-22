#!/usr/bin/env python3
import os 
import pandas as pd
import matplotlib.pyplot as plt 

def plot(stride_list, accessed_size_meta_list, latency_meta_list, figure_name='output.png', **kwargs):
	if 'xlabel' not in kwargs: kwargs['xlabel'] = 'Accessed size (!= Arraysize)'
	if 'ylabel' not in kwargs: kwargs['ylabel'] = 'Latency (rdtsc_ticks)'
	if 'title' not in kwargs: kwargs['title'] = 'Pointer-chasing microbenchmark'
	if 'label' not in kwargs: kwargs['label'] = '??'

	label_list = ['64 B','128 B','256 B','512 B','1 kB','2 kB','4 kB','8 kB','16 kB','32 kB','64 kB','128 kB','256 kB']

	for i in range(len(stride_list)):
		stride = stride_list[i]
		latency_list = latency_meta_list[i]
		accessed_size_list = accessed_size_meta_list[i]
		plt.plot(accessed_size_list, latency_list, label = label_list[i])

	plt.xscale('log',base=2)
	plt.xlabel(kwargs['xlabel'])
	plt.ylabel(kwargs['ylabel'])
	plt.title(kwargs['title'])

	xtick_range = [64 * (2 ** x) for x in range(21)]
	xtick_names = ['64 B','128 B','256 B','512 B','1 kB','2 kB','4 kB','8 kB','16 kB','32 kB','64 kB','128 kB','256 kB','512 kB','1 MB','2 MB','4 MB','8 MB','16 MB','32 MB','64 MB'] #for KB

	plt.xticks(
		xtick_range,
		xtick_names,
		rotation='-30',
		size='small' 
	)
	plt.legend()
	plt.grid()
	plt.tight_layout()
	plt.savefig(figure_name, dpi=300)

def main():
	from sys import argv
	if len(argv) < 1:
		print("Usage time")	

	time = argv[1]
	stride_list = os.listdir(os.path.join('result', time))
	stride_list = [int(x) for x in stride_list]
	stride_list.sort()

	access_size_meta_list = []
	latency_meta_list = []

	for stride in stride_list:
		csv_name = os.listdir(os.path.join('result', time, str(stride)))[0]
		df_path = os.path.join('result', time, str(stride), csv_name)
		df = pd.read_csv(df_path)
		access_size = df['Accessed_size'].to_list()
		latency = df['Latency'].to_list()
		access_size_meta_list.append(access_size)
		latency_meta_list.append(latency)



	plot(stride_list, access_size_meta_list, latency_meta_list, figure_name=time+'.png')

if __name__ == '__main__':
	main()
