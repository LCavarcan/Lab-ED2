bool RemoveArvB(ArvB **arvore, int chave)
{
  if ((*arvore) == NULL){
    printf("Erro em RemoveArvB: Árvore inexistente\n");
    exit(1);
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
    return(true);
  }
  return(false);
}

bool RemoveArvBRec(NoArvB **no, int chave, bool *underflow)
{
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

     } else { /* achou a chave no nó folha, então ela pode ser removida. */
        RemoveChaveEmNo(*no, pos);
     }
     /* verifica se a folha ficou em underflow */

     (*underflow) = NoComMenosChavesQuePermitido(*no); 
     return(true);   /* remoção bem sucedida */
  } else { /* procura a chave nos nós abaixo */
     if ((*no)->filha[pos] != NULL) {
        if (RemoveArvBRec(&((*no)->filha[pos]), chave, underflow)) {
	            /* a chave foi encontrada e removida da folha (raiz da árvore
	            filha). Trata um possível underflow e verifica novamente se
	            ele será propagado para cima (volta da recursão). */
	          if (*underflow)
               TrataNoComMenosChavesQuePermitido(no, pos);
	          (*underflow) = NoComMenosChavesQuePermitido(*no);
	          return(true);
        } else {
	          return(false); /* chego na folha e não achou a chave */
        }
     } else {  /* estamos na raiz e a chave não está neste nível, logo ela não existe. */
        (*underflow) = false;
         return(false);
     }
  }
}

bool BuscaChaveNoArvB(NoArvB *no, int chave, int *pos)
{
  for ((*pos)=0; (no->chave[*pos] < chave) && (*pos) < no->nregs; (*pos)++);

  /* "pos" contém a posição da chave no nó */
  if (((*pos) < no->nregs)&&(no->chave[*pos] == chave))
     return(true);

  /* "pos" contém a posição da árvore filha na qual a chave poderá ser encontrada */
  return(false);
}


void TrocaChaveComAntecessora(NoArvB *no, int pos)
{
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

void TrataNoComMenosChavesQuePermitido(NoArvB **no, int pos)
{
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

void RemoveChaveEmNo(NoArvB *no, int pos)
{
  int i;
  /* desloca as chaves e filhas para trás */
  for(i=pos; i < no->nregs; i++)
  {
      no->chave[i] = no->chave[i+1];
      no->filha[i] = no->filha[i+1];
  }
  /* completa o deslocamento para a filha à direita da última chave */ 
  no->filha[i]   = no->filha[i+1];
  no->filha[i+1] = NULL;
  /* ajusta o número de registros */
  no->nregs--;
}

bool NoComMaisChavesQuePossivel(NoArvB *no)
{
  return(no->nregs > no->ordem);
}

bool NoComMenosChavesQuePermitido(NoArvB *no)
{
  return(no->nregs < (no->ordem/2));
}