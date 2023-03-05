#include <stdio.h>
#include <string.h>

#include <openssl/err.h>

#include "aes.h"


void read_key_iv(unsigned char* key, unsigned char* iv)
{
    FILE* key_fp = fopen("key", "r");
    FILE* iv_fp = fopen("iv", "r");

    for(int i=0; i<32; i++) key[i] = fgetc(key_fp);
    for(int i=0; i<16; i++) iv[i] = fgetc(iv_fp);

    fclose(key_fp);
    fclose(iv_fp);
}

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

void show_plaintext(unsigned char* plaintext, int len)
{
    printf("plaintext:\n");
    printf("----------\n");
    for(int i=0; i<len; i++)
	printf("%c", plaintext[i]);
    printf("\n----------\n\n");
}

void get_count(unsigned char* arr, int len, char* filename)
{
    FILE* fp = fopen(filename, "w");
    int count[256];
    for(int i=0; i<256; i++) count[i] = 0;
    for(int i=0; i<len; i++)
	count[arr[i]]++;

    /* printf("Count:\n"); */
    /* printf("------\n"); */
    for(int i=0; i<256; i++)
    {
	//printf("count[%d]: %d\n", i, count[i]);
	fprintf(fp, "%d    %d\n", i, count[i]);
    }
    //printf("\n------\n\n");
    fclose(fp);
}


void decrypt_test(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, char* mode)
{
    // buffer for decrypted text
    unsigned char *decryptedtext = (unsigned char *)malloc(ciphertext_len * sizeof(unsigned char));
    int decryptedtext_len;
		      
    printf("decryption-key: ");
    for(int i=0; i<32; i++) printf("%2x ", key[i]); printf("\n\n");
    printf("decryption-iv: ");
    for(int i=0; i<16; i++) printf("%2x ", iv[i]); printf("\n\n");


    // decrypt using correct key
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
				    decryptedtext, mode);

    /* Add a NULL terminator. We are expecting printable text */
    /* decryptedtext[decryptedtext_len] = '\0'; */
    /* /\* Show the decrypted text *\/ */
    printf("Decrypted text is:\n");
    printf("%s\n", decryptedtext);
    printf("\n----------------------decrypted-text-end\n\n");

    // output count
    get_count(decryptedtext, decryptedtext_len, "count-correct-key.dat");
    FILE* fp = fopen("output-correct-key.dat", "w");
    for(int i=0; i<decryptedtext_len; i++)
	fputc(decryptedtext[i], fp);
    fclose(fp);

    // generate random key
    unsigned char random_key[32];
    for(int i=0; i<32; i++) random_key[i] = rand() % 0xff;

    // decrypt using random key
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, random_key, iv,
				    decryptedtext, mode);

    // output count
    get_count(decryptedtext, decryptedtext_len, "count-random-key.dat");
    fp = fopen("output-random-key.dat", "w");
    for(int i=0; i<decryptedtext_len; i++)
	fputc(decryptedtext[i], fp);
    fclose(fp);

}


// ./a.out [file] [mode]
// eg. ./a.out harry-potter-1.txt obc
int main(int argc, char** argv)
{
    if(argc < 2)
    {
	printf("Enter File Name\n");
	return 0;
    }
    char* file = argv[1];

    if(argc < 3)
    {
	printf("Enter Mode\n");
	return 0;
    }
    char* mode = argv[2];

    srand(time(0));
    unsigned char key[32], iv[16];
    read_key_iv(key, iv);

    /* Message to be encrypted */
    int len = get_file_size(file);
    unsigned char *plaintext = (unsigned char *)malloc(len * sizeof(unsigned char));
    read_plaintext(plaintext, len, file);
    show_plaintext(plaintext, len);

    get_count(plaintext, len, "plaintext_count.dat");

    /*
     * Buffer for ciphertext. Ensure the buffer is long enough for the
     * ciphertext which may be longer than the plaintext, depending on the
     * algorithm and mode.
     */
    unsigned char *ciphertext = (unsigned char *)malloc((len+16) * sizeof(unsigned char));
    // + 16 for padding


    printf("encryption-key: ");
    for(int i=0; i<32; i++) printf("%2x ", key[i]); printf("\n\n");
    printf("encryption-iv: ");
    for(int i=0; i<16; i++) printf("%2x ", iv[i]); printf("\n\n");

    /* Encrypt the plaintext */
    int ciphertext_len = encrypt(plaintext, len, key, iv, ciphertext, mode);

    printf("Ciphertext is:\n");
    BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
    printf("\n\n");

    printf("ciphertext len: %d\n\n", ciphertext_len);

    get_count(ciphertext, ciphertext_len, "count.dat");

    // test decryption returns plaintext 
    decrypt_test(ciphertext, ciphertext_len, key, iv, mode);

    printf("file: %s\n", file);
    printf("mode: %s\n", mode);
    printf("key:\n");
    for(int i=0; i<32; i++) printf("%x ", key[i]); printf("\n");
    printf("iv:\n");
    for(int i=0; i<16; i++) printf("%x ", iv[i]); printf("\n");
    printf("\n\n");

    return 0;
}
