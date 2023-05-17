# expected structure
#	/root
#		rank.py
#		/result-folder
#			/results-m1-d5...
#			/results-m2-d5...
#			/results-m3-d5...

import os
import json
import copy
import numpy as np

values = {}

def avg_ranking(ranks, funcs, dims):
    funcs = [str(i) for i in funcs]
    dims = [str(i) for i in dims]
    combined = copy.deepcopy(values[funcs[0]][dims[0]])
    for c in combined.keys():
        combined[c] = 0

    for func in ranks.keys():
        for dim in ranks[func].keys():
            if func in funcs and dim in dims:
                for i in range(len(rankings[func][dim])):
                    combined[rankings[func][dim][i]] += i+1

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

for dp, dn, filenames in os.walk("./result1/"):
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
        # print(rankings[func][dim])
        rankings[func][dim] = {k: v for k, v in sorted(rankings[func][dim].items(), key=lambda item: item[1])}
        rankings[func][dim] = list(rankings[func][dim].keys())

rank_dims = [5]
rank_funcs = list(range(1,25))
print(rank_dims, rank_funcs)
print(json.dumps(avg_ranking(rankings, rank_funcs, rank_dims)))
