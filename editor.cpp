#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include <conio2.h>
#include "editor.h"

void init (Descritor **desc)
{
	*desc=(Descritor*)malloc(sizeof(Descritor));
	(*desc)->inicio=(*desc)->fim=NULL;
	(*desc)->nroL=0;
}

void inserirPalavraDicionario(NoDicionario **raiz, const char palavra[]) 
{
    int i = 0;
    int tamanho = strlen(palavra);
    NoDicionario **atual = raiz; 
    int achouLetra;

    while (i < tamanho) 
    {
        achouLetra = 0; 

        while (*atual != NULL && achouLetra == 0) 
        {
            if ((*atual)->letra == palavra[i]) 
            {
                achouLetra = 1; 
            } 
            else 
            {
                atual = &((*atual)->prox); 
            }
        }
        if (achouLetra == 0) 
        {
            *atual = (NoDicionario*)malloc(sizeof(NoDicionario));
            
            if (*atual != NULL) 
            {
                (*atual)->letra = palavra[i];
                (*atual)->finalPalavra = 0; 
                (*atual)->prim = NULL;
                (*atual)->prox = NULL;
            }
        }
        if (i == tamanho - 1 && *atual != NULL) 
        {
            (*atual)->finalPalavra = 1; 
        }
        if (*atual != NULL) 
        {
            atual = &((*atual)->prim);
        }
        
        i++; 
    }
}

void obterPalavraAtual(Descritor *descritor, Cursor *cursor, char palavra[]) 
{
    Caixa *caixaAtual;
    int i = 0;
    int j;
    char temporario[100];
    int continuar = 1;
    
    palavra[0] = '\0';
    
    if (cursor->pos == NULL) {
        caixaAtual = descritor->fim;
    } else {
        caixaAtual = cursor->pos->ant;
    }
    while (caixaAtual != NULL && continuar == 1) 
    {
        if (caixaAtual->letra == ' ' || caixaAtual->letra == '\n' || 
            caixaAtual->letra == '.' || caixaAtual->letra == ',' || i >= 99) 
        {
            continuar = 0;
        } 
        else 
        {
            temporario[i] = caixaAtual->letra;
            i++;
            caixaAtual = caixaAtual->ant;
        }
    }
    for (j = 0; j < i; j++) {
        palavra[j] = temporario[i - 1 - j];
    }
    palavra[i] = '\0';
}

int buscarSugestao(NoDicionario *raiz, const char prefixo[], char sugestao[]) 
{
    NoDicionario *atual = raiz;
    int i = 0;
    int tamanho = strlen(prefixo);
    int resultado = 0; 
    int continuar = 1;
    int s = 0;

    if (tamanho > 0) 
    {
        while (i < tamanho && continuar == 1) 
        {
            int achou = 0;
            while (atual != NULL && achou == 0) {
                if (atual->letra == prefixo[i]) {
                    achou = 1;
                } else {
                    atual = atual->prox;
                }
            }
            
            if (achou == 0) {
                continuar = 0;
            } else {
                if (i < tamanho - 1) {
                    atual = atual->prim;
                }
                i++;
            }
        }
        if (continuar == 1) 
        {
            atual = atual->prim;
            
            if (atual == NULL) {
                continuar = 0;
            }
            
            while (atual != NULL && continuar == 1) 
            {
                if (atual->prox != NULL) 
                {
                    continuar = 0;
                } 
                else 
                {
                    sugestao[s] = atual->letra;
                    s++;
                    sugestao[s] = '\0';
                    
                    if (atual->finalPalavra == 1) {
                        resultado = 1;
                        continuar = 0;
                    } else {
                        atual = atual->prim;
                    }
                }
            }
        }
    }
    return resultado;
}

void aprenderPalavraAnterior(Descritor *descritor, Cursor *cursor, NoDicionario **dicionario) 
{
    Caixa *caixaAtual;
    int i = 0;
    int j;
    char temporario[100];
    char palavraNova[100];
    int continuar = 1;

    if (cursor->pos == NULL) {
        caixaAtual = descritor->fim;
    } else {
        caixaAtual = cursor->pos->ant;
    }

    if (caixaAtual != NULL) {
        caixaAtual = caixaAtual->ant;
    }

    while (caixaAtual != NULL && continuar == 1) {
        if (caixaAtual->letra == ' ' || caixaAtual->letra == '\n' || 
            caixaAtual->letra == '.' || caixaAtual->letra == ',' || 
            caixaAtual->letra == '!' || caixaAtual->letra == '?' || i >= 99) 
        {
            continuar = 0;
        } else {
            temporario[i] = caixaAtual->letra;
            i++;
            caixaAtual = caixaAtual->ant;
        }
    }

    if (i > 2) 
    {
        for (j = 0; j < i; j++) {
            palavraNova[j] = temporario[i - 1 - j];
        }
        palavraNova[i] = '\0';
        inserirPalavraDicionario(dicionario, palavraNova);
    }
}

