import socket

class Commutator:
    end_line = "\r\n"

    def __init__(self, hostname = "localhost", port = "1234"):
        self.port = int(port)
        self.host = hostname
        self.connected = False
        self.buffer_size = 1024
        self.buffer = []
        self.socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

        
    def up_connection(self):
        try:        
            self.socket.connect((self.host,self.port))
            self.connected = True
            return True
        except socket.error:
            print "Error, problems with connection to host!"
            return False

    def send_all(self,msg):
        msg += self.end_line
        try:
            if (self.connected):
                self.socket.sendall(msg) 
        except:
            print "Error, problems with sending!"

    def recv_all(self):
        try:
            self.buffer = self.socket.recv(self.buffer_size)
            if(len(self.buffer) > 0):
                index = self.buffer.find(self.end_line)
                if (index != -1):
                    self.buffer = self.buffer.replace(self.end_line,'')
                return str(self.buffer)
            else:
                raise socket.error
        except:
            print "Error, problems with receiving!"
            return ""

    def exchange(self,request):
        if(self.connected):
            self.send_all(request)
            return self.recv_all()
        else:
            return ""

    def hand_shake(self,hand_shake_msg):
        if(self.connected):
            self.send_all(hand_shake_msg)
            print self.recv_all()
            return True
        else:
            return False

    def down_connection(self,last_msg):
        if(self.connected):
            self.send_all(last_msg)
            self.connected = False
            self.socket.close()
            return True
        else:
            return False


#cm = Commutator("192.168.0.2","2001")
#cm.up_connection()
#cm.hand_shake("0")
#cm.down_connection("EOG")

