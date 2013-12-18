import controller
import random
import time
import signal
import sys

class Robot:
	def __init__(self, ID, IP, PORT):
		self.controller = controller.Controller(IP,PORT,ID)
		self.angle = 0

	def mainLoop(self):
		while(True):
			self.performStrategy()
		

	def turn_on(self):
		if(self.controller.ready()):
			self.mainLoop()

	def turn_off(self):
		self.controller.end_game()
		exit(0)

	########### STRATEGY ##########
	def performStrategy(self):
		(targets,obstacles) = self.scanningTactics()
		if(len(targets) != 0):
			self.attackingTactics(targets, 1)
		if(len(obstacles) != 0):
			self.evasionTactics()
		self.movingTactics(5.0, 3.0)
	########### ######## ########## 

	########### TACTICS ###########
	def scanningTactics(self):
		scanAnswer = self.controller.scan()
		if(scanAnswer.state == -1):
			self.turn_off()
		objectsLength = len(scanAnswer.scannedWorldObjects)
		if(objectsLength == 0):
			return ([],[])

		targets = []
		obstacles = []
		for i in range(objectsLength):
			if(scanAnswer.scannedWorldObjects[i].object_type == "RobotMapElement"):
				targets.append(scanAnswer.scannedWorldObjects[i])
		return (targets,obstacles)

	def evasionTactics(self):
		state = 1
		scanAnswer = self.controller.scan()
		objectsLength = len(scanAnswer.scannedWorldObjects)
		while (objectsLength != 0):
			state *= self.controller.rotate(3.0)
			scanAnswer = self.controller.scan()
			objectsLength = len(scanAnswer.scannedWorldObjects)

	def attackingTactics(self,targetList, attacksCount):
		state = 1
		for i in range(attacksCount):
			for target in targetList:
				state *= self.controller.rotate(target.angle)
				time.sleep(1)
				state *= self.controller.fire()
				state *= self.controller.rotate(-target.angle)
				time.sleep(1)
			if(state == -1):
				self.turn_off()

	def movingTactics(self,rotationAngle, movingDistance):
		state = 1
		rotationMagic = 1
		moveMagic = 1
		#if(random.random() > 0.5):
		#	rotationMagic = -1
		#if(random.random() > 0.5):
		#	moveMagic = -1
		self.angle += rotationAngle
		state *= self.controller.rotate(rotationAngle * rotationMagic)
		state *= self.controller.move(movingDistance * moveMagic)
		if(state == -1):
				self.turn_off()
	########### ######## ###########



if(len(sys.argv) == 4):
	robot = Robot(sys.argv[1],sys.argv[2],sys.argv[3])

	def ctrlc_handler(signum, frame):
	    robot.turn_off()
	    exit(0)

	signal.signal(signal.SIGINT, ctrlc_handler)

	robot.turn_on()
else:
	print "TRY: programm_name <ID> <IP> <PORT>"
	exit(0)
