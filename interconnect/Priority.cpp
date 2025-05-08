#include <iostream>
#include <bits/stdc++.h>
#include <queue>
#include "msg.hpp"
/**
 * @struct CompareQoS
 * @brief Comparador para ordenar mensajes por nivel de QoS.
 * 
 * Este comparador define el criterio para que la cola de prioridad
 * coloque primero los mensajes con mayor QoS (calidad de servicio).
 */
struct CompareQoS {
    /**
     * @brief Operador de comparación para mensajes.
     * @param a Primer mensaje.
     * @param b Segundo mensaje.
     * @return true si a.qos < b.qos (es decir, b tiene mayor prioridad).
     */
    bool operator()(const msg& a, const msg& b) const {
        return a.qos < b.qos;  // (mayor QoS primero)
    }
};
/**
 * @class Priority
 * @brief Cola de prioridad basada en el campo QoS de los mensajes.
 * 
 * Esta clase encapsula una priority_queue que almacena objetos de tipo `msg`
 * y los ordena automáticamente de forma descendente por el campo `qos`.
 */
class Priority{
    private:
        std::priority_queue<msg, std::vector<msg>, CompareQoS> pQueue;

    public:
        /**
         * @brief Agrega un mensaje a la cola.
         * 
         * @param message Mensaje que se desea insertar.
         */
        void addMsg(msg message){
            pQueue.push(message);
        }
        /**
         * @brief Extrae el mensaje con mayor QoS.
         * 
         * @return El mensaje con más alta prioridad (mayor `qos`).
         * 
         * @warning No verifica si la cola está vacía antes de acceder. 
         * El usuario debe asegurar que hay mensajes disponibles.
         */
        msg getMsg(){
            msg message = pQueue.top();
            pQueue.pop();
            return message;
        }
};