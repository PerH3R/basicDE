import numpy as np
import pandas as pd
import os
import matplotlib.pyplot as plt


# portfolio_size = 15
# ops_perms = ["".join(seq) for seq in itertools.product("01", repeat=16) if seq.count('1') == portfolio_size]
# ops_perms = [int(x, 2) for x in ops_perms]


base_path = "./Qresults/a2m0/-ops"
config = "1111111111111110"
complete_path = base_path + config

dim20 = []
dim5 = []
dim20_normalized = []
dim5_normalized = []


def process_file(file):
	print(file)
	runs = []
	num_runs = 1
	longest_run = 0
	#seperate the runs
	with open(file, 'r') as opened_file:
		temp = []
		for line in opened_file:
			# print(line)
			splitline = line.split(',')
			splitline[-1] = splitline[-1].replace("\n", "")
			# print(splitline)
			if splitline[0] == "Budget": #new header
				runs.append(temp)
				num_runs += 1
				# print("new")
				if temp != []:
					temp = []
					temp.append(splitline)
				
			else:				
				for i in range(len(splitline)):
					if splitline[i] == '':
						splitline[i] = splitline[i-1]
				# print(splitline)
				splitline = [float(x) for x in splitline]
				temp.append(splitline)
		# print(temp)
		# print("--")
		runs.append(temp)

	for run in runs:
		# print(len(runs))
		# print(len(run))
		if (len(run)) > longest_run:
			longest_run = len(run)
		for it in range(len(run[1:])):
			# print(run[it+1])
			it_sum = sum(run[it+1][1:])
			for i in range(len(run[it+1][1:])):
				run[it+1][i+1] = run[it+1][i+1] / it_sum

	print(longest_run)
	combined = np.zeros(longest_run)
	timeline = range(0, longest_run, 5)
	print(timeline)
	for run in runs:
		for it in range(len(run[1:])):
			for op in range(len(run[1:][it+1])):
				pass
		print('=================')


	pass

for file in os.listdir(complete_path):
	if file.find("DIM5") != -1:
		process_file(complete_path + '/' + file)
		pass
	elif file.find("DIM20") != -1:

		pass
	else:
		raise Exception("Unrecognized file" + file)