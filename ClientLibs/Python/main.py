import controller

ctrl = controller.Controller("192.168.0.2","2001","0")

if(ctrl.ready()):
    print "start"
    for i in range(1000):
        print ctrl.move(0.1),ctrl.fire(),ctrl.rotate(10),ctrl.scan().state
    ctrl.end_game()
    print "end"
        
