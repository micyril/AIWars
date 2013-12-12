import controller

ctrl = controller.Controller("192.168.0.105","2560","1")

if(ctrl.ready()):
    print "start"
    for i in range(10000):
        print ctrl.move(0.1),ctrl.fire(),ctrl.rotate(-2),ctrl.scan().state
    ctrl.end_game()
    print "end"
        
