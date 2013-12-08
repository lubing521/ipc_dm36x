#include <osa_cmem.h>

//#define OSA_DEBUG_CMEM
#define OSA_CMEM_DUMP

#ifdef OSA_CMEM_DUMP
#define CMEM_MB_CONVERT (1024 * 1024)
float totalcmemused = 0;
unsigned char cmemfirsttime = 1;
FILE *cmemfp = NULL;
unsigned int count = 0;

#endif

Uint8 *OSA_cmemAllocCached(Uint32 size, Uint32 alignment)
{
    CMEM_AllocParams  prm;
    Uint8 *virtAddr = NULL;

    prm.type      = CMEM_HEAP;
    prm.flags     = CMEM_CACHED;
    prm.alignment = alignment;

    virtAddr = (Uint8*)CMEM_alloc(size, &prm);

#ifdef OSA_DEBUG_CMEM
    OSA_printf(" OSA_CMEM: %08x %d bytes\n", (Uint32)virtAddr, size );
#endif

#ifdef OSA_CMEM_DUMP
    //if (cmemfirsttime)
    {
        cmemfp = fopen("/tmp/cmemdebug.txt", "ab+");
        //cmemfirsttime = 0;
    }
    count++;
    totalcmemused += size;
    fprintf(cmemfp, "CMEM[%2d]: [0x%lx %8d = %5.2f M] Used = %5.2f M Cached\n",
                        count,
                        (unsigned long)virtAddr,
                        size,
                        (float)size/CMEM_MB_CONVERT,
                        totalcmemused/CMEM_MB_CONVERT);
    fclose(cmemfp);
#endif

#ifdef OSA_DEBUG_CMEM
    fprintf(stderr, "CMEM[%2d]: [0x%lx %8d = %5.2f M] Used = %5.2f M Cached\n",
                        count,
                        (unsigned long)virtAddr,
                        size,
                        (float)size/CMEM_MB_CONVERT,
                        totalcmemused/CMEM_MB_CONVERT);
    
#endif
    return virtAddr;
}

Uint8 *OSA_cmemAlloc(Uint32 size, Uint32 alignment)
{
    CMEM_AllocParams  prm;
    Uint8 *virtAddr = NULL;

    prm.type      = CMEM_HEAP;
    prm.flags     = CMEM_NONCACHED;
    prm.alignment = alignment;

    virtAddr = (Uint8*)CMEM_alloc(size, &prm);

#ifdef OSA_DEBUG_CMEM
    OSA_printf(" OSA_CMEM: %08x %d bytes\n", (Uint32)virtAddr, size );
#endif

#ifdef OSA_DEBUG_CMEM
    OSA_assert(virtAddr!=NULL);
#endif

#ifdef OSA_CMEM_DUMP
    //if (cmemfirsttime)
    {
        cmemfp = fopen("/tmp/cmemdebug.txt", "ab+");
        //cmemfirsttime = 0;
    }
    count++;
    totalcmemused += size;
    fprintf(cmemfp, "CMEM[%2d]: [0x%lx %8d = %5.2f M] Used = %5.2f M\n",
                        count,
                        (unsigned long)virtAddr,
                        size,
                        (float)size/CMEM_MB_CONVERT,
                        totalcmemused/CMEM_MB_CONVERT);
    fclose(cmemfp);
#endif

#ifdef OSA_DEBUG_CMEM
    fprintf(stderr, "CMEM[%2d]: [0x%lx %8d = %5.2f M] Used = %5.2f M\n",
                        count,
                        (unsigned long)virtAddr,
                        size,
                        (float)size/CMEM_MB_CONVERT,
                        totalcmemused/CMEM_MB_CONVERT);
    
#endif
    return virtAddr;
}

int OSA_cmemFree(Uint8 *ptr)
{
    CMEM_AllocParams prm;
    if (ptr == NULL)
    {
        return OSA_EFAIL;
    }

    prm.type      = CMEM_HEAP;
    prm.flags     = 0;
    prm.alignment = 0;

#ifdef OSA_CMEM_DUMP
    fprintf(cmemfp, "CMEM DEBUG FREE: 0x%lx\n", (unsigned long)ptr);
#endif

    return CMEM_free(ptr, &prm);
}

Uint8* OSA_cmemGetPhysAddr(Uint8 *virtAddr)
{
    Uint8 *physAddr = NULL;
    if (virtAddr == NULL)
    {
        return NULL;
    }

    physAddr = (Uint8*)CMEM_getPhys(virtAddr);

#ifdef OSA_DEBUG_CMEM
    OSA_printf(" OSA_CMEM: %08x, %08x\n", (Uint32)virtAddr, (Uint32)physAddr);
#endif

#ifdef OSA_DEBUG_CMEM
    OSA_assert(physAddr!=NULL);
#endif

    return physAddr;
}

int OSA_cmemTestMain(int argc, char **argv)
{
    Uint32 size;
    Uint8 *virtAddr[100], *physAddr[100];
    int i, num=1;

    CMEM_init();

    if (argc < 3)
    {
        OSA_printf(" USAGE : %s cmem <size>\n", argv[0]);
        return 0;
    }

    size = atoi(argv[2])/num;

    for (i=0; i<num; i++)
    {
        virtAddr[i] = OSA_cmemAlloc(size, 32);

        physAddr[i] = OSA_cmemGetPhysAddr(virtAddr[i]);

        if (virtAddr[i]!=NULL)
            printf("VIRT=0x%08x PHYS=0x%08x SIZE=%d\n", (Uint32)virtAddr[i], (Uint32)physAddr[i], size);
        else
            printf("ERROR: could not allocate 0x%08x size memory\n", size);
    }

    for (i=0; i<num; i++)
        OSA_cmemFree(virtAddr[i]);

    CMEM_exit();

    return 0;
}

