import os


for dp, dn, filenames in os.walk("./"):
	for f in filenames:
		if os.path.splitext(f)[1] == '.json':
			print(os.path.join(dp, f))
			data = open(os.path.join(dp, f), 'r').readlines()
			print(data[1])
			data[1] = data[1].replace("\"0.0.0\"", "\"0.3.9\"")
			print(data[1])
			with open(os.path.join(dp, f), 'w') as out:
				out.writelines(data)