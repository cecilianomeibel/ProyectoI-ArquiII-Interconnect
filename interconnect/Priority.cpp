#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include "msg.h"

struct CompareQoS {
    bool operator()(const msg& a, const msg& b) const {
        return a.QoS < b.QoS;  // (mayor QoS primero)
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