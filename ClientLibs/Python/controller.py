import commutator
import commands
import protocol
class Controller:
    def __init__(self,host="localhost",port="2345",ID = 0):
        self.ID = str(ID)
        self.comm = commutator.Commutator(host,port)
        self.MoveCommand = commands.MoveCommand()
        self.RotateCommand = commands.RotateCommand()
        self.FireCommand = commands.FireCommand()
        self.ScanCommand = commands.ScanCommand()
    
    def state_process(self,state):
        if(state == -1):
            print "End of Game!"
            self.end_game()
            return -1
        if(state == 1):
                return 1
        if(state == 0):
                return 0
        return 0

    def ready(self):
        if(self.comm.up_connection()):
            return self.comm.hand_shake(protocol.Protocol.get_simple_msg_line(str(self.ID)))
        else:
            return False

    def end_game(self):
        return self.comm.down_connection(protocol.Protocol.get_simple_msg_line("EOG"))

    def move(self,destination):
        request = self.MoveCommand.produce_request(self.ID,destination)
        response = self.comm.exchange(request)
        return self.state_process(self.MoveCommand.parse_response(response))

    def rotate(self,destination):
        request = self.RotateCommand.produce_request(self.ID,destination)
        response = self.comm.exchange(request)
        return self.state_process(self.RotateCommand.parse_response(response))

    def fire(self):
        request = self.FireCommand.produce_request(self.ID)
        response = self.comm.exchange(request)
        return self.state_process(self.FireCommand.parse_response(response))

    def scan(self):
        request = self.ScanCommand.produce_request(self.ID)
        response = self.comm.exchange(request)
        answer = self.ScanCommand.parse_response(response)
        self.state_process(answer.state)
        return answer


