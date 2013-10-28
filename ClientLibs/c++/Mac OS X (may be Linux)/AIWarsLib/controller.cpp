#include "controller.h"


Controller::Controller(std::string host , std::string port, std::string ID){
    this->ID = Tools::FromStrConverter<int>::convert(ID);
    this->comm = Commutator(host,port);
}

bool Controller::state_process(int state){
    switch(state){
        case -1:
            std::cout << "End of Game !" << std::endl;
            this->comm.down_connection();
            exit(0);
            break;
        case 0:
            return true;
        case 1:
            return false;
        default:
            return false;
    }
}

bool Controller::ready(){
    if(this->comm.up_connection()){
        return this->comm.hand_shake(Tools::ToStrConverter<int>::convert(this->ID)+"\r\n");
    } else return false;
}

bool Controller::end_game(){
    return this->comm.down_connection();
}

bool Controller::move(float destination){
    std::string request = this->move_cmd.produce_request(this->ID,destination);
    std::string response = this->comm.exchange(request);
    return this->state_process(this->move_cmd.parse_response(response));
}

bool Controller::rotate(float angle){
    std::string request = this->rotate_cmd.produce_request(this->ID,angle);
    std::string response = this->comm.exchange(request);
    return this->state_process(this->rotate_cmd.parse_response(response));
}

bool Controller::fire(){
    std::string request = this->fire_cmd.produce_request(this->ID);
    std::string response = this->comm.exchange(request);
    return this->state_process(this->fire_cmd.parse_response(response));
}

std::vector<WorldObject> Controller::scan(){
    std::string request = this->scan_cmd.produce_request(this->ID);
    std::string response = this->comm.exchange(request);
    return this->scan_cmd.parse_response(response).second;
}
