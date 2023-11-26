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
# then avg_ranking ranks 

import os
import json
import copy
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

locations = ["./m0",
            "./m1", "./m2","./m3", "./m4","./m5", 
            "./m6","./m7", "./m8","./m9", "./m10",
            "./m11", "./m12","./m13", "./m14","./m15"
            ]
values = {}

def avg_ranking(ranks, funcs, dims):
    funcs = [str(i) for i in funcs]
    dims = [str(i) for i in dims]
    print(values)
    combined = copy.deepcopy(values[funcs[0]][dims[0]])
    for c in combined.keys():
        combined[c] = 0
    print(len(combined.keys()))

    for func in ranks.keys():
        for dim in ranks[func].keys():
            if (func in funcs) and (dim in dims):
                # print(func, dim, len(rankings[func][dim]),'\n')
                # print(rankings[func][dim])
                # print('\n')
                for i in range(len(rankings[func][dim])):
                    combined[rankings[func][dim][i]] += i

    for c in combined.keys():
        combined[c] /= len(funcs) * len(dims)
    combined = {k: v for k, v in sorted(combined.items(), key=lambda item: item[1])}
    return combined


def combine_into(d: dict, combined: dict) -> None:
    for k, v in d.items():
        if isinstance(v, dict):
            combine_into(v, combined.setdefault(k, {}))
        else:
            combined[k] = v

for location in locations:
    print("===============================================")
    print(location)
    for dp, dn, filenames in os.walk(location):
        for f in filenames:
            if os.path.splitext(f)[1] == '.json':
                data = json.load(open(os.path.join(dp, f), 'r'))
                name = data["algorithm"]["name"]
                func = str(data["function_id"])
                for s in data["scenarios"]:
                    dim = str(s["dimension"])
                    count = 0
                    # if name == "1" and dim == "5" and func == "1":
                    #     pass
                    #     print(s["runs"])
                    res = []
                    for r in s["runs"]:
                        count = max(r["instance"], count)
                        res.append(float(r["best"]["y"]))
                        if name == "1" and dim == "5" and func == "1":
                            pass
                            # print(float(r["best"]["y"]))
                        # print(r["best"]["y"])
                    # print(avg, count, name, func, dim)
                    avg = np.average(res)
                    # print(avg, res)
                    this_dict = {func: {dim: {name: avg}}}
                    combine_into(this_dict, values)
    				# count = 0

    rankings = copy.deepcopy(values)
    for func in rankings.keys():
        for dim in rankings[func].keys():
            # print("===", func, dim)
            # print(rankings[func][dim], '\n')
            rankings[func][dim] = {k: v for k, v in sorted(rankings[func][dim].items(), key=lambda item: item[1])}
            # print(rankings[func][dim], '\n')
            rankings[func][dim] = list(rankings[func][dim].keys())
            # print(rankings[func][dim])

    rank_dims = [5,20]
    rank_funcs = list(range(1,25))
    print("dimensions:", rank_dims)
    print("functions:", rank_funcs)
    print("total number of configurations: ", end='')
    final_ranking = avg_ranking(rankings, rank_funcs, rank_dims)
    print(final_ranking)
    # for x in json.dumps(avg_ranking(rankings, rank_funcs, rank_dims)):
    #     print(x)

    if True:
        F_values = []
        Cr_values = []
        framename = ""
        # gather configuration data to initialize dataframe
        for config_name in final_ranking.keys():
            name_split = config_name.split("_")
            framename = name_split[0]
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
                if param[0] == 'F':
                    f_val = param[1:]
                    if best_f == "":
                        best_f = f_val
                if param[:2] == 'Cr':
                    cr_val = param[2:]
                    if best_cr == "":
                        best_cr = cr_val
            frame[cr_val][f_val] = float(final_ranking[config_name])
        print("Dataframe:")
        print("\\/F \t Cr>")
        print(frame)

        # generate heatmap
        plt.title(framename)
        ax = sns.heatmap(frame, annot=True, fmt='.1f', vmin=0.0, vmax=(len(frame.columns)*len(frame.index)-1.0))
        # highlight lowest value
        ax.add_patch(Rectangle((Cr_values.index(best_cr), F_values.index(best_f)),1,1, fill=False, edgecolor='green', lw=2))
        savename  = location.split('/')[1][1:] + "-" + framename.replace("/", "") + ".png"
        plt.xlabel("Cr")
        plt.ylabel("F")
        plt.savefig(savename)
        plt.clf()
    # clear values
    values = {}