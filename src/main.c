#include <stdio.h>

int main()
{
    int M,N,T;
    int i,j,length;
    //int A_Piture[4][4];
    int _TurnFlag;
    
    scanf("%d %d %d",&M,&N,&T);
   
  //  printf("value = %d %d %d",M,N,T);
    if(T < 0 || T > 1)
        printf("T vaule is false.\n");
    else
        _TurnFlag = T;
    int A_Piture[N][M];
  //  printf("value = %d %d %d.\n",M,N,T);
    for(i = 0;i < M;i++)
    {
       // printf("%d \t",i);
        for(j = 0;j < N;j++)
        {
             scanf("%d ",&(A_Piture[i][j]));  
           // printf("i = %d,j = %d,value = %d.\n",i,j,A_Piture[j][i]);
        }   
        printf("\n");
    }
   // printf("value = %d %d %d.\n",M,N,T);
 /*   for(i=0;i < M;i++)
    {
        for(j = 0;j < N;j++)
	     printf("%d \t",A_Piture[i][j]);
        printf("\n");
    }
    printf("\n");*/
    int tmp;
    if(_TurnFlag)
    {
       for(i=0;i < (M+1)/2;i++)
        {
            for(j = 0;j < N;j++)
            {
                tmp = A_Piture[i][j];
                A_Piture[i][j] = A_Piture[M-1-i][j];
               // printf("%d %d %d\t",A_Piture[M-1-i][j],M-1-i,j);
                A_Piture[M-1-i][j] = tmp;
            }
           tmp = 0;
            printf("\n");
        } 
        printf("_TurnFlag = 1\n");
    } 
    else
    {
        for(i=0;i < M;i++)
        {
            for(j = 0;j < (N+1)/2;j++)
            {
                tmp = A_Piture[i][j];
                A_Piture[i][j] = A_Piture[i][N-1-j];
                A_Piture[i][N-1-j] = tmp;
            }
           tmp = 0;
            printf("\n");
        } 
        
    }
     for(i=0;i < M;i++)
    {
        for(j = 0;j < N;j++)
	        printf("%d ",A_Piture[i][j]);
        if(i != M-1)
        printf("\n");
    }
    return 0;
}