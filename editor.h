#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <conio2.h>
struct caixa
{
	struct caixa *ant, *prox;
	char letra;
};
typedef struct caixa Caixa;

struct descritor
{
	Caixa *inicio, *fim;
	int nroL;
};
typedef struct descritor Descritor;

struct cursor
{
    Caixa *pos;
};
typedef struct cursor Cursor;

struct noDicionario {
    char letra;
    int finalPalavra;             //  1 para T (Final), 0 para F (False)
    struct noDicionario *prim; 
    struct noDicionario *prox;   
};
typedef struct noDicionario NoDicionario;

// Assinaturas das funções do TAD
void inserirPalavraDicionario(NoDicionario **raiz, const char palavra[]);
void exibirNaMoldura(Descritor *desc, Cursor *cursor, int *cX, int *cY, Caixa *inicioTela);
void init(Descritor **desc);
void inserirCaracter(Descritor *desc, Cursor *cursor, char letra);
void destruir(Descritor *desc);
void abrirArquivo(Descritor *desc, char nome[]);
void salvarArquivo(Descritor *desc, char nome[]);
void exibir(Descritor *desc);
void linhasGrandes(Descritor *desc);
void exibirFormatado(Descritor *descritor);
void obterPalavraAtual(Descritor *descritor, Cursor *cursor, char palavra[]);
int buscarSugestao(NoDicionario *raiz, const char prefixo[], char sugestao[]);
void aprenderPalavraAnterior(Descritor *descritor, Cursor *cursor, NoDicionario **dicionario);
void calcularPosicaoCursor(Descritor *descritor, Cursor *cursor, int *linhaAtual, int *colunaAtual);








