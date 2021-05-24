#include <stdio.h>
#include <stdlib.h>
typedef struct node
{
    int page_no;
    struct node* left;
    struct node* right;
    struct node* hashnext;
}node;

node *hashtable[25];
node *head=NULL;
int count=0;
void printcache(int c,int req, FILE *fptr)
{
    fprintf(fptr,"%d-",req);
    if(c==0)
        fprintf(fptr,"Page Miss");
    else
        fprintf(fptr,"Page Hit");
    fprintf(fptr,"\n");
    node* temp=head;
    if(temp->right==NULL)
    {
        fprintf(fptr,"%d\n\n",temp->page_no);
        return;
    }
    while(temp!=NULL)
    {
        fprintf(fptr,"%d ",temp->page_no);
        temp=temp->right;
    }
    fprintf(fptr,"\n\n");
}
int checkpage(int hash, int pageno)
{
    node *temp=hashtable[hash];
    if(hashtable[hash]==NULL)
        return 0;
    while(temp!=NULL)
    {
    if(temp->page_no==pageno)
    {
        return 1;
    }
    else
    {
        temp=temp->hashnext;
    }
    }
    return 0;
}
node* create_node(int pageno)
{
    node *temp=(node*)malloc(sizeof(node));
    temp->page_no=pageno;
    temp->left=temp->right=NULL;
    temp->hashnext=NULL;
    return temp;
}

void delete_node(int req,int hash)
{
    node *temp=head,*temp1,*temp2,*temp3;
    while(temp->page_no!=req)
        temp=temp->right;
    if(count<=25)
        count--;
    if(temp!=head){
        temp->left->right=temp->right;
    }
    else{

        head=head->right;
    }
    if (temp->right!=NULL)
        temp->right->left=temp->left;
    free(temp);

}
void remove_hash(int req,int hash)
{
    node *temp=head,*temp1,*temp2=NULL,*temp3;
    temp1=hashtable[hash];
    if(temp1->hashnext==NULL)
        {
        hashtable[hash]=NULL;
        }
    else
    {
        while(temp1!=NULL &&temp1->page_no!=req)
        {
            temp2=temp1;
            temp1=temp1->hashnext;
        }
        if (temp2==NULL)
            hashtable[hash]=temp1->hashnext;
        else
            temp2->hashnext=temp1->hashnext;
    }
}
int alloc_cache(int req)
{
    node *temp=NULL,*temp1=NULL,*temp2,*temp3,*temp4;
    int hash=req%25,hash1,page;
    count++;
    int c=checkpage(hash,req);
    if(c==1)
    {
        remove_hash(req,hash);
        delete_node(req,hash);
        temp=create_node(req);
        temp1=head;
    }
    else{
    temp=create_node(req);
    if(count==1)
    {
        head=temp;
        hashtable[hash]=temp;
        return c;
    }
    else if(count<=25)
    {
        temp1=head;

    }
    else
    {
        temp2=head;
        hash1=head->page_no%25;
        page=head->page_no;
        head=head->right;
        remove_hash(page,hash1);
        temp1=head;
        free(temp2);
    }
    }
    while(temp1->right!=NULL)
    {
        temp1=temp1->right;
    }
    temp1->right=temp;
    temp->left=temp1;
    temp3=hashtable[hash];
    if(temp3==NULL)
        hashtable[hash]=temp;
    else
    {
        while(temp3->hashnext!=NULL)
        {
            temp3=temp3->hashnext;
        }
        temp3->hashnext=temp;
    }
    return c;
}

int main()
{
    FILE *fptr;
    fptr=fopen("output.txt","w");
    int i=0;
    if(fptr==NULL)
        exit(1);
    int req=0,lower=1,upper=100,c;
    for(i=0;i<1000;i++)
    {
        req=(rand()%(upper - lower +1)) + lower;
        c=alloc_cache(req);
        printcache(c,req,fptr);
    }
}
