#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#define max_mem 4096
#define stkmax 16
#define GET 0
#define SET 1


void opcode0(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    stack_t* stack_inst =get_set_stack(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1])); 
    switch(opcode_placeholder){
        case 0x00ee:
            stack_inst->stack_ptr=stack_inst->stack_ptr+1;
            instance->pc=stack_inst->stk[stack_inst->stack_ptr];
            break;
        case 0x00e0:
            break;
        default:
            instance->pc=(opcode_placeholder&0x0fff);
            break;
    }
}

void opcode1(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1])); 
    short jmp_ad = opcode_placeholder&0x0FFF;
    instance->pc=jmp_ad;
}

void opcode2(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    stack_t* stack_inst = get_set_stack(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    short call_ad = opcode_placeholder&0x0FFF;
    stack_inst->stack_ptr=stack_inst->stack_ptr-1;
    stack_inst->stk[stack_inst->stack_ptr]=call_ad;
}

void opcode3(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    reg_t* reg_inst = get_set_reg(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    uint8_t reg_no = opcode_placeholder & 0x0F00;
    uint8_t cmp_byte = opcode_placeholder &0x00FF;
    if((reg_inst->v[reg_no])==cmp_byte){
        instance->pc=(instance->pc)+2;
    }
}

void opcode4(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    reg_t* reg_inst = get_set_reg(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    uint8_t reg_no = opcode_placeholder & 0x0F00;
    uint8_t cmp_byte = opcode_placeholder &0x00FF;
    if((reg_inst->v[reg_no])!=cmp_byte){
        instance->pc=(instance->pc)+2;
    }
}

void opcode5(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    reg_t* reg_inst = get_set_reg(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    uint8_t f_reg=opcode_placeholder&0x0f00;
    uint8_t l_reg=opcode_placeholder&0x00f0;
    if((reg_inst->v[f_reg])==(reg_inst->v[l_reg])){
        instance->pc=(instance->pc)+2;
    }
}

void opcode6(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    reg_t* reg_inst = get_set_reg(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    uint8_t reg_no = opcode_placeholder & 0x0F00;
    uint8_t ld_byte = opcode_placeholder &0x00FF;
    reg_inst->v[reg_no]=ld_byte;
}

void opcode7(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    reg_t* reg_inst = get_set_reg(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    uint8_t reg_no = opcode_placeholder & 0x0F00;
    uint8_t add_byte = opcode_placeholder &0x00FF;
    reg_inst->v[reg_no]=reg_inst->v[reg_no]+add_byte;
}

void opcode8(short count){
    mem_t* instance = get_set_mem(NULL,GET);
    reg_t* reg_inst = get_set_reg(NULL,GET);
    uint16_t opcode_placeholder=((instance->program_mem[count])<<8)+((instance->program_mem[count+1]));
    uint8_t l_op=opcode_placeholder&0x000f;
    uint8_t f_reg=opcode_placeholder&0x0f00;
    uint8_t l_reg=opcode_placeholder&0x00f0;
    switch(l_op){
        case 0:
            reg_inst->v[f_reg]=reg_inst->v[l_reg];
            break;
        case 1:
            reg_inst->v[f_reg]=reg_inst->v[f_reg]|reg_inst->v[l_reg];
            break;
        case 2:
            reg_inst->v[f_reg]=reg_inst->v[f_reg]&reg_inst->v[l_reg];
            break;
        case 3:
            reg_inst->v[f_reg]=reg_inst->v[f_reg]^reg_inst->v[l_reg];
            break;
        case 4:
            reg_inst->v[f_reg]=reg_inst->v[f_reg]+reg_inst->v[l_reg];
            short carry_chk=reg_inst->v[f_reg]+reg_inst->v[l_reg];
            if (carry_chk>255)
            {
                reg_inst->v[0xf]=1;
            }
            break;
        default:
            break;
    }
}

void opcode9(short count){

}

typedef struct opcodes{
    void (*opcode0)(short);
    void (*opcode1)(short);
    void (*opcode2)(short);
    void (*opcode3)(short);
    void (*opcode4)(short);
    void (*opcode5)(short);
    void (*opcode6)(short);
    void (*opcode7)(short);
    void (*opcode8)(short);
    void (*opcode9)(short);
    void (*opcodea)(short);
    void (*opcodeb)(short);
    void (*opcodec)(short);
    void (*opcoded)(short);
    void (*opcodee)(short);
    void (*opcodef)(short);

}ops_t;

typedef struct regist{
   uint8_t v[16];
}reg_t;

typedef struct stack{
    uint16_t stk[stkmax];
    short stack_ptr;
}stack_t;

typedef struct program_memory{
    uint8_t program_mem[max_mem];
    short pc;
    short pc_max;
}mem_t;

stack_t *get_set_stack(stack_t *stk,int mode){
    static stack_t *current;
    if(mode == SET){
        current = stk;
        return current;
    }else if (mode == GET)
    {
        return current;
    }
    
}

reg_t *get_set_reg(reg_t *reg,int mode){
    static reg_t* reg_cur;
    if(mode == SET){
        reg_cur = reg;
        return reg_cur;
    }
    else if(mode == GET){
        return reg_cur;
    }
}

mem_t *get_set_mem(mem_t *mem,int mode){
    static mem_t* mem_cur;
    if(mode==SET){
        mem_cur=mem;
        return mem_cur;
    }
    else if(mode==GET){
        return mem_cur;
    }
}


void printmem(){
    mem_t* instance;
    instance=get_set_mem(NULL,GET);
    for(int i=0;i<instance->pc_max;i++){
        printf("\n%x",instance->program_mem[i]);
    }
}


int main(int argc,char* argv[]){
    stack_t main_stack;
    reg_t main_reg;
    mem_t main_mem;
    get_set_stack(&main_stack,SET);
    main_stack.stack_ptr=stkmax;
    get_set_reg(&main_reg,SET);
    main_mem.pc=0x200;
    get_set_mem(&main_mem,SET);
    
    if((argc==1)){
        printf("\nno ROM specified.");
        return -1;
    }
    else if(argc>2){
        printf("\nToo many inputs are recieved.");
        return -1;
    }
    FILE* fptr;
    fptr=fopen(argv[1],"rb");
    if(fptr==NULL){
        printf("\nfile load failed,program will now exit.");
        return -2;
    }
    else{
        printf("\nfile has openned successfully");
    }

    main_mem.pc_max=fread(main_mem.program_mem,1,4096,fptr);
    
    return 0;
}


int fetch_decode(){
    mem_t* mem_inst;
    mem_inst=get_set_mem(NULL,GET);
    while((mem_inst->pc)=!(mem_inst->pc_max)){
        switch ((mem_inst->program_mem[mem_inst->pc]&0xf0)>>4)
        {
        case 0x0:
            opcode0(mem_inst->pc);
            break;
        case 0x1:
            opcode1(mem_inst->pc);
            break;
        case 0x2:
            opcode2(mem_inst->pc);
            break;
        case 0x3:
            opcode3(mem_inst->pc);
            break;
        case 0x4:
            opcode4(mem_inst->pc);
            break;
        case 0x5:
            opcode5(mem_inst->pc);
            break;
        case 0x6:
            opcode6(mem_inst->pc);
            break;
        case 0x7:
            opcode7(mem_inst->pc);
            break;
        case 0x8:
            opcode8(mem_inst->pc);
            break;
        case 0x9:
            opcode9(mem_inst->pc);
            break;
        case 0xa:
            opcodea();
            break;
        case 0xb:
            opcodeb();
            break;
        case 0xc:
            opcodec();
            break;
        case 0xd:
            opcoded();
            break;
        case 0xe:
            opcodee();
            break;
        case 0xf:
            opcodef();
            break;
            
        default:
            break;
        }
        mem_inst->pc=mem_inst->pc+1;
    }
}



