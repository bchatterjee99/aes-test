#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main()
{
    srand(time(0));

    unsigned char key[32];
    unsigned char iv[16];

    for(int i=0; i<32; i++) key[i] = rand() & 0xff;
    for(int i=0; i<16; i++) iv[i] = rand() & 0xff;

    FILE* key_fp = fopen("key", "w");
    FILE* iv_fp = fopen("iv", "w");

    for(int i=0; i<32; i++)
	fputc(key[i], key_fp);

    for(int i=0; i<16; i++)
	fputc(iv[i], iv_fp);

    fclose(key_fp);
    fclose(iv_fp);
    return 0;
}

