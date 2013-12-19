from websocket import create_connection

ws = create_connection("ws://87.251.167.242")

while (True):
	ws.recv()