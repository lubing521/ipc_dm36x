#include <osa_file.h>

int OSA_fileReadFile(char *fileName, Uint8 *addr, Uint32 readSize, Uint32 *actualReadSize)
{
    int retVal = OSA_SOK;
    Uint8  *curAddr;

    Uint32 readDataSize, fileSize, chunkSize = 1024 * 100;
    Uint32 userReadSize;

    FILE *hndlFile;


    hndlFile = fopen(fileName, "rb");
    if (hndlFile == NULL)
    {
        retVal = OSA_EFAIL;
        goto exit;
    }

    curAddr      = addr;
    fileSize     = 0;
    userReadSize = readSize;

    while (1)
    {
        if (userReadSize != 0)
        {
            if (chunkSize > userReadSize)
            {
                chunkSize = userReadSize;
            }
            readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
            fileSize += readDataSize;
            if (chunkSize != readDataSize)
            {
                break;
            }
            if (userReadSize==fileSize)
            {
                break;
            }
            curAddr += chunkSize;
        }
        else
        {
            readDataSize = fread(curAddr, 1, chunkSize, hndlFile);
            fileSize += readDataSize;
            if (chunkSize!=readDataSize)
            {
                break;
            }
            curAddr += chunkSize;
        }
    }
#ifdef OSA_DEBUG_FILE
    OSA_printf("Done. [%d bytes] \r\n", fileSize);
#endif
    fclose(hndlFile);

exit:
    if (retVal != OSA_SOK)
    {
#ifdef OSA_DEBUG_FILE
        OSA_printf("open %s ERROR \r\n", fileName);
#endif
        fileSize = 0;
    }
    if (actualReadSize != NULL)
    {
        *actualReadSize = fileSize;
    }

    return retVal;
}

int OSA_fileWriteFile(char *fileName, Uint8 *addr, Uint32 size)
{
    int retVal = OSA_SOK;
    Uint32 writeDataSize;
    Bool errorInWriting = FALSE;
    FILE *hndlFile = NULL;
    if (size == 0)
    {
        return OSA_SOK;
    }

#ifdef OSA_DEBUG_FILE
    OSA_printf(" [FILE ] Writing to file [%s] (%d bytes) ... ", fileName, size);
#endif
    hndlFile = fopen(fileName, "wb");
    if (hndlFile == NULL)
    {
        retVal = OSA_EFAIL;
        goto exit;
    }

    {
        // write in units of chunkSize
        Int32 chunkSize = 96 * 1024;
        Int32 fileSize  = size;
        Int8  *curAddr  = (Int8*)addr;

        while (fileSize > 0)
        {
            if (fileSize < chunkSize)
            {
                chunkSize = fileSize;
            }
            writeDataSize = 0;
            writeDataSize = fwrite(curAddr, 1, chunkSize, hndlFile);
            if (writeDataSize != chunkSize)
            {
                // error in writing, abort
                errorInWriting = TRUE;
                retVal         = OSA_EFAIL;
                break;
            }
            curAddr  += chunkSize;
            fileSize -= chunkSize;
        }
        writeDataSize = size - fileSize;
    }

#ifdef OSA_DEBUG_FILE
    OSA_printf("Done. [%d bytes] \r\n", writeDataSize);
#endif
    fflush(hndlFile);
    fclose(hndlFile);

exit:
    if (retVal != OSA_SOK)
    {
#ifdef OSA_DEBUG_FILE
        OSA_printf("write %s ERROR \r\n", fileName);
#endif
    }
    return retVal;

}

int OSA_fileWriteVideoData(char *fileName, Uint8 *Yaddr, Uint8 *UVaddr, Uint32 width, Uint32 height, Uint32 pitch)
{
    int retVal = OSA_SOK;
    Uint32 writeDataSize = 0;
    FILE *hndlFile = NULL;
    int writesize = (width * height * 3) >> 1;

    if ((width == 0) || (height == 0) || (pitch == 0))
    {
        return OSA_SOK;
    }

#ifdef OSA_DEBUG_FILE
    OSA_printf(" [FILE ] Writing to file [%s] (%d bytes) ... ", fileName, writesize);
#endif

    hndlFile = fopen(fileName, "wb");
    if (hndlFile == NULL)
    {
        retVal = OSA_EFAIL;
        goto filewrite_abort;
    }

    {
        // write Y data as per pitch
        Int32 fileSize = width * height, chunkSize = width;
        Int8  *curAddr = (Int8*)Yaddr;
        while (fileSize > 0)
        {
            writeDataSize = fwrite(curAddr, 1, chunkSize, hndlFile);
            if (writeDataSize != chunkSize)
            {
                // error in writing, abort
                retVal = OSA_EFAIL;
                goto filewrite_abort;
            }
            curAddr += pitch;
            fileSize -= chunkSize;
        }
    }

    {
        // write UV data as per pitch
        Int32 fileSize = (width * height)>>1, chunkSize = width;
        Int8  *curAddr = (Int8*)UVaddr;
        while (fileSize > 0)
        {
            writeDataSize = fwrite(curAddr, 1, chunkSize, hndlFile);
            if (writeDataSize != chunkSize)
            {
                // error in writing, abort
                retVal = OSA_EFAIL;
                goto filewrite_abort;
            }
            curAddr  += pitch;
            fileSize -= chunkSize;
        }
    }

#ifdef OSA_DEBUG_FILE
    OSA_printf("Done. [%d bytes] \r\n", writesize);
#endif
    fflush(hndlFile);



filewrite_abort:
    if (hndlFile != NULL)
    {
        fclose(hndlFile);
    }
    if (retVal != OSA_SOK)
    {
#ifdef OSA_DEBUG_FILE
        OSA_printf("write video %s ERROR \r\n", fileName);
#endif
    }
    return retVal;

}

