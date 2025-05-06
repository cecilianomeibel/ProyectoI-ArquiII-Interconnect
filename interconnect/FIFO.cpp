#include <iostream>
#include <bits/stdc++.h>
#include "msg.h"

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