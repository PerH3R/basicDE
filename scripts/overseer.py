import os
import subprocess
import sys
import shutil
import pandas as pd

run_type = "in" #in|out
opslist = []
webserver_port = 0
base_result_dir = os.getcwd() + "/results/a2m0/"
exp_name = ""

def refine():
    table_path = os.path.abspath("./table/filtered/" + exp_name + '/')
    with open(table_path + "/table_5.csv") as dim5, open(table_path + "/table_20.csv") as dim20:
        df5 = pd.read_csv(dim5)
        df20 = pd.read_csv(dim20)
        df5.rename(columns = {'Runtime': 'Runtime-5', 
                                'AUC': 'AUC-5', 
                                'AOC': 'AOC-5'}, inplace=True)
        df20.rename(columns = {'Runtime': 'Runtime-20', 
                                'AUC': 'AUC-20', 
                                'AOC': 'AOC-20'}, inplace=True)
        print(df5)
        print(df20)

        max_auc5 = max(df5['AUC-5'])
        min_auc5 = min(df5['AUC-5'])
        print(min_auc5, max_auc5)
        df5['val_norm-5'] = (df5['AUC-5'] - min_auc5) / (max_auc5 - min_auc5)
        df5.sort_values('val_norm-5', ascending=False, inplace=True)
        df5['rank-5'] = list(range(len(df5['AUC-5'])))
        print(df5)
        max_auc20 = max(df20['AUC-20'])
        min_auc20 = min(df20['AUC-20'])
        print(min_auc20, max_auc20)
        df20['val_norm-20'] = (df20['AUC-20'] - min_auc20) / (max_auc20 - min_auc20)
        df20.sort_values('val_norm-20', ascending=False, inplace=True)
        df20['rank-20'] = list(range(len(df20['AUC-20'])))
        print(df20)
        merged = pd.merge(df5, df20, on='ID')
        print(merged)
        merged['AUC-avg'] = (merged['AUC-5'] + merged['AUC-20']) / 2
        merged['rank-avg'] = (merged['rank-5'] + merged['rank-20']) / 2
        merged['val_norm-avg'] = (merged['val_norm-5'] + merged['val_norm-20']) / 2
        merged.sort_values('val_norm-avg', ascending=False, inplace=True)
        print(merged)
        print(merged['ID'][0][3:])
        return merged['ID'][0][3:]


def move_new_folders():
    folders = os.listdir(base_result_dir)
    print(folders)
    folders = [f for f in folders if f[0] == '-']
    print(folders)
    new_path = base_result_dir + '/' + run_type + '/' + exp_name + '/'
    print(new_path)
    if not os.path.exists(new_path):
        os.makedirs(new_path)
    for f in folders:
        print(base_result_dir +  f)
        print(new_path + f)
        shutil.move(base_result_dir + '/' + f, new_path + f)
    shutil.make_archive(base_result_dir + '/' + run_type + '/' + exp_name, 'zip', new_path)
    shutil.move(base_result_dir + '/' + run_type + '/' + exp_name + '.zip', new_path + exp_name + '.zip')
    return new_path + exp_name + '.zip'


while len(opslist) != 16:
    exp_name = str(len(opslist)) + run_type

    # initiate experiment run
    pf_command = ["python3"]
    pf_command.append(os.getcwd() + "/run_portfolio.py")
    pf_command.append("type=" + str(15-len(opslist))) 
    pf_command.append("ops=") 

    for ops in opslist:
        pf_command[-1] += str(ops) + ','
    if pf_command[-1][-1] == ',':
        pf_command[-1] = pf_command[-1][:-1]
    print(pf_command)
    p = subprocess.Popen((pf_command), stdout=sys.stdout, stderr=sys.stderr)
    #wait for done
    p.wait()

    # prepare crude results
    path_to_zip = move_new_folders()

    # get filtered data from webserver
    req_command = ["python3"]
    req_command.append(os.getcwd() + "/IOHrequester.py")
    req_command.append("port=" + str(webserver_port))
    req_command.append("zip=" + path_to_zip)
    req_command.append("zip=" + path_to_zip)
    p = subprocess.Popen((req_command), stdout=sys.stdout, stderr=sys.stderr)
    p.wait()

    # refine data
    best_result = refine()

    # update opslist
    added = 0
    print("from:", opslist)
    if run_type == "in":
        for idx in range(len(best_result)):
            if (best_result[idx] == '1') and (idx not in opslist):
                opslist.append(idx)
                added += 1
    if run_type == "out":
        for idx in range(len(best_result)):
            if (best_result[idx] == '0') and (idx not in opslist):
                opslist.append(idx)
                added += 1
    if added > 1:
        print("WARNING: Added more than one operator <================================")
    print("to: ", opslist)