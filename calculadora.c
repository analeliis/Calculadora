#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "expressao.h"

#define TAM_MAX_PILHA 512
#define PI 3.14159265


typedef struct
{
    float dados[TAM_MAX_PILHA];
    int topo;
} PilhaFloat;


typedef struct
{
    char texto[512]; 
    int prioridade;  
} ItemInfixo;

typedef struct
{
    ItemInfixo dados[128];
    int topo;
} PilhaInfixo;

void pilhaInfixo_init(PilhaInfixo *p)
{
    p->topo = -1;
}

int pilhaInfixo_push(PilhaInfixo *p, ItemInfixo item)
{
    if (p->topo >= 127)
        return 0; // pilha cheia
    p->topo++;
    p->dados[p->topo] = item;
    return 1;
}

int pilhaInfixo_pop(PilhaInfixo *p, ItemInfixo *item)
{
    if (p->topo < 0)
        return 0; // pilha vazia
    *item = p->dados[p->topo];
    p->topo--;
    return 1;
}

int prioridadeOperador(const char *op)
{
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1; // menor prioridade
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0)
        return 2;
    if (strcmp(op, "^") == 0)
        return 3;
    return 4; // números e funções
}

// Inicia a pilha: começa vazia
void pilhaFloat_init(PilhaFloat *p)
{
    p->topo = -1;
}

// Verifica se está vazia
int pilhaFloat_vazia(PilhaFloat *p)
{
    return (p->topo < 0);
}

// Empilha valor
int pilhaFloat_push(PilhaFloat *p, float valor)
{
    if (p->topo >= TAM_MAX_PILHA - 1)
    { // pilha cheia

        return 0;
    }

    p->topo++;                 // anda com o topo
    p->dados[p->topo] = valor; // coloca o valor lá
    return 1;
}

// Desempilha valor
int pilhaFloat_pop(PilhaFloat *p, float *saida)
{
    if (pilhaFloat_vazia(p))
    {
        // não tem nada pra tirar
        return 0;
    }

    *saida = p->dados[p->topo]; // pega o valor do topo
    p->topo--;                  // desce o topo
    return 1;
}


int ehNumero(const char *token)
{
    int i = 0;

   
    if (token == NULL || token[0] == '\0')
        return 0;

    // se começar com sinal + ou -, pula ele
    if (token[0] == '+' || token[0] == '-')
        i = 1;

    int temDigito = 0;

    for (; token[i] != '\0'; i++)
    {
        
        if (token[i] >= '0' && token[i] <= '9')
        {
            temDigito = 1;
        }
       
        else if (token[i] == '.')
        {
            
            continue;
        }
       
        else
        {
            return 0;
        }
    }

   
    return temDigito;
}

float getValorPosFixa(char *StrPosFixa)
{
    char copia[512];
    strcpy(copia, StrPosFixa);

    PilhaFloat pilha;
    pilhaFloat_init(&pilha);

    char *token = strtok(copia, " ");

    while (token != NULL)
    {
        if (ehNumero(token))
        {
            float valor;
            sscanf(token, "%f", &valor);
            pilhaFloat_push(&pilha, valor);
        }
        else if (strcmp(token, "+") == 0)
        {
            float a, b;
            pilhaFloat_pop(&pilha, &b);
            pilhaFloat_pop(&pilha, &a);
            float r = a + b;
            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "-") == 0)
        {
            float a, b;
            pilhaFloat_pop(&pilha, &b);
            pilhaFloat_pop(&pilha, &a);
            float r = a - b;
            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "*") == 0)
        {
            float a, b;
            pilhaFloat_pop(&pilha, &b);
            pilhaFloat_pop(&pilha, &a);
            float r = a * b;
            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "/") == 0)
        {
            float a, b;
            pilhaFloat_pop(&pilha, &b);
            pilhaFloat_pop(&pilha, &a);
            float r = a / b;
            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "%") == 0)
        {
            float a, b;
            pilhaFloat_pop(&pilha, &b);
            pilhaFloat_pop(&pilha, &a);

            int ia = (int)a;
            int ib = (int)b;

            int r = ia % ib;

            pilhaFloat_push(&pilha, (float)r);
        }
        else if (strcmp(token, "^") == 0)
        {
            float a, b;
            pilhaFloat_pop(&pilha, &b); // expoente
            pilhaFloat_pop(&pilha, &a); // base

            int exp = (int)b; // expoente ainda é inteiro
            float resultado = 1.0f;

            for (int i = 0; i < exp; i++)
            {
                resultado = resultado * a; // usa a como float!
            }

            pilhaFloat_push(&pilha, resultado);
        }
        else if (strcmp(token, "raiz") == 0)
        {
            float x;
            pilhaFloat_pop(&pilha, &x);

            float r = sqrt(x);
            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "sen") == 0)
        {
            float x;
            pilhaFloat_pop(&pilha, &x);

            // converter de graus para radianos
            float rad = x * PI / 180.0f;
            float r = sin(rad);

            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "cos") == 0)
        {
            float x;
            pilhaFloat_pop(&pilha, &x);

            float rad = x * PI / 180.0f;
            float r = cos(rad);

            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "tg") == 0)
        {
            float x;
            pilhaFloat_pop(&pilha, &x);

            float rad = x * PI / 180.0f;
            float r = tan(rad);

            pilhaFloat_push(&pilha, r);
        }
        else if (strcmp(token, "log") == 0)
        {
            float x;
            pilhaFloat_pop(&pilha, &x);

            float r = log10(x); // log na base 10

            pilhaFloat_push(&pilha, r);
        }
        else
        {
           
            return 0.0f;
        }

        token = strtok(NULL, " ");
    }

    float resultado;
    pilhaFloat_pop(&pilha, &resultado);

    return resultado;
}