void inserirCaracter(Descritor *desc, Cursor *cursor, char letra)
{
    Caixa *nova = (Caixa*)malloc(sizeof(Caixa));

    if (nova != NULL) 
    {
        nova->letra = letra;
        nova->ant = NULL;
        nova->prox = NULL;

        if (letra == '\n') {
            desc->nroL++;
        }

        if (desc->inicio == NULL) {
            desc->inicio = nova;
            desc->fim = nova;
        } else if (cursor->pos == NULL) {
            nova->ant = desc->fim;
            desc->fim->prox = nova;
            desc->fim = nova;
        } else if (cursor->pos == desc->inicio) {
            nova->prox = desc->inicio;
            desc->inicio->ant = nova;
            desc->inicio = nova;
        } else {
            nova->ant = cursor->pos->ant;
            nova->prox = cursor->pos;
            cursor->pos->ant->prox = nova;
            cursor->pos->ant = nova;
        }
    }
}


void destruir (Descritor*desc)
{
	Caixa *atual, *prox;
	if(desc->inicio!=NULL)
	{
		atual = desc->inicio;
		while(atual!=NULL)
		{
			prox=atual->prox;
			free(atual);
			atual=prox;
		}
		desc->inicio=desc->fim=NULL;
		desc->nroL=0;
	}
}

void backspace(Descritor *desc, Cursor *cursor)
{
    Caixa *apagar;
    if(cursor->pos == NULL)
        apagar = desc->fim;
    else
        apagar = cursor->pos->ant;
    if(apagar != NULL)
    {
    	if(apagar->ant != NULL)
	        apagar->ant->prox = apagar->prox;
	    else
	        desc->inicio = apagar->prox;
	
	    if(apagar->prox != NULL)
	        apagar->prox->ant = apagar->ant;
	    else
	        desc->fim = apagar->ant;
	        
	    free(apagar);
    }
}

void del (Descritor *desc, Cursor *cursor)
{
	Caixa*apagar;
	if (cursor->pos!=NULL)
	{
		apagar=cursor->pos;
		if(apagar->ant!=NULL)
			apagar->ant->prox=apagar->prox;
		else
			desc->inicio=apagar->prox;
			
		if(apagar->prox!=NULL)
			apagar->prox->ant=apagar->ant;
		else
			desc->fim=apagar->ant;
			
		cursor->pos = apagar->prox;
   		free(apagar);	
	}
}

void home (Descritor*desc, Cursor*cursor)
{
	Caixa*p;
	p=cursor->pos;
	
	if(p==NULL)
		p=desc->fim;
		
	while(p != NULL && p->ant != NULL && p->ant->letra != '\n')
        p = p->ant;
    cursor->pos = p;
}

void end(Descritor *desc, Cursor *cursor)
{
    Caixa *p;
    p=cursor->pos;
    if(p == NULL)
        p = desc->fim;
    while(p != NULL && p->prox != NULL && p->prox->letra != '\n')
        p = p->prox;
    cursor->pos = p;
}

void exibir (Descritor *desc)
{
	Caixa*p;
	p=desc->inicio;
	while(p!=NULL)
	{
		printf("%c",p->letra);
		p=p->prox;
	}
}

void abrirArquivo (Descritor *desc, char nome[])
{
    int c;
    FILE *ptr = fopen(nome, "r");
    Cursor cur_temp;
    
    destruir(desc);
    
    if (ptr == NULL) {
        printf("ERRO\n");
    } else {
        cur_temp.pos = NULL;
        c = fgetc(ptr);
        while (c != EOF)
        {
            inserirCaracter(desc, &cur_temp, c);
            c = fgetc(ptr);
        }
        fclose(ptr);
    }
}

void salvarArquivo (Descritor*desc, char nome[])
{
	FILE*ptr=fopen(nome,"w");
	Caixa*p;
	p=desc->inicio;
	if (ptr == NULL)
    	printf("ERRO\n");
    else
    {
	   	while(p!=NULL)
		{
			fputc(p->letra,ptr);
			p=p->prox;
		}
		fclose(ptr);
    }	
}

