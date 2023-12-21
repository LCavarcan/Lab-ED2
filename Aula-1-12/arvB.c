#include <stdio.h>
#include <stdlib.h>

#define MAX 3
#define MIN 1
#define bool int
#define false 0
#define true 1
#define NMAXREGS  5

int NVLMAX = 0;

typedef struct ArvB {
    int nregs;
    int count;
    int ordem;

    struct ArvB* filha[NMAXREGS+1];
    int chave[NMAXREGS+1];
} NoArvB,ArvB,No;

ArvB *raiz;

void inserir(int chave);
int setNo(int chave, int *pval, No *no, No **filho);
void InserirValor(int chave, int pos, No *no, No *filho);
No *criarNo(int chave, No *filho);
void divideNo(int chave, int *pval, int pos, No *no,No *filho, No **novoNo);
void procura(No *noSelecionado);
void procura2(No *noSelecionado, int *achou);
bool RemoveArvB(ArvB **arvore, int chave);
bool RemoveArvBRec(NoArvB **no, int chave, bool *underflow);
bool BuscaChaveNoArvB(NoArvB *no, int chave, int *pos);
void TrocaChaveComAntecessora(NoArvB *no, int pos);
void TrataNoComMenosChavesQuePermitido(NoArvB **no, int pos);
void RemoveChaveEmNo(NoArvB *no, int pos);
bool NoComMaisChavesQuePossivel(NoArvB *no);
bool NoComMenosChavesQuePermitido(NoArvB *no);
void altura(No *noSelecionado, int nvl);
int nivelAtual(No *noSelecionado,int nivelA, int nivelB);
int busca(No *noSelecionado,int elem,int* achou);

bool RemoveArvB(ArvB **arvore, int chave) {
  if ((*arvore) == NULL){
    printf("\nArvore nao existe\n");
    return false;
  }
  
  bool underflow=false;
  
  if(RemoveArvBRec(arvore, chave, &underflow)){ /* A chave foi removida */

    /* Se o underflow chegou até a raiz da arvore, entao a raiz antiga
       deve ser eliminada e a nova será a sua única filha, reduzindo
       assim a altura da arvore em 1.
    */
    
    if(underflow) {
      NoArvB *no = *arvore;
      *arvore = (*arvore)->filha[0];
      free(no);
    }

    return true;
  }
  return(false);
}

bool RemoveArvBRec(NoArvB **no, int chave, bool *underflow) {
  int pos;
  
  /* Verifica se a chave está no nó atual. Se tiver e esse nó for uma
  folha, realiza a remoção. Caso contrário, troca a chave pela
  antecessora e continua o procedimento recursivo para remoção. */

  if (BuscaChaveNoArvB(*no, chave, &pos)) {     /* a chave está no nó deste nível */
     if ((*no)->filha[pos]!=NULL) {    
            /* O nó atual não é uma folha,
			 		  então a chave precisa ser trocada pela antecessora, reduzindo ao caso de remoção na folha. */

          TrocaChaveComAntecessora(*no, pos);   /* troca a chave pela sua antecessora -- i.e., a
					       chave mais à direita na árvore à esquerda. continua o procedimento recursivo para
                 reencontrar e remover a chave do nó folha */

          RemoveArvBRec(&((*no)->filha[pos]), chave, underflow);
          /* trata underflow, se for o caso, após a remoção da chave */
          if (*underflow) 
             TrataNoComMenosChavesQuePermitido(no, pos); 

     } 
     else { /* achou a chave no nó folha, então ela pode ser removida. */
        RemoveChaveEmNo(*no, pos);
     }
     /* verifica se a folha ficou em underflow */

     (*underflow) = NoComMenosChavesQuePermitido(*no); 

     return true;   /* remoção bem sucedida */
    } 
    else { /* procura a chave nos nós abaixo */
        if ((*no)->filha[pos] != NULL) {
        if (RemoveArvBRec(&((*no)->filha[pos]), chave, underflow)) {
	            /* a chave foi encontrada e removida da folha (raiz da árvore
	            filha). Trata um possível underflow e verifica novamente se
	            ele será propagado para cima (volta da recursão). */
	          if (*underflow)
               TrataNoComMenosChavesQuePermitido(no, pos);
	          (*underflow) = NoComMenosChavesQuePermitido(*no);
	          return true;
        } 
        else {
	          return false; /* chego na folha e não achou a chave */
        }
     } 
     else {  /* estamos na raiz e a chave não está neste nível, logo ela não existe. */
        (*underflow) = false;
         return false;
     }
  }
}

bool BuscaChaveNoArvB(NoArvB *no, int chave, int *pos) {
  for ((*pos)=0; (no->chave[*pos] < chave) && (*pos) < no->nregs; (*pos)++);

  /* "pos" contém a posição da chave no nó */
  if (((*pos) < no->nregs)&&(no->chave[*pos] == chave))
     return true;

  /* "pos" contém a posição da árvore filha na qual a chave poderá ser encontrada */
  return false;
}

