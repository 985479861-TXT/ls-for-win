#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define COLOR_DIR 11
#define COLOR_FILE 15
#define COLOR_RESET 7

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main(int argc, char *argv[]){
    char search_path[MAX_PATH];
    WIN32_FIND_DATA find_data;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    if (argc < 2) {
        snprintf(search_path, sizeof(search_path), ".\\*");
    }
    else{
        snprintf(search_path, sizeof(search_path), "%s\\*", argv[1]);
    }

    hFind = FindFirstFile(search_path, &find_data);

    if (hFind == INVALID_HANDLE_VALUE){
        printf("Dir not found or no priviledge");
        return 1;
    }

    do{
        if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0){
            continue;
        }
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            set_color(COLOR_DIR);
            printf("%-25s\t[DIR]\n", find_data.cFileName);
        }
        else {
            set_color(COLOR_FILE);

            unsigned long long file_size = ((unsigned long long)find_data.nFileSizeHigh << 32) | find_data.nFileSizeLow;
            printf("%-25s\t%llu bytes\n", find_data.cFileName, file_size);
        }
        
    } while (FindNextFile(hFind, &find_data) != 0);

    set_color(COLOR_RESET);
    FindClose(hFind);

    return 0;
}

//gcc t1.c -o ls | mv C:/bin/