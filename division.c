#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern char* strdup(const char*);

void delZeros(char *a){
  int i=0, l=0, len = strlen(a);
  while(a[l] == '0') l++; 
  if (l == len) {a[0] = '0'; a[1] = '\0'; return;}
  for (; i< len-l; i++) a[i] = a[i+l];
  a[i] = '\0';
}

int compare(char *a, char *b){
  int la = strlen(a), lb = strlen(b);
  if (la > lb) return 1;
  if (la < lb) return -1;
  return strcmp(a,b);
}

char *partSubtract(char *a, int lb, char *b, int *neg){
  int carry = 0, j=lb-1; char *c = calloc((strlen(a)+2),sizeof(char)); 
  for(int i=strlen(a)-1; i>=0; --i){
    if (j>=0) c[i] = (a[i] + '0' - b[j--] - carry); 
    else c[i] = a[i] - carry;
    if (c[i] < '0'){c[i] +=10; carry=1;}
    else carry = 0;
  }
  if (carry) *neg = 1; 
  return c; 
} 

void partDivide(char *a, int lb, char *b, char ***res){
  char i = '0'; int neg=0; char *diff, *prev = a; 
  while(1) {
    diff = partSubtract(prev, lb, b,&neg);
    if (neg) break;
    prev = diff; i++;   
  }  
  diff[0] = i; diff[1] = '\0'; 
  (*res)[0] = diff; (*res)[1] = prev;
}

char *slice(char *a, int end){
  int i; char *sub = malloc((end+2)*sizeof(char));
  for (i=0; i<end; ++i) sub[i] = a[i]; sub[i]='\0'; 
  return sub; 
}

char *adjust(char *a, char* r){
  int i=0, j=0, lr = strlen(r), la = strlen(a); 
  char *arr = calloc(la+lr+1,sizeof(char));
  while(r[i] == '0') i++;
  for (; i<lr; ++i) arr[j++] = r[i];
  for (; j<la; ++j) arr[j] = (a)[i++];
  arr[j] = '\0';
  free(a); 
  return arr; 
}

int isZero(char *a){
  int i=0; while(a[i] == '0') i++;
  return i == strlen(a);
}

char **divide_strings(char *a, char *b){
  char **res = malloc(2 * sizeof(char *));
  a = strdup(a); b = strdup(b);
  delZeros(a); delZeros(b); 
  int la = strlen(a), lb = strlen(b); 
  if (isZero(b)) { 
    fprintf(stderr, "Division by zero! Exit.\n");
    exit(EXIT_FAILURE);
  }
  if (compare(a,b)<0){
    if (la==1 && a[0] == '1') {
      res[0] = a; res[1] = b; 
      return res; 
    }
    b[0]='0'; b[1]='\0';  
    res[0] = b; res[1] = a;   
    return res;
  }  
  char *part, *qu; int ind=0, size=1, q=0;  
  qu = malloc((la+1)*sizeof(char)); 
  
  while (ind<la) {
    part = slice(a, size);
    if (compare(part,b)>=0){
      partDivide (part, lb, b, &res);
      qu[q++] = res[0][0]; ind++; 
      a= adjust(a,res[1]); delZeros(res[1]);
      if (isZero(res[1])) size = 1; 
      else size = strlen(res[1])+1;  
      free(part); continue; 
    }
    free(part); qu[q++] = '0';
    size++; ind++;  
  } 
  qu[q] = '\0'; delZeros(qu); 
  if (!isZero(a)) {free(res[1]); res[1] = a;}
  res[0] = qu; delZeros(res[1]);
  return res; 
}