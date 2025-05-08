#include <iostream>
#include <bits/stdc++.h>
#include "msg.hpp"

class FIFO {
    private:
        std::queue<msg> queue;

    public:
        void addMsg(msg message){
            queue.push(message);
        }

        msg getMsg(){
            msg message = queue.front();
            queue.pop();
            return message;
        }
};