# expected structure
#	/root
#		rank.py
#		/result-folder
#			/results-m1-d5...
#			/results-m2-d5...
#			/results-m3-d5...

# what it do?
# go over every json file and record avg. 
# sort this according to 
#     func 1
#         dim 5
#             DE config 1
#             DE config 2
#         dim 20
#             DE config 1 
#             DE config 2
#     func ...
#         dim ...
#             DE config ...
#             etc.
# then avg_ranking goes trough this dictionary and 



# TODO remove Cr=0

import os
import json
import copy
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

locations = [
            "./m0",
            "./m1", "./m2"
            ,"./m3", "./m4","./m5", 
            "./m6","./m7", "./m8","./m9", "./m10",
            "./m11", "./m12","./m13", "./m14","./m15",
            # "./m16"
            ]

# settings
calc_median = True # else Average
filter_Cr0 = True
rank_dims = [5]
rank_funcs = list(range(1,25))

# global variables
values = {}

# ranker
def avg_ranking(values, funcs, dims):
    ranks = copy.deepcopy(values)
    scores = copy.deepcopy(values)
    for func in ranks.keys():
        for dim in ranks[func].keys():
            scores[func][dim] = [v for k, v in sorted(ranks[func][dim].items(), key=lambda item: item[1])]
            ranks[func][dim] = [k for k, v in sorted(ranks[func][dim].items(), key=lambda item: item[1])]


    funcs = [str(i) for i in funcs]
    dims = [str(i) for i in dims]
    # print(ranks)
    # print("/n")
    # print(values)
    combined = copy.deepcopy(values[funcs[0]][dims[0]])
    for c in combined.keys():
        combined[c] = []
    print(len(combined.keys()))

    # add all ranks
    for func in ranks.keys():
        for dim in ranks[func].keys():
            if (func in funcs) and (dim in dims): #filter out unwanted dimensions or functions
                first_tie = 0
                streak = False
                # current_score = float('-inf')
                for i in range(len(ranks[func][dim])-1):
                    current_score = ranks[func][dim][i]
                    next_score = ranks[func][dim][i+1]

                    
                    if next_score == current_score:     # tie
                        if streak:  # additional ties
                            pass    # continue looking for further ties
                        else:       # new tie
                            streak = True
                            # first_tie = i
                    else:                               # no tie with next value
                        shared_score = (first_tie + i) / 2
                        for conf_name in range(first_tie, i+1): # get config names from postion of first tie up to and including position i                            
                            combined[ranks[func][dim][conf_name]].append(shared_score)
                        streak = False
                        first_tie = i+1
                last_i = len(ranks[func][dim])
                shared_score = (first_tie + last_i) / 2
                for conf_name in range(first_tie, last_i): # get config names from postion of first tie up to and including position i 
                    combined[ranks[func][dim][conf_name]].append(shared_score)


    combined_final = copy.deepcopy(combined)
    for c in combined_final.keys():
        # print(c, combined_final[c])
        if calc_median == True:
            combined_final[c] = np.median(combined[c])
        else:
            combined_final[c] = np.average(combined[c])
        # print(c, combined_final[c], '\n')
        # combined[c] /= len(funcs) * len(dims)
    combined_final = {k: v for k, v in sorted(combined_final.items(), key=lambda item: item[1])}
    return combined_final

# dict combiner
def combine_into(d: dict, combined: dict) -> None:
    for k, v in d.items():
        if isinstance(v, dict):
            combine_into(v, combined.setdefault(k, {}))
        else:
            combined[k] = v

# data collector
for location in locations:
    print("===============================================")
    print(location)
    for dp, dn, filenames in os.walk(location):
        for f in filenames:
            if os.path.splitext(f)[1] == '.json':
                data = json.load(open(os.path.join(dp, f), 'r'))
                name = data["algorithm"]["name"]
                args = data["algorithm"]["info"].split(" ")
                func = str(data["function_id"])
                if not (filter_Cr0 and (args[args.index("-Cr")+1] == "0")):
                    for s in data["scenarios"]:
                        dim = str(s["dimension"])
                        count = 0
                        res = []
                        for r in s["runs"]:
                            count = max(r["instance"], count)
                            res.append(float(r["best"]["y"]))
                        avg = np.average(res)
                        this_dict = {func: {dim: {name: avg}}}
                        combine_into(this_dict, values)

    

    print("dimensions:", rank_dims)
    print("functions:", rank_funcs)
    print("total number of configurations: ", end='')
    final_ranking = avg_ranking(values, rank_funcs, rank_dims)
    print(final_ranking)
    # for x in json.dumps(avg_ranking(rankings, rank_funcs, rank_dims)):
    #     print(x)

    if True:
        F_values = []
        Cr_values = []
        framename = ""
        savename = ""
        if calc_median:
            framename += "Median "
            savename += "./median/"
            os.makedirs(os.path.dirname(savename), exist_ok=True)
        else:
            framename += "Average "
            savename += "./average/"
        savename += '-'.join([str(d) for d in rank_dims]) + '/'
        os.makedirs(os.path.dirname(savename), exist_ok=True)

        # gather configuration data to initialize dataframe
        first_config = True
        for config_name in final_ranking.keys():
            name_split = config_name.split("_")
            if first_config:
                framename += name_split[0]
                first_config = False
            for param in name_split:
                if param[0] == 'F' and param[1:] not in F_values:
                    F_values.append(param[1:])
                if param[:2] == 'Cr' and param[2:] not in Cr_values:
                    Cr_values.append(param[2:])
        F_values.sort()
        Cr_values.sort()
        print(F_values)
        print(Cr_values)
        frame = pd.DataFrame(index=F_values, columns=Cr_values, dtype=float)

        best_f, best_cr = "", ""
        # fill in values
        for config_name in final_ranking.keys():
            name_split = config_name.split("_")
            f_val = ""
            cr_val = ""
            for param in name_split:
                # save best score for highlight
                if param[0] == 'F':
                    f_val = param[1:]                    
                    if best_f == "":
                        best_f = f_val
                if param[:2] == 'Cr':
                    cr_val = param[2:]
                    if best_cr == "":
                        best_cr = cr_val
            frame[cr_val][f_val] = float(final_ranking[config_name])

        # filter cr=0.0 #TODO rerun without cr=0.0
        # if location != "./m16" and filter_Cr0:
        #     frame = frame.drop('0', axis=1)

        print("Dataframe:")
        print("\\/F \t Cr>")
        print(frame)
        frame = frame[::-1]
        framename += " " + ','.join([str(d) for d in rank_dims])

        num_NaN = frame.isnull().sum().sum()

        # generate heatmap
        plt.title(framename)
        ax = sns.heatmap(frame, annot=True, fmt='.1f', vmin=0.0, vmax=(len(frame.columns)*len(frame.index)-1.0-num_NaN))
        # highlight lowest value
        # if filter_Cr0:
        #     ax.add_patch(Rectangle((Cr_values.index(best_cr)-1, F_values.index(best_f)),1,1, fill=False, edgecolor='green', lw=2))
        # else:
        ax.add_patch(Rectangle((Cr_values.index(best_cr), F_values[::-1].index(best_f)),1,1, fill=False, edgecolor='green', lw=2))
        savename += location.split('/')[1][1:] + "-" + framename.replace("/", "") + ".png"
        plt.xlabel("Cr")
        plt.ylabel("F")
        plt.savefig(savename)
        plt.clf()
    # clear values
    values = {}