import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt


# portfolio_size = 15
# ops_perms = ["".join(seq) for seq in itertools.product("01", repeat=16) if seq.count('1') == portfolio_size]
# ops_perms = [int(x, 2) for x in ops_perms]


base_path = "./Qresults/a2m0/11out/-ops"
config = "0000000101010110"
complete_path = base_path + config

dim20 = []
dim5 = []
dim20_normalized = []
dim5_normalized = []



def plotpie(runs, operators):
	print(runs)
	print(runs.shape)
	data = np.average(runs, axis = 0)
	print(data)
	print(data.shape)

	it_sum = sum(data)
	for i in range(len(data)):
		data[i] = data[i] / it_sum

	print(len(operators))
	plt.title(config)
	plt.pie(data, labels=operators, autopct='%1.1f%%')
	plt.savefig(config + '.png')




def postprocess(runs):
	for line in range(len(runs)):
		for idx in range(len(runs[line])):
			if runs[line][idx] == float(-1):
				runs[line][idx] = runs[line-1][idx]
	return runs

def process_file(file, runs, operators):
	print(file)

	#seperate the runs
	with open(file, 'r') as opened_file:
		for line in opened_file:
			# print(line)
			splitline = line.split(',')
			splitline[-1] = splitline[-1].replace("\n", "")
			for idx in range(len(splitline)):
				if splitline[idx] == '':
					splitline[idx] = -1

			if splitline[0] == "Budget": #new header
				if operators == []:
					operators = splitline[1:]
					print(operators)
				pass
				
			else:
				splitline = [float(x) for x in splitline[1:]]
				runs.append(splitline)
	return runs, operators

runs = []
operators = []
for file in os.listdir(complete_path):	
	if file.find("csv") != -1:
		runs, operators = process_file(complete_path + '/' + file, runs, operators)
	else:
		# raise Exception("Unrecognized file" + file)
		pass

runs = postprocess(runs)
plotpie(np.asarray(runs), operators)