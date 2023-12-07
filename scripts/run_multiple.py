import os
import subprocess
from tqdm import tqdm

num_threads = 20

dim = [5,20]
budgets = [2000]
#M_range = [0,1,2,3,4,5,7,8,9,99]
adaptation_mode = 0;
M_range = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15] #16

F_kononova = [0.1, 0.15, 0.2, 0.3, 0.5, 0.8, 0.9, 1.0]
F_zaharie = [0.05, 0.2, 0.4, 0.7, 0.9]
Cr_zaharie = [0.05, 0.4, 0.7, 0.9, 0.99]
Cr_per = [0, 0.05, 0.1, 0.2, 0.5, 0.8, 0.9, 1.0] # values changes smaller than 0.05 == useless for dim < 20, rates always slightly higher than rates because of minimum 1 dimension crossing over
F_per = [0.05, 0.1, 0.2, 0.5, 0.8, 0.9, 1.0, 1.1, 1.2]

commands = []
com2 = []

for m in M_range:
    for F in F_per:
        for Cr in Cr_per:
            for budget in budgets:
                for d in dim:
                    com2.append(["./main", "-a", str(adaptation_mode), "-F", str(F), "-Cr", str(Cr), "-budget", str(budget), "-m", str(m), "-d", str(d)])
                    line = " ".join(["./main", "-a", str(adaptation_mode), "-F", str(F), "-Cr", str(Cr), "-budget", str(budget), "-m", str(m), "-d", str(d)])
                    commands.append(line)

#random parameters #do with optimal cr f values found
# for m in M_range:
#     for F in F_per:
#         for Cr in Cr_per:
#             for budget in budgets:
#                 for d in dim:
#                     com2.append(["./main", "-a", str(adaptation_mode), "-F", str(F), "-Cr", str(Cr), "-budget", str(budget), "-m", str(m), "-d", str(d)])
#                     line = " ".join(["./main", "-a", str(adaptation_mode), "-F", str(F), "-Cr", str(Cr), "-budget", str(budget), "-m", str(m), "-d", str(d)])
#                     commands.append(line)

#random search
# for d in dim:
#     com2.append(["./main", "-a", str(adaptation_mode), "-budget", str(budget), "-m", str(15), "-d", str(d)])
#     line = " ".join(["./main", "-a", str(adaptation_mode), "-budget", str(budget), "-m", str(15), "-d", str(d)])
#     commands.append(line)

print(commands)
processes = set()
devnull = open(os.devnull, 'w')
with open(os.devnull, 'w') as devnull:
    for c in tqdm(com2):
        processes.add(subprocess.Popen((c), stdout=devnull, stderr=devnull))
        if len(processes) >= num_threads:
            os.wait()
            processes.difference_update([
                p for p in processes if p.poll() is not None])

#wait for all to close
for p in processes:
    if p.poll() is None:
        p.wait()