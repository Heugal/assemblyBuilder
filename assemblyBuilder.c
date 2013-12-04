#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int count_word(FILE *);
int count_line(FILE *);

typedef struct _mem_ad_{
    char *line_num;
    char *address;
}mem_ad;

typedef struct _word_{
    char *word;
    int line_number;
} word;  

#define MAX_LINE 65

int main(int argc, char** argv) {
    int count = 0, num_words = 0, num_lines = 0, i = 0, line = 0, del_len = 0;
    FILE *ifp2, *ofp;
    char *token = NULL;
    
    for (count; count < argc; count++){
        printf("argv[%d] = %s\n", count, argv[count]);
    }
    
    ifp2 = fopen(argv[count-2], "r");
    if (ifp2 == NULL){
        printf("Cannot locate or open file %s.", argv[count-2]);
        exit(2);
    }
    ofp = fopen(argv[count-1], "w");
    if (ofp == NULL){
        printf("Cannot locate or open file %s.", argv[count-1]);
        exit(3);
    }
    
    num_words = count_word(ifp2);
    printf("number of words in the file: %d\n",num_words);
    num_lines = count_line(ifp2);
    printf("number of lines in the file: %d\n",num_lines);
    char lines[MAX_LINE];
    word line_array[num_words*2];
    word table_label[num_words+1];
    
    /*stores the assembly code into an array of structs containing a pointer 
     to the word and the line number where the word is found*/
    while (fgets(lines, sizeof(lines),ifp2)){
        token = strtok(lines, " ,!?\t");
        while(token != NULL){
            line_array[i].line_number = line;
            del_len = strlen(token);
            if (del_len >66){
                printf("File lines cannot be longer than 65 characters.");
                exit(0);
            }
            if (token[del_len-1] == '\n'){
                line++;
                token[del_len-2] = '\0';
            }
            line_array[i].word = malloc(MAX_LINE*sizeof(char));
            strcpy(line_array[i].word, token);
            token = strtok(NULL, " ,!?\t");
            i++;
        }
    }
    int table_ref = 0, len = 0;
    /*the heart of the assemblybuilder program starts here*/
    /*stores the assembly labels and their line numbers into an array*/
    for (i=0;i<num_words;i++){
        len = strlen(line_array[i].word);
        if (line_array[i].word[len-1] == ':'){
            line_array[i].word[len-1] = '\0';
            table_label[table_ref].word = malloc(MAX_LINE*sizeof(char));
            table_label[table_ref].word = line_array[i].word;
            table_label[table_ref].line_number = line_array[i].line_number;
            table_ref++;
        }
    }
    
    mem_ad address_reg[num_lines*2];
    int M2 = 0, M1 = 0 , acc3 = 0, acc2 = 0, acc1 = 0, table_trav = 0, mem_acc = 0, j = 0;
    int line3 = 0, line2 = 0, line1 = 0, line0 = 0, mem_line = 0, checksum = 0, incr = 0;
    char mem_builder[5], line_builder[5];
    for (i=0;i<num_words;i++){
        M2 = 0, M1 = 0, acc3 = 0, acc2 = 0, acc1 = 0;
        line3 = 0, line2 = 0, line1 = 0, line0 = 0;
        checksum = 0;
        address_reg[incr].address = (char *)malloc(4*sizeof(char));
        address_reg[incr].line_num = (char *)malloc(4*sizeof(char));
        
        if (strcmp(line_array[i].word, "halt") == 0){
            M2 = 0;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, ".word") == 0){
            M2 = 0;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, ".end") == 0){
            M2 = 0;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "ld") == 0){
            M2 = 1;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "st") == 0){
            M2 = 2;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "add") == 0){
            M2 = 3;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "sub") == 0){
            M2 = 4;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "lda") == 0){
            M2 = 5;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "jmp") == 0){
            M2 = 6;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "skip") == 0){
            M2 = 7;
            M1 = 0;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "skeq") == 0){
            M2 = 7;
            M1 = 1;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "skne") == 0){
            M2 = 7;
            M1 = 2;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "skgt") == 0){
            M2 = 7;
            M1 = 3;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "skge") == 0){
            M2 = 7;
            M1 = 4;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "sklt") == 0){
            M2 = 7;
            M1 = 5;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "skle") == 0){
            M2 = 7;
            M1 = 6;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "in") == 0){
            M2 = 8;
            M1 = 0;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "out") == 0){
            M2 = 8;
            M1 = 1;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "clr") == 0){
            M2 = 8;
            M1 = 2;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "inc") == 0){
            M2 = 8;
            M1 = 3;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "dec") == 0){
            M2 = 8;
            M1 = 4;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "neg") == 0){
            M2 = 8;
            M1 = 5;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "shftl") == 0){
            M2 = 8;
            M1 = 6;
            checksum = 1;
        }
        else if (strcmp(line_array[i].word, "shftr") == 0){
            M2 = 8;
            M1 = 7;
            checksum = 1;
        }
        if (checksum == 1){
            if ((M2 > 0) && (M2 < 7)){
                for (table_trav=0; table_trav < table_ref; table_trav++){
                    if (strcmp(line_array[i+1].word, table_label[table_trav].word)==0){
                        mem_acc = table_label[table_trav].line_number;
                        break;
                    } 
                }
                if (mem_acc/100 != 0)
                    acc3 = mem_acc/100;
                if (mem_acc/10 != 0)
                    acc2 = mem_acc/10;
                acc1 = mem_acc%10;
            }
            if (M2 == 7 || M2 == 8){
                j = incr;
                sprintf(mem_builder, "%d%d%d%d", M2, M1, 0, 0);
                incr = j;
            }
            else{
                j = incr;
                sprintf(mem_builder, "%d%d%d%d", M2, acc3, acc2, acc1);
                incr = j;
            }
            if (mem_builder != "0000"){
                strcpy(address_reg[incr].address, mem_builder);
            }
            else{
                j = incr;
                sprintf(mem_builder, "%03d", M2);
                incr = j;
                strcpy(address_reg[incr].address, mem_builder);
            }
            
            mem_line = line_array[i].line_number;
            if (mem_line/1000 != 0)
                line3 = mem_line/1000;
            if (mem_line/100 != 0)
                line2 = mem_line/100;
            if (mem_line/10 != 0)
                line1 = mem_line/10;
            line0 = mem_line%10;
        
            sprintf(line_builder, "%d%d%d%d", line3, line2, line1, line0);
            strcpy(address_reg[incr].line_num, line_builder);
            incr++;
        }
        
    }
    
    /*outputs the assembly code. compiled using windows, so to make a new line,
     the char '/r' (line feed) must be added along with the '/n' (new line)
     character*/
    int line_ct = 0, col = 0;
    for (i=0;i<=num_words;i++){
        checksum = 0;
        if (i == 0){
            fputs(address_reg[i].line_num, ofp);
            fputc(' ', ofp);
            fputs(address_reg[i].address, ofp);
            fputc('\t', ofp);
        }
        if (line_array[i].line_number != line_ct){
            line_ct++;
            fputc('\r', ofp);
            fputc('\n', ofp);
            fputs(address_reg[line_ct].line_num, ofp);
            fputc(' ', ofp);
            fputs(address_reg[line_ct].address, ofp);
            fputc('\t', ofp);
            col = 0;
        }
        if (line_ct%10 != 9){
            for (table_trav=0; table_trav < table_ref; table_trav++){
                if (strcmp(line_array[i].word, table_label[table_trav].word)==0){
                    if (line_array[i].line_number == table_label[table_trav].line_number)
                        checksum = 1;
                }
            }
            if (checksum == 1){
                fputs(line_array[i].word, ofp);
                fputc(':', ofp);
                fputc(' ', ofp);
            }
            else{
                fputc('\t', ofp);
                fputs(line_array[i].word, ofp);
            }
        }
        else{
                        for (table_trav=0; table_trav < table_ref; table_trav++){
                if (strcmp(line_array[i].word, table_label[table_trav].word)==0){
                    if (line_array[i].line_number == table_label[table_trav].line_number)
                        checksum = 1;
                }
            }
            if (checksum == 1){
                fputs(line_array[i].word, ofp);
                fputc(':', ofp);
                fputc(' ', ofp);
            }
            else{
                fputs(line_array[i].word, ofp);
            }
        }
    }
    
    for(i=0;i<num_words;i++){
        free(line_array[i].word);
    }
    fclose(ofp);
    fclose(ifp2);
    return (EXIT_SUCCESS);
}

int count_word(FILE *ifp){
    int c;
    int word = 0;
    while (c != EOF){
        c = getc(ifp);
        if (c == '\n' || c ==' '){
            word++;
        }
    }
    rewind(ifp);
    return word;
}

int count_line(FILE *ifp){
    int c;
    int line = 0;
    while (c != EOF){
        c = getc(ifp);
        if (c == '\n')
            line++;
    }
    rewind(ifp);
    return line;
}

