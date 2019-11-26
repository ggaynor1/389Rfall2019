#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <openssl/rand.h>

#include "crypto.h"
#include "common.h"

#include <stdlib.h>
#include <time.h>

#define LEDGER_FILE "ledger.bin"
#define PERMISSIONS (S_IRUSR | S_IWUSR)

//Random String Generator
char *randomStr(size_t len) {
	static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
	char* randomString = NULL;
	int n;
	
	if(len) {
		randomString = malloc(sizeof(char) * (len + 1));
		
		if(randomString) {
			for (n = 0; n < len; n++) {
				int key = rand() % (int)(sizeof(charset) - 1);
				randomString[n] = charset[key];
			}
			
			randomString[len] = '\0';
		}
	}
	return randomString;
}

int main(int argc, char **argv) {

	struct cipher_params params;
	struct stat st;
	memset(&params, 0, sizeof(struct cipher_params));
	unsigned char *string;
	
	srand(time(NULL));
	
    if(argc < 0) {
		printf("Usage: ./ledger writeup/crack\n");	
		return 1;
	} else {
	
		if (stat(LEDGER_FILE, &st) != 0) {
        	// file does not yet exist:  no need to check
        	printf("No existing ledger: creating new\n");
        	RAND_bytes(params.iv, 16);
    	}
		
		unsigned char *key, *key_hash;		//hashes is stored in these variables
		
		int i, j, flag = 0; 				//loop variables
		int fd; 							//file pointer
		unsigned char fd_key_hash[16], fd_ctext_hash[16], *ctext_hash, *ptext;
		int ctext_len = st.st_size - 48, ptext_len;

		
		//read from ledger
		fd = open(LEDGER_FILE, O_RDONLY, PERMISSIONS);
        read(fd, fd_key_hash, 16);

		//On each iteration, we generate random strings of length 16, hash it,
		//then zero all but the first two bytes and hash again
		//then compare that to the key hash in the ledger file
		while(flag != 1) {
			string = randomStr(16); //generate random string
			
			key = md5_hash(string, 16);
			memset(key+2, 0, 14);
			key_hash = md5_hash(key, 2);
			
			if(memcmp(key_hash, fd_key_hash, 16) == 0) {
				flag = 1;
				memcpy(params.key, key, 16);
				memcpy(params.key_hash, key_hash, 16);
			}
		}

		//correct key is found
		read(fd, fd_ctext_hash, 16);
        read(fd, params.iv, 16);
        
        params.msg = malloc(ctext_len);
        params.len = ctext_len;
        read(fd, params.msg, ctext_len);
        
        ctext_hash = md5_hash(params.msg, params.len);
        
        //check ciphertext hash in file = actual hash
        if (memcmp(ctext_hash, fd_ctext_hash, 16) != 0) {
           die("invalid ciphertext");
        }
        
        free(ctext_hash);
                
        ptext_len = aes128_decrypt(&params, &ptext);
        
        printf("----- BEGIN LEDGER -----\n");
        for (i = 0; i < ptext_len; i++) {
            printf("%c", ptext[i]);
        }
        printf("----- END LEDGER -----\n");
        
        free(params.msg);
        params.msg = ptext;
        params.len = ptext_len;
        
        close(fd);
        
		
	}
	
	return 0;
}



