#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <openssl/rand.h>

#include "crypto.h"
#include "common.h"

#define LEDGER_FILE "ledger.bin"
#define PERMISSIONS (S_IRUSR | S_IWUSR)

int main(int argc, char **argv) {

	struct cipher_params params;
	struct stat st;
	memset(&params, 0, sizeof(struct cipher_params));
	
    if(argc != 2) {
		printf("Usage: ./ledger writeup/crack\n");	
		return 1;
	} else {
	
		if (stat(LEDGER_FILE, &st) != 0) {
        	// file does not yet exist:  no need to check
        	printf("No existing ledger: creating new\n");
        	RAND_bytes(params.iv, 16);
    	}
		
		unsigned char hash_key [2] = {0,0}; //2 key bytes to test
		unsigned char *test_key; 			//hash is stored in this string
		int i, j, flag = 0; 				// loop variables
		int fd; 							//file pointer
		unsigned char fd_key_hash[16], fd_ctext_hash[16], *ctext_hash, *ptext;
		int ctext_len = st.st_size - 48, ptext_len;

		
		//read from ledger
		fd = open(LEDGER_FILE, O_RDONLY, PERMISSIONS);
        read(fd, fd_key_hash, 16);

		//try possible character combinations until key is found
		for(i = 0; i < 256; i++) {
			for(j = 0; j < 256; j++) {
				test_key = md5_hash(hash_key, 2);
				if(flag == 0) {
					if(memcmp(test_key, fd_key_hash, 16) == 0) {
						flag = 1;
						//printf("%s\n", test_key);
						//printf("%s\n", hash_key);
						memcpy(params.key, hash_key, 16);
						memcpy(params.key_hash, test_key, 16);
						break;
					}
				}
				hash_key[1]++;
			}
			if(flag == 1) {
				break;
			}
			hash_key[0]++;		
		}
		free(test_key);
		
		//correct key is found
		read(fd, fd_ctext_hash, 16);
        read(fd, params.iv, 16);
        
        params.msg = malloc(ctext_len) - 1;
        params.len = ctext_len;
        read(fd, params.msg, ctext_len);
        
        ctext_hash = md5_hash(params.msg, params.len);
        
        //check ciphertext hash in file = actual hash
        if (memcmp(ctext_hash, fd_ctext_hash, 16) != 0) {
           printf("invalid ciphertext");
        }
        
        free(ctext_hash);
                
        //print param contents to test
        //printf("%s\n", params.key);
        //printf("%s\n", params.key_hash);
        //printf("%s\n", params.iv);
        //printf("%s\n", params.msg);
        //printf("%d\n", params.len);
        //
        ptext_len = aes128_decrypt(&params, &ptext);
        
        printf("----- BEGIN LEDGER -----\n");
        for (i = 0; i < ptext_len; i++) {
            printf("%c", ptext[i]);
        }
        printf("----- END LEDGER -----\n");
        
        close(fd);
		
	}
	
	return 0;
}
