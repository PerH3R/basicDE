from requests import get
import time
import sys
import os
from bs4 import BeautifulSoup
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support import wait
from selenium.webdriver.support.select import Select
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.firefox.service import Service as FirefoxService
from webdriver_manager.firefox import GeckoDriverManager

# Before use:
# start up local IOHanalyzer webserver and note port down below

zip_path = "F:/Per/Documents/thesis/basicDE/scripts/tabler/14out.zip"
port = 4145
exp_name = ""
webserver = "http://127.0.0.1:" + str(port) + '/'
ssid = ""

for arg in sys.argv:
	print(arg)
	match (arg.split('=')[0]):
		case "port":
			port = arg.split('=')[1]
		case "exp_name":
			exp_name = arg.split('=')[1]
		case "zip":
			zip_path = arg.split('=')[1]
		case _:
			print("unknown arg", arg)



def wait_for_load(session):
	time.sleep(.2)
	loading = True
	while(loading):
		try: 
			(session.find_element(by=By.CLASS_NAME, value="shiny-busy"))
		except:
			loading = False
	time.sleep(.5)
	return


def zip_data(path):
	return zip_path

def upload_zip(session, path):	
	inp = session.find_element(by=By.ID, value="upload.add_zip")
	print("Uploading data...")
	inp.send_keys(os.path.abspath(path))
	# wait for data to finish loading
	print("Wait for data to finish loading")
	while (True):
		try:
			p_table = session.find_element(by=By.ID, value="DataTables_Table_0")
			print("Data loaded!")
			return
		except:
			time.sleep(1)

	return 

def get_ssid(session):
	local_ssid = ""
	while(local_ssid == ""):
		status = session.get(webserver)
		time.sleep(1)
		timeout_in_seconds = 10
		WebDriverWait(session, timeout_in_seconds).until(ec.presence_of_element_located((By.ID, 'ssid_token')))
		html = session.page_source
		soup = BeautifulSoup(html, features="html.parser")
		ssid_str = str(soup.find(id="process_data_promt"))

		if("ssid" in ssid_str):
			local_ssid = ssid_str.split(':')[-1].split('<')[0]
			print("ssid: ", local_ssid)
		else:
			print(soup.find(id="process_data_promt"))
			pass
	return local_ssid

def move_to_EAF(session):
	print("moving to EAF-CDF tab")
	sidebar = session.find_element(by=By.CLASS_NAME, value="sidebar-menu")
	EA_dropdown = sidebar.find_element(By.PARTIAL_LINK_TEXT, "Empirical Attainment")
	EA_dropdown.click()
	time.sleep(.5)
	sidebar = session.find_element(by=By.CLASS_NAME, value="sidebar-menu")
	EA_aggr = sidebar.find_element(By.PARTIAL_LINK_TEXT, "Multiple Functions")
	EA_aggr.click()
	time.sleep(.5)
	sidebar = session.find_element(by=By.CLASS_NAME, value="sidebar-menu")
	EAF_CDF = sidebar.find_element(By.PARTIAL_LINK_TEXT, "EAF-based CDF")
	EAF_CDF.click()
	wait_for_load(session)

def set_values(session, dim):
	print("setting values for Dim ", str(dim))
	wait_for_load(session)

	xrange_val = str(10000 * dim)

	# fill in desired dimension
	dimbox = session.find_element(By.ID, "Overall.Dim-selectized")
	dimbox.send_keys(webdriver.Keys.BACKSPACE)
	dimbox.send_keys(str(dim))
	dimbox.send_keys(webdriver.Keys.ENTER)

	wait_for_load(session)
	# allow range selection
	XCBrange = session.find_element(by=By.ID, value="EAF.MultiCDF.CustomRangeX")
	if not XCBrange.is_selected():
		XCBrange.click()	
	time.sleep(.5)

	# fill the max x range dependent on dim soze
	Xrange = session.find_element(by=By.ID, value="EAF.MultiCDF.xMax")
	Xrange.send_keys(webdriver.Keys.CONTROL + "a")
	Xrange.send_keys(webdriver.Keys.DELETE)
	Xrange.send_keys(xrange_val)

	refresh_btn = session.find_element(by=By.ID, value="EAF.MultiCDF.Refresh")
	refresh_btn.click()
	wait_for_load(session)
	
def download_file(url, file_path):
    reply = get(url, stream=True)
    with open(file_path, 'wb') as file:
        for chunk in reply.iter_content(chunk_size=1024): 
            if chunk:
                file.write(chunk)

def download_table(session, dim):
	dl_btn = session.find_element(By.ID, "EAF.AUC.Table.Download")
	url = dl_btn.get_attribute('href')
	dl_path = os.path.abspath("./table/filtered/" + exp_name + '/')
	if not os.path.exists(dl_path):
		os.makedirs(dl_path)
	if not exp_name == "":
		dl_path += "_{}".format(exp_name) 
	dl_path += "table_{:d}.csv".format(dim)
	download_file(url, dl_path)




options = webdriver.FirefoxOptions()
if not os.path.exists("./IOHrequester_DLs"):
		os.makedirs("./IOHrequester_DLs")
options.set_preference("browser.download.dir", os.path.abspath("./IOHrequester_DLs"))
# options.add_argument('--headless')
session = webdriver.Firefox(options=options, service=FirefoxService(GeckoDriverManager().install()))
# session.maximize_window();


ssid = get_ssid(session)
upload_zip(session, zip_path)
move_to_EAF(session)
set_values(session, 5)
download_table(session, 5)
set_values(session, 20)
download_table(session, 20)
session.quit()
