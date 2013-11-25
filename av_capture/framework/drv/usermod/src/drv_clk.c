
#include <drv.h>
#include <drv_csl.h>

int DRV_clkGetClkHz(DRV_ClkHz *pClk, Bool printResults)
{
  CSL_VpssClkDiv       pllClkDiv;
  int status;
  DRV_ClkHz clk;
  float pll1Clk, pll2Clk;

  memset(&clk, 0, sizeof(clk));

  status = CSL_vpssGetClkDiv(&gCSL_vpssHndl, &pllClkDiv);
  if(status==OSA_SOK) {
    #if 0  
    if(printResults) {
      int i;
      
      OSA_printf(" \n");
      OSA_printf(" PLL1 Settings,\n");
      OSA_printf(" PLLM    = 0x%08x\n", pllClkDiv.PLL1_M);
      OSA_printf(" PREDIV  = 0x%08x\n", pllClkDiv.PLL1_PREDIV);
      OSA_printf(" POSTDIV = 0x%08x\n", pllClkDiv.PLL1_POSTDIV);
      for(i=1; i<10; i++)
        OSA_printf(" PLLDIV%d = 0x%08x\n", i, pllClkDiv.PLL1_PLLDIV[i]);                                                            
      OSA_printf(" \n");    
      OSA_printf(" PLL2 Settings,\n");
      OSA_printf(" PLLM    = 0x%08x\n", pllClkDiv.PLL2_M);
      OSA_printf(" PREDIV  = 0x%08x\n", pllClkDiv.PLL2_PREDIV);
      OSA_printf(" POSTDIV = 0x%08x\n", pllClkDiv.PLL2_POSTDIV);
      for(i=1; i<10; i++)
        OSA_printf(" PLLDIV%d = 0x%08x\n", i, pllClkDiv.PLL2_PLLDIV[i]);                                                            
      OSA_printf(" \n");            
      OSA_printf(" PERICLK = 0x%08x\n", pllClkDiv.PERICLKCTL);
      OSA_printf(" VPSSCLK = 0x%08x\n", pllClkDiv.VPSSCLKCTL);                  
    }
    #endif
    
    pll1Clk = ((((float)DRV_REFERENCE_CLK_HZ)*(pllClkDiv.PLL1_M*2))/(pllClkDiv.PLL1_PREDIV+1))/(pllClkDiv.PLL1_POSTDIV+1);
    pll2Clk = ((((float)DRV_REFERENCE_CLK_HZ)*(pllClkDiv.PLL2_M*2))/(pllClkDiv.PLL2_PREDIV+1))/(pllClkDiv.PLL2_POSTDIV+1);    

    clk.imcopClkHz       = pll1Clk/(pllClkDiv.PLL1_PLLDIV[3]+1);    
    clk.vpssClkHz        = pll1Clk/(pllClkDiv.PLL1_PLLDIV[5]+1); 

    if(pllClkDiv.PERICLKCTL & (1<<29))
      clk.armClkHz         = pll2Clk/(pllClkDiv.PLL2_PLLDIV[2]+1);
    else
      clk.armClkHz         = pll1Clk/(pllClkDiv.PLL1_PLLDIV[2]+1);
    
    if(pllClkDiv.PERICLKCTL & (1<<27))    
      clk.ddrClkHz         = pll2Clk/(pllClkDiv.PLL2_PLLDIV[3]+1);   
    else
      clk.ddrClkHz         = pll1Clk/(pllClkDiv.PLL1_PLLDIV[7]+1);   
      
    clk.ddrClkHz /= 2;  
        
    if(printResults) {
      OSA_printf(" \n");
      OSA_printf(" CLK Hz,\n");
      OSA_printf(" ARM   Hz = %10d\n", clk.armClkHz);
      OSA_printf(" DDR   Hz = %10d\n", clk.ddrClkHz);
      OSA_printf(" VPSS  Hz = %10d\n", clk.vpssClkHz);
      OSA_printf(" IMCOP Hz = %10d\n", clk.imcopClkHz);
      OSA_printf(" \n");      
    }
  }
  if(pClk!=NULL) {
    memcpy(pClk, &clk, sizeof(clk));
  }
  
  return status;
}
