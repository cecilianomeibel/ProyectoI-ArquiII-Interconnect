/**
 * @file interprete.cpp
 * @brief Módulo para interpretar un archivo de texto con instrucciones de múltiples procesadores (PEs).
 *        Cada línea puede ser una instrucción o una declaración de PE y QoS. Convierte el contenido
 *        del archivo en un arreglo dinámico de estructuras `instruction`.
 *
 * Estructura admitida en el archivo:
 *   PE<número> <QoS>
 *   <instrucción> <parámetros separados por coma>
 * 
 * Las instrucciones válidas son:
 *   - WRITE_MEM addr, num_lines, start_line
 *   - READ_MEM addr [, size]
 *   - BROADCAST_INVALIDATE cache_line
 *   - INV_ACK
 *   - INV_COMPLETE dest
 *   - READ_RESP dest, data
 *   - WRITE_RESP dest, status
 */
#include "interprete.hpp"
/// @brief Analiza y devuelve el código numérico de una instrucción textual.
/// @param text Cadena de texto que representa la instrucción completa.
/// @return Un número entero identificando la instrucción, o EXIT_FAILURE si no es válida.
int analicer_opcode(char *text){
        char text_op[32];
        const char* operations[] = {
            "WRITE_MEM", 
            "READ_MEM", 
            "BROADCAST_INVALIDATE",
            "INV_ACK", 
            "INV_COMPLETE", 
            "READ_RESP", 
            "WRITE_RESP"
        };
        int op_num = sizeof(operations) / sizeof(operations[0]); //len de las operaciones 
        int i = 0;
        while(text[i] != ' ' && text[i] != '\0' && text[i] != '\n'){ //Copia la op en el un char
        text_op[i] = text[i]; 
        i++;
        }
        text_op [i] = '\0';

        for(int j = 0; j < op_num ; j++ ) //Vericacion de que OPCODE tiene la linea

        if (strcmp(text_op, operations[j])==0){
            return j;
        }

        return EXIT_FAILURE;
}

/**
 * @brief Parsea una línea de instrucción y rellena una estructura `instruction`.
 * 
 * @param line Línea de texto con la instrucción y sus parámetros (modificada internamente con strtok).
 * @param inst Puntero a la estructura `instruction` que se llenará con los datos extraídos.
 * 
 * Se utiliza `strtok()` para dividir la línea en tokens. El campo `opcode` determina
 * qué parámetros deben interpretarse y almacenarse.
 */
void parse_instruction(char* line, instruction* inst){
    char* token = strtok(line," \n"); //De la linea saca la primer palabra, es decir la funcion (op)
    if(!token) return; //Verificar que hay opcode
    inst -> opcode = analicer_opcode(line); //Indentificar el opcode
    token = strtok(nullptr, ", \n"); //Declarando en el token el resto de parametos de la linea

    //Como ya sabemos que instruccion es ya sabemos que parametros usara 

    switch (inst->opcode)
    {
    case 0: // WRITE_MEM 
        if (token) { inst->addr = strtoull(token, nullptr, 0);       token = strtok(nullptr, ", \n"); }
        if (token) { inst->num_of_cache_lines = (uint32_t)strtoul(token, nullptr, 0);        token = strtok(nullptr, ", \n"); }
        if (token) { inst->start_cache_line = (uint32_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        break;

    case 1: // READ_MEM 
        if (token) { inst->addr = strtoull(token, nullptr, 0);       token = strtok(nullptr, ", \n"); }
        if (token) { inst->size = (uint32_t)strtoul(token, nullptr, 0); token = strtok(nullptr, ", \n"); }
        if (!token) {inst->size = 4;} //Si no se especifica el size 
        break;
    case 2: //BROADCAST_INVALIDATE
        if (token) { inst->cache_line = (uint32_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        break;
    case 3:
        break;    
    case 4://INV COMPLETE
        if (token) { inst->dest = (uint8_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        break;
    case 5://READ_RESP
        if (token) { inst->dest = (uint8_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        if (token) { inst->data = (uint32_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        break;
    case 6://WRITE_RESP
        if (token) { inst->dest = (uint8_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        if (token) { inst->status = (uint8_t)strtoul(token,nullptr,0);   token = strtok(nullptr, ", \n");}
        break;

    default:
        break;
    }



}
/**
 * @brief Carga y analiza un archivo de texto con instrucciones formateadas, generando un arreglo dinámico de instrucciones.
 * 
 * @param path Ruta al archivo que contiene las instrucciones.
 * @param size Puntero a entero donde se guardará la cantidad de instrucciones leídas exitosamente.
 * @return Puntero a un arreglo dinámico de estructuras `instruction`, o nullptr si hubo error de lectura o de memoria.
 * 
 * El arreglo devuelto debe ser liberado con `free()` después de usarse.
 * Esta función puede manejar archivos con más de 32 instrucciones, realocando memoria automáticamente.
 */
instruction *interpretate(const char* path, int* size){

    FILE* fp = std::fopen(path,"r");

    if(!fp){
        std::perror("Opening file failed");
    }
    instruction *instructions = (instruction*)malloc(128 * sizeof(instruction)); //esta forma pide la memoria necesaria para la lista de instrucciones.
    //instruction instructions[32]; 
    if (!instructions)
    {
        std::perror("Memory Allocation failed");
        return nullptr;
    }
    

    uint8_t current_src = 0xFF;
    uint8_t current_qos = 0xFF;
    int i = 0;
    char line[256];//Buffer del tamano de la linea 
    while (fgets(line,sizeof(line),fp))
    {
        if(line[0] == '\n' || line[0] == '\0' ) continue;//ignorar lineas en vacias
        printf("Analizando: %s \n",line);
        
        if (strncmp(line, "PE", 2) == 0) {//Declarar el PE y qos para todas la lineas
            int pe_num;
            uint8_t qos;
            if (sscanf(line, "PE%d %hhx", &pe_num, &qos) == 2) {
                current_src = (uint8_t)pe_num;
                current_qos = qos;
            } else {
                fprintf(stderr, "Formato PE inválido: %s", line);
            }
            continue;
        }
        instructions[i].src = current_src;
        instructions[i].qos = current_qos;

        parse_instruction(line,&instructions[i]);
        i++;
        
    }

    printf("%d \n",instructions[5].opcode);

    std::fclose(fp);
    *size = i;
   
    return instructions;   
}



/*
int main(){

    int count = 16;

    instruction* program = interpretate("code_test.txt");
    //Procesador cpu;

    if (program) {
        for (int i = 0; i < count; ++i) {
           
           printf(
            "inst #%d: op=%d,src=0x%02X, addr=0x%lX, size=%u, lines=%u, start_line=%u, cache_line=%u, dest=0x%02X, data=%u, status=0x%02X, qos=0x%02X\n",
            i,
            program[i].opcode,
            program[i].src,
            program[i].addr,
            program[i].size,
            program[i].num_of_cache_lines,
            program[i].start_cache_line,
            program[i].cache_line,
            program[i].dest,
            program[i].data,
            program[i].status,
            program[i].qos
        );
        
        }
        free(program);
    }
}

*/


	
