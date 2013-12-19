
from subprocess import Popen

import time


class ServerTester:
	def __init__(self,server_ip, server_port, start_id):
		self.drivers = []
		self.processes = []
		self.start_id = start_id
		
		self.server_ip = server_ip
		self.server_port = server_port

	def start_testing(self, tests_count):
		for i in range(tests_count):
			print i
			self.processes.append(Popen("python ./testClient/webSocketClient.py", shell=True))


		for i in range(tests_count):
			args_str = str(i + self.start_id) + " " + str(self.server_ip) + " " + str(self.server_port)
			self.processes.append(Popen("python ./testClient/robotExample.py " + args_str , shell=True))

	def wait_and_stop_testing(self, timeout):
		time.sleep(timeout)

		for proc in self.processes :
			proc.kill()




tester = ServerTester("87.251.167.242","2560",32)
tester.start_testing(30)
tester.wait_and_stop_testing(300)