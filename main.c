/***************************************************************************//**
  @file         main.c
  @author       Jerome Enrico M. Requioma
  @date         Monday, 26 November 2018
  @brief        RSH (Requioma SHell)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <time.h>
#include <string.h>
#include <direct.h>
#include <Windows.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>

#define SIZE 15

char *token[5];
int tokenCount = 0;

/**
   @brief prints the working directory.
   @param buffer the space for input
   @return none.
 */
void printcwd(char *buffer){
    if( (buffer = _getcwd( NULL, 0 )) == NULL ) // current working directory
        perror( "_getcwd error" );
    else{
        printf( "%s>", buffer);
        free(buffer);
    }
}
/**
   @brief counts the given string's length
   @param *string The string to be counted
   @return number of chars in the given string.
 */
int getStringCount(char *string) {
    int i = 0;
    while(1) {
        if(string[i] != '\0') {
            i++;
        } else {
            break;
        }
    }
    return i;
}

/**
   @brief prints a not found path error.
   @param none
   @return none.
 */
void pathNotFound() {
    printf("The system cannot find the path specified.\n");
}

/**
   @brief prints an error when a command given is not recognized.
   @param none
   @return none.
 */
void printNotValid(){
    printf("'%s' is not recognized as an external or internal command,\noperable program or batch file.\n", token[0]);
}

/**
   @brief changes directory
   @param none
   @return none.
 */
void execCD(){
    char *buffer = "";
    char directory[1024];

    //  checks if there is no token after the cd command
    if(token[1] == NULL){
        if(strlen(token[0]) == 4){
            if(token[0][2] == '.' && token[0][3] == '.'){       // checks for ".." beside cd
                chdir("..");
            }
        }
    }

        //  if it's not NULL
    else if(token[1] != NULL){
        strcpy(directory, "./");
        strcpy(directory, token[1]);
        if(token[1][0] == '.' && token[1][1] == '.'){       // checks for ".." beside second token
            chdir(directory);
        }
        else if(chdir(directory) == -1) {       // no such directory exists!
            pathNotFound();
        }
    }
    printcwd(buffer);

    return ;
}

/**
   @brief prints the list of available and acceptable commands.
   @param none
   @return none.
 */
void execHelp(){
    printf("For more information on a specific command, type HELP command-name\n");
    printf("CD\t\tDisplays the name of or changes the current directory.\n");
    printf("CHDIR\t\tChanges the current directory.\n");
    printf("CLS\t\tClears the screen.\n");
    printf("CMD\t\tStarts a new instance of the command interpreter.\n");
    printf("COLOR\t\tSets the default console foreground and background colors.\n");
    printf("COPY\t\tCopies one or more files to another location.\n");
    printf("DATE\t\tDisplays or sets the date.\n");
    printf("DEL\t\tDeletes one or more files.\n");
    printf("DIR\t\tDisplays a list of files and subdirectories in a directory.\n");
    printf("HELP\t\tProvides Help information for Windows commands.\n");
    printf("IPCONFIG\tDisplays all current TCP/IP network configuration values and refreshes Dynamic Host Configuration Protocol (DHCP) and Domain Name System (DNS) settings.\n");
    printf("MD\t\tCreates a directory.\n");
    printf("MKDIR\t\tCreates a directory.\n");
    printf("MOVE\t\tMoves one or more files from one directory to another directory.\n");
    printf("RENAME\t\tRenames a file or files.\n");
    printf("RD\t\tRemoves a directory.\n");
    printf("RMDIR\t\tRemoves a directory.\n");
    printf("TIME\t\tDisplays or sets the system time.\n");
    printf("TYPE\t\tDisplays the contents of a text file.\n");

}

/**
   @brief types contents into a file
   @param none
   @return none.
 */
void execType(){
    FILE *file;//a pointer to a file
    int filechar;//variable holder for a character

    file = fopen(token[1], "r");//opens the file for reading

    //error handling
    if (file == NULL){
        printNotValid();
        return ;
    }
    else{
        //prints out character by character if file is not null
        while ((filechar = fgetc(file)) != EOF){
            printf("%c", filechar);
        }
        fclose(file);//closes file
    }
    printf("\n");
    return ;
}

/**
   @brief displays the current system time
   @param none
   @return none.
 */

