import protocol

class CommandAnswer:
    def __init__(self,state = 1):
        self.state = state

class WorldObject:
    def __init__(self,obj_type = "" , distance = 0.0 , angle = 0.0):
        self.object_type = obj_type
        self.distance = distance
        self.angle = angle


class ScanCommandAnswer(CommandAnswer):
    def __init__(self,scWorldsObjects = [] ,state = 1):
        self.scannedWorldsObjects = scWorldsObjects
        self.state = state


class Command:
    ACK = "ACK"
    NAK = "NAK"
    EOG = "EOG"
    RET = "RET"
    
    NAME=""
    
    def __init__(self):
        pass
    
    def produce_request(self,response):
        return response + protocol.Message.end_line

    def parse_response(self,response):
        if(response == ""):
            return -1
        
        msg = protocol.Protocol.get_msg_by_line(response)       

        if (msg.head == self.ACK):
            return 1
        if (msg.head == self.NAK):
            return 0
        if (msg.head == self.EOG):
            return -1
        return 0


class MoveCommand(Command):
    def __init__(self):
        self.NAME = "MOV"
    def produce_request(self,ID,dest):
        return Command.produce_request(self,protocol.Protocol.get_line_by_fields(str(ID),self.NAME,str(dest)))

class RotateCommand(Command):
    def __init__(self):
        self.NAME = "ROT"
    def produce_request(self,ID,angle):
        return Command.produce_request(self,protocol.Protocol.get_line_by_fields(str(ID),self.NAME,str(angle)))


class FireCommand(Command):
    def __init__(self):
        self.NAME = "FR"
    def produce_request(self,ID):
        return Command.produce_request(self,protocol.Protocol.get_line_by_fields(str(ID),self.NAME,""))

class ScanCommand(Command):
    def __init__(self):
        self.NAME = "SC"
    def produce_request(self,ID):
        return Command.produce_request(self,protocol.Protocol.get_line_by_fields(str(ID),self.NAME,""))

    def parse_response(self,response):
        answer = ScanCommandAnswer()
        if(response == ""):
            answer.state = -1
            return answer
        msg = protocol.Protocol.get_msg_by_line(response)
        if (msg.head == self.ACK):
            answer.state = 1
        if (msg.head == self.NAK):
            answer.state = 0
            return answer
        if (msg.head == self.EOG):
            answer.state = -1
            return answer

        if(msg.raw_command != self.RET):
            answer.state = 0;
            return answer

        args = msg.args_line.split(' ')
        parsed_args = []

        for i in range(2,len(args),3):
            parsed_args.append(WorldObject(args[i-2],float(args[i-1]),float(args[i])))

        answer.scannedWorldsObjects = parsed_args
        return answer


               






