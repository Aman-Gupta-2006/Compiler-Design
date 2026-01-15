#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isWhileKeyword(char keyword[]){
    int s = 0;
    int i = 0;
    int n = strlen(keyword);
    
    while(i < n){
        char ch = keyword[i];
        switch(s){
            case 0: 
                if(ch == 'w'){
                    s = 1;
                } else {
                    return 0;
                }
                
                break;
                
            case 1:
                if(ch == 'h'){
                    s = 2;
                } else {
                    return 0;
                }
                break;
            case 2: 
                if(ch == 'i'){
                    s = 3;
                } else {
                    return 0;
                }
                break;
            case 3:
                if(ch == 'l'){
                    s = 4;
                } else {
                    return 0;
                }
                break;
            case 4: 
                if(ch == 'e'){
                    s = 5;
                } else {
                    return 0;
                }
                break;
            case 5:
                return 0;
        }
        
        i++;
    }
    return s == 5;
}

int main() {
    char keyword[100];

    printf("Enter a Keyword: ");
    scanf("%s", keyword);

    if (isWhileKeyword(keyword)) {
        printf("Valid Keyword\n");
        printf("Token Generated is <Keyword, %s>", keyword); 
    } else {
        printf("Invalid Keyword\n");
    }
        

    return 0;
}