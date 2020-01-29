#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 20

typedef struct {
    char** text;
    int size;
}Text;
//Очистка памяти
void clear_memory(char** text, int size){
    for(int i=0;i<size;i++){
        free(text[i]);
    }
    free(text);
}
//Вывод текста
void output_text(char** text, int size){
    for(int i=0;i<size;i++){
        printf("%s\n",text[i]);
    }
    printf("\n");
}
//Проверка на одинаковые предложения
int is_clone(char** text, char* sentense, int size){
    for(int i=0;i<size;i++){
        if(strlen(text[i])==strlen(sentense)){
            int str_len=strlen(sentense);
            int flag=0;
            for(int j=0;j<str_len;j++){
                if (tolower(text[i][j])==tolower(sentense[j]))
                    flag++;
            }
            if(flag==str_len)
                return 1;
        }
    }
    return 0;
}
//Счетчик слов
int word_count(char* sentense){
    int count=1;
    for(int i=1;i<strlen(sentense);i++){
        if((sentense[i-1]==' ' || sentense[i-1]==',' || sentense[i-1]=='\t') && (sentense[i]!=' ' && sentense[i]!=',' && sentense[i]!='.'))
            count++;
    }
    return count;
}
//Удаление предложений, в которых кол-во слов нечетно
int delete_odd_sentense(char** text,int size){
    int deleted=0;
    for(int i=0;i<size;i++){
        if((word_count(text[i]))%2!=0){
            free(text[i]);
            for(int j=i;j<size-1;j++){
                text[j]=text[j+1];
            }
            size--;
            deleted++;
            i--;
        }
    }
    output_text(text,size);
    return deleted;
}
//Преобразование всех слов без цифр в предложении
void format_sentense_to_upper(char* sentense, int size){
    for(int i=0;i<size;i++){
        if(isalpha(sentense[i]) && (i==0 || sentense[i-1]==' ' || sentense[i-1]==',' || sentense[i-1]=='\t')){
            int j=i;
            int flag=0;
            while(sentense[j]!=' ' && sentense[j]!=',' && sentense[j]!='\t' && j<size && sentense[j]!='.'){
                if(isdigit(sentense[j]))
                    flag=1;
                j++;
            }
            if (flag==0){
                for(int k=i;k<j;k++){
                    sentense[k]=toupper(sentense[k]);
                }
                sentense[j-1]=tolower(sentense[j-1]);
            }
            i=j--;
        }
    }
}
//в тексте
void format_text_to_upper(char** text,int size){
    for(int i=0;i<size;i++){
        format_sentense_to_upper(text[i],strlen(text[i]));
    }
}
//Проверка на наличие заглавных букв
int check_lower_sentense(char* sentense){
    int flag=0;
    for(int i=0;i<strlen(sentense);i++){
        if(isupper(sentense[i])){
            flag=1;
            break;
        }
    }
    return flag==1?0:1;
}


//Вывод предложений в которых нет заглавных букв
void print_lower_sentense(char** text, int size){
    for(int i=0;i<size;i++){
        if(check_lower_sentense(text[i])){
            printf("%s\n",text[i]);
        }
    }
}
//ввод предложения
char* input_sentense(){
    int max_size=SIZE;
    int current_size=0;
    char* sentense=(char*)malloc(max_size* sizeof(char));
    char c;
    do{
        if(max_size-1<=current_size){
            max_size+=10;
            sentense=(char*)realloc(sentense,max_size* sizeof(char));
        }
        c=fgetc(stdin);
        sentense[current_size]=c;
        if(sentense[0]==' ' || sentense[0]=='\t')
            continue;
        current_size++;
        if(sentense[0]=='\n')
            return sentense;
    }while (!strchr(".",c));
    sentense[current_size]='\0';
    return sentense;
}
//ввод текста
Text input_text(){
    char** text=(char**)malloc(sizeof(char*));
    int current_size=0;
    while (1){
        char* sentense=input_sentense();
        if(sentense[0]=='\n')
            break;
        if(is_clone(text,sentense,current_size))
            continue;
        text[current_size]=sentense;
        current_size++;
        text=(char**)realloc(text,(current_size+1)* sizeof(char*));
    }
    Text created_text;
    created_text.text=text;
    created_text.size=current_size;
    return created_text;
}
//Компаратор
int cmp (const void* i, const void* k){
    int* i1=(int*)i;
    int* k1=(int*)k;
    if(*i1>*k1)
        return 1;
    else if(*i1==*k1)
        return 0;
    else
        return -1;
}

