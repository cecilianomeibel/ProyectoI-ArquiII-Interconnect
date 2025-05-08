#include "interconnect.hpp"

Interconnect::Interconnect(){
    scheduler = 1; //FIFO por defecto
    counter = 0;
};
Interconnect& Interconnect::getInstance(){
    static Interconnect interconnect;
    return interconnect;
};

void Interconnect::set_scheduler(int sched){
    scheduler = sched;
};

void Interconnect::schedule(msg message){
    if (scheduler == 1)
    {
        fifo.addMsg(message);
    }
    priority.addMsg(message);
};

void Interconnect::write_mem(uint8_t SRC, uint64_t ADDR, uint32_t NUM_OF_CACHE_LINES, uint32_t START_CACHE_LINE, uint8_t QoS){
    msg message = {};
    message.src = SRC;
    message.num_of_cache_lines = NUM_OF_CACHE_LINES;
    message.start_cache_line = START_CACHE_LINE;
    message.qos = QoS;
    schedule(message);

    msg exec_msg;
    if(scheduler == 1){
        exec_msg = fifo.getMsg();
    }else {
        exec_msg = priority.getMsg();    
    }
    std::cout << "[WRITE_INTERCONNECT] Ejecutando mensaje SRC: " << (int)exec_msg.src << ", ADDR: " << exec_msg.addr
    << ", CACHE_LINES: " << exec_msg.num_of_cache_lines << ", QoS: " << exec_msg.qos << std::endl;

};
void Interconnect::read_mem(uint8_t SRC, uint64_t ADDR, uint32_t SIZE, uint8_t QoS){
    msg message = {};
    message.src = SRC;
    message.addr = ADDR;
    message.size = SIZE;
    message.qos = QoS;
    schedule(message);

    msg exec_msg;
    if(scheduler == 1){
        exec_msg = fifo.getMsg();
    }else {
        exec_msg = priority.getMsg();    
    }
    std::cout << "[READ_INTERCONNECT] Ejecutando mensaje SRC: " << (int)exec_msg.src << ", ADDR: " << exec_msg.addr
    << ", SIZE: " << exec_msg.size << ", QoS: " << exec_msg.qos << std::endl; 
};
void Interconnect::broadcast_invalidate(uint8_t SRC, uint32_t CACHE_LINE, uint8_t QoS){
    msg message = {};
    message.src = SRC;
    message.cache_line = CACHE_LINE;
    message.qos = QoS;
    schedule(message);

    msg exec_msg;
    if(scheduler == 1){
        exec_msg = fifo.getMsg();
    }else {
        exec_msg = priority.getMsg();    
    }
    std::cout << "[BROADCAST_INTERCONNECT] Ejecutando mensaje SRC: " << (int)exec_msg.src << ", CACHE_LINE: " << exec_msg.cache_line
    << ", QoS: " << exec_msg.qos << std::endl; 

};
void Interconnect::inv_ack(uint8_t SRC, uint8_t QoS){
    msg message = {};
    message.src = SRC;
    message.qos = QoS;
    schedule(message);

    msg exec_msg;
    if(scheduler == 1){
        exec_msg = fifo.getMsg();
    }else {
        exec_msg = priority.getMsg();    
    }
    std::cout << "[INV_ACK INTERCONNECT] Ejecutando mensaje SRC: " << (int)exec_msg.src << ", QoS: " << exec_msg.qos << std::endl; 
};
void Interconnect::inv_complete(uint8_t DEST, uint8_t QoS){
    msg message = {};
    message.dest = DEST;
    message.qos = QoS;
    schedule(message);
};
void Interconnect::read_resp(uint8_t DEST, uint32_t DATA, uint8_t QoS){

};
void Interconnect::write_resp(uint8_t DEST, uint8_t STATUS, uint8_t QoS){

};