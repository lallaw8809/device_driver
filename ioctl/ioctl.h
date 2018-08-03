
#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H

/* _IO, _IOW, _IOR, _IORW are helper macros to create a unique ioctl identifier and 
   add the required R/W needed features (direction).
   These can take the following params: magic number, the command id, and the data
   type that will be passed (if any)
*/
#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)

#endif