void desenharMoldura(void)
{
    int i;
    clrscr();
    textbackground(0);
    textcolor(11);
    gotoxy(1, 1);
    putchar(201); 
    for(i = 0; i < 78; i++) putchar(205);
    putchar(187); 

    gotoxy(1, 2); putchar(186);
    gotoxy(80, 2); putchar(186);
    gotoxy(2, 2);
    textbackground(0); textcolor(14); 
    printf(" F2");
    textcolor(7);
    printf("-Abrir  ");
    textcolor(14);
    printf("F3");
    textcolor(7);
    printf("-Salvar  ");
    textcolor(14);
    printf("F4");
    textcolor(7);
    printf("-Sair  ");
    textcolor(14);
    printf("F5");
    textcolor(7);
    printf("-Exibir");

    textcolor(11);
    gotoxy(1, 3);
    putchar(204); 
    for(i = 0; i < 78; i++) putchar(205); 
    putchar(185); 


    for(i = 4; i <= 22; i++)
    {
        gotoxy(1,  i); putchar(186);
        gotoxy(80, i); putchar(186);
    }


    gotoxy(1, 23);
    putchar(204);
    for(i = 0; i < 78; i++) putchar(205); 
    putchar(185); 

    gotoxy(1, 24); putchar(186);
    gotoxy(2, 24);
    textcolor(10);
    printf(" COL=");
    textcolor(15); printf("1");
    textcolor(10); printf("   LIN=");
    textcolor(15); printf("1");
    textcolor(10); printf("   INSERT  ||||  LISTA");
    gotoxy(80, 24); textcolor(11); putchar(186);
    textcolor(11);
    gotoxy(1, 25);
    putchar(200); 
    for(i = 0; i < 78; i++) putchar(205);
    putchar(188); 
    textbackground(0);
    textcolor(7);
}

void limparAreaTexto(void){
    int lin, col;
    textbackground(0);
    for(lin = 4; lin <= 22; lin++)
    {
        gotoxy(2, lin);
        for(col = 2; col <= 79; col++)
        {
            putchar(' ');
    	}
	}
}

void exibirNaMoldura(Descritor *desc, Cursor *cursor, int *cX, int *cY, Caixa *inicioTela)
{
    Caixa *p;
    int col = 2; 
    int lin = 4; 
    int continuar = 1; 
    int cursorX = 2; 
    int cursorY = 4;
    int modoNegrito = 0; 

    textbackground(0);
    textcolor(7); 

    if (inicioTela == NULL) {
        p = desc->inicio;
    } else {
        p = inicioTela;
    }
    
    while(p != NULL && continuar == 1)
    {
        if(lin > 22) { continuar = 0; }
        
        if (continuar == 1) 
        {
            if (p == cursor->pos) {
                cursorX = col;
                cursorY = lin;
            }

            if(p->letra == '\n') {
                lin++;
                col = 2;
            } 
            else if (p->letra == 21)
            {

                if (modoNegrito == 0) {
                    modoNegrito = 1;
                    textcolor(15);
                } else {
                    modoNegrito = 0;
                    textcolor(7);
                }
                if(col <= 79) {
                    gotoxy(col, lin);
                    putchar(245); 
                    col++;
                }
            }
            else {
                if(col <= 79) {
                    gotoxy(col, lin);
                    putchar(p->letra);
                    col++;
                }
            }
            p = p->prox;
        }
    }

    if (cursor->pos == NULL) {
        cursorX = col;
        cursorY = lin;
        if (cursorY > 22) { cursorY = 22; cursorX = 79; }
        if (cursorX > 79) { cursorX = 79; }
    }

    textcolor(7);
    
    *cX = cursorX;
    *cY = cursorY;
    
    gotoxy(cursorX, cursorY); 
}

void linhasGrandes(Descritor *desc)
{
    Caixa *p = desc->inicio;
    Caixa *ultimoEspaco = NULL;
    int col = 0;
    while (p != NULL)
    {
        if (p->letra == '\n')
        {
            col = 0;
            ultimoEspaco = NULL;
            p = p->prox;
        }
        else
        {
            col++;
            
            if (p->letra == ' ')
            {
                ultimoEspaco = p;
            }
            if (col > 79 && ultimoEspaco != NULL)
            {
                ultimoEspaco->letra = '\n';
                p = ultimoEspaco; 
            }
            else
            {
                p = p->prox;
            }
        }
    }
}

