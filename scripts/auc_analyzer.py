import pandas as pd
import numpy as np

#import data
dim5_df = pd.read_csv("./AUC_ECDFEAF_MULTI_DIM5.csv")
dim20_df = pd.read_csv("./AUC_ECDFEAF_MULTI_DIM20.csv")

#rename columns in preperation of merging
dim5_df = dim5_df.rename(columns={'Runtime' : 'Runetime5', 
									'AUC' : 'AUC-5',
									'AOC' : 'AOC-5'})

dim20_df = dim20_df.rename(columns={'Runtime' : 'Runetime20', 
									'AUC' : 'AUC-20',
									'AOC' : 'AOC-20'})

#combine df's
print(dim5_df.shape)
print(dim20_df.shape)
combined_df = pd.merge(dim5_df, dim20_df, on=["ID"])
print(combined_df)


combined_df = combined_df.sort_values(by=['AUC-5'], ascending=False)
combined_df.insert(4, 'AUC-5-RANK', range(combined_df.shape[0]))
combined_df = combined_df.sort_values(by=['AUC-20'], ascending=False)
combined_df.insert(combined_df.shape[1], 'AUC-20-RANK', range(combined_df.shape[0]))
print(combined_df)

#add avg AUC and rank
combined_df['AUC-AVG'] = combined_df[['AUC-5', 'AUC-20']].mean(axis=1)
combined_df['AUC-AVG-RANK'] = combined_df[['AUC-5-RANK', 'AUC-20-RANK']].mean(axis=1)

print(combined_df)

def avg_keyword_rank(keyword=""):
	count = 0
	rank = 0
	for index, row in combined_df.iterrows():
		if row['ID'].find(keyword) > 0:
			# print(row['ID'], "true")
			count += 1
			rank += row['AUC-AVG-RANK']
		# else:
		# 	print(row['ID'], "false")
	if count > 0:
		return rank/count
	return -1

print('Bin:', str(avg_keyword_rank("Bin")))
print('sel0:', str(avg_keyword_rank("sel0")))
print('sel1:', str(avg_keyword_rank("sel1")))
print("=========")
print('crd0:', str(avg_keyword_rank("crd0")))
print('crd1:', str(avg_keyword_rank("crd1")))
print('crd2:', str(avg_keyword_rank("crd2")))
print('crd3:', str(avg_keyword_rank("crd3")))
print("=========")
print('lp1:', str(avg_keyword_rank("lp1")))
print('lp5:', str(avg_keyword_rank("lp5")))
print('lp10:', str(avg_keyword_rank("lp10")))
print('lp20:', str(avg_keyword_rank("lp20")))
print('lp50:', str(avg_keyword_rank("lp50")))
print('lp100:', str(avg_keyword_rank("lp100")))
print('lp200:', str(avg_keyword_rank("lp200")))