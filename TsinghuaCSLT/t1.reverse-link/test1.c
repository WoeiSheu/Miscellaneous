#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

typedef struct  av tav;

struct av {
    tav *lnk;
    int v;
};

tav * gen_lnk(int n) {
    int i;
    if (n<=0) return NULL;

    tav *p = (tav *)malloc(sizeof(tav));
    p->v=0;
    p->lnk=NULL;

    tav *q=p;

    for (i=1; i<n; i++){
        q->lnk=(tav*)malloc(sizeof(tav));
        q->lnk->lnk=0;
        q->lnk->v=i;
        q=q->lnk;
    }

    return p;
}

void print_lnk(tav *p) {
    int i=0;
    while (p != NULL) {
        printf("%d : %d \n", i, p->v);
        p = p->lnk;
        i++;
    }
}
//prototype needs to fill
tav * reverse_lnk(tav *p) {
    if (p==NULL||p->lnk==NULL) {
        return NULL;
    }
    tav* pre = (tav*)malloc(sizeof(tav));
    tav* cur = (tav*)malloc(sizeof(tav));
    tav* next = (tav*)malloc(sizeof(tav));

    pre = p;
    cur = pre->lnk;
    next = cur->lnk;
    pre->lnk = NULL;

    while(next!=NULL) {
        cur->lnk = pre;
        pre = cur;
        cur = next;
        next = cur->lnk;
    }

    cur->lnk = pre;
    return cur;
}

int main(){
    tav *p = gen_lnk(10);
    print_lnk(p);

    printf("\n");

    p=reverse_lnk(p);
    print_lnk(p);
}