void TrocaChaveComAntecessora(NoArvB *no, int pos) {
  ArvB *no_aux;
  int   chave_aux;
  
  no_aux = no->filha[pos]; /* árvore filha à esquerda da chave na posição "pos" */

  while(no_aux->filha[no_aux->nregs]!=NULL) /* busca a chave mais à direira na àrvore esquerda (i.e., a antecessora da chave que está em "no" na posição "pos") */
      no_aux = no_aux->filha[no_aux->nregs];

  /* troca a chave por sua antecessora na árvore B */
  chave_aux  = no->chave[pos];
  no->chave[pos] = no_aux->chave[no_aux->nregs-1];
  no_aux->chave[no_aux->nregs-1] = chave_aux;
}

void TrataNoComMenosChavesQuePermitido(NoArvB **no, int pos) {
  int chave,i,j;
  NoArvB *irma_esq=NULL, *filha=NULL, *irma_dir=NULL;

  filha = (*no)->filha[pos];
  
  if (pos > 0) { /* existe irmã à esquerda da filha */ 
     irma_esq = (*no)->filha[pos-1];

     if(irma_esq->nregs > (((*no)->ordem)/2)) { /* A irmã à esquerda tem chaves para emprestar. A sua última chave sobe
						  para o nó pai e a chave do nó pai  desce para a filha na posição mais à esquerda. A filha à direira da chave que
						  subiu passa a ser filha à esquerda daquela que desceu.*/
         chave = (*no)->chave[pos-1]; 
         (*no)->chave[pos-1]=irma_esq->chave[irma_esq->nregs-1]; 
         NoArvB *filha_esq          = irma_esq->filha[irma_esq->nregs]; 

         RemoveChaveEmNo((*no)->filha[pos-1],(*no)->filha[pos-1]->nregs);

         filha->nregs++;
         filha->filha[0] = filha_esq;
         filha->chave[0] = chave;

         return;
     }
  }
  
  if (pos < (*no)->nregs) { /* existe irmã à direita */ 
    irma_dir = (*no)->filha[pos+1];

    if (irma_dir->nregs > (((*no)->ordem)/2)) { /* A irmã à direita tem chaves para
						  emprestar. A sua primeira chave sobe para o nó pai e a chave do nó pai desce para a filha na posição mais à
						  direita. A filha à esquerda da chave que subiu passa a ser filha à direita daquela que desceu. */
        chave   = (*no)->chave[pos];
        (*no)->chave[pos] = irma_dir->chave[0];
        NoArvB *filha_dir = irma_dir->filha[0];

        RemoveChaveEmNo((*no)->filha[pos+1],0);

        filha->nregs++;
        filha->filha[filha->nregs]   = filha_dir;
        filha->chave[filha->nregs-1] = chave;

        return;
    }    
  }
}

void RemoveChaveEmNo(NoArvB *no, int pos) {
  int i;
  /* desloca as chaves e filhas para trás */
  for(i=pos; i < no->nregs; i++) {
      no->chave[i] = no->chave[i+1];
      no->filha[i] = no->filha[i+1];
  }
  /* completa o deslocamento para a filha à direita da última chave */ 
  no->filha[i]   = no->filha[i+1];
  no->filha[i+1] = NULL;
  /* ajusta o número de registros */
  no->nregs--;
}

bool NoComMaisChavesQuePossivel(NoArvB *no) {
  return(no->nregs > no->ordem);
}

bool NoComMenosChavesQuePermitido(NoArvB *no) {
  return(no->nregs < (no->ordem/2));
}

void altura(No *noSelecionado, int nvl) {
    int i;

    if(noSelecionado) {
		if(nvl > NVLMAX) 
			NVLMAX = nvl;

        for (i = 0; i < noSelecionado->count; i++)
            altura(noSelecionado->filha[i],nvl + 1);
            //printf("%d ", noSelecionado->chave[i + 1]);

		altura(noSelecionado->filha[i],nvl + 1);
	} 
}

int nivelAtual(No *noSelecionado,int nivelA, int nivelB) {
    int i, n;
    
    if(noSelecionado) {
        for(i=0; i<noSelecionado->count; i++) {
            nivelAtual(noSelecionado->filha[i],nivelB,nivelA+1);

            if(nivelB == nivelA)
                printf("%d ", noSelecionado->chave[i + 1]);            
        }

        nivelAtual(noSelecionado->filha[i],nivelB,nivelA+1);
    }  
}

