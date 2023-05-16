# expected structure
#	/root
#		rank.py
#		/result-folder
#			/results-m1-d5...
#			/results-m2-d5...
#			/results-m3-d5...

import os
import json



for dp, dn, filenames in os.walk("./"):
	for f in filenames:
		if os.path.splitext(f)[1] == '.json':
			data = json.load(f)
			for s in data["scenarios"]:
				
