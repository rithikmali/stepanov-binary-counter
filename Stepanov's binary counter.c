#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>
#include<math.h>
#include"assignment_2.h"

#ifndef bin_struct
#define bin_struct

// used to keep track of the elements that have been compared with the main node
struct track_node 
{
    int data;
    struct track_node* next;
};
typedef struct track_node tnode;

// used to keep the maximum element and a list a tnodes to keep track of elements
struct main_node 
{
    int ele;
    tnode* list;
    struct main_node* next;
};
typedef struct main_node mnode;

//Used to represent columns.
struct column_node
{
    mnode* list;
};
typedef struct column_node cnode;

#endif

//function to create and initialize main node
mnode* create_mnode(int ele)
{
    mnode* temp = (mnode*)malloc(sizeof(mnode));
    temp->ele=ele;
    temp->list=NULL;
    temp->next=NULL;
    return temp;
}

//function to create and initialize track node
tnode* create_tnode(int ele)
{
    tnode* temp = (tnode*)malloc(sizeof(tnode));
    temp->data=ele;
    temp->next=NULL;
    return temp;
}

//function to print "20 10 x 30 1"
void printbefore(mnode* curr)
{
    mnode* temp = curr;
    tnode* tt;
    while (temp!=NULL)
    {
        tt = temp->list;
        printf("%d ",temp->ele);
        while (tt!=NULL)
        {
            printf("%d ",tt->data);
            tt=tt->next;
        }
        temp=temp->next;
        if(temp!=NULL)
            printf("x ");
    }
}

//function to print "= 30 20 1"
void printafter(mnode* curr)
{
    tnode* tt = curr->list;
    printf("= %d ",curr->ele);
    while (tt!=NULL)
    {
        printf("%d ",tt->data);
        tt=tt->next;
    }
    printf("\n");
}

//this function goes through the columns to check if there are anymore comparisons to be made
// it stops comparing, when the last column is filled => theere are no more comparisons to be made
void checkForNotPowerOf2(cnode* c,int length)
{
    int last = ceil(log2f(length));
    int k=0;
    while (c[last].list==NULL && k<last)
    {
        //if theres a node in any column, it enters the if statement
        if(c[k].list!=NULL)
        {
            //if theres only 1 main node in that column, move it to next column
            if(c[k].list->next==NULL)
            {
                mnode* curr=c[k+1].list;
                //if theres no node in the next column, make this the first node
                if(curr==NULL)
                {
                    c[k+1].list=c[k].list;
                }
                //if theres already a node in the next column, append the current node to that node
                else
                {
                    c[k+1].list->next=c[k].list;
                }
                c[k].list=NULL;
            }

            //if there are 2 nodes in the column, then do the comparisons and move it to next column
            else
            {
                mnode* curr = c[k].list;
                printbefore(curr);
                if(curr->ele < curr->next->ele)
                {
                    tnode* temp = create_tnode(curr->ele);
                    tnode* second = curr->next->list;
                    curr->next->list = temp;
                    temp->next=second;
                    c[k].list=curr->next;
                    free(curr);
                    
                }
                else
                {
                    tnode* temp = create_tnode(curr->next->ele);
                    tnode* second = curr->list;
                    curr->list = temp;
                    temp->next=second;
                    free(curr->next);
                    curr->next=NULL;
                }
                curr = c[k].list;
                printafter(curr);

                //move the node to the next column after comparisons
                curr = c[k+1].list;
                if(curr==NULL)
                {
                    c[k+1].list=c[k].list;
                }
                else
                {
                    c[k+1].list->next=c[k].list;
                }
                
                c[k].list=NULL;
            }
            
        }
        ++k;
    }
    
}


void find_second_greatest(int *numbers, int length)
{
    int i=0,j=0;
    cnode* c = (cnode*)malloc(sizeof(cnode)*(ceil(log2f(length))+1));
    for(int k=0;k<ceil(log2f(length))+1;k++)
    {
        c[k].list=NULL;
    }
    int k=0;

    //iterates through all elements
    while(i<length)
    {
        while(k<ceil(log2f(length)))
        {
            //if theres no element in the first column, make a new node
            if(c[k].list==NULL && k==0)
            {
                mnode* temp = create_mnode(numbers[i]);
                c[k].list=temp;
                break;
            }
            //if theres already a element in the first column, compare the elements and place the node in the next column
            else if(k==0)
            {
                printf("%d x %d ", c[k].list->ele , numbers[i]);
                if(c[k].list->ele < numbers[i])
                {
                    tnode* temp = create_tnode(c[k].list->ele);
                    c[k].list->ele = numbers[i];
                    tnode* second = c[k].list->list;
                    c[k].list->list = temp;
                    temp->next=second;
                }
                else
                {
                    tnode* temp = create_tnode(numbers[i]);
                    tnode* second = c[k].list->list;
                    c[k].list->list = temp;
                    temp->next=second;
                }
                printf("= %d %d\n", c[k].list->ele , c[k].list->list->data);
                mnode* curr = c[k+1].list;
                if(curr==NULL)
                {
                    c[k+1].list=c[k].list;
                }
                else
                {
                    c[k+1].list->next=c[k].list;
                }
                c[k].list=NULL;
            }

            //break if current column is empty
            else if(c[k].list==NULL)
                break;
            //break if only one main node in a column
            else if(c[k].list->next==NULL)
                break;
            //if there are more than one main nodes in the same column, then compare them and move it to next column 
            else
            {
                mnode* curr = c[k].list;
                printbefore(curr);
                if(curr->ele < curr->next->ele)
                {
                    tnode* temp = create_tnode(curr->ele);
                    tnode* second = curr->next->list;
                    curr->next->list = temp;
                    temp->next=second;
                    c[k].list=curr->next;
                    free(curr);
                    
                }
                else
                {
                    tnode* temp = create_tnode(curr->next->ele);
                    tnode* second = curr->list;
                    curr->list = temp;
                    temp->next=second;
                    free(curr->next);
                    curr->next=NULL;
                }
                curr = c[k].list;
                printafter(curr);
                curr = c[k+1].list;

                
                if(curr==NULL)
                {
                    c[k+1].list=c[k].list;
                }
                else
                {
                    c[k+1].list->next=c[k].list;
                }
                
                c[k].list=NULL;
            }
            ++k;
        }
        k=0;
        ++i;
    }
    
    if(log2f(length)!=ceil(log2f(length)))
        checkForNotPowerOf2(c,length);

    tnode* current= c[(int)ceil(log2f(length))].list->list;
    int max=INT_MIN;
    while (current!=NULL)
    {
        if(current->data > max)
            max = current->data;
        current=current->next;
    }
    printf("\n%d\n",max);
}

#if 1

int main()
{
    int n;
    scanf("%d",&n);
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        scanf("%d ",&arr[i]);
    }
    
    find_second_greatest(arr,n);
}

#endif