int main() {
    int chave, ch, n, achou, op;

    do{
        puts("\n\n-------- MENU --------\n\n\t1- Buscar\n\t2- Inserir\n\t3- Ver a arvore\n\t4- Remover\n\t5- Ver quantidade de niveis\n\t6- Ver a arvore por niveis\n\t0- Sair\n\n");
        scanf("%d",&op);
        printf("\n----------------------\n");

        switch (op) {
            case 0:
                break;
            
            case 1:
                achou =0;

                printf("\nInsira o numero que deseja buscar:\n");
                scanf("%d",&n);

                busca(raiz,n,&achou);

                if(achou==0)
                    puts("\nO elemento nao foi encontrado\n");

                break;
            
            case 2:
                achou = 0;

                printf("\nInsira o numero que deseja inserir:\n");
                scanf("%d",&n);

                busca(raiz,n,&achou);

                if(achou)
                    puts("\nO valor ja existe\n");

                else
                    inserir(n);
            
                break;

            case 3:
                procura(raiz);

                break;

            case 4:
                printf("\nInsira o numero que deseja remover:\n");
                scanf("%d",&n);

                if(RemoveArvB(&(raiz), n))
                    puts("\nRemovido\n");
                
                else
                    puts("\nNao foi possivel remover\n");

                break;

            case 5:
                altura(raiz,0);
                printf("qtd niveis: %d",NVLMAX+1);

                break;

            case 6:
                NVLMAX = 0;

                altura(raiz,0);

                printf("Quantidade de niveis: %d\n",NVLMAX+1);

                for(int i = 0;i <= NVLMAX;i++){
                    printf("\n");
                    nivelAtual(raiz,i,0);
                }

                printf("\n");

                break;
            
            default:
                printf("\nValor invalido\n");

                break;
        }
    }while(op!= 0);    
}

void inserir(int chave) {
    int flag, i;
    No *filho;

    flag = setNo(chave, &i, raiz, &filho);

    if (flag)
        raiz = criarNo(i, filho);
}

int setNo(int chave, int *pval, No *no, No **filho) {
    int pos;

    if (!no) {
        *pval = chave;
        *filho = NULL;

        return 1;
    }

    if (chave < no->chave[1]) 
        pos = 0;

    else
        for (pos = no->count; (chave < no->chave[pos] && pos > 1); pos--);
            if (chave == no->chave[pos]) {
                printf("\nNao e possivel inserir valor duplicado\n");

                return 0;
            }

    if (setNo(chave, pval, no->filha[pos], filho)) {
        if (no->count < MAX) 
            InserirValor(*pval, pos, no, *filho);

        else {
            divideNo(*pval, pval, pos, no, *filho, filho);
            
            return 1;
        }
    }

    return 0;
}

void InserirValor(int chave, int pos, No *no, No *filho) {
    int j = no->count;

    while (j > pos) {
        no->chave[j + 1] = no->chave[j];
        no->filha[j + 1] = no->filha[j];
        j--;
    }

    no->chave[j + 1] = chave;
    no->filha[j + 1] = filho;
    no->count++;
}

No *criarNo(int chave, No *filho) {
    No *novoNo;

    novoNo = (No *)malloc(sizeof(No));
    novoNo->chave[1] = chave;
    novoNo->count = 1;
    novoNo->filha[0] = raiz;
    novoNo->filha[1] = filho;

    return novoNo;
}

void divideNo(int chave, int *pval, int pos, No *no,No *filho, No **novoNo) {
    int mediana, j;

    if (pos > MIN)
        mediana = MIN + 1;

    else
        mediana = MIN;

    *novoNo = (No *)malloc(sizeof(No));
    j = mediana + 1;

    while (j <= MAX) {
        (*novoNo)->chave[j - mediana] = no->chave[j];
        (*novoNo)->filha[j - mediana] = no->filha[j];
        j++;
    }

    no->count = mediana;
    (*novoNo)->count = MAX - mediana;

    if (pos <= MIN)
        InserirValor(chave, pos, no, filho);
    
    else
        InserirValor(chave, pos - mediana, *novoNo, filho);
    
    *pval = no->chave[no->count];
    (*novoNo)->filha[0] = no->filha[no->count];
    no->count--;
}

void procura(No *noSelecionado) {
    int i;

    if(noSelecionado) {
        for(i = 0; i < noSelecionado->count; i++) {
            procura(noSelecionado->filha[i]);

            printf("%d ", noSelecionado->chave[i + 1]);
        }

        procura(noSelecionado->filha[i]);
    }
}

int busca(No *noSelecionado,int elem,int* achou) {
    int i, n;
    
    if(noSelecionado) {
        for (i = 0; i < noSelecionado->count; i++) {
            busca(noSelecionado->filha[i],elem,achou);

            if(noSelecionado->chave[i+1]==elem) {
                (*achou) =1;

                printf("\nAchou\n");

                return 1;
            } 
        }

        busca(noSelecionado->filha[i],elem,achou);
    }
}