void exibirFormatado(Descritor *descritor)
{
    Caixa *caixaAtual = descritor->inicio;
    int recuoPrimeiraLinha = 15;
    int recuoEsquerdo = 5;
    int recuoDireito = 75;
    
    int coluna;
    int linha = 4;
    int continuar = 1;
    int inicioDeParagrafo = 1;
    char letraAnterior = ' ';  

    limparAreaTexto();
    textbackground(0);
    textcolor(11);

    coluna = recuoPrimeiraLinha;

    while (caixaAtual != NULL && continuar == 1)
    {
        if (linha > 22) 
        {
            continuar = 0;
        }

        if (continuar == 1) 
        {
            if (caixaAtual->letra == '\n') 
            {
                if (letraAnterior == '.' || letraAnterior == '!' || letraAnterior == '?') 
                {
                    inicioDeParagrafo = 1;
                } 
                else 
                {
                    inicioDeParagrafo = 0;
                }   
                linha++;
                if (inicioDeParagrafo == 1) 
                {
                    coluna = recuoPrimeiraLinha;
                } 
                else 
                {
                    coluna = recuoEsquerdo;
                }
            } 
            else 
            {
                if (coluna <= recuoDireito) 
                {
                    gotoxy(coluna, linha);
                    putchar(caixaAtual->letra);
                    coluna++;
                }
                letraAnterior = caixaAtual->letra; 
            }
            
            caixaAtual = caixaAtual->prox;
        }
    }
    gotoxy(2, 24);
    textcolor(14);
    printf(" FORMATADO (F5). APERTE QUALQUER TECLA PARA VOLTAR A EDICAO.   ");
    
    getch();
    textcolor(7); 
}

void calcularPosicaoCursor(Descritor *descritor, Cursor *cursor, int *linhaAtual, int *colunaAtual)
{
    Caixa *auxiliar = descritor->inicio;
    int lin = 1;
    int col = 1;
    int continuar = 1;
    while (auxiliar != NULL && continuar == 1)
    {
        if (auxiliar == cursor->pos)
        {
            continuar = 0;
        }
        else
        {
            if (auxiliar->letra == '\n')
            {
                lin++;
                col = 1;
            }
            else
            {
                col++;
            }
            auxiliar = auxiliar->prox;
        }
    }
    *linhaAtual = lin;
    *colunaAtual = col;
}

