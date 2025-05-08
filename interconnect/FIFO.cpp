#include <iostream>
#include <bits/stdc++.h>
#include "msg.hpp"
/**
 * @class FIFO
 * @brief Cola FIFO (First-In, First-Out) para mensajes.
 * 
 * Esta clase implementa una cola simple donde los mensajes (`msg`) se insertan
 * al final y se extraen por el frente, siguiendo el orden en que fueron añadidos.
 */
class FIFO {
    private:
        std::queue<msg> queue;

    public:
        /**
         * @brief Agrega un mensaje al final de la cola.
         * 
         * @param message El mensaje que se desea insertar.
         */
        void addMsg(msg message){
            queue.push(message);
        }
        /**
         * @brief Extrae el primer mensaje de la cola.
         * 
         * @return El mensaje más antiguo (primero en entrar). 
         * Si la cola está vacía, devuelve un mensaje inválido con `{NULL}`.
         * 
         * @warning El valor de retorno `{NULL}` debe ser manejado externamente 
         * como indicador de cola vacía si no se usa `empty()`.
         */
        msg getMsg(){
            if(!queue.empty()){
                msg message = queue.front();
                queue.pop();
                return message;
            }
            return {NULL};
        }
        /**
         * @brief Verifica si la cola está vacía.
         * 
         * @return true si no hay mensajes en la cola, false en caso contrario.
         */
        bool empty(){
            return queue.empty();
        }
};