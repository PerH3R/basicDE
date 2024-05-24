import os
import subprocess
import itertools
from tqdm import tqdm

budget = 10000
adaptation_mode = 2
num_threads = 10
portfolio_size = 15
ops_perms = ["".join(seq) for seq in itertools.product("01", repeat=16) if seq.count('1') == portfolio_size]
ops_perms = [int(x, 2) for x in ops_perms]

print(ops_perms)



com2 = []

for perm in ops_perms:
	com2.append(["./main", "-a", str(adaptation_mode), "-budget", str(budget), "-ops", str(perm)])




print(com2)
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