int main()
{
    Descritor *desc;
    Cursor cursor;       
    NoDicionario *dicionario = NULL; 
    int rodando = 1; 
    int ch; 
    int modoInsert = 1;
    int linhaLogica, colunaLogica;       
    char palavraAtual[100];
    char sugestao[100];
    int temSugestao = 0;    
    int cX = 2, cY = 4;
    Caixa *inicioTela = NULL;

    init(&desc);
    cursor.pos = NULL;   
    inserirPalavraDicionario(&dicionario, "asa");
    inserirPalavraDicionario(&dicionario, "assa");
    inserirPalavraDicionario(&dicionario, "passado");
    desenharMoldura();
    abrirArquivo(desc, "exemplo.txt");

    while (rodando == 1) 
    {
        linhasGrandes(desc);
        limparAreaTexto();
        
       if (inicioTela == NULL && desc->inicio != NULL) {
            inicioTela = desc->inicio;
        }

        if (inicioTela != NULL) 
        {
            int linhaCamera = 1, linhaCursor = 1, colTemp;
            
            calcularPosicaoCursor(desc, &cursor, &linhaCursor, &colTemp);
            Cursor tempCam;
            tempCam.pos = inicioTela;
            calcularPosicaoCursor(desc, &tempCam, &linhaCamera, &colTemp);

            while (linhaCursor - linhaCamera >= 19 && inicioTela != NULL) 
            {
                while (inicioTela != NULL && inicioTela->letra != '\n') {
                    inicioTela = inicioTela->prox;
                }
                if (inicioTela != NULL) {
                    inicioTela = inicioTela->prox; 
                    linhaCamera++;
                }
            }
            
            while (linhaCursor < linhaCamera && inicioTela != NULL && inicioTela->ant != NULL)
            {
                inicioTela = inicioTela->ant;
                while (inicioTela != NULL && inicioTela->ant != NULL && inicioTela->ant->letra != '\n') {
                    inicioTela = inicioTela->ant;
                }
                linhaCamera--;
            }
        }
        exibirNaMoldura(desc, &cursor, &cX, &cY, inicioTela);
        
        obterPalavraAtual(desc, &cursor, palavraAtual);
        temSugestao = 0;
        sugestao[0] = '\0';
        
        if (strlen(palavraAtual) > 0) {
            temSugestao = buscarSugestao(dicionario, palavraAtual, sugestao);
        }
        
        calcularPosicaoCursor(desc, &cursor, &linhaLogica, &colunaLogica);
        
        gotoxy(2, 24);
        textcolor(10);
        printf(" COL=");
        textcolor(15); printf("%-3d", colunaLogica);
        textcolor(10); printf(" LIN=");
        textcolor(15); printf("%-3d", linhaLogica);
        
        textcolor(10); 
        if (modoInsert == 1) {
            printf(" INSERT |||| ");
        } else {
            printf(" SOBRES |||| ");
        }
        
        if (temSugestao == 1) {
            textcolor(14); 
            printf("%s%s             ", palavraAtual, sugestao);
        } else {
            printf("                            "); 
        }
        textcolor(7);
        gotoxy(cX, cY);

        ch = getch();
        
        if (ch == 0 || ch == 224) 
        {
            ch = getch(); 
            if (ch == 62) {
                rodando = 0; 
            }
            else if (ch == 63) {
                exibirFormatado(desc);
            }
            else if (ch == 68) {
                inserirCaracter(desc, &cursor, 21); 
            }
            else if (ch == 75) {
                if (cursor.pos == NULL) {
                    cursor.pos = desc->fim;
                } else if (cursor.pos->ant != NULL) {
                    cursor.pos = cursor.pos->ant;
                }
            }
            else if (ch == 77) {
                if (cursor.pos != NULL) {
                    cursor.pos = cursor.pos->prox;
                }
            }
            else if (ch == 71) {
                home(desc, &cursor);
            }
            else if (ch == 79) {
                end(desc, &cursor);
            }
            else if (ch == 83) {
                del(desc, &cursor);
            }
        	else if (ch == 82)
            {
                if (modoInsert == 1) {
                    modoInsert = 0;
                } else {
                    modoInsert = 1;
                }
            }
			else if (ch == 73)
            {
                int linhas = 0;
                while (cursor.pos != NULL && cursor.pos->ant != NULL && linhas < 18) 
                {
                    cursor.pos = cursor.pos->ant;
                    if (cursor.pos->letra == '\n') {
                        linhas++;
                    }
                }
                while (cursor.pos != NULL && cursor.pos->ant != NULL && cursor.pos->ant->letra != '\n') 
                {
                    cursor.pos = cursor.pos->ant;
                }
                inicioTela = cursor.pos;
            }
            else if (ch == 81)
            {
                int linhas = 0;
                while (cursor.pos != NULL && cursor.pos->prox != NULL && linhas < 18) 
                {
                    if (cursor.pos->letra == '\n') {
                        linhas++;
                    }
                    cursor.pos = cursor.pos->prox;
                }
                while (cursor.pos != NULL && cursor.pos->ant != NULL && cursor.pos->ant->letra != '\n') 
                {
                    cursor.pos = cursor.pos->ant;
                }
                inicioTela = cursor.pos;
            }
        } 
        else 
        {
            if (ch == 8) {
                backspace(desc, &cursor);
            }
            else if (ch == 13) {
                if (temSugestao == 1) {
                    int indice = 0;
                    while (sugestao[indice] != '\0') {
                        inserirCaracter(desc, &cursor, sugestao[indice]);
                        indice++;
                    }
                } else {
                    inserirCaracter(desc, &cursor, '\n'); 
                }
            } 
            else if (ch >= 32 && ch <= 126) 
            {
                if (modoInsert == 0 && cursor.pos != NULL && cursor.pos->letra != '\n') 
                {
                    cursor.pos->letra = ch;
                    cursor.pos = cursor.pos->prox;
                } 
                else 
                {
                    inserirCaracter(desc, &cursor, ch);
                }
                
                if (ch == ' ' || ch == '.' || ch == ',' || ch == '!' || ch == '?') {
                    aprenderPalavraAnterior(desc, &cursor, &dicionario);
                }
            }
        }
    }
    salvarArquivo(desc, "exemplo.txt");
    destruir(desc);
    free(desc);
    
    return 0;
}