int execTime(){
    char newTime[SIZE];//variable holder for the new time
    char currentTime[100];//variable holder for the current time
    time_t now = time (0);//system time
    char *timeArray[SIZE];//variable holder for getting time elements
    int i = 0, hour, min, sec, msec;

    strftime (currentTime, 100, "%H:%M:%S.000", localtime (&now)); //gets current time
    printf ("The current time is: %s\n", currentTime);

    printf("Enter the new time: (hh:min:sec.msec) ");//gets new time input from user
    gets(newTime);

    //tokenizes newTime per element
    timeArray[i] = strtok(newTime,".:");
    while(timeArray[i]!=NULL){
        timeArray[++i] = strtok(NULL,".:");
    }
    if(i < 3){
        printf("A required privilege is not held by the client.\n");
        return 0;
    }
    //converts string element of newTime to integer
    hour = atoi(timeArray[0]);
    min = atoi(timeArray[1]);
    sec = atoi(timeArray[2]);
    msec = atoi(timeArray[3]);

    SYSTEMTIME str_t;
    GetSystemTime(&str_t);

    struct tm str_time;

    //sets time elements
    str_time.tm_hour = hour;
    str_time.tm_min = min;
    str_time.tm_sec = sec;
    str_time.tm_isdst = msec;

    mktime(&str_time);//sets the new time
    //printf("New time is: %d:%d:%d.%d\n", str_time.tm_hour, str_time.tm_min, str_time.tm_sec, str_time.tm_isdst);
    printf("A required privilege is not held by the client.\n");
    return 1;
}

/**
   @brief deletes a specified directory
   @param none
   @return none.
 */
int execRmdir(){
    int i = 1;

    if(token[1] == NULL){
        printNotValid();
        return 0;
    }

    //removes directory if found
    while(token[i] != NULL) {
        if(rmdir(token[i]) == -1) {
            pathNotFound();//else prints an error statement
        }
        i++;
    }
    return 1;
}

/**
   @brief renames a file/directory
   @param none
   @return none.
 */
int execRename(){
    int result;


    if(token[1] == NULL || token[2] == NULL){
        printNotValid();
        return 0;
    }
    result = rename(token[1], token[2]);//renames token[1] file name to token[2] file name

    //error handling
    if(result != 0) {
        pathNotFound();
    }
    return 1;
}

/**
   @brief creates a directory
   @param none
   @return none.
 */
void execMkdir(){
    int i = 1;

    while(token[i] != NULL) {
        if(mkdir(token[i]) == -1) {
            printf("A subdirectory or file %s already exists.\n", token[i]);
        }
        i++;
    }
    return ;
}

/**
   @brief prints the contents of the current directory
   @param *dp, *opendis
   @return none.
 */
void execDir(DIR *dp, char *opendis){
    struct dirent *dir;
    dp = opendir(opendis);
    struct stat attr;
    char time[20];
    char size[10];
    char *newsize;
    int length;
    int commas;
    struct tm * timeinfo;
    int files = 0;
    int dirs = 0;
    char *buffer = "";

    printf("Volume in drive C is OS\n");
    printf("Volume ");
    system("wmic path win32_physicalmedia get SerialNumber");
    printf("Directory of ");
    printcwd(buffer);

    if (dp) {
        printf("\n\n");
        while ((dir = readdir(dp)) != NULL) {
            stat(dir->d_name, &attr);
            timeinfo = localtime (&attr.st_mtime);
            strftime(time, sizeof(time), "%m/%d/%Y %I:%M %p", timeinfo);
            printf("%20s", time);
            if (S_ISDIR(attr.st_mode)) {
                printf("%7s", "<DIR>");
                printf("%15s", " ");
                dirs++;
            }
            else {
                printf("%10s", " ");
                itoa((unsigned)attr.st_size, size, 10);
                length = getStringCount(size);
                if(length % 3 == 0) {
                    commas = (length / 3) - 1;
                } else {
                    int mod = length % 3;
                    commas = (length - mod) / 3;
                }
                length = commas + getStringCount(size);

                newsize = (char *)malloc(length);

                for(int i = getStringCount(size)-1; i >= 0; i--) {
                    if(((getStringCount(size)-i)%3 == 0) && i != 0) {
                        newsize[i + commas] = size[i];
                        newsize[i + --commas] = ',';
                    } else {
                        newsize[i + commas] = size[i];
                    }
                }
                newsize[length] = '\0';

                printf("%10s", newsize);
                printf("%2s", " ");
                files++;
            }
            printf("%s\n", dir->d_name);
        }
        closedir(dp);
    }
    else {
        printf("File not found.\n");
    }

}
/**
   @brief deletes a file
   @param none
   @return none.
 */
