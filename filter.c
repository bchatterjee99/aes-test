#include <stdio.h>
#include <stdlib.h>


int get_file_size(char* filename)
{
    FILE* fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fclose(fp);
    return len;
}

void read_plaintext(unsigned char* plaintext, int len, char* filename)
{
    FILE* fp = fopen(filename, "r");
    for(int i=0; i<len; i++)
	plaintext[i] = fgetc(fp);
    fclose(fp);
}

int main()
{
    int len = get_file_size("harry-potter.txt");
    unsigned char *plaintext = (unsigned char *)malloc(len * sizeof(unsigned char));
    read_plaintext(plaintext, len, "harry-potter.txt");

    FILE* fp = fopen("harry-potter-filtered.txt", "w");
    for(int i=0; i<len; i++)
	if(plaintext[i] <= 127)
	    fputc(plaintext[i], fp);
    fclose(fp);

    printf("\n\n  thikache\n\n");
    return 0;
}
