#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>
#include <math.h>
#include <regex.h>
#include <curl/curl.h>

static char *host = "localhost";
static char *user = "zeriix";
static char *pass = "root";
static char *dbname = "William";

unsigned int port = 0;
static char *unix_socket = NULL;
unsigned int flag = 0;

void viderBuffer() {
    int c = 0;
    while(c != '\n' && c != EOF)
    {
        c = getchar();
    }
}
void print_char(char *res){

    printf("%s\n", res);

}
void espace_char(char * espace){
    strcat(espace, " ");
}
char *recup_src(int pos){

    char *dest;
    dest = malloc(sizeof(char) * 255);

    FILE * file = fopen("/Users/williamflorentin/Documents/CODE/language_C/cours_C_2021/projet_v3/config/src.txt", "r");
    fseek(file, pos, SEEK_SET);
    fscanf(file, "%s", dest);
    fclose(file);

    return dest;
}
char **recup_info_conn(char *name){

    char **dest;
    dest = malloc(2 * sizeof(char));
    char *tmp;
    char usr[50];
    char passwd[50];
    tmp = malloc(sizeof(char) * 255);
    unsigned long int i = strlen(name) + 1;
    char *src = recup_src(2);
    strcat(src, name);
    strcat(src, ".txt");

    FILE * file = fopen(src, "r");
    if (file == NULL){
        printf("ERROR\n");
        exit(1);
    }
    fseek(file, 0, SEEK_SET);
    fscanf(file, "%s", tmp);

    for (i; i < strlen(tmp); i++){
        if(tmp[i] != '/'){
            usr[i] = tmp[i];
        } else {
            break;
        }
    }
    i++;
    for (i ; i < strlen(tmp); i++){
        passwd[i] = tmp[i];
    }

    dest[0] = malloc(strlen(usr) * sizeof(char));
    strcat(dest[0], usr);
    dest[1] = malloc(strlen(passwd)* sizeof(char));
    strcat(dest[1], passwd);

    free(tmp); free(src);

    return dest;
}
void save_info_conn(char *usr, char *passwd, char *db){

    char *tmp;
    char *src;
    tmp = recup_src(2);
    src = malloc((strlen(tmp) + strlen(db)) * sizeof(char));
    strcat(src, tmp);
    strcat(src, db);
    strcat(src, ".txt");

    FILE *file = fopen(src, "r");
    if (file == NULL){
        //printf("error");
        fclose(file);
        file = fopen(src, "w+");
        fprintf(file, "%s/%s/%s\n", db, usr, passwd);
        fclose(file);
    } else {
        printf("ERROR save !");
        exit(1);
    }

}
void write_log(char * log) {

    char date[20];
    int day, mois, an;
    char days[3], moiss[3], ans[5], et[300];
    time_t now;
    char *src;
    src = recup_src(87);

    struct tm *local = localtime(&now);
    day = local->tm_mday + 1;
    mois = local->tm_mon + 1;
    an = local->tm_year + 1952;

    sprintf(days, "%d", day);
    sprintf(moiss, "%d", mois);
    sprintf(ans, "%d", an);

    strcat(date, days);
    strcat(date, "/");
    strcat(date, moiss);
    strcat(date, "/");
    strcat(date, ans);

    strcat(et, src);
    strcat(et, "log");
    strcat(et, days);
    strcat(et, "-");
    strcat(et, moiss);
    strcat(et, "-");
    strcat(et, ans);
    strcat(et, ".txt");

    printf("%s\n", et);

    FILE *file = fopen(et, "r");
    if (file == NULL){
        //printf("error");
        fclose(file);
        file = fopen(et, "w+");
        fprintf(file, "[%s] : %s", date+1, log);
        fclose(file);
    } else {
        //printf("gg");
        fclose(file);
        file = fopen(et, "a");
        fseek(file, 0, SEEK_END);
        fprintf(file, "[%s] : %s\n", date+1, log);
        fclose(file);
    }


}
char *substr(char *chaineSource,int pos,int len) {

    char * dest=NULL;
    if (len>0) {
        dest = (char *)calloc(len+1, 1);
        if(NULL != dest) {
            strncat(dest,chaineSource+pos,len);
        }
    }
    return dest;
}
char *search_str(char *src) {

    FILE *fp = fopen(recup_src(344), "r");
    char *buffer;
    char *res = malloc(100 * sizeof(char));
    char *tmp;
    char *cp = malloc(100 * sizeof(char));

    if (fp == NULL){
        printf("ERROR\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    int sizefp = ftell(fp);
    //printf("init : %d\n", sizefp);
    fseek(fp, 0, SEEK_SET);
    //printf("reset : %lu\n", ftell(fp));

    buffer = malloc(sizefp * sizeof(char));

    fread(buffer, sizeof(char), sizefp, fp);
    tmp = strstr(buffer, src);
    int i = 0;
    for (i; i < strlen(tmp); i++) {
        if (tmp[i] == '>'){
            break;
        }
    }
    cp = substr(tmp, i+1, 100);

    for (int j = 0; j < strlen(cp); j++){
        if (cp[j] == '<'){
            break;
        }
        res[j] = cp[j];
    }

    free(cp);
    free(buffer);
    fclose(fp);

    return res;
}
size_t got_data(char *buffer, size_t itemsize, size_t nitems, void *ignorethis){

    FILE * file = fopen(recup_src(344), "a");
    fseek(file, 0, SEEK_SET);
    size_t bytes = itemsize * nitems;
    int linenumber = 1;
    fprintf(file, "New chunck(%zu bytes)\n", bytes);
    fprintf(file, "%d:\t", linenumber);

    /*for (int i = 0; i < bytes; i++){
        fprintf(file, "%c", buffer[i]);
        if (buffer[i] == '\n'){
            linenumber++;
            fprintf(file, "%d:\t", linenumber);
        }
    }
    fprintf(file, "\n\n");*/


    fprintf(file, "%s", buffer);

    fclose(file);

    return bytes;
}
void use_curl(char *src){

    CURL *curl = curl_easy_init();

    if (!curl){
        fprintf(stderr, "init failed\n");
        exit(1);
    }

    curl_easy_setopt(curl, CURLOPT_URL, src); // "https://fr.wikipedia.org/wiki/Erreur_de_segmentation"
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK){
        fprintf(stderr, "download problem : %s\n", curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);

}
void print_2D_tab(char **tab){

    for (int i = 0; i < 20; i++){
        if (strcmp(tab[i], "NULL") == 0){
            printf("break: %d\n", i);
            break;
        }
        printf("%s\n", tab[i]);
    }

}
void vider_2D_tab(char **tab){

    for(int i = 0; i < 20; i++) {
        if (strcmp(tab[i], "NULL") == 0){
            printf("break: %d\n", i);
            free(tab[i]);
            break;
        }
        free(tab[i]);
    }
    free(tab);

}
char *dat(){

    char *date;
    date = malloc(16 * sizeof(char));
    //char date[16];
    int day, mois, an;
    char days[3], moiss[3], ans[5];
    time_t now;

    struct tm *local = localtime(&now);
    day = local->tm_mday + 1;
    mois = local->tm_mon + 1;
    an = local->tm_year + 1952;

    sprintf(days, "%d", day);
    sprintf(moiss, "%d", mois);
    sprintf(ans, "%d", an);

    strcat(date, days);
    strcat(date, "/");
    strcat(date, moiss);
    strcat(date, "/");
    strcat(date, ans);

    return date;
}
void start(){

    printf("____________________________________________________\n");
    printf("||  |¯¯\\  |¯¯\\  |¯¯¯|  ¯¯|¯¯ |¯¯¯ ¯¯|¯¯    /¯¯\\   ||\n");
    printf("||  |__/  |__/  |   |    |   |--    |     |       ||\n");
    printf("||  |     |  \\  |___| |__|   |___   |      \\__/   ||\n");
    printf("¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
    printf("-Created by William FLORENTIN 2i2 (ESGI)\n");
}
void exec(char *req, MYSQL * mysql){
    if(!(mysql_query(mysql, req))){
        fprintf(stderr, "\n ERROR: %s [%d]\n", mysql_error(mysql), mysql_errno(mysql));
        exit(1);
    }
}
void connecte(MYSQL * mysql, char *u, char *ps, char *db){
    mysql = mysql_init(NULL);
    if (!(mysql_real_connect(mysql, host, u, ps, db, port, unix_socket, flag))){
        fprintf(stderr, "\n ERROR: %s [%d]\n", mysql_error(mysql), mysql_errno(mysql));
        exit(1);
    }
    printf("Connection successful !\n");
}

void print_name_of_colums(MYSQL *mysql, char *name){

    char req[255] = "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME =\'";
    strcat(req, name);
    strcat(req,"\'");
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    printf("colums:\n");
    printf("_____________________\n");

    while (row = mysql_fetch_row(res)) {
        for(int i = 3; i < sizeof(row)/2; i=i+4){
            printf("-%s\n", row[i]);
        }
    }
    printf("_____________________\n");

    mysql_free_result(res);
}

char **recup_name_of_colums(MYSQL *mysql, char *name){

    char **tab;
    char req[255] = "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME =\'";
    strcat(req, name);
    strcat(req,"\'");
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    int c = 0;
    tab = malloc(sizeof(char) * 100); // donnée dans le fichier config à faire
    while (row = mysql_fetch_row(res)) {
        for(int i = 3; i < (sizeof(row)/2); i=i+4){
            tab[c] = malloc(sizeof(char) * strlen(row[i]));
            strcat(tab[c], row[i]);
            c++;
        }
    }
    mysql_free_result(res);

    return tab;
}

void print_contenu_of_table(MYSQL *mysql, char *name){

    char req[255] = "SELECT * FROM ";
    strcat(req, name);
    write_log(req);
    char **type;

    MYSQL_RES *res;
    MYSQL_ROW row;

    type = recup_name_of_colums(mysql, name);
    int c = 0;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    printf("contenu table:\n");
    printf("_____________________\n");

    while (row = mysql_fetch_row(res)) {
        for(int i = 0; i < sizeof(row)/2; i++){
            printf("%s", type[c]);
            printf("-%s\n", row[i]);
            c++;
        }
        c = 0;
        printf("_____________________\n");
    }
    mysql_free_result(res);
}

char **recup_contenu_of_table(MYSQL *mysql, char *name){

    char **tab;
    char req[255] = "SELECT * FROM ";
    strcat(req, name);
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    int c = 0;
    tab = malloc(sizeof(char) * 100); // donnée dans le fichier config à faire
    while (row = mysql_fetch_row(res)) {
        for(int i = 0; i < (sizeof(row)/2); i++){
            tab[c] = malloc(sizeof(char) * strlen(row[i]));
            strcat(tab[c], row[i]);
            c++;
        }
        tab[c] = malloc(sizeof(char));
        strcat(tab[c], "S");
        c++;
    }
    mysql_free_result(res);

    return tab;
}

void print_name_all_table(MYSQL *mysql, char *name){

    char req[255] = "SHOW TABLES FROM ";
    strcat(req, name);
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    printf("table:\n");
    printf("_____________________\n");
    while (row = mysql_fetch_row(res)) {
        for(int c = 0; c < sizeof(row); c=c+6){
            printf("-%s\n", row[c]);
            break;
        }
    }
    printf("_____________________\n");

    mysql_free_result(res);
}

char **recup_name_all_table(MYSQL *mysql, char *name){

    char **tab;
    char req[255] = "SHOW TABLES FROM ";
    strcat(req, name);
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    int c = 0;
    tab = malloc(sizeof(char) * 100); // donnée dans le fichier config à faire
    while (row = mysql_fetch_row(res)) {
        for(int i = 0; i < (sizeof(row)); i=i+6){
            tab[c] = malloc(sizeof(char) * strlen(row[i]));
            strcat(tab[c], row[i]);
            break;
        }
        c++;
    }
    mysql_free_result(res);

    return tab;
}

void print_name_of_database(MYSQL *mysql){

    char req[255] = "SHOW DATABASES";
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    printf("database:\n");
    int c = 0;
    printf("_____________________\n");
    while (row = mysql_fetch_row(res)) {
        for(int i = 0; i < (sizeof(row)); i++){
            printf("-%s\n", row[i]);
            break;
        }
        c++;
    }
    printf("_____________________\n");
    mysql_free_result(res);

}

char **recup_name_of_database(MYSQL *mysql){

    char **tab;
    char req[255] = "SHOW DATABASES";
    write_log(req);
    MYSQL_RES *res;
    MYSQL_ROW row;

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    int c = 0;
    tab = malloc(sizeof(char) * 100); // donnée dans le fichier config à faire
    while (row = mysql_fetch_row(res)) {
        for(int i = 0; i < (sizeof(row)); i++){
            tab[c] = malloc(sizeof(char) * strlen(row[i]));
            strcat(tab[c], row[i]);
            break;
        }
        c++;
    }
    mysql_free_result(res);

    return tab;
}

void print_in_mouv(char *txt, int type, int rm){

    char *src;
    src = recup_src(0);
    char bdd[3][1] = {
            {"*"}, // name table
            {"-"}, // name sous partie
            {"!"} // name priamry key
    };

    FILE *file = fopen(src, "r");
    if (file == NULL){
        //printf("error");
        fclose(file);
        file = fopen(src, "w+");
        fprintf(file, "%s%s\n", bdd[type], txt);
        fclose(file);
    } else {
        //printf("gg");
        fclose(file);
        file = fopen(src, "a");
        fseek(file, 0, SEEK_END);
        fprintf(file, "%s%s\n", bdd[type], txt);
        fclose(file);
    }


}

char **recup_in_mouv() {

    char *src;
    src = recup_src(0);
    FILE *file = fopen(src, "r");
    char ligne[50];

    if (file == NULL) {
        printf("error");
        fclose(file);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char **tab;
    tab = malloc(sizeof(char) * size);

    for (int i = 0; i < size + 1; i++) {
        fscanf(file, "%s\n", ligne);
        tab[i] = malloc(sizeof(char) * strlen(ligne));
        strcat(tab[i], ligne);
    }
    tab[size+2] = malloc(sizeof(char) * strlen(ligne));
    strcat(tab[size+2], "NULL");

    return tab;
}

void req_insert(char *name, char **element) {

    char req[255] = "INSERT INTO ";
    strcat(req, name);
    strcat(req, " VALUES ('");
    int size = 0;

    //size = size_2D_tab(element);

    for(int i =0; i < size; i++){

    }

}

int recup_num_attribut_of_table(char *db, char *tab){

    unsigned int end = 0;
    char *src;
    char c;
    int found = 0;
    int pos = 0;

    src = recup_src(250);
    strcat(src, db);
    strcat(src, ".txt");

    FILE *file = fopen(src, "r");
    if (file == NULL){
        exit(1);
    }

    unsigned long int length = strlen(tab);
    while( fread(&c,1,1,file)>0 && !found ) {

        if( c==tab[pos] ) {
            pos++;
        } else {
            if(pos!=0) {
                // On doit rembobiner !
                fseek(file,-pos,SEEK_CUR);
                pos=0;
            }
        }

        found = (pos==length);

    }
    char *rep = malloc(100 * sizeof(char));
    fseek(file, 0, SEEK_CUR);
    fscanf(file, "%s", rep);
    fclose(file);

    for (int i = 0; i < strlen(rep); i++){
        end += (rep[i] - '0')* pow(10, (strlen(rep) - i));
        if (i == strlen(rep) - 1){
            end = end/10;
        }
    }

    return end;
}

char **recup_type_colums(char *name, MYSQL *mysql, char *db){

    MYSQL_RES *res;
    MYSQL_ROW row;

    char req[255] = "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = N'";
    char **tab;
    tab = malloc(recup_num_attribut_of_table(db, name)* sizeof(char));
    strcat(req, name);
    strcat(req, "\'");

    mysql_query(mysql, req);
    res = mysql_store_result(mysql);

    int c = 0;
    while (row = mysql_fetch_row(res)) {
        for(int i = 7; i < sizeof(row); i++){
            //printf("%s\n", row[i]);
            tab[c] = malloc(strlen(row[i]) * sizeof(char));
            strcat(tab[c], row[i]);
            break;
        }
        c++;
    }

    mysql_free_result(res);

    return tab;
}




int main(int argc, char *argv[]){

    MYSQL *mysql;
    char u[50], ps[50];
    int select = 0;
    char **tab;
    char choice;
    char **con;
    char *log;

    mysql = mysql_init(NULL);
    if (!(mysql_real_connect(mysql, host, user, pass, dbname, port, unix_socket, flag))){
        fprintf(stderr, "\n ERROR: %s [%d]\n", mysql_error(mysql), mysql_errno(mysql));
        exit(1);
    }
    printf("start\n\n");

    if (argv[1] != NULL) {

        char **cmp;
        int ct = 0;

        cmp = malloc(5 * sizeof(char));
        cmp[0] = "test";
        cmp[1] = "bonjour";
        cmp[2] = "samedi";
        cmp[3] = "jaune";
        cmp[4] = "curl";

        printf("commande : ");
        while(argv[1][ct] != '\0'){
            printf("%c", argv[1][ct]);
            ct++;
        }
        printf("\n");

        if (strcmp(argv[1], cmp[0]) == 0){
            print_char("lundi");
        }
        if (strcmp(argv[1], cmp[1]) == 0){
            print_char("mardi");
        }
        if (strcmp(argv[1], cmp[2]) == 0){
            print_char("mercredi");
        }
        if (strcmp(argv[1], cmp[3]) == 0){
            print_char("jeudi");
        }
        if (strcmp(argv[1], cmp[4]) == 0){
            if (argv[2] == NULL){
                printf("error lien\n");
                exit(1);
            }
            use_curl(argv[2]);
            char *num = search_str("<title>");
            printf("%s\n", num);
            free(num);
        }
    } else {

        start();

        printf("Select your database :\n");
        print_name_of_database(mysql);
        tab = recup_name_of_database(mysql); // tab - name of all db
        scanf("%d", &select);
        viderBuffer();

        con = recup_info_conn(tab[select]);
        if (con[0] == NULL) {
            printf("Enter user : ");
            scanf("%s", u);
            viderBuffer();
            printf("Enter password : ");
            scanf("%s", ps);
            viderBuffer();
            printf("do you want to save connection information ? (y/n)\n");
            scanf("%c", &choice);
            viderBuffer();
            if (choice == 'y') {
                save_info_conn(u, ps, tab[select]);

                log = malloc(200 * sizeof(char));
                strcat(log, "info connexion save for ");
                strcat(log, u);
                strcat(log, "in ");
                strcat(log, tab[select]);
                write_log(log);
                free(log);
            }
            connecte(mysql, u, ps, tab[select]);
        } else {
            connecte(mysql, con[0], con[2], tab[select]);
        }

        log = malloc(150 * sizeof(char));
        strcat(log, "connect db ");
        strcat(log, tab[select]);
        write_log(log);
        free(log);

    }
    mysql_close(mysql);
    return EXIT_SUCCESS;
}


/*
char strs[3][20] =
                {
                        {"January"},
                        {"February"},
                        {""}
                };

        unsigned long int arraysize = sizeof(strs)/sizeof(strs[0]);
        printf("%d\n", arraysize);
        arraysize = sizeof(strs)/sizeof(char);
        printf("%d\n", arraysize);

        tab = recup_type_colums("bureau", mysql, dbname);

        for (int i = 0; i <= (recup_num_attribut_of_table(dbname, "bureau") - 1); i++){
            printf("%s\n", tab[i]);
            free(tab[i]);
        }
        free(tab);
 */