#include "msg.hpp"

class execute_unit{
    private:
        bool clk;
        void WRITE_M(msg message){
            //TODO usar funcion de escritura memoria
        };
        void READ_M(msg message){
            //TODO usar funcion de lectura memoria
        };
        void BROADCAST(msg message){
            
        };
        void INV_ACK(msg message){

        };
        void INV_COMPLETE(msg message){

        };
        void READ_RESP(msg message){

        };
        void WRITE_RESP(msg message){

        };
    
    public:
        void execute(msg message){
            switch (message.opcode){
                case 0:
                    WRITE_M(message);
                    break;
                case 1:
                    READ_M(message);
                    break;
                case 2:
                    BROADCAST(message);
                    break;
                case 3:
                    INV_ACK(message);
                    break;
                case 4:
                    INV_COMPLETE(message);
                    break;
                case 5:
                    READ_RESP(message);
                    break;
                case 6:
                    WRITE_RESP(message);
                    break;
                
                default:
                    break;
            }
        };

};