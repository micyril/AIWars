from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from subprocess import Popen

import time


class ServerTester:
	def __init__(self, driver, drivers_count, server_ip, server_port = 2560, start_id):
		self.drivers = []
		self.processes = []
		self.drivers_count = drivers_count
		self.start_id = start_id
		for i in range(drivers_count):
			self.drivers.append(driver())
		
		self.server_ip = server_ip
		self.server_port = server_port

	def start_testing(self, tests_count):
		for i in range(tests_count):
			for j in range(self.drivers_count):
				self.drivers[j].get("http://" + self.server_ip + "/");
				body = self.drivers[j].find_element_by_tag_name("body")
				body.send_keys(Keys.COMMAND + 't')
				#body.send_keys(Keys.CONTROL + 't')

		for i in range(tests_count * self.drivers_count):
			args_str = str(i + start_id) + " " + str(self.server_ip) + " " + str(self.server_port)
			self.processes.append(Popen("python testClient/robotExample.py " + args_str , shell=True))

	def wait_and_stop_testing(self, timeout):
		time.sleep(timeout)
		for driver in self.drivers:
			driver.close()

		for proc in self.processes :
			proc.kill()




tester = ServerTester(webdriver.Firefox,3,"87.251.167.242","2560",0)
tester.start_testing(4)
tester.wait_and_stop_testing(120)