/*******************************************
*  modified by Jung Dae Ho
*******************************************/

#ifndef __DEF_H__
#define __DEF_H__

#define U32 unsigned int
#define U16 unsigned short
#define S32 int
#define S16 short int
#define U8  unsigned char
#define	S8  char

#define TRUE 	1   
#define FALSE 	0

#define ADDR32(A)			(*((volatile U32 *)(A)))
#define ADDR16(A)			(*((volatile U16 *)(A)))
#define ADDR8(A)			(*((volatile U8 *)(A)))


#ifndef __cplusplus
typedef int				bool;
#define	true			1
#define false			0
#endif


typedef enum {
	VAR_LONG=32,
	VAR_SHORT=16,
	VAR_CHAR=8
} VAR_TYPE;

#endif /*__DEF_H__*/

