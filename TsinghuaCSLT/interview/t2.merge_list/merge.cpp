#include <cstdio>
#include <cstdlib>

struct Node {
    Node(int value): value(value), next(NULL) { }
    int value;
    Node *next;
};

Node *createList(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp)
        return NULL;
    static const int BUFLEN = 16;
    char buf[BUFLEN];
    Node *head = NULL, *p = NULL;
    while (fgets(buf, BUFLEN, fp)) {
        Node *q = new Node(atoi(buf));
        if (!head) {
            head = p = q;
        } else {
            p->next = q;
            p = q;
        }
    }
    fclose(fp);
    return head;
}

void printList(Node *p) {
    while (p) {
        printf("%d ", p->value);
        p = p->next;
    }
    printf("\n");
}


void sort_links(Node *&head1,Node *&head2,int count1,int count2){

int i,j,temp=0;
Node *p;

  for(i=0;i<count1-1;++i) {
   for(p=head1;p->next!=NULL;p=p->next) {
    if(p->value>p->next->value)
    {
    temp=p->value;
    p->value=p->next->value;
    p->next->value=temp;
    }
   }
  }
  for(i=0;i<count2-1;++i) {
   for(p=head2;p->next!=NULL;p=p->next) {
    if(p->value>p->next->value)
    {
    temp=p->value;
    p->value=p->next->value;
    p->next->value=temp;
    }
   }
  }
}


int main(int argc, char *argv[]) {
    Node *head1 = createList("list1.txt");
    Node *head2 = createList("list2.txt");
    printf("list 1: ");
    printList(head1);
    printf("list 2: ");
    printList(head2);
    Node *head = NULL;

    //TODO: your code here
    sort_links(head1,head2,20,20);
    printf("sorted list 1: ");
    printList(head1);
    printf("sorted list 2: ");
    printList(head2);

    Node * head3 = (Node *)malloc(sizeof(Node));
    head3->next = NULL;
    Node *p = (Node *)malloc(sizeof(Node));
    Node *q = (Node *)malloc(sizeof(Node));

    while(head1!=NULL&&head2!=NULL) {
        if(head1->value<=head2->value) {
            p=head1->next;
            head1->next=head3->next;
            head3->next=head1;
            head1=p;
        } else {
            q=head2->next;
            head2->next=head3->next;
            head3->next=head2;
            head2=q;
        }
    }

    if(head1!=NULL) {
        p=head1;
        while(p!=NULL) {
            q=p->next;
            p->next=head3->next;
            head3->next=p;
            p=q;
        }
    }
    if(head2!=NULL) {
       q=head2;
       while(q!=NULL) {
        p=q->next;
        q->next=head3->next;
        head3->next=q;
        q=p;
       }
    }

    q=head3->next;
    head3->next=NULL;
    while(q!=NULL)
    {
       p=q->next;
       q->next=head3->next;
       head3->next=q;
       q=p;
    }

    printf("merged list: ");
    printList(head3);

    printList(head);
    return 0;
}
