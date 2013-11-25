

#include <csl_ldcIoctl.h>

CSL_Status CSL_ldcHwControl(CSL_LdcHandle hndl, Uint32 cmd, void *prm)
{
  CSL_Status status = CSL_SOK;
  Bool32    isBusy;
  static CSL_LdcFrameConfig frameConfig;
  static CSL_LdcHwSetup hwSetup;
  Uint32 *table = NULL;
  Uint32  tableSize = CSL_LDC_TABLE_MAX_ENTRIES*4;

  switch (cmd) {

  case CSL_LDC_CMD_HW_SETUP:


    if (status == CSL_SOK)
      status = CSL_copyFromUser(&hwSetup, prm, sizeof(hwSetup));

    if (status == CSL_SOK) {
      if(hwSetup.frameConfig) {
        status = CSL_copyFromUser(&frameConfig, hwSetup.frameConfig, sizeof(frameConfig));
        
        hwSetup.frameConfig = &frameConfig;
      }
    }

    if (status == CSL_SOK) {
      if(hwSetup.table) {
      
        table = CSL_sysMemAlloc(tableSize);
        if (table == NULL)
          status = CSL_EFAIL;
        
        if(table!=NULL) {      
          status = CSL_copyFromUser(table, hwSetup.table, tableSize);
        
          hwSetup.table = table;
        }
      }
    }

    if (status == CSL_SOK)
      status = CSL_ldcHwSetup(hndl, &hwSetup);

    if (table != NULL)
      CSL_sysMemFree(table);

    break;

  case CSL_LDC_CMD_HW_RESET:
    status = CSL_ldcHwReset(hndl);
    break;

  case CSL_LDC_CMD_FRAME_CONFIG:

    status = CSL_copyFromUser(&frameConfig, prm, sizeof(frameConfig));

    if (status == CSL_SOK)
      status = CSL_ldcFrameConfig(hndl, &frameConfig);

    break;

  case CSL_LDC_CMD_ENABLE:
    status = CSL_ldcEnable(hndl, (Bool32) prm);
    break;

  case CSL_LDC_CMD_IS_BUSY:
    status = CSL_ldcIsBusy(hndl, &isBusy);

    status |= CSL_putUser(isBusy, (Bool32 *) prm);
    break;

  case CSL_LDC_CMD_WRITE_TABLE:

    table = CSL_sysMemAlloc(tableSize);
    if (table == NULL)
      status = CSL_EFAIL;

    if (status == CSL_SOK)
      status = CSL_copyFromUser(table, prm, tableSize);

    if (status == CSL_SOK)
      status = CSL_ldcWriteTable(hndl, table);

    if (table != NULL)
      CSL_sysMemFree(table);

    break;

  case CSL_LDC_CMD_READ_TABLE:

    table = CSL_sysMemAlloc(tableSize);
    if (table == NULL)
      status = CSL_EFAIL;

    if (status == CSL_SOK)
      status = CSL_ldcReadTable(hndl, table);

    if (status == CSL_SOK)
      status = CSL_copyToUser(prm, table, tableSize);

    if (table != NULL)
      CSL_sysMemFree(table);

    break;

  case CSL_LDC_CMD_INT_ENABLE:
    status = CSL_ldcIntEnable(hndl, (Bool32) prm);
    break;

  case CSL_LDC_CMD_INT_CLEAR:
    status = CSL_ldcIntClear(hndl);
    break;

  case CSL_LDC_CMD_INT_WAIT:
    status = CSL_ldcIntWait(hndl);
    break;

  default:
    status = CSL_EFAIL;
    break;
  }

  return status;
}
