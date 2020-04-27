#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct super_block{
    unsigned int i_state[16];
    unsigned int d_state[32];
};

struct inode_list{
    unsigned type : 1;
    unsigned di : 10;
    unsigned sin : 10;
    unsigned din : 10;
    int size;
    struct tm time;
};

struct file_info{
    char data[129];
};

struct directory_info{
    char name[18][5];
    short number[18];
};

struct indirect_info{
    short number[64];
};

union data_block{
    struct file_info file;
    struct directory_info directory;
    struct indirect_info indirect;
};

struct my_file_system{
    unsigned boot_block : 16;
    struct super_block super;
    struct inode_list inode[512];
    union data_block block[1024];
}myfs;

struct present_working_directory{
    char name[10][5];
    char number[10];
}pwd;

struct linked{
    int number;
    struct linked *next;
};

struct ls_list{
    char name[4];
    int inode;
    int type;
    int size;
    struct tm time;
    struct ls_list *next;
};

struct mytree{
    char name[4];
    int step;
    struct mytree *next;
    struct mytree *down;
};

struct linked *tlist[1024];
struct mytree *tree;


void mycat(char [], char [], char []);
void myls(char []);
struct ls_list *ls_link(struct inode_list *);
int ls_count(struct ls_list *);
void ls_sort(struct ls_list *, int);
void ls_print(struct ls_list *, int, int);
void myshowfile(int, int, char []);
void print_byte(int, int, int);
void mypwd(struct present_working_directory);
void mycp(char[], char[]);
void mycpto(char [], char []);
void mycpfrom(char [], char []);
void mymkdir(char []);
void myrmdir(char []);
void myrm(char []);
void mytouch(char []);
void myshowinode(int);
void mytree(struct mytree *);

void usage_plus(unsigned int [], int, int);
void usage_minus(unsigned int [], int, int);
int usage_check(unsigned int [], int, int);
int usage_count(unsigned int [], int n);
int where_am_i(struct present_working_directory);
int get_inode();
int get_block();
struct present_working_directory where_is_there(char []);
int find_file(struct inode_list *, char []);
int del_file(struct inode_list *, char []);
void print_block_list(int);
void tree_add(char [], int, struct present_working_directory, struct mytree *);
void tree_del(char [], int, struct present_working_directory, struct mytree *);

static int del_down = 1, del_next = 0;

int main(){
    int i, n, m;
    char mycmd[][12] = {"mymkfs", "myfs_shell", "myls", "mycat", "myshowfile", "mypwd", "mycd", "mycp", "mycpto", "mycpfrom", "mymkdir", "myrmdir", "myrm", "mymv", "mytouch", "myshowinode", "myshowblock", "mystate", "mytree", "byebye"};
    char command[150], cmd[5][30];
    time_t now = time(NULL);
    FILE *save, *load;
    struct inode_list *inode = NULL;

    //초기화
    strcpy(pwd.name[0], "/");
    pwd.number[0] = 1;
    usage_plus(myfs.super.i_state, 16, 1);
    usage_plus(myfs.super.d_state, 32, 1);
    inode = &myfs.inode[0];
    inode->type = 0;
    inode->size = 0;
    inode->time = *localtime(&now);
    inode->di = 1;
    for(i=0; i<1024; i++){
        tlist[i] = malloc(sizeof(struct linked));
        tlist[i] -> number = i;
        tlist[i] -> next = NULL;
    }
    tree = malloc(sizeof(struct mytree));
    strcpy(tree->name, "/");
    tree->step = 1;
    tree->next = NULL;

    //명령어 받기
    while(1){
        printf("$ ");
        scanf("%[^\n]", command);
        getchar();

        if(!strcmp(command, "myfs_shell")){
            if((load = fopen("myfs", "r")) == NULL){
                printf("error : no myfs\n");
            }
            else{
                printf("\n");
                break;
            }
        }
        else if(!strcmp(command, "mymkfs")){
            if((load = fopen("myfs", "r")) == NULL){
                save = fopen("myfs", "w");
            }
            else{
                printf("error : myfs exists\n");
            }
        }
        else{
            printf("error : myfs_shell is not started\n");
        }
        printf("\n");
    }


    while(1){
        printf("[");
        mypwd(pwd);
        printf(" ]$ ");
        for(i=0; i<5; i++){
            strcpy(cmd[i], "");
        }
        //명령어 받기
        scanf("%[^\n]", command);
        getchar();
        sscanf(command, "%s %s %s %s %s", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4]);
        for(i=0; i<20; i++){
            if(!strcmp(cmd[0], mycmd[i])){
                if(i==0 || i==1){
                    i = 0;
                    break;
                }
                i--;
                break;
            }
        }

        //명령어 해석
        switch(i){
            //mkmyfs, myfs_shell
            case 0 :
                printf("error : improper command\n");
                break;
                //myls
            case 1 :
                myls(cmd[1]);
                break;
                //mycat
            case 2 :
                mycat(cmd[1], cmd[2], cmd[3]);
                break;
                //myshowfile
            case 3 :
                n = atoi(cmd[1]);
                m = atoi(cmd[2]);
                myshowfile(n, m, cmd[3]);
                break;
                //mypwd
            case 4 :
                mypwd(pwd);
                printf("\n");
                break;
                //mycd
            case 5 :
                pwd = where_is_there(cmd[1]);
                break;
                //mycp
            case 6 :
                printf("mycp is not ready\n");
                break;
                //mycpto
            case 7 :
                mycpto(cmd[1], cmd[2]);
                break;
                //mycpfrom
            case 8 :
                mycpfrom(cmd[1], cmd[2]);
                break;
                //mymkdir
            case 9 :
                mymkdir(cmd[1]);
                break;
                //myrmdir
            case 10 :
                myrmdir(cmd[1]);
                break;
                //myrm
            case 11 :
                myrm(cmd[1]);
                break;
                //mymv
            case 12 :
                printf("mymv is not ready\n");
                break;
                //mytouch
            case 13 :
                mytouch(cmd[1]);
                break;
                //myshowinode
            case 14 :
                n = atoi(cmd[1]);
                myshowinode(n);
                break;
                //myshowblock
            case 15 :
                n = atoi(cmd[1]);
                if(usage_check(myfs.super.d_state, 32, n)){
                    printf("%s\n", myfs.block[n-1].file.data);
                }
                else{
                    printf("error : empty data block\n");
                }
                break;
                //mystate
            case 16 :
                printf("free inode : %d\n", 512 - usage_count(myfs.super.i_state, 16));
                printf("free data block : %d\n", 1024 - usage_count(myfs.super.d_state, 32));
                break;
                //mytree
            case 17 :
                mytree(tree);
                break;
                //byebye
            case 18 :
                return 0;
            default :
                system(command);
        }
        printf("\n");
    }
    return 0;
}

