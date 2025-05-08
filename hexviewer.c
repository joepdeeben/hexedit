#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


void take_str(char *str){

    fgets(str, 100, stdin);
    str[strcspn(str, "\n")] = 0;
}

char* read_file(char *path, struct stat *sb){
    if ((stat(path, sb) != 0)){
        perror("filesize lookup failed!");
        return NULL;
    };
    int len = sb -> st_size;
 


    
    int fp = open(path, O_RDONLY);

    if (fp < 0) {
        perror("open failed");
        return NULL;
    }
    char *start_map = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fp, 0);
    if (start_map == MAP_FAILED){
        perror("accessing failed!");
        return NULL;
    };

    close(fp);

    return start_map;
}


int main()  {
    struct stat sb;

    char str[100];
    take_str(str);


    printf("%s", str);
    char* file_contents = read_file(str, &sb);
    if (!file_contents) {
        return 1;
    }

    printf("file size is: %ld bytes\n", sb.st_size);
    printf("assigning buffer and reading file contents\n");
    
    for (size_t i = 0; i < sb.st_size; i++) {
        printf("%02X ", (unsigned char)file_contents[i]);
    }
    printf("\n");
    munmap(file_contents, sb.st_size);
    return 0;
}