int execDelete(){

    int i = 1;
    char cwd[1024];

    if(token[1] == NULL){
        printNotValid();
        return 0;
    }
    while(token[i] != NULL) {
        if(remove(token[i]) == -1) {
            getcwd(cwd, sizeof(cwd));
            if(i == 1) {
                pathNotFound();
                return 0;
            }
        }
        i++;
    }

    return 1;
}

/**
   @brief helper function to delete a file/directory during a move
   @param none
   @return none.
 */
int execDeleteToMove(){

    if(remove(token[1]) == -1) {
        pathNotFound(); // else prints an error statement
        return 0;
    }

    return 1;
}

/**
   @brief displays the current system date
   @param none
   @return none.
 */
int execDate(){
    char newdate[SIZE]; // new date holder
    char *dateElements[SIZE];   // date elements holder
    int i = 0, year, month, day, go = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("The current date is: %d/%d/%d\n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);

    while(go == 0){
        printf("Enter the new date: (mm-dd-yy) ");
        gets(newdate);

        //tokenize date
        dateElements[i] = strtok(newdate,"-/");
        while(dateElements[i]!=NULL){
            dateElements[++i] = strtok(NULL,"-/");
        }

        if(i < 2){
            printf("The system cannot accept the time entered.\n");
        }
        else
            go = 1;
    }

    //converts string date elements to integer
    month = atoi(dateElements[0]);
    day = atoi(dateElements[1]);
    year = atoi(dateElements[2]);

    SYSTEMTIME str_t;
    GetSystemTime(&str_t);

    struct tm str_time;

    //sets the date elements
    str_time.tm_year = year-1900;
    str_time.tm_mon = month-1;
    str_time.tm_mday = day;

    mktime(&str_time);
    //printf("New Date is: %d/%d/%d\n", str_time.tm_mon+1, str_time.tm_mday, str_time.tm_year+1900);
    printf("A required privilege is not held by the client.\n");
    return 1;
}

/**
   @brief changes the color of the shell
   @param none
   @return none.
 */
void execColor(){
    char color[50]="";

    if(token[1] == NULL){
        printNotValid();
        return ;
    }

    strcat(color, token[0]);
    strcat(color, " ");
    strcat(color, token[1]);
    printf("%s", color);

    system(color);
    return ;
}

/**
   @brief copies a file
   @param none
   @return none.
 */
int execCopy(){
    FILE *file1, *file2;//variables for file pointers
    char fileChar;//character holder for file
    int filesCopied = 1;

    file1 = fopen(token[1], "r");//open first file for reading

    //error handling
    if(token[1] == NULL || token[2] == NULL){
        printNotValid();
        return 0;
    }
    else{
        file2 = fopen(token[2], "w");

        //writes character by character
        while (1) {
            fileChar = getc(file1);
            if (fileChar == EOF){
                break;
            }
            else{
                fputc(fileChar, file2);
            }
        }

        if(strcmp(token[0],"copy") == 0)
            printf("\t %d file(s) copied.\n", filesCopied);
    }

    fclose(file1);
    fclose(file2);

    return 1;
}
/**
   @brief moves the file from one directory to another
   @param none
   @return none.
 */
void execMove(){

    int filesMoved = 1;

    if(token[1] == NULL || token [2] == NULL){
        pathNotFound();
    }
    else{
        if(execCopy() == 1 && execDeleteToMove() == 1)
            printf("\t %d file(s) moved.\n", filesMoved);
        else
            pathNotFound();
    }

}

/**
   @brief creates a new instance of a CMD
   @param none
   @return none.
 */
void execCmd(){
    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;

    dwVersion = GetVersion();

    // Get the Windows version.
    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.
    if (dwVersion < 0x80000000)
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Microsoft Windows [Version %ld.%ld.%ld]\n(c) 2018 Microsoft Corporation. All rights reserved.\n",
           dwMajorVersion,
           dwMinorVersion,
           dwBuild);
}

/**
   @brief clears the screen
   @param none
   @return none.
 */
void execCLS(){
    system("cls");
    return ;
}

/**
   @brief moves the file from one directory to another
   @param none
   @return none.
 */