char *getFormaInFixa(char *Str)
{
    char copia[512];
    strcpy(copia, Str);

    PilhaInfixo pilha;
    pilhaInfixo_init(&pilha);

    char *token = strtok(copia, " ");

    while (token != NULL)
    {
        // 1) Número
        if (ehNumero(token))
        {
            ItemInfixo item;
            strcpy(item.texto, token);
            item.prioridade = 4; // número/função
            pilhaInfixo_push(&pilha, item);
        }
        // 2) Operadores binários
        else if (strcmp(token, "+") == 0 ||
                 strcmp(token, "-") == 0 ||
                 strcmp(token, "*") == 0 ||
                 strcmp(token, "/") == 0 ||
                 strcmp(token, "%") == 0 ||
                 strcmp(token, "^") == 0)
        {
            ItemInfixo dir, esq;
            if (!pilhaInfixo_pop(&pilha, &dir)) return NULL;
            if (!pilhaInfixo_pop(&pilha, &esq)) return NULL;

            int prOp = prioridadeOperador(token);

            char left[512], right[512], novo[512];

            if (esq.prioridade < prOp)
                sprintf(left, "(%s)", esq.texto);
            else
                strcpy(left, esq.texto);

            if (dir.prioridade < prOp)
                sprintf(right, "(%s)", dir.texto);
            else
                strcpy(right, dir.texto);

            sprintf(novo, "%s%s%s", left, token, right);

            ItemInfixo res;
            strcpy(res.texto, novo);
            res.prioridade = prOp;
            pilhaInfixo_push(&pilha, res);
        }
        // 3) Funções unárias
        else if (strcmp(token, "raiz") == 0 ||
                 strcmp(token, "sen") == 0 ||
                 strcmp(token, "cos") == 0 ||
                 strcmp(token, "tg") == 0 ||
                 strcmp(token, "log") == 0)
        {
            ItemInfixo arg;
            if (!pilhaInfixo_pop(&pilha, &arg)) return NULL;

            char novo[512];

            if (strcmp(token, "raiz") == 0)
                sprintf(novo, "raiz(%s)", arg.texto);
            else if (strcmp(token, "sen") == 0)
                sprintf(novo, "sen(%s)", arg.texto);
            else if (strcmp(token, "cos") == 0)
                sprintf(novo, "cos(%s)", arg.texto);
            else if (strcmp(token, "tg") == 0)
                sprintf(novo, "tg(%s)", arg.texto);
            else 
                sprintf(novo, "log(%s)", arg.texto);

            ItemInfixo res;
            strcpy(res.texto, novo);
            res.prioridade = 4;
            pilhaInfixo_push(&pilha, res);
        }
        else
        {
            // token desconhecido
            return NULL;
        }

        token = strtok(NULL, " ");
    }

    // tem que sobrar exatamente 1 item
    if (pilha.topo != 0)
    {
        return NULL;
    }

    // monta string final em buffer temporário
    char temp[512];
    strcpy(temp, pilha.dados[pilha.topo].texto);

    // agora sim aloca dinamicamente para o chamador poder dar free()
    char *resultado = (char *)malloc(strlen(temp) + 1);
    if (resultado == NULL)
    {
        return NULL;
    }

    strcpy(resultado, temp);
    return resultado;
}