int* find_date_sentense(char* s, int* data_size){//функция возвращает массив дат в предложении и через ссылку кол-во дат
    int size = SIZE;
    char pattern[] = "d00m00y0000";
    int pattern_size = 11;
    int * DATES = (int*)malloc(size * sizeof(int));
    int num = 0;
    int i, j;
    int *prefix_func = (int*) malloc(pattern_size * sizeof(int));
    prefix_func[0] = 0;
    for(i = 1, j = 0; i < pattern_size; i++){
        while(j > 0 && pattern[j] != pattern[i]){
            j = prefix_func[j-1];
        }
        if(pattern[j] == pattern[i])
            j++;
        prefix_func[i] = j;
    }
    int size_str=strlen(s);
    for (i = 0, j = 0; i < size_str; i++) {
        while (j > 0 && ((pattern[j] != s[i] || (pattern[j] != '0' && isdigit(s[i])) ||
                          (pattern[j] == '0' && !isdigit(s[i]))) && (pattern[j] != '0' && isdigit(s[i]))))
            j = prefix_func[j - 1];
        if (pattern[j] == s[i] || pattern[j] == '0' && isdigit(s[i]))
            j++;
        if (j == pattern_size) {
            if ((s[i - j + 2] > '3' || s[i - j + 2] == '3' && s[i - j + 3] > '1')
                || (s[i - j + 5] > '1' || s[i - j + 5] == '1' && s[i - j + 5] > '1')) {
                j = 0;
                continue;
            }

            DATES[num++] = (s[i - j + 8] - '0') * 10000000
                           + (s[i - j + 9] - '0') * 1000000
                           + (s[i - j + 10] - '0') * 100000
                           + (s[i - j + 11] - '0') * 10000
                           + (s[i - j + 5] - '0') * 1000
                           + (s[i - j + 6] - '0') * 100
                           + (s[i - j + 2] - '0') * 10
                           + (s[i - j + 3] - '0') * 1;
            if (num == size) {
                size <<= 1;
                DATES = (int *) realloc(DATES, size * sizeof(int));
            }
            j = 0;
        }
    }
    *data_size=num;
    free(prefix_func);
    return DATES;
}

void print_find_date(char** text, int size){
    int* ALLDATES=(int*)malloc(sizeof(int)*100);
    int maxdates_count=100;
    int alldates_count=0;
    int dates_count=0;
    for(int i=0;i<size;i++){
        int *DATES=find_date_sentense(text[i], &dates_count);
        for(int j=0;j<dates_count;j++){
            ALLDATES[alldates_count++]=DATES[j];
            if(alldates_count+5==maxdates_count){
                maxdates_count+=100;
                ALLDATES=(int*)realloc(ALLDATES,maxdates_count * sizeof(int));
            }
        }
        free(DATES);
    }
    qsort(ALLDATES,alldates_count, sizeof(int),cmp);
    for(int i=0;i<alldates_count;i++){
        printf("%d:%d:%d\n",ALLDATES[i]%100,(ALLDATES[i]%10000)/100,ALLDATES[i]/10000);
    }
    free(ALLDATES);
}

int main() {
    int deleted=0;
    int step=0;
    printf("Enter the text consisting of Latin letters, after entering the last sentence, press ENTER\n");
    Text txt=input_text();
    printf("_________________________________________________________________________________________________________________________________\n");
    printf("1.Find in the text dates and display them in ascending order in the format DD: MM: YYYY\n");
    printf("2.Delete all sentences in which the number of words is odd.                                                                      \n");
    printf("3.Convert all words in which there are no numbers so that all letters except the last are uppercase.                             \n");
    printf("4.Print all sentences in which there are no capital letters.                                                                     \n");
    printf("5.Exit the program.                                                                                                              \n");
    printf("_________________________________________________________________________________________________________________________________\n");
    output_text(txt.text,txt.size);
    while(1){
        printf("What to do? (enter a number from 1 to 5)\n");
        scanf("%d",&step);
        switch(step){
            case 1:{
                print_find_date(txt.text,txt.size-deleted);
                break;
            }
            case 2:{
                deleted=delete_odd_sentense(txt.text,txt.size-deleted);
                break;
            }
            case 3:{
                format_text_to_upper(txt.text,txt.size-deleted);
                printf("The text is formatted!\n");
                output_text(txt.text,txt.size-deleted);
                break;
            }
            case 4:{
                print_lower_sentense(txt.text,txt.size-deleted);
                break;
            }
            case 5:{
                clear_memory(txt.text,txt.size-deleted);
                printf("the program has completed its work!\n");
                return 0;
            }
            default:{
                printf("no such command\n");
            }
        }
    }
}
