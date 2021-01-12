#!/usr/bin/env python3

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
	from csv import DictReader as csvread
	from sys import argv
	if len(argv) < 2:
		print("Usage file.csv label")

	result = []
	with open(argv[1], 'r') as f:
		reader = csvread(f)
		for row in reader:
			result.append((int(row['Accessed_size']),int(row['Latency'])))
	outfilename=argv[1].strip('.csv')+'.png'
	plot(result,figure_name=outfilename,label=argv[2])

if __name__ == '__main__':
	main()
