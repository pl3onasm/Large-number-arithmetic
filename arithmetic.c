/*
  ┌─────────────────────────────────────────────────────────┐
  │  File name: arithmetic.c                                │
  │  Author: David De Potter, pl3onasm@gmail.com            │
  │  License: refer to the license file in this repository  │
  │  Description: enables basic arithmetic with large       │
  │  integers represented as strings                        │ 
  └─────────────────────────────────────────────────────────┘
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern char *strdup(const char *);

void delZeros(char *a){
  int i=0, l=0, len = strlen((a));
  while(a[l] == '0') l++; 
  if (l == len) {a[0] = '0'; a[1] = '\0'; return;}
  for (; i< len-l; i++) a[i] = a[i+l];
  a[i] = '\0';
}

char *negate(char *a){
  int i=0, la = strlen(a); 
  char* b = calloc(la+2, sizeof(char));
  for (i=la; i>=1; --i) b[i] = a[i-1];
  b[0] = '-'; b[la+1] = '\0';
  free(a);
  return b; 
}

void swap(char **a, char **b){
  char *temp = *a;
  *a = *b; *b = temp;
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
  char i = '0'; int neg=0; char *diff, *prev = strdup(a); 
  while(1) {
    diff = partSubtract(prev, lb, b,&neg);
    if (neg) break;
    free(prev); prev = diff; i++;   
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
  for (; j<la; ++j) arr[j] = a[i++];
  arr[j] = '\0'; free(a); 
  return arr; 
}

int isZero(char *a){
  int i=0; while(a[i] == '0') i++;
  return i == strlen(a);
}

char **divide(char *a, char *b){
  char **res = malloc(2 * sizeof(char *));
  char *c=strdup(a); delZeros(c); delZeros(b); 
  int lc = strlen(c), lb = strlen(b); 
  if (isZero(b)) { 
    fprintf(stderr, "Division by zero! Exit.\n");
    exit(EXIT_FAILURE);
  }
  if (compare(c,b)<0){
    if (lc==1 && c[0] == '1') 
      {res[0] = c; res[1] = b; return res;}
    b[0]='0'; b[1]='\0';  
    res[0] = b; res[1] = c;   
    return res;
  }  
  char *part, *qu; int ind=0, size=1, q=0;  
  qu = malloc((lc+1)*sizeof(char)); 
  while (ind<lc) {
    part = slice(c, size);
    if (compare(part,b)>=0){
      partDivide (part, lb, b, &res);
      qu[q++] = res[0][0]; ind++; 
      c = adjust(c,res[1]); delZeros(res[1]);
      if (isZero(res[1])) size = 1; 
      else size = strlen(res[1])+1;  
      free(part); continue; 
    }
    free(part); qu[q++] = '0';
    size++; ind++;  
  } 
  qu[q] = '\0'; delZeros(qu); 
  if (!isZero(c)) {free(res[1]); res[1] = c;}
  res[0] = qu; delZeros(res[1]);
  return res; 
}

char *subtract(char *a, char *b){
  delZeros(a); delZeros(b);
  int neg= 0; 
  if (compare(a,b)<0) {swap(&a,&b); neg = 1;}
  int la = strlen(a), lb = strlen(b); 
  char *c = calloc(strlen(a)+1,sizeof(char));
  int carry= 0, j= lb-1; 
  for(int i=la-1; i>=0; --i){
    if (j>=0) c[i] = a[i] + '0' - b[j--] - carry; 
    else c[i] = a[i] - carry;
    if (c[i] < '0'){c[i] += 10; carry=1;}
    else carry = 0;
  }
  c[la]='\0'; delZeros(c);
  if (neg) c = negate(c); 
  return c; 
} 

char *add(char *a, char *b){
  delZeros(a); delZeros(b);
  if (compare(a,b)<0) swap(&a,&b);
  int la = strlen(a), lb = strlen(b); 
  char *c = calloc(strlen(a)+2,sizeof(char));
  int carry= 0, j= lb-1, i=0; 
  for (i=la-1; i>=0; --i){
    if (j>=0) {c[i+1] = a[i] + b[j--] - '0' + carry;}
    else c[i+1] = a[i] + carry; 
    if (c[i+1] > '9') {c[i+1] -= 10; carry=1;}
    else carry = 0;
  }
  if (carry) {c[0] = '1'; c[la+1] = '\0';}
  else {c[0] = '0'; delZeros(c);}
  return c; 
}

char *multiply(char *a, char *b){
  delZeros(a); delZeros(b);
  int la = strlen(a), lb = strlen(b); 
  char *c = calloc(la+lb+1,sizeof(char)); 
  for (int i=la-1; i>=0; --i)
    for (int j=lb-1; j>=0; --j){
      int s = c[i+j+1] + (a[i]-'0')*(b[j]-'0');  
      c[i+j+1] = s%10; c[i+j] += s/10; 
    }
  for (int i=0; i<la+lb; ++i) c[i] += '0';
  c[la+lb] = '\0'; delZeros(c); 
  return c; 
}

int powr(int n, int exp) {
  int p=1;
  while (exp!=0)
    if (exp%2 == 0) {n *= n; exp /= 2;} 
    else {p *= n; exp--;}
  return p;
}

int toDec(char *a){
  int res = 0; 
  for (int i=0; i<strlen(a); ++i) 
    res += (a[i]-'0')*powr(10,strlen(a)-i-1);
  return res; 
}

char *power(char *a, char *exp){
  delZeros(a); delZeros(exp);
  int la = strlen(a), lim = toDec(exp);
  char *c = strdup(a), *newc; 
  for (int i=0; i<lim; ++i) 
    {newc = multiply(c,c); free(c); c = newc;}
  return newc; 
}

void readInput(char **a){
  char n; int index = 0, cap = 128;
  char *arr = calloc(cap,sizeof(char)); 
  while (scanf("%c", &n) && n != '\n' && n != EOF){
    index++;
    if (index % cap == 0) {
      arr = realloc(arr, 2*index*sizeof(char));
      cap *= 2;
    }
    arr[index-1] = n;
  }
  *a = realloc(arr, (index)*sizeof(char));
  (*a)[index-1] = '\0'; 
}

int main(int argc, char *argv[]) {
  char op, *a, *b, *c, *d, **e; 
  scanf("%c\n", &op); 
  readInput(&a);  
  readInput(&b); 
  
  switch(op){
    case '+': printf("%s\n", c=add(a,b)); break;
    case '-': printf("%s\n", c=subtract(a,b)); break;
    case '*': printf("%s\n", c=multiply(a,b)); break;
    case '/': e=divide(a,b); printf("q: %s\nr: %s\n", \
              c=divide(a,b)[0], d=divide(a,b)[1]); 
              free(d); free(e); break;
    case '^': printf("%s\n", c=power(a,b)); break;
    default: printf("Incorrect operator!\n"); break;
  } 
  free(a); free(b); free(c); 
  return 0;
}