
class Message:
## ACK = 1, NAK = 0, EOG = -1
    end_line = "\r\n"
    def __init__(self,head = "", raw_command = "", args_line = ""):
        self.head = head
        self.raw_command = raw_command
        self.args_line = args_line
        

class Protocol:
    @staticmethod
    def get_msg_by_line(line):
        index = line.find(Message.end_line)
        if (index != -1):
            line = line.replace(Message.end_line,'')
        words = line.split(' ',2)
        result_msg = Message()
        words_ln = len(words)
        if(words_ln == 3):
            result_msg.args_line = words[2]
        if(words_ln > 1 & words_ln < 4):
            result_msg.raw_command = words[1]
        if(words_ln > 0 & words_ln < 4):
            result_msg.head = words[0]
        return result_msg
    
    @staticmethod
    def get_line_by_msg(msg):
        return msg.head+" "+msg.raw_command+" "+msg.args_line+" "+Message.end_line
    
    @staticmethod
    def get_line_by_fields(head,raw_command,args_line):
        return head+" "+raw_command+" "+args_line+" "+Message.end_line
    @staticmethod
    def get_simple_msg_line(str_msg):
        return str_msg+Message.end_line;

    


