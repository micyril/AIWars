#include "controller.h"


Controller::Controller(std::string host , std::string port, std::string ID){
    this->ID = Tools::FromStrConverter<int>::convert(ID);
    this->comm = Commutator(host,port);
}

int Controller::state_process(int state){
    switch(state){
        case -1:
            std::cout << "End of Game !" << std::endl;
            this->comm.down_connection();
            //exit(0);
        case 1:
        case 0:
            return state;
    }
    return 0;
}

bool Controller::ready(){
    if(this->comm.up_connection()){
        return this->comm.hand_shake(Tools::ToStrConverter<int>::convert(this->ID));
    } else return false;
}

bool Controller::end_game(){
    return this->comm.down_connection();
}

int Controller::move(float destination){
    std::string request = this->move_cmd.produce_request(this->ID,destination);
    std::string response = this->comm.exchange(request);
    return this->state_process(this->move_cmd.parse_response(response));
}

int Controller::rotate(float angle){
    std::string request = this->rotate_cmd.produce_request(this->ID,angle);
    std::string response = this->comm.exchange(request);
    return this->state_process(this->rotate_cmd.parse_response(response));
}

int Controller::fire(){
    std::string request = this->fire_cmd.produce_request(this->ID);
    std::string response = this->comm.exchange(request);
    return this->state_process(this->fire_cmd.parse_response(response));
}

ScanCommandAnswer Controller::scan(){
    std::string request = this->scan_cmd.produce_request(this->ID);
    std::string response = this->comm.exchange(request);
    ScanCommandAnswer answer = this->scan_cmd.parse_response(response);
    this->state_process(answer.state);
    return answer;
}
