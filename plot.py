#!/usr/bin/env python3
import os 
import pandas as pd
import argparse





def plot(data_list,figure_name='output.png',**kwargs):
	if 'xlabel' not in kwargs: kwargs['xlabel'] = 'Accessed size (!= Arraysize)'
	if 'ylabel' not in kwargs: kwargs['ylabel'] = 'Latency (rdtsc_ticks)'
	if 'title' not in kwargs: kwargs['title'] = 'Pointer-chasing microbenchmark'
	if 'label' not in kwargs: kwargs['label'] = '??'
	import matplotlib.pyplot as plt 
	x_axis = [t[0] for t in data_list]
	y_axis = [t[1] for t in data_list]
	plt.plot(x_axis, y_axis, label = kwargs['label'])

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

	time = argv[1]

	if len(argv) < 1:
		print("Usage time")	

	stride_list = os.listdir('result')
	stride_list = [int(x) for x in stride_list]
	stride_list.sort()

	access_size_meta_list = []
	latency_meta_list = []

	for stride in stride_list:
		csv_name = os.listdir(os.path.join('result', str(stride)))[0]
		df_path = os.path.join('result', str(stride), csv_name)
		df = pd.read_csv(df_path)
		access_size = df['Accessed_size'].to_list()
		latency = df['Latency'].to_list()
		access_size_meta_list.append(access_size)
		latency_meta_list.append(latency)

	print(access_size_meta_list)


	# plot(result,figure_name=outfilename,label=argv[2])

if __name__ == '__main__':
	main()
