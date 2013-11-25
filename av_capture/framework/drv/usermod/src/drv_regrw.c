
#include <drv.h>
#include <drv_csl.h>

int DRV_regDump(Uint32 regAddr, Uint32 numRegs, Uint32 numPrintCols)
{
  int offset, i;
  volatile Uint32 *regVirtAddr;

  if(numPrintCols<1)
    numPrintCols = 4;

  if(numPrintCols>6)
    numPrintCols = 6;

  if(numRegs<=1) {
    numRegs = 4;
  }

  offset = regAddr - gCSL_vpssHndl.regBasePhysAddr;
  if(offset<0 || (offset+numRegs) > gCSL_vpssHndl.regBaseLen)
  {
    OSA_printf(" REG DUMP: Invalid address range : 0x%x .. 0x%x\n", regAddr, regAddr+numRegs*4);
    return OSA_EFAIL;
  }

  regVirtAddr = (Uint32*)((Uint32)gCSL_vpssHndl.regBaseVirtAddr + offset);

  OSA_printf(" \n");

  while(numRegs) {
    OSA_printf(" 0x%08x:", regAddr);
    for(i=0; i<numPrintCols; i++) {

      OSA_printf(" %08x", *regVirtAddr);

      regVirtAddr++;
      regAddr+=4;
      numRegs--;

      if(numRegs==0)
        break;
    }
    OSA_printf("\n");
  }

  OSA_printf(" \n");

  return OSA_SOK;
}

int DRV_regWrite(Uint32 regAddr, Uint32 regValue)
{
  Int32 offset;
  volatile Uint32 *regVirtAddr;

  offset = regAddr - gCSL_vpssHndl.regBasePhysAddr;
  if(offset<0 || (offset) > gCSL_vpssHndl.regBaseLen)
  {
    OSA_printf(" REG WR: Invalid address : 0x%x\n", regAddr);
    return OSA_EFAIL;
  }

  regVirtAddr = (Uint32*)((Uint32)gCSL_vpssHndl.regBaseVirtAddr + offset);

  OSA_printf(" \n");
  OSA_printf(" REG ORG 0x%08x: %x\n", regAddr, *regVirtAddr);

  *regVirtAddr = regValue;

  OSA_printf(" REG NEW 0x%08x: %x\n", regAddr, *regVirtAddr);
  OSA_printf(" \n");

  return OSA_SOK;
}


int DRV_regTestShowUsage(char *str)
{
  OSA_printf("\n");
  OSA_printf(" VPSS Register read/write utility, \n");
  OSA_printf(" WRITE REG, %s: -w <regPhysAddrInHex> <regValueInHex>\n", str);
  OSA_printf(" READ  REG, %s: <regPhysAddrInHex> <numRegsInDec> -c <numCols>\n", str);
  OSA_printf("\n");
  OSA_printf(" Example,\n");
  OSA_printf(" WRITE REG, %s -w 1c7001c 1234abcd \n", str);
  OSA_printf(" READ  REG, %s 1c70000 16 \n", str);
  OSA_printf(" READ  REG, %s 1c70000 16 -c 1 \n", str);
  OSA_printf("\n");
  OSA_printf(" DM365 Register Base Addresses,\n");
  OSA_printf(" ISP5    : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_ISP5_0_OFFSET);
  OSA_printf(" VPSS    : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_VPSS_0_OFFSET);
  OSA_printf(" RSZ     : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_RSZ_0_OFFSET);
  OSA_printf(" IPIPE   : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPE_0_OFFSET);
  OSA_printf(" ISIF    : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_ISIF_0_OFFSET);
  OSA_printf(" IPIPEIF : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_IPIPEIF_0_OFFSET);
  OSA_printf(" H3A     : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_H3A_0_OFFSET);
  OSA_printf(" LDC     : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_LDC_0_OFFSET);
  OSA_printf(" FACE.D  : 0x%08x \n", CSL_ISP5_BASE_PHYS_ADDR + CSL_FACE_DETECT_0_OFFSET);
  OSA_printf("\n");

  return OSA_SOK;
}

int DRV_regTestMain(int argc, char **argv)
{
  int status;
  Uint32 regAddr;
  Uint32 regValue=0;
  Uint32 numRegs=8;
  Uint32 numCols=0;
  Bool   regDump;

  if(argc<2) {
    DRV_regTestShowUsage(argv[0]);
    return OSA_EFAIL;
  }

  if(strcmp(argv[1], "-w")==0) {
    regDump=FALSE;
    if(argc < 4) {
      DRV_regTestShowUsage(argv[0]);
      return OSA_EFAIL;
    }
    regAddr = xstrtoi(argv[2]);
    regValue = xstrtoi(argv[3]);
  }
  else {
    regDump=TRUE;

    if(argc<2) {
      DRV_regTestShowUsage(argv[0]);
      return OSA_EFAIL;
    }

    regAddr = xstrtoi(argv[1]);
    if(argc>2)
      numRegs = atoi(argv[2]);

    if(argc==5) {
      if(strcmp(argv[3], "-c")==0)
        numCols = atoi(argv[4]);
    }

  }

  status = CSL_vpssOpen(&gCSL_vpssHndl);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_sysInit()\n");
    return status;
  }

  if(regDump) {
    DRV_regDump(regAddr, numRegs, numCols);
  } else {
    DRV_regWrite(regAddr, regValue);
  }

  status = CSL_vpssClose(&gCSL_vpssHndl);

  return status;
}


