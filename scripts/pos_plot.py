import csv
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
from collections import Counter
import numpy as np
from tqdm import tqdm


# filename = "1_Sphere_5_01"
filename = "1_Sphere_5_11"
timeline = []

with open("./positions/" + filename, 'r') as readfile:
	for line in readfile:
		current_generation = []
		population = line.split(';')
		for individual in population:
			individual_positions = individual.split(',')
			if individual_positions[0] != '\n':
				individual_positions = individual_positions[0:-1]
				individual_positions = [float(x) for x in individual_positions]
				current_generation.append(individual_positions)
		timeline.append(current_generation)


gen_counter = 0
for generation in tqdm(timeline):
	plt.xlim(-5, 5)
	plt.ylim(-5, 5)
	for individual in generation:
		# print(individual)
		plt.scatter(individual[0], individual[1])
	# plt.show()
	save_location = "./pictures/" + filename + "/" + str(gen_counter) + ".png"
	plt.savefig(save_location)
	gen_counter += 1
	plt.clf()