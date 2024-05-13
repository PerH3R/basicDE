import pandas as pd
import numpy as np


#TODO: exclude keywords in avg_keyword_rank | selelct column to average (default auc-avg-rank)


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

def avg_keyword_rank(keyword="", banword="", col_name="AUC-AVG-RANK"):
	count = 0
	rank = 0
	for index, row in combined_df.iterrows():
		if row['ID'].find(keyword) > 0:
			if not (row['ID'].find(banword) > 0 and banword==""):
				# print(row['ID'], "true")
				count += 1
				rank += row[col_name]
		# else:
		# 	print(row['ID'], "false")
	if count > 0:
		return rank/count
	return -1

print("====ALL====")
print("keyword\t\t dim5\t dim20\t dim5&20")
print('_ (75): \t', 	f'{avg_keyword_rank("_", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("_", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("_"):.2f}')
print("====NON-MAB====")
print("keyword\t\t dim5\t dim20\t dim5&20")
print('Bin (16): \t', 		f'{avg_keyword_rank("Bin", col_name="AUC-5-RANK"):.2f}', 		"\t", f'{avg_keyword_rank("Bin", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("Bin"):.2f}')
print('Rand_search(1):\t', 	f'{avg_keyword_rank("search", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("search", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("search"):.2f}')
print('Rand_vec (1): \t', 	f'{avg_keyword_rank("vectors", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("vectors", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("vectors"):.2f}')
print('Rand_op (1): \t', 	f'{avg_keyword_rank("operators", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("operators", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("operators"):.2f}')
print("====OPERATOR SELECTOR====")
print("keyword\t\t dim5\t dim20\t dim5&20")
print('sel0 (28): \t', 	f'{avg_keyword_rank("sel0", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("sel0", col_name="AUC-20-RANK"):.2f}', "\t",f'{avg_keyword_rank("sel0"):.2f}')
print('sel1 (28): \t', 	f'{avg_keyword_rank("sel1", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("sel1", col_name="AUC-20-RANK"):.2f}', "\t",f'{avg_keyword_rank("sel1"):.2f}')
print("====CREDIT ASSIGNER====")
print("keyword\t\t dim5\t dim20\t dim5&20")
print('crd0 (14): \t', 	f'{avg_keyword_rank("crd0", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("crd0", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("crd0"):.2f}')
print('crd1 (14): \t', 	f'{avg_keyword_rank("crd1", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("crd1", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("crd1"):.2f}')
print('crd2 (14): \t', 	f'{avg_keyword_rank("crd2", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("crd2", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("crd2"):.2f}')
print('crd3 (14): \t', 	f'{avg_keyword_rank("crd3", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("crd3", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("crd3"):.2f}')
print("====LEARNING PERIOD====")
print("keyword\t\t dim5\t dim20\t dim5&20")
print('lp1 (8): \t', 	f'{avg_keyword_rank("lp1", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("lp1", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp1"):.2f}')
print('lp5 (8): \t', 	f'{avg_keyword_rank("lp5", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("lp5", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp5"):.2f}')
print('lp10 (8): \t', 	f'{avg_keyword_rank("lp10", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("lp10", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp10"):.2f}')
print('lp20 (8): \t', 	f'{avg_keyword_rank("lp20", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("lp20", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp20"):.2f}')
print('lp50 (8): \t', 	f'{avg_keyword_rank("lp50", col_name="AUC-5-RANK"):.2f}', 	"\t", f'{avg_keyword_rank("lp50", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp50"):.2f}')
print('lp100 (8): \t', 	f'{avg_keyword_rank("lp100", col_name="AUC-5-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp100", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp100"):.2f}')
print('lp200 (8): \t', 	f'{avg_keyword_rank("lp200", col_name="AUC-5-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp200", col_name="AUC-20-RANK"):.2f}', "\t", f'{avg_keyword_rank("lp200"):.2f}')
