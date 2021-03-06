#include <stdio.h>  
#include <stdlib.h>  
#include <stdio.h>

#define STATUS_BEGIN 0
#define STATUS_ANAS_CHAR 1
#define STATUS_ANAS_BIT  2
#define STATUS_END 0

#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128

char BITS[] = {BIT7,BIT6,BIT5,BIT4,BIT3,BIT2,BIT1,BIT0};
int status = STATUS_BEGIN;
char symbol = 0;
//char anyresult[256];  

int main(void)  
{  
     FILE * fp;
     FILE * out;    
     char *line = NULL;  
     size_t len = 0;  
     ssize_t read;

     char *outbuff = (char *)malloc(1024*1024*1024);
     char *anyresult = (char *)malloc(256);
     char *databuff = (char*)malloc(256);
     char *datahead = databuff;
     char *resulthead = anyresult;

     char *outP = outbuff;
     //memset(line,0,1024);
     memset(outbuff,0,1024*1024*1024); 
     memset(anyresult,0,256); 
     memset(databuff,0,256); 

     fp = fopen("hakaku.txt", "r");  
     if (fp == NULL)  
         exit(EXIT_FAILURE);  

     out = fopen("out.txt", "wb");  
     int lineindex = 0;
     while (1) {  
         read = getline(&line, &len, fp);
         
         if(read == -1) {
             printf("read finish %d \n,lineindex is %d",read,lineindex);
             break;
         }
         lineindex++;

         //printf("Retrieved line of length %zu :\n", read);  
         //printf("%s", line);  
         
         sprintf(outP,"//%s",line);
         
         outP = outP + read + 1;
         outP++;
         *outP = '\n';
         //we should analyse the line
         int len = read;
         printf("read lineindex is %d str is %s\n",lineindex,line);

         char *lineP = malloc(1024);
         memset(lineP,0,1024);
         memcpy(lineP,line,strlen(line));
         if(len == 1) {
             int codelen = strlen(resulthead);
             sprintf(outP,"%s",resulthead);
             outP += codelen;

             *databuff--;
             *databuff = 0;
             int datalen = strlen(datahead);
             sprintf(outP,"%s",datahead);
             //printf("wangsl,outP is %s \n",outP);
             outP += datalen;

             char buff[32];
             memset(buff,0,32);
             sprintf(buff,"};",0);
             int len = strlen(buff);
             //buff+=len;
             buff[len] = '\n';

             sprintf(outP,"%s",buff);
             outP += len;
             outP[len] = '\n';
             outP++;
             outP[len] = '\n';
             //outP++;
             //clear all
             memset(datahead,0,256);
             memset(resulthead,0,256);
             databuff = datahead;
             anyresult = resulthead;
             continue;
         }
         
         for(;len > 0;len--) 
         {
              int strLen = strlen(line);
              if(lineP[0] == '.' || lineP[0] == '*') 
              {	
                  //this is a graph,we should change it to a bit
                  char mark[8];
                  int index = 0;
                  memset(mark,0,8);

                  for(;index <8;index++) 
                  {
                      if(lineP[index] == '*') 
                      {
                          mark[index] = 1;
                      } 
                      else 
                      {
                          mark[index] = 0;
                      } 
                  }

                  index = 0;
                  char val = 0;
                  for(;index < 8;index++) {
                      val += BITS[index]*mark[index];
                  }
                  sprintf(databuff,"0x%x,",val&0x00ff);
                  //printf("databuff is %s \n",datahead);
                  databuff += (strlen(databuff));
                  break;
              } 
              else if(lineP[0] == 'c')
              {
                  //this is a char,we should change to 0
                  memset(anyresult,0,256);
                  for(;strLen >0;strLen--) 
                  {
                      if(*(lineP+strLen) == ' ') {
                          break;
                      }
                  }

                  char *asic = lineP + strLen;
                  int data = strtol(asic,NULL,16);
                  char buff[128];
                  memset(buff,0,128);
                  sprintf(buff,"char font_%s[] = { \n",&data);
                  int len = strlen(buff);
                  buff[len + 1] = '\n';

                  sprintf(anyresult,"%s",buff);
                  anyresult += len + 1;
              }
         }         

         //printf("%s", line);  
     }  
       
     if (line)  
         free(line);  

     printf("result is %s",outbuff);
     fwrite(outbuff,strlen(outbuff),1,out);
     exit(EXIT_SUCCESS);  
}  



