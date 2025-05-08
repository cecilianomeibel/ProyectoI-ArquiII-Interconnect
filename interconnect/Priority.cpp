#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include "msg.hpp"

struct CompareQoS {
    bool operator()(const msg& a, const msg& b) const {
        return a.qos < b.qos;  // (mayor QoS primero)
    }
};

class Priority{
    private:
        std::priority_queue<msg, std::vector<msg>, CompareQoS> pQueue;

    public:
        void addMsg(msg message){
            pQueue.push(message);
        }

        msg getMsg(){
            msg message = pQueue.top();
            pQueue.pop();
            return message;
        }
};