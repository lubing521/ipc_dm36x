
#ifndef _ALG_CRYPTO_H_
#define _ALG_CRYPTO_H_

#include <alg.h>


#define ALG_CRYPTO_MODE_NORMAL   0
#define ALG_CRYPTO_MODE_FAST     1

#define ALG_CRYPTO_MAX_KEY_LEN   (16)

typedef struct {

  Uint8  key[ALG_CRYPTO_MAX_KEY_LEN];
  Uint16 mode;

} ALG_CryptoCreate;

void *ALG_encryptCreate(ALG_CryptoCreate *create);
int   ALG_encryptRun(void *hndl, Uint8 *inAddr, Uint8 *outAddr, Uint32 size);
int   ALG_encryptDelete(void *hndl);

void *ALG_decryptCreate(ALG_CryptoCreate *create);
int   ALG_decryptRun(void *hndl, Uint8 *inAddr, Uint8 *outAddr, Uint32 size);
int   ALG_decryptDelete(void *hndl);

#endif
