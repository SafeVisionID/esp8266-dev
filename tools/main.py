# -*- coding: utf-8 -*-

import os
import sys
import subprocess as sp
from PyQt5 import QtWidgets

def prepare_gui():
	if not os.path.exists('Ui_esp8266tool.py'):
		print("Preparing GUI")
		if os.path.exists('esp8266tool.ui'):
			print("Compile GUI againts PyQt5")
			pyuic = sp.Popen('pyuic5 esp8266tool.ui > Ui_esp8266tool.py', shell=True,stdout=sp.PIPE)
			output = pyuic.communicate()[0]
			retcode = pyuic.returncode
			
			if retcode!=0:
				print("Comile GUI failed")
				sys.exit()
			else:
				print("Compile GUI Success")
		else:
			print("required GUI doesn't exist'")
			sys.exit()
	else:
		print("GUI is ready")
	
	print("Launch main process")

if __name__ == "__main__":
	prepare_gui()
	from esp8266tool import MainWindow
	
	app = QtWidgets.QApplication(sys.argv)
	app.setStyle('Fusion')
	ui = MainWindow()
	ui.show()
	sys.exit(app.exec_())
