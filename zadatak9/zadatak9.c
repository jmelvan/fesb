#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node * Edge;
struct Node{
  int n;
  Edge L, R;
};

Edge newEdge();
Edge addNew(Edge);
Edge add(Edge, int);
Edge search(Edge, int);
Edge findMax(Edge);
Edge deleteInput(Edge);
Edge delete(Edge, int);
int searchInput();
int display(Edge, int);
int tree(Edge);
int deleteAll(Edge);

int main(){
  Edge root = newEdge();
  int loop = 1, choose = 0;
  while(loop){
    printf("\n1-Ispis\n2-Unos\n3-Brisanje\n4-Trazenje\n5-Izlaz");
    printf("\nUnesite komandu: ");
    scanf(" %d", &choose);
    switch(choose){
      case 1:
        tree(root->R);
      break;
      case 2:
        root->R = addNew(root->R);
      break;
      case 3:
        root->R = deleteInput(root->R);
      break;
      case 4:
        searchInput(root->R);
      break;
      case 5:
        loop = 0;
      break;
    }
  }
  deleteAll(root);
  return 0;
}

Edge newEdge(){
  Edge new = (Edge)malloc(sizeof(struct Node));
  if(new == NULL) { printf("\nMalloc failed\n"); return NULL; }
  new->L = NULL;
  new->R = NULL;
  return new;
}

Edge addNew(Edge position){
  int n = 0;
  printf("\nUnesite novu vrijednost:");
  scanf(" %d", &n);
  return add(position, n);
}

Edge add(Edge position, int n){
  if(position == NULL){
    position = newEdge();
    position->n = n;
  } else {
    if(position->n == n) return NULL;
    position->n < n ? (position->R = add(position->R, n)) : (position->L = add(position->L, n));
  }
  return position;
}

Edge deleteInput(Edge position){
  int n = 0;
  printf("\nUnesite vrijednost za brisanje:");
  scanf(" %d", &n);
  return delete(position, n);
}

Edge delete(Edge position, int n){
  if(position == NULL) return position;
  position->n < n ? (position->R = delete(position->R, n)) : (position->L = delete(position->L, n));
  if(position->n == n){
    if(position->R == NULL){
      Edge temp = position->L;
      free(position);
      return temp;
    } else if(position->L == NULL){
      Edge temp = position->L;
      free(position);
      return temp;
    } else {
      Edge temp = findMax(position->L);
			position->n = temp->n;
			position->L = delete(position->L, temp->n);
    }
  }
  return position;
}

int searchInput(Edge position){
  int n = 0;
  printf("\nUnesite vrijednost za trazenje:");
  scanf(" %d", &n);
  if(search(position, n) == NULL)
    printf("\nElement ne postoji");
  else
    printf("\nElement postoji");
  return 0;
}

Edge search(Edge position, int n){
  if(position == NULL) return NULL;
  if(position->n == n) return position;
  if(position->n < n)
    return search(position->R, n);
  else
    return search(position->L, n);
  return position;
}

Edge findMax(Edge position){
  if(position->R != NULL) return findMax(position->R);
  return position;
}

int tree(Edge position){
  printf("\n");
  display(position, 0);
  return 0;
}

int display(Edge position, int depth){
  if(position == NULL) return 0;
  position->R != NULL && display(position->R, depth+1);
  for(int i = 0; i < depth; i++)
    (i == 0 || i == depth) ? printf("  |") : printf("   ");
  depth > 1 && printf("|");
  printf("--%d\n", position->n);
  position->L != NULL && display(position->L, depth+1);
  return 0;
}

int deleteAll(Edge position){
  if(position == NULL) return 0;
  deleteAll(position->L);
  deleteAll(position->R);
  free(position);
  return 0; 
}