//mycat 함수
void mycat(char name[], char option[], char file[]){
    int n, check, last;
    struct inode_list *inode;

    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    check = find_file(inode, name);

    if(check == 0){
        printf("error : no such file\n");
        return;
    }
    else if(myfs.inode[check-1].type == 0){
        printf("error : %s is not regular file\n", name);
        return;
    }
    else if(!strcmp(option, "")){
        last = inode->size % 128;
        n = tlist[(myfs.inode[check-1].di)-1] -> number;
        while(1){
            if(tlist[n]->next == NULL){
                for(int i=0; i<last; i++){
                    printf("%c", myfs.block[n].file.data[i]);
                }
                return;
            }
            printf("%s", myfs.block[n].file.data);
            n = tlist[n] -> next -> number;
        }
    }
}

//myls 함수
void myls(char option[]){
    struct ls_list *head, *head2;
    int n = where_am_i(pwd), i = 0, l = 0, k, count, a, b;
    struct inode_list *inode;
    char opt[2] = {0};

    //옵션 구분하기
    if(!strcmp(option, "-il") || !strcmp(option, "-li")){
        i = 1;
        l = 1;
    }
    else if(!strcmp(option, "-i")){
        i = 1;
    }
    else if(!strcmp(option, "-l")){
        l = 1;
    }
    inode = &myfs.inode[n-1];

    //현재 디렉터리와 이전 디렉터리를 나타내는 자기참조구조체 생성
    head = malloc(sizeof(struct ls_list));
    head2 = malloc(sizeof(struct ls_list));
    strcpy(head -> name, ".");
    head -> inode = n;
    head -> type = 0;
    head -> size = 0;
    head -> time = myfs.inode[head->inode-1].time;
    head -> next = head2;
    strcpy(head2 -> name, "..");
    for(k=0; k<10; k++){
        if(!strcmp(pwd.name[k], "")){
            if(k == 1){
                k = 0;
                break;
            }
            k = k - 2;
            break;
        }
    }
    head2 -> inode = pwd.number[k];
    head2 -> type = 0;
    head2 -> size = 0;
    head2 -> time = myfs.inode[head2->inode-1].time;

    //나머지 링크 연결하기
    head2 -> next = ls_link(inode);
    count = ls_count(head);
    //정렬하기
    ls_sort(head, count);
    //출력하기
    ls_print(head, i, l);
}

//myls 리스트 만들기
struct ls_list *ls_link(struct inode_list *inode){
    int i, j, k, list, check, block;
    static int count = 0;
    struct ls_list *temp = NULL;

    //다이렉트 블럭에서
    list = inode -> di;
    for(i=0; i<18; i++){
        if(count == i){
            if(!strcmp(myfs.block[list-1].directory.name[i], "")){
                count = 0;
                return NULL;
            }
            else{
                if(myfs.block[list-1].directory.number[i] != 0){
                    temp = malloc(sizeof(struct ls_list));
                    strcpy(temp -> name, myfs.block[list-1].directory.name[i]);
                    temp -> inode = myfs.block[list-1].directory.number[i];
                    temp -> type = myfs.inode[(temp->inode)-1].type;
                    temp -> size = myfs.inode[(temp->inode)-1].size;
                    temp -> time = myfs.inode[(temp->inode)-1].time;
                    count++;
                    temp -> next = ls_link(inode);
                    return temp;
                }
                check = 1;
                break;
            }
        }
    }
    if(check == 0){
        //싱글 인다이렉트 블럭에서
        block = inode -> sin;
        for(i=0; i<64; i++){
            list = myfs.block[block-1].indirect.number[i];
            for(j=0; j<18; j++){
                if(count == (19 + i*64 + j)){
                    if(!strcmp(myfs.block[list-1].directory.name[j], "")){
                        count = 0;
                        return NULL;
                    }
                    else{
                        if(myfs.block[list-1].directory.number[j] != 0){
                            temp = malloc(sizeof(struct ls_list));
                            strcpy(temp -> name, myfs.block[list-1].directory.name[j]);
                            temp -> inode = myfs.block[list-1].directory.number[j];
                            temp -> type = myfs.inode[temp->inode-1].type;
                            temp -> size = myfs.inode[temp->inode-1].size;
                            temp -> time = myfs.inode[temp->inode-1].time;
                            count++;
                            temp -> next = ls_link(inode);
                            return temp;
                        }
                        check = 1;
                        break;
                    }
                }
            }
        }
    }
    if(check == 0){
        //더블 인다이렉트 블럭에서
        for(i=0; i<64; i++){
            block = myfs.block[(inode -> din)-1].indirect.number[i];
            for(j=0; j<64; j++){
                list = myfs.block[block-1].indirect.number[j];
                for(k=0; k<18; k++){
                    if(count == (1172 + i*64 + j*64 + k)){
                        if(!strcmp(myfs.block[list-1].directory.name[k], "")){
                            count = 0;
                            return NULL;
                        }
                        else{
                            if(myfs.block[list-1].directory.number[k] != 0){
                                temp = malloc(sizeof(struct ls_list));
                                strcpy(temp -> name, myfs.block[list-1].directory.name[k]);
                                temp -> inode = myfs.block[list-1].directory.number[k];
                                temp -> type = myfs.inode[temp->inode-1].type;
                                temp -> size = myfs.inode[temp->inode-1].size;
                                temp -> time = myfs.inode[temp->inode-1].time;
                                count++;
                                temp -> next = ls_link(inode);
                                return temp;
                            }
                            check = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    count++;
    temp = ls_link(inode);
    return temp;
}

//myls 파일/디렉터리 개수 세기
int ls_count(struct ls_list *head){
    int i = 1;
    if(head == NULL){
        return 0;
    }
    return (1 + ls_count(head -> next));
}

//myls 정렬 함수
void ls_sort(struct ls_list *head, int count){
    int i, j;
    struct ls_list *a = head, *b = head, *temp = malloc(sizeof(struct ls_list));

    for(i=0; i<count-1; i++){
        for(j=i+1; j<count; j++){
            if(strcmp(a->name, b->next->name)>0){
                strcpy(temp->name, a->name);
                strcpy(a->name, b->next->name);
                strcpy(b->next->name, temp->name);

                temp->inode = a->inode;
                a->inode = b->inode;
                b->inode = temp->inode;

                temp->type = a->type;
                a->type = b->type;
                b->type = temp->type;

                temp->size = a->size;
                a->size = b->size;
                b->size = temp->size;

                temp->time = a->time;
                a->time = b->time;
                b->time = temp->time;
            }
            b = (b->next);
        }
        a = (a->next);
        b = a;
    }
}

//myls 출력하기
void ls_print(struct ls_list *head, int i, int l){
    if(head == NULL){
        return;
    }
    if(i){
        printf("%4d ", head -> inode);
    }
    if(l){
        printf("%c ", (head -> type) ? '-' : 'd');
        printf("%5d ", head -> size);
        printf("%d/%d/%d %02d:%02d:%02d ", head->time.tm_year + 1900, head->time.tm_mon + 1, head->time.tm_mday, head->time.tm_hour, head->time.tm_min, head->time.tm_sec);
    }
    printf("%-4s\n", head->name);

    ls_print(head -> next, i, l);
}

//myshowfile 함수
void myshowfile(int num1, int num2, char file[]){
    int n;
    char name[4] = {0};
    struct inode_list *inode;

    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(file, "%4s", name);

    n = find_file(inode, name);
    if(n==0){
        printf("error : no such file\n");
        return;
    }
    print_byte(num1, num2, n);
}

//바이트단위 출력
void print_byte(int num1, int num2, int n){
    int i, start1, start2, count, num;
    union data_block *block;

    //시작위치 설정
    start1 = num1 / 128;
    start2 = num1 % 128;
    count = num2 - num1 + 1;
    num = tlist[(myfs.inode[n-1].di)-1]->number;
    block = &(myfs.block[num]);

    //시작위치 찾기
    while(1){
        if(start1 == 0){
            i = start2 - 1;
            while(1){
                if(count==0){
                    printf("\n");
                    return;
                }
                if(i==128){
                    num = tlist[num] -> next -> number;
                    block = &(myfs.block[num]);
                    i = 0;
                }
                printf("%c", block -> file.data[i]);
                i++;
                count--;
            }
        }
        else{
            num = tlist[num] -> next -> number;
            block = &(myfs.block[num]);
            start1--;
        }
    }
}

//현재 위치 출력
void mypwd(struct present_working_directory pwd){
    int i;
    for(i=0; i<10; i++){
        if(!printf("%s", pwd.name[i])){
            break;
        }
        else if(i != 0){
            printf("/");
        }
    }
}

//mycp 함수
void mycp(char load[], char save[]){

}

//mycpto 함수
void mycpto(char file[], char save[]){
    char temp[128] = {0};
    int i, n, check, f_size, num_block, num_block_remain;
    FILE *cpsave;
    struct inode_list *inode;

    //myfs안의  파일 정보 가져오기
    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(file, "%4s", file);
    check = find_file(inode, file);
    if(check == 0){
        printf("error : no such file\n");
        return;
    }
    else if(myfs.inode[check-1].type == 0){
        printf("error : %s is not regular file\n", file);
        return;
    }
    else{
        //파일 저장하기
        if((cpsave = fopen(save, "wb")) == NULL){
            printf("error : cannot open %s on host file system\n", save);
            return;
        }
        else{
            f_size = myfs.inode[check-1].size;
            num_block = f_size / 128;
            num_block_remain = f_size % 128;
            n = (myfs.inode[check-1].di)-1;
            for(i=0; i<num_block; i++){
                fwrite(myfs.block[n].file.data, 128, 1, cpsave);
                n = tlist[n]->next->number;
            }
            for(i=0; i<num_block_remain; i++){
                fwrite(&myfs.block[n].file.data[i], 1, 1, cpsave);
            }
            fclose(cpsave);
            return;
        }
    }
}

//mycpfrom 함수
void mycpfrom(char load[], char file[]){
    char temp[128] = {0};
    int i, j, k, l, n, check, list, block, number, number2, f_size, num_block, num_block_remain;
    FILE *cpload;
    struct inode_list *inode;
    time_t now = time(NULL);

    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(file, "%4s", file);
    check = find_file(inode, file);
    if(check != 0){
        printf("error : %s already exists\n", file);
        return;
    }
    else if((cpload = fopen(load, "rb")) == NULL){
        printf("error : cannot open %s on host file system\n", load);
        return;
    }
    else{
        //파일의 기본정보 갖고오기
        fseek(cpload, 0, SEEK_END);
        f_size = ftell(cpload);
        num_block = f_size / 128;
        num_block_remain = f_size % 128;
        rewind(cpload);
        number = get_inode();
        number2 = get_block();
        //다이렉트 블럭에서
        list = inode -> di;
        for(i=0; i<18; i++){
            if(!strcmp(myfs.block[list-1].directory.name[i], "")){
                strcpy(myfs.block[list-1].directory.name[i], file);
                myfs.block[list-1].directory.number[i] = number+1;
                myfs.inode[number].type = 1;
                myfs.inode[number].size = f_size;
                myfs.inode[number].time = *localtime(&now);
                myfs.inode[number].di = number2+1;
                if(num_block != 0){
                    check = fread(&temp, 128, 1, cpload);
                    strcpy(myfs.block[number2].file.data, temp);
                    num_block--;
                }
                else{
                    for(l=0; l<num_block_remain; l++){
                        myfs.block[number2].file.data[l] = temp[l];
                    }
                    for(i=0; i<10; i++){
                        if(!strcmp(pwd.name[i], "")){
                            i = i - 1;
                            break;
                        }
                    }
                    tree_add(file, i, pwd, tree);
                    return;
                }
                if(num_block != 0){
                    myfs.inode[number].sin = get_block()+1;
                    for(int m=0; m<64; m++){
                        myfs.block[(myfs.inode[number].sin)-1].indirect.number[m] = 0;
                    }
                    for(l=0; l<64; l++){
                        block = get_block();
                        myfs.block[(myfs.inode[number].sin)-1].indirect.number[l] = block+1;
                        if(l == 0){
                            tlist[(myfs.inode[number].di)-1] -> next = tlist[block];
                        }
                        else{
                            tlist[(myfs.block[(myfs.inode[number].sin)-1].indirect.number[l-1])-1] -> next = tlist[block];
                        }
                        check = fread(temp, 128, 1, cpload);
                        strcpy(myfs.block[block].file.data, temp);
                        num_block--;
                        if(num_block == 0){
                            block = get_block();
                            myfs.block[(myfs.inode[number].sin)-1].indirect.number[l+1] = block+1;
                            if(l == 0){
                                tlist[myfs.block[(myfs.inode[number].sin)-1].indirect.number[l]] -> next = tlist[block];
                                tlist[block]->next = NULL;
                            }
                            else{
                                tlist[(myfs.block[(myfs.inode[number].sin)-1].indirect.number[l])-1] -> next = tlist[block];
                            }
                            check = fread(temp, num_block_remain, 1, cpload);
                            for(l=0; l<num_block_remain; l++){
                                myfs.block[block].file.data[l] = temp[l];
                            }
                            for(i=0; i<10; i++){
                                if(!strcmp(pwd.name[i], "")){
                                    i++;
                                    break;
                                }
                            }
                            tree_add(file, i, pwd, tree);
                            return;
                        }
                    }
                }
            }
        }
        //싱글 인다이렉트 블럭에서
        block = inode -> sin;
        if(block == 0){
            block = get_block()+1;
            inode->sin = block;
            for(i=0; i<64; i++){
                myfs.block[block-1].indirect.number[i] = 0;
            }
        }
        for(i=0; i<64; i++){
            list = myfs.block[block-1].indirect.number[i];
            if(list == 0){
                block = get_block()+1;
                if(i==0){
                    tlist[(inode->di)-1]->next = tlist[block-1];
                }
                else{
                    tlist[myfs.block[block-1].indirect.number[i-1]]->next = tlist[block-1];
                }
                for(l=0; l<18; l++){
                    myfs.block[block-1].directory.number[l] = 0;
                    strcpy(myfs.block[block-1].directory.name[l], "");
                }
            }
            for(j=0; j<18; j++){
                if(!strcmp(myfs.block[list-1].directory.name[j], "")){
                    strcpy(myfs.block[list-1].directory.name[j], file);
                    myfs.block[list-1].directory.number[j] = number+1;
                    myfs.block[list-1].directory.number[i] = number+1;
                    myfs.inode[number].type = 1;
                    myfs.inode[number].size = f_size;
                    myfs.inode[number].time = *localtime(&now);
                    myfs.inode[number].di = number2+1;
                    if(num_block != 0){
                        check = fread(temp, 128, 1, cpload);
                        strcpy(myfs.block[number2].file.data, temp);
                        num_block--;
                    }
                    else{
                        for(l=0; l<num_block_remain; l++){
                            myfs.block[number2].file.data[l] = temp[l];
                        }
                        for(i=0; i<10; i++){
                            if(!strcmp(pwd.name[i], "")){
                                i++;
                                break;
                            }
                        }
                        tree_add(file, i, pwd, tree);
                        return;
                    }
                    if(num_block != 0){
                        myfs.inode[number].sin = get_block()+1;
                        for(int m=0; m<64; m++){
                            myfs.block[(myfs.inode[number].sin)-1].indirect.number[m] = 0;
                        }
                        for(l=0; l<64; l++){
                            block = get_block();
                            myfs.block[(myfs.inode[number].sin)-1].indirect.number[l] = block+1;
                            if(l == 0){
                                tlist[(myfs.inode[number].di)-1] -> next = tlist[block];
                            }
                            else{
                                tlist[(myfs.block[(myfs.inode[number].sin)-1].indirect.number[l-1])-1] -> next = tlist[block];
                            }
                            check = fread(temp, 128, 1, cpload);
                            strcpy(myfs.block[block].file.data, temp);
                            num_block--;
                            if(num_block == 0){
                                block = get_block();
                                myfs.block[(myfs.inode[number].sin)-1].indirect.number[l+1] = block+1;
                                if(l == 0){
                                    tlist[myfs.block[(myfs.inode[number].sin)-1].indirect.number[l]] -> next = tlist[block];
                                    tlist[block]->next = NULL;
                                }
                                else{
                                    tlist[(myfs.block[(myfs.inode[number].sin)-1].indirect.number[l])-1] -> next = tlist[block];
                                }
                                check = fread(temp, num_block_remain, 1, cpload);
                                for(l=0; l<num_block_remain; l++){
                                    myfs.block[block].file.data[l] = temp[l];
                                }
                                for(i=0; i<10; i++){
                                    if(!strcmp(pwd.name[i], "")){
                                        i++;
                                        break;
                                    }
                                }
                                tree_add(file, i, pwd, tree);
                                return;
                            }
                        }
                        return;
                    }
                }
            }
        }
        //더블 인다이렉트 블럭에서
        if(inode->din == 0){
            inode->din = get_block()+1;
            for(i=0; i<64; i++){
                myfs.block[(inode->din)-1].indirect.number[i] = 0;
            }
        }
        printf("error : cannot save file\n");
        return;
        for(i=0; i<64; i++){
            block = myfs.block[(inode->din)-1].indirect.number[i];
            if(block == 0){
                block = get_block();
                for(l=0; l<64; l++){
                    myfs.block[block-1].indirect.number[l] = 0;
                }
            }
            for(j=0; j<64; j++){
                list = myfs.block[block-1].indirect.number[j];
                if(list == 0){
                    list = get_block()+1;
                    if(j==0){
                        tlist[(inode->di)-1]->next = tlist[block-1];
                    }
                    else{
                        tlist[myfs.block[block-1].indirect.number[j-1]]->next = tlist[block-1];
                    }
                    for(l=0; l<18; l++){
                        myfs.block[block-1].directory.number[l] = 0;
                        strcpy(myfs.block[block-1].directory.name[l], "");
                    }
                }
                for(k=0; k<18; k++){
                    if(!strcmp(myfs.block[list-1].directory.name[k], "")){
                        strcpy(myfs.block[list-1].directory.name[k], file);
                        myfs.block[list-1].directory.number[k] = number+1;
                        myfs.inode[number].type = 1;
                        myfs.inode[number].size = f_size;
                        myfs.inode[number].time = *localtime(&now);
                        myfs.inode[number].di = number2+1;
                        return;
                    }
                }
            }
        }
    }
}

//mymkdir 함수
void mymkdir(char name[]){
    int n, i, j, k, l, check, block, list, number, number2;
    struct inode_list *inode;
    time_t now = time(NULL);

    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(name, "%4s", name);
    check = find_file(inode, name);
    if(check != 0){
        printf("error : %s already exists\n", name);
        return;
    }
    else{
        number = get_inode();
        number2 = get_block();
        //다이렉트 블럭에서
        list = inode -> di;
        for(i=0; i<18; i++){
            if(!strcmp(myfs.block[list-1].directory.name[i], "")){
                strcpy(myfs.block[list-1].directory.name[i], name);
                myfs.block[list-1].directory.number[i] = number+1;
                myfs.inode[number].type = 0;
                myfs.inode[number].size = 0;
                myfs.inode[number].time = *localtime(&now);
                myfs.inode[number].di = number2+1;
                for(i=0; i<18; i++){
                    strcpy(myfs.block[number2].directory.name[i], "");
                    myfs.block[number2].directory.number[i] = 0;
                }
                for(i=0; i<10; i++){
                    if(!strcmp(pwd.name[i], "")){
                        i++;
                        break;
                    }
                }
                tree_add(name, i, pwd, tree);
                return;
            }
        }
        //싱글 인다이렉트 블럭에서
        block = inode -> sin;
        if(block == 0){
            block = get_block()+1;
            inode->sin = block;
            for(i=0; i<64; i++){
                myfs.block[block-1].indirect.number[i] = 0;
            }
        }
        for(i=0; i<64; i++){
            list = myfs.block[block-1].indirect.number[i];
            if(list == 0){
                block = get_block()+1;
                if(i==0){
                    tlist[(inode->di)-1]->next = tlist[block-1];
                }
                else{
                    tlist[myfs.block[block-1].indirect.number[i-1]]->next = tlist[block-1];
                }
                for(l=0; l<18; l++){
                    myfs.block[block-1].directory.number[l] = 0;
                    strcpy(myfs.block[block-1].directory.name[l], "");
                }
            }
            for(j=0; j<18; j++){
                if(!strcmp(myfs.block[list-1].directory.name[j], "")){
                    strcpy(myfs.block[list-1].directory.name[j], name);
                    myfs.block[list-1].directory.number[j] = number+1;
                    myfs.inode[number].type = 0;
                    myfs.inode[number].size = 0;
                    myfs.inode[number].time = *localtime(&now);
                    myfs.inode[number].di = number2+1;
                    for(i=0; i<18; i++){
                        strcpy(myfs.block[number2].directory.name[i], "");
                        myfs.block[number2].directory.number[i] = 0;
                    }
                    for(i=0; i<10; i++){
                        if(!strcmp(pwd.name[i], "")){
                            i++;
                            break;
                        }
                    }
                    tree_add(name, i, pwd, tree);
                    return;
                }
            }
        }
        //더블 인다이렉트 블럭에서
        if(inode->din == 0){
            inode->din = get_block()+1;
            for(i=0; i<64; i++){
                myfs.block[(inode->din)-1].indirect.number[i] = 0;
            }
        }
        for(i=0; i<64; i++){
            block = myfs.block[(inode->din)-1].indirect.number[i];
            if(block == 0){
                block = get_block();
                for(l=0; l<64; l++){
                    myfs.block[block-1].indirect.number[l] = 0;
                }
            }
            for(j=0; j<64; j++){
                list = myfs.block[block-1].indirect.number[j];
                if(list == 0){
                    list = get_block()+1;
                    if(j==0){
                        tlist[(inode->di)-1]->next = tlist[block-1];
                    }
                    else{
                        tlist[myfs.block[block-1].indirect.number[j-1]]->next = tlist[block-1];
                    }
                    for(l=0; l<18; l++){
                        myfs.block[block-1].directory.number[l] = 0;
                        strcpy(myfs.block[block-1].directory.name[l], "");
                    }
                }
                for(k=0; k<18; k++){
                    if(!strcmp(myfs.block[list-1].directory.name[k], "")){
                        strcpy(myfs.block[list-1].directory.name[k], name);
                        myfs.block[list-1].directory.number[k] = number+1;
                        myfs.inode[number].type = 0;
                        myfs.inode[number].size = 0;
                        myfs.inode[number].time = *localtime(&now);
                        myfs.inode[number].di = number2+1;
                        for(i=0; i<18; i++){
                            strcpy(myfs.block[number2].directory.name[i], "");
                            myfs.block[number2].directory.number[i] = 0;
                        }
                        for(i=0; i<10; i++){
                            if(!strcmp(pwd.name[i], "")){
                                i++;
                                break;
                            }
                        }
                        tree_add(name, i, pwd, tree);
                        return;
                    }
                }
            }
        }
    }
}

//myrmdir 함수
void myrmdir(char file[]){
    int i, n, check;
    struct inode_list *inode, i_clear;
    union data_block d_clear;
    struct ls_list *temp = malloc(sizeof(struct ls_list));


    i_clear.type = 0;
    i_clear.size = 0;
    i_clear.di = 0;
    i_clear.sin = 0;
    i_clear.din = 0;
    strcpy(d_clear.file.data, "");

    //파일이 있는지 확인
    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(file, "%4s", file);
    check = find_file(inode, file);
    if(check == 0){
        printf("error : no such directory\n");
        return;
    }
    else if(myfs.inode[check-1].type == 1){
        printf("error : %s is a file\n", file);
        return;
    }
    else{
        //파일 제거
        temp -> next = ls_link(&myfs.inode[check-1]);
        if(temp -> next != NULL){
            printf("error : %s is not empty\n", file);
            return;
        }
        usage_minus(myfs.super.i_state, 16, check);
        usage_minus(myfs.super.d_state, 32, myfs.inode[check-1].di);
        n = tlist[myfs.block[(myfs.inode[check-1].sin)-1].indirect.number[0]]->number;
        while(1){
            usage_minus(myfs.super.d_state, 32, n+1);
            myfs.block[n] = d_clear;
            if(tlist[n] -> next == NULL){
                break;
            }
            check = tlist[n] -> next -> number;
            tlist[n] -> next = NULL;
            n = check;
        }
        for(i=0; i<10; i++){
            if(!strcmp(pwd.name[i], "")){
                i++;
                break;
            }
        }
        tree_del(file, i, pwd, tree);
    }
}

//myrm 함수
void myrm(char file[]){
    int i, n, check;
    struct inode_list *inode, i_clear;
    union data_block d_clear;

    i_clear.type = 0;
    i_clear.size = 0;
    i_clear.di = 0;
    i_clear.sin = 0;
    i_clear.din = 0;
    strcpy(d_clear.file.data, "");

    //파일이 있는지 확인
    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(file, "%4s", file);
    check = del_file(inode, file);
    if(check == 0){
        printf("error : no such file\n");
        return;
    }
    else if(myfs.inode[check-1].type == 0){
        printf("error : %s is a directory\n", file);
        return;
    }
    else{
        //파일 제거
        usage_minus(myfs.super.i_state, 16, check);
        usage_minus(myfs.super.d_state, 32, myfs.inode[check-1].di);
        n = tlist[myfs.block[(myfs.inode[check-1].sin)-1].indirect.number[0]]->number;
        while(1){
            usage_minus(myfs.super.d_state, 32, n+1);
            myfs.block[n] = d_clear;
            if(tlist[n] -> next == NULL){
                break;
            }
            check = tlist[n] -> next -> number;
            tlist[n] -> next = NULL;
            n = check;
        }
        for(i=0; i<10; i++){
            if(!strcmp(pwd.name[i], "")){
                i++;
                break;
            }
        }
        tree_del(file, i, pwd, tree);
    }
}

//mytouch 함수
void mytouch(char file[]){
    int i, j, k, l, n, check, list, block, number, number2;
    struct inode_list *inode;
    time_t now = time(NULL);

    n = where_am_i(pwd);
    inode = &myfs.inode[n-1];
    sscanf(file, "%4s", file);
    check = find_file(inode, file);

    if(check){
        myfs.inode[check-1].time = *localtime(&now);
        return;
    }
    else{
        number = get_inode();
        number2 = get_block();
        //다이렉트 블럭에서
        list = inode -> di;
        for(i=0; i<18; i++){
            if(!strcmp(myfs.block[list-1].directory.name[i], "")){
                strcpy(myfs.block[list-1].directory.name[i], file);
                myfs.block[list-1].directory.number[i] = number+1;
                myfs.inode[number].type = 1;
                myfs.inode[number].size = 0;
                myfs.inode[number].time = *localtime(&now);
                myfs.inode[number].di = number2+1;
                tree_add(file, i, pwd, tree);
                return;
            }
        }
        //싱글 인다이렉트 블럭에서
        block = inode -> sin;
        if(block == 0){
            block = get_block()+1;
            inode->sin = block;
            for(i=0; i<64; i++){
                myfs.block[block-1].indirect.number[i] = 0;
            }
        }
        for(i=0; i<64; i++){
            list = myfs.block[block-1].indirect.number[i];
            if(list == 0){
                block = get_block()+1;
                if(i==0){
                    tlist[(inode->di)-1]->next = tlist[block-1];
                }
                else{
                    tlist[myfs.block[block-1].indirect.number[i-1]]->next = tlist[block-1];
                }
                for(l=0; l<18; l++){
                    myfs.block[block-1].directory.number[l] = 0;
                    strcpy(myfs.block[block-1].directory.name[l], "");
                }
            }
            for(j=0; j<18; j++){
                if(!strcmp(myfs.block[list-1].directory.name[j], "")){
                    strcpy(myfs.block[list-1].directory.name[j], file);
                    myfs.block[list-1].directory.number[j] = number+1;
                    myfs.block[list-1].directory.number[i] = number+1;
                    myfs.inode[number].type = 1;
                    myfs.inode[number].size = 0;
                    myfs.inode[number].time = *localtime(&now);
                    myfs.inode[number].di = number2+1;
                    tree_add(file, i, pwd, tree);
                    return;
                }
            }
        }
        //더블 인다이렉트 블럭에서
        if(inode->din == 0){
            inode->din = get_block()+1;
            for(i=0; i<64; i++){
                myfs.block[(inode->din)-1].indirect.number[i] = 0;
            }
        }
        for(i=0; i<64; i++){
            block = myfs.block[(inode->din)-1].indirect.number[i];
            if(block == 0){
                block = get_block();
                for(l=0; l<64; l++){
                    myfs.block[block-1].indirect.number[l] = 0;
                }
            }
            for(j=0; j<64; j++){
                list = myfs.block[block-1].indirect.number[j];
                if(list == 0){
                    list = get_block()+1;
                    if(j==0){
                        tlist[(inode->di)-1]->next = tlist[block-1];
                    }
                    else{
                        tlist[myfs.block[block-1].indirect.number[j-1]]->next = tlist[block-1];
                    }
                    for(l=0; l<18; l++){
                        myfs.block[block-1].directory.number[l] = 0;
                        strcpy(myfs.block[block-1].directory.name[l], "");
                    }
                }
                for(k=0; k<18; k++){
                    if(!strcmp(myfs.block[list-1].directory.name[k], "")){
                        strcpy(myfs.block[list-1].directory.name[k], file);
                        myfs.block[list-1].directory.number[k] = number+1;
                        myfs.inode[number].type = 1;
                        myfs.inode[number].size = 0;
                        myfs.inode[number].time = *localtime(&now);
                        myfs.inode[number].di = number2+1;
                        tree_add(file, i, pwd, tree);
                        return;
                    }
                }
            }
        }
    }
}

//myshowinode 함수
void myshowinode(int n){
    struct inode_list *inode = NULL;

    if(usage_check(myfs.super.i_state, 16, n)){
        inode = &myfs.inode[n-1];
        printf("file type : %s\n", (inode->type == 0) ? "directory" : "regular file");
        printf("file size : %d byte\n", inode->size);
        printf("modified time : %d/%d/%d %02d:%02d:%02d\n", inode->time.tm_year + 1900, inode->time.tm_mon + 1, inode->time.tm_mday, inode->time.tm_hour, inode->time.tm_min, inode->time.tm_sec);
        printf("data block list : %d", inode->di);
        print_block_list(n);
        printf("\n");
    }
    else{
        printf("error : empty inode\n");
    }
}

//mytree 함수
void mytree(struct mytree *check){
    int i;

    if(check == NULL){
        return;
    }
    else{
        if(check->step != 1){
            printf("--");
            for(i=0; i<(check->step)-2; i++){
                printf("---");
            }
            printf("* ");
        }
        printf("%-4s\n", check->name);
        mytree(check->down);
        mytree(check->next);
        return;
    }
}

//슈퍼블럭에 비트열로 추가
void usage_plus(unsigned int c[], int n, int x){
    int i, j, l = sizeof(int) * 8;
    unsigned int mask = 1 << (l - 1);

    for(i=0; i<n; i++){
        for(j=0; j<l; j++){
            if(x == (i * l + j + 1)){
                mask >>= j;
                c[i] = c[i] | mask;
            }
        }
    }
}

//슈퍼블럭에 비트열로 제거
void usage_minus(unsigned int c[], int n, int x){
    int i, j, k ,l = sizeof(int) * 8;
    unsigned int minus = 1;

    for(i=0; i<n; i++){
        for(j=0; j<32; j++){
            if(x == (i * 32 + j + 1)){
                for(k=0; k<31-j; k++){
                    minus = minus * 2;
                }
                c[i] = c[i] - minus;
            }
        }
    }
}

//슈퍼블럭에서 가용 아이노드/데이터 블럭인지 확인
int usage_check(unsigned int c[], int n, int x){
    int i, j, k, l = sizeof(int) * 8;
    unsigned int mask = 1 << (l - 1);

    for(i=0; i<n; i++){
        for(j=0; j<l; j++){
            if( x == (i * l + j + 1)){
                mask >>= j;
                return ((c[i] & mask) != 0) ? 1 : 0;
            }
        }
    }

    return 0;
}

//몇개의 아이노드/데이터 블럭이 사용중인지 확인
int usage_count(unsigned int c[], int n){
    int a, i, j, count = 0, l = sizeof(int) * 8;
    unsigned int mask = 1 << (l - 1);

    for(i=0; i<n; i++){
        a = c[i];
        for(j=1; j<=l; j++){
            if((a & mask) != 0){
                count++;
            }
            a <<= 1;
        }
    }
    return count;
}

//현재 디렉터리의 아이노드 번호를 리턴
int where_am_i(struct present_working_directory pwd_call){
    int i;
    for(i=0; i<10; i++){
        if(!strcmp(pwd_call.name[i], "")){
            i--;
            return pwd_call.number[i];
        }
    }
}

//빈 아이노드 찾기
int get_inode(){
    int i;

    for(i=0; i<512; i++){
        if(!usage_check(myfs.super.i_state, 16, i+1)){
            usage_plus(myfs.super.i_state, 16, i+1);
            return i;
        }
    }
}

//빈 데이터 블럭 찾기
int get_block(){
    int i;

    for(i=0; i<1024; i++){
        if(!usage_check(myfs.super.d_state, 32, i+1)){
            usage_plus(myfs.super.d_state, 32, i+1);
            return i;
        }
    }
}

//경로를 알아내는 함수
struct present_working_directory where_is_there(char location[]){
    int i, j, loc, temp;
    char step[5][10] = {0};
    struct present_working_directory pwd_temp = pwd, pwd_result = pwd;

    //절대경로로 시작하면
    if(location[0] == '/'){
        for(i=0; i<10; i++){
            strcpy(pwd_result.name[i], "");
            pwd_result.number[i] = 0;
        }
        strcpy(pwd_result.name[0], "/");
        pwd_result.number[0] = 1;
        sscanf(location, "/%s", location);
    }

    //아무것도 없으면
    if(!strcmp(location, "")){
        for(i=0; i<10; i++){
            strcpy(pwd_result.name[i], "");
            pwd_result.number[i] = 0;
        }
        strcpy(pwd_result.name[0], "/");
        pwd_result.number[0] = 1;
        sscanf(location, "/%s", location);
        return pwd_result;
    }

    //경로 단위로 분할
    sscanf(location, "%[^/]/%[^/]/%[^/]/%[^/]/%[^/]", step[0], step[1], step[2], step[3], step[4]);
    for(i=0; i<5; i++){
        sscanf(step[i], "%4s", step[i]);
    }

    for(i=0; i<5; i++){
        loc = where_am_i(pwd_result);
        //끝이면
        if(!strcmp(step[i], "")){
            return pwd_result;
        }
        //.이면
        else if(!strcmp(step[i], ".")){
            continue;
        }
        //..이면
        else if(!strcmp(step[i], "..")){
            for(j=0; j<10; j++){
                if(!strcmp(pwd_result.name[j], "")){
                    j--;
                    break;
                }
            }
            strcpy(pwd_result.name[j], "");
            pwd_result.number[j] = 0;
        }
        //디렉터리 이름이면
        else{
            temp = find_file(&myfs.inode[loc-1], step[i]);
            //파일이 없으면
            if(temp==0){
                printf("error : %s is not exist\n", step[i]);
                pwd_result = pwd_temp;
                return pwd_result;
            }
            if(myfs.inode[loc-1].type==0){
                for(j=0; j<10; j++){
                    if(!strcmp(pwd_result.name[j], "")){
                        break;
                    }
                }
                strcpy(pwd_result.name[j], step[i]);
                pwd_result.number[j] = temp;
            }
            //디렉터리가 아니면
            else{
                printf("error : %s is not a directory\n", step[i]);
                pwd_result = pwd_temp;
                return pwd_result;
            }
        }
    }
}

//파일의 아이노드 찾기
int find_file(struct inode_list *inode, char file[]){
    int i, j, k, n, check, block, list;

    list = inode -> di;
    //다이렉트 블럭에서
    for(i=0; i<18; i++){
        if(!strcmp(myfs.block[list-1].directory.name[i], "")){
            return 0;
        }
        if(!strcmp(myfs.block[list-1].directory.name[i], file)){
            n = myfs.block[list-1].directory.number[i];
            return n;
        }
    }
    //싱글 인다이렉트 블럭에서
    block = inode -> sin;
    if(block == 0){
        return 0;
    }
    for(i=0; i<64; i++){
        list = myfs.block[block-1].indirect.number[i];
        for(j=0; j<18; j++){
            if(!strcmp(myfs.block[list-1].directory.name[j], "")){
                return 0;
            }
            if(!strcmp(myfs.block[list-1].directory.name[j], file)){
                n = myfs.block[list-1].directory.number[j];
                return n;
            }
        }
    }
    //더블 인다이렉트 블럭에서
    for(i=0; i<64; i++){
        block = myfs.block[(inode -> din)-1].indirect.number[i];
        if(block == 0){
            return 0;
        }
        for(j=0; j<64; j++){
            list = myfs.block[block-1].indirect.number[j];
            for(k=0; k<18; k++){
                if(!strcmp(myfs.block[list-1].directory.name[k], "")){
                    return 0;
                }
                if(!strcmp(myfs.block[list-1].directory.name[k], file)){
                    n = myfs.block[list-1].directory.number[k];
                    return n;
                }
            }
        }
    }
}


//파일이나 디렉터리를 찾아서 지워주는 함수
int del_file(struct inode_list *inode, char file[]){
    int i, j, k, n, check, block, list;

    list = inode -> di;
    //다이렉트 블럭에서
    for(i=0; i<18; i++){
        if(!strcmp(myfs.block[list-1].directory.name[i], "")){
            return 0;
        }
        if(!strcmp(myfs.block[list-1].directory.name[i], file)){
            n = myfs.block[list-1].directory.number[i];
            myfs.block[list-1].directory.number[i] = 0;
            strcpy(myfs.block[list-1].directory.name[i], "");
            return n;
        }
    }
    //싱글 인다이렉트 블럭에서
    block = inode -> sin;
    if(block == 0){
        return 0;
    }
    for(i=0; i<64; i++){
        list = myfs.block[block-1].indirect.number[i];
        for(j=0; j<18; j++){
            if(!strcmp(myfs.block[list-1].directory.name[j], "")){
                return 0;
            }
            if(!strcmp(myfs.block[list-1].directory.name[j], file)){
                n = myfs.block[list-1].directory.number[j];
                myfs.block[list-1].directory.number[j] = 0;
                strcpy(myfs.block[list-1].directory.name[j], "");
                return n;
            }
        }
    }
    //더블 인다이렉트 블럭에서
    for(i=0; i<64; i++){
        block = myfs.block[(inode -> din)-1].indirect.number[i];
        if(block == 0){
            return 0;
        }
        for(j=0; j<64; j++){
            list = myfs.block[block-1].indirect.number[j];
            for(k=0; k<18; k++){
                if(!strcmp(myfs.block[list-1].directory.name[k], "")){
                    return 0;
                }
                if(!strcmp(myfs.block[list-1].directory.name[k], file)){
                    n = myfs.block[list-1].directory.number[k];
                    myfs.block[list-1].directory.number[k] = 0;
                    strcpy(myfs.block[list-1].directory.name[k], "");
                    return n;
                }
            }
        }
    }
}
//블럭 리스트 출력
void print_block_list(int n){
    int i = 0, j, block;

    block = myfs.inode[n-1].sin;
    if(block==0){
        return;
    }
    else{
        for(i=0; i<64; i++){
            if(myfs.block[block-1].indirect.number[i]!=0){
                printf(", %d", myfs.block[block-1].indirect.number[i]);
            }
            else{
                return;
            }
        }
    }

    //더블 인다이렉트 블럭에서
    for(j=0; j<64; j++){
        block = myfs.block[myfs.inode[n-1].din-1].indirect.number[j];
        if(block==0){
            return;
        }
        else{
            for(i=0; i<64; i++){
                if(myfs.block[block-1].indirect.number[i]){
                    printf(", %d", myfs.block[block-1].indirect.number[i]);
                }
                else{
                    return;
                }
            }
        }
    }
}

//mytree에 파일 더하기
void tree_add(char name[], int step, struct present_working_directory phase, struct mytree *check){
    int i;

    //추가할 위치 찾기
    if(step == (check->step)+1){
        if((check->name[0] == phase.name[step-2][0]) && (check->name[1] == phase.name[step-2][1]) && (check->name[2] == phase.name[step-2][2]) && (check->name[3] == phase.name[step-2][3])){
            if(check->down == NULL){
                check->down = malloc(sizeof(struct mytree));
                for(i=0; i<4; i++){
                    check->down->name[i] = name[i];
                }
                check->down->step = step;
                check->down->next = NULL;
                check->down->down = NULL;
            }
            else{
                tree_add(name, step, phase, check->down);
            }
        }
        else{
            tree_add(name, step, phase, check->next);
        }
        return;
    }
    else if(step == (check->step)){
        if(check->next == NULL){
            check->next = malloc(sizeof(struct mytree));
            for(i=0; i<4; i++){
                check->next->name[i] = name[i];
            }
            check->next->step = step;
            check->next->next = NULL;
            check->next->down = NULL;
        }
        else{
            tree_add(name, step, phase, check->next);
        }
        return;
    }
    else{
        for(int i=2; i<10; i++){
            if(step == (check->step)+i){
                if((check->name[0] == phase.name[step-i-1][0]) && (check->name[1] == phase.name[step-i-1][1]) && (check->name[2] == phase.name[step-i-1][2]) && (check->name[3] == phase.name[step-i-1][3])){
                    tree_add(name, step, phase, check->down);
                    return;
                }
                else{
                    tree_add(name, step, phase, check->next);
                    return;
                }
            }
        }
    }
}

//mytree에 파일 지우기
void tree_del(char name[], int step, struct present_working_directory phase, struct mytree *check){
    int i;
    struct mytree temp;

    //제거할 위치 찾기
    if(step == (check->step)+1){
        if((check->name[0] == phase.name[step-2][0]) && (check->name[1] == phase.name[step-2][1]) && (check->name[2] == phase.name[step-2][2]) && (check->name[3] == phase.name[step-2][3])){
            if((check->down->name[0] == name[0]) && (check->down->name[1] == name[1]) && (check->down->name[2] == name[2]) && (check->down->name[3] == name[3])){
                temp = *(check->down);
                free(check->down);
                check->down = temp.next;
            }
            else{
                tree_del(name, step, phase, check->down);
            }
        }
        else{
            tree_del(name, step, phase, check->next);
        }
        return;
    }
    else if(step == (check->step)){
        if((check->next->name[0] == name[0]) && (check->next->name[1] == name[1]) && (check->next->name[2] == name[2]) && (check->next->name[3] == name[3])){
            temp = *(check->next);
            free(check->next);
            check->next = temp.next;
        }
        else{
            tree_del(name, step, phase, check->next);
        }
        return;
    }
    else{
        for(int i=2; i<10; i++){
            if(step == (check->step)+i){
                if((check->name[0] == phase.name[step-i-1][0]) && (check->name[1] == phase.name[step-i-1][1]) && (check->name[2] == phase.name[step-i-1][2]) && (check->name[3] == phase.name[step-i-1][3])){
                    tree_del(name, step, phase, check->down);
                    return;
                }
                else{
                    tree_del(name, step, phase, check->next);
                    return;
                }
            }
        }
    }
}
