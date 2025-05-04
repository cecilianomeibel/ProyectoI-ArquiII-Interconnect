#include  <cstdio> 
#include <cstdlib>
#include <cstdint>
#include <cstring>

struct instruction {
    int opcode;
    // 0: WRITE_MEM, 1: READ_MEM, etc.
    int num_pe;
    // 0 – 7. Numero de núcleo
    uint8_t src = 0xFF;
    // default: inválido
    uint64_t addr = 0;
    uint32_t size = 0;
    uint32_t num_of_cache_lines = 0;
    uint32_t start_cache_line = 0;
    uint32_t cache_line = 0;
    uint8_t dest = 0xFF; // para INV_COMPLETE y RESP
    uint32_t data; // opcional
    uint8_t status = 0xFF; // OK o NOT_OK
    uint8_t qos = 0xFF;
    // prioridad
    };

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
        //text_op [i] = '\0';

        for(int j = 0; j < op_num ; j++ ) //Vericacion de que OPCODE tiene la linea

        if (strcmp(text_op, operations[j])==0){
            return j;
        }

        return EXIT_FAILURE;
}


instruction *interpretate(const char* path){

    FILE* fp = std::fopen(path,"r");

    if(!fp){
        std::perror("Opening file failed");
    }
    instruction *instructions = (instruction*)malloc(32 * sizeof(instruction)); //esta forma pide la memoria necesaria para la lista de instrucciones.
    //instruction instructions[32]; 
    if (!instructions)
    {
        std::perror("Memory Allocation failed");
        return nullptr;
    }
    

    int i = 0;
    char line[256];//Buffer del tamano de la linea 
    while (fgets(line,sizeof(line),fp))
    {
        printf("Analizando: %s \n",line);
        instructions[i].opcode = analicer_opcode(line);
        i++;
        
    }

    printf("%d \n",instructions[5].opcode);

    std::fclose(fp);
   
    return instructions;   
}


int main(){

    int count = 32;

    instruction* program = interpretate("code_test.txt");

    if (program) {
        for (int i = 0; i < count; ++i) {
            printf("Opcode #%d: %d\n", i, program[i].opcode);
        }
        free(program);
    }
}
	