void execChdir(){
    char *buffer = "";
    char directory[1024];

    //  checks if there is no token beside the chdir command
    if(token[1] == NULL){
        if(strlen(token[0]) == 7){
            if(token[0][5] == '.' && token[0][6] == '.'){
                chdir("..");
            }
        }
    }

        //  if it's not NULL,
    else if(token[1] != NULL){
        strcpy(directory, "./");
        strcpy(directory, token[1]);
        if(token[1][0] == '.' && token[1][1] == '.'){
            chdir(directory);
        }
        else if(chdir(directory) == -1) {
            printNotValid();
        }
    }
    printcwd(buffer);

    return ;
}

/**
   @brief displays the ip address of the machine
   @param none
   @return none.
 */
void execIpConfig() {
    system("ipconfig");
}

/**
   @brief validates whether given input tokens are allowed
   @param *command the commmand given by the user
   @return none.
 */
void evaluateToken(char *command){
    //the series of if statements evaluates the input command to which function to execute
    if(strcmp(command, "cd") == 0 || strcmp(command, "cd..") == 0){
        execCD();
        printf("\n");
    }
    else if(strcmp(command, "chdir") == 0 || strcmp(command, "chdir..") == 0){
        execChdir();
        printf("\n");
    }
    else if(strcmp(command, "cls") == 0){
        execCLS();
    }
    else if(strcmp(command, "cmd") == 0){
        execCmd();
    }
    else if(strcmp(command, "copy") == 0){
        execCopy();
    }
    else if(strcmp(command, "color") == 0){
        execColor();
    }
    else if(strcmp(command, "date") == 0){
        execDate();
    }
    else if(strcmp(command, "del") == 0){
        execDelete();
    }
    else if(strcmp(command, "ipconfig") == 0) {
        execIpConfig();
    }
    else if(strcmp(command, "dir") == 0){
        char cwd[1024];
        DIR *d;
        int i = 1;
        if(token[1] == NULL) {
            d = opendir(getcwd(cwd, sizeof(cwd)));
            execDir(d, ".");
        } else {
            while(token[i] != NULL){
                d = opendir(token[i]);
                if(d == NULL) {
                    printf("File not found\n");
                } else {
                    printf("\nDirectory of %s\n", token[i]);
                    execDir(d, token[i]);
                }
                i++;
            }
        }

    }
    else if(strcmp(command, "mkdir") == 0 || strcmp(command, "md") == 0){
        execMkdir();
    }
    else if(strcmp(command, "move") == 0){
        execMove();
    }
    else if(strcmp(command, "rename") == 0 || strcmp(command, "ren") == 0){
        execRename();
    }
    else if(strcmp(command, "rmdir") == 0 || strcmp(command, "rd") == 0){
        execRmdir();
    }
    else if(strcmp(command, "time") == 0){
        execTime();
    }
    else if(strcmp(command, "type") == 0){
        execType();
    }
    else if(strcmp(command, "help") == 0){
        execHelp();
    }
    else if(strcmp(command, "exit") == 0){
        exit(0);
    }
    else{
        printNotValid();
    }
    return ;
}

/**
   @brief tokenizes the input
   @param *command the commmand given by the user
   @return none.
 */
void tokenize(char *command){
    int i = 0;

    //tokenize the input
    token[i] = strtok(command," \t");
    while(token[i]!=NULL){
        token[++i] = strtok(NULL," \t");
        tokenCount++;//counts the number of tokens
    }

    return ;
}
/**
   @brief converts the given command into lowercase.
   @param command The command from user
   @return none.
 */
void toLowercase(char *command){
    int i = 0;

    while( command[i] ) {
        command[i] = (tolower(command[i]));//converts to lower case
        i++;
    }
}
/**
   @brief converts the given command into lowercase.
   @param none
   @return none.
 */
void inputFunctions(){
    char command[1024];
    gets(command);      // gets the user input
    toLowercase(command);     // converts the user input to lowercase
    tokenize(command);  // tokenizes the user input
    evaluateToken(token[0]);    // checks the first token
    tokenCount = 0;     // number of tokens in a line is set to zero
}

/**
   @brief non terminating function being called in main
   @param none
   @return none.
 */
void start(){
    execCmd();
    char *currentDirectory;

    while(1){
        if( (currentDirectory = _getcwd( NULL, 0 )) == NULL )
            perror("getcwd error");
        else{

            printf( "\n%s>", currentDirectory);
            free(currentDirectory);
        }
        inputFunctions();
    }
}

int main(){
    token[0] = "";   // set token to the empty string
    start();
    return(0);
}