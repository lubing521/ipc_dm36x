

#ifndef _DEV_DEBUG_H_
#define _DEV_DEBUG_H_

#ifdef DEBUG
#define FUNCDBG(format, a...) \
        printk( KERN_INFO "DRV :%s:%d: " format, __func__, __LINE__, ## a )
#else
#define FUNCDBG(...)
#endif

#define FUNCMSG(format, a...) \
        printk( KERN_INFO "DRV :%s:%d: " format, __func__, __LINE__, ## a )

#define FUNCERR(format, a...) \
    printk( KERN_ERR " DRV ERR :%s:%d: " format, __func__, __LINE__, ## a)


#endif /* _DEV_DEBUG_H_  */

