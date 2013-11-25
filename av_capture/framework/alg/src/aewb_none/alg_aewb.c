
#include <alg_aewb.h>

void *ALG_aewbCreate(ALG_AewbCreate *create)
{
  return (void*)1;
}

int ALG_aewbRun(void *hndl, ALG_AewbRunPrm *prm, ALG_AewbStatus *status)
{
  memset(status, 0, sizeof(*status));
 
  return 0;
}

int ALG_aewbDelete(void *hndl)
{
  return 0;
}


