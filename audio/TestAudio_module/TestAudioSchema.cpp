/************************************************
 *
 *	Autogenerated schema file
 *
 ***********************************************/

// For Windows we need to use the MFC heap
#ifdef WIN32
#include "stdafx.h"
#endif

// Include required headers
#include "Framework.h"

#ifdef __cplusplus
extern "C" {
#endif

// Schema string
static const unsigned char s_schema[] =
{
	0xdd, 0xdd, 0xcb, 0xc5, 0xde, 0xcc, 0xf1, 0xd1, 0xd2, 0xc6, 0xf8, 0xd5, 0xe0, 0xfa, 0xff, 0x80, 
	0xb3, 0xd7, 0xec, 0xe6, 0xe0, 0xf6, 0xf2, 0x81, 0xe3, 0xcc, 0x82, 0x86, 0xed, 0xcb, 0xd9, 0xcf, 
	0xdd, 0xdd, 0xcb, 0xc5, 0xde, 0xcc, 0x0d, 0x0a, 0xd8, 0x0d, 0x0a, 0x85, 0x94, 0x81, 0x92, 0xcf, 
	0xc1, 0xe8, 0xfa, 0xf4, 0xe3, 0xf7, 0xdd, 0xc2, 0xf1, 0xd6, 0xc0, 0x94, 0x93, 0x80, 0xa9, 0x8e, 
	0x86, 0x8f, 0x8a, 0x8a, 0x8a, 0xb0, 0x92, 0xc6, 0xde, 0xc6, 0x89, 0x85, 0x94, 0x81, 0x92, 0x99, 
	0x80, 0xa4, 0x0d, 0x0a, 0xa9, 0x8e, 0x86, 0x8f, 0xcb, 0xc6, 0xcd, 0xff, 0xe1, 0xdb, 0xd1, 0xc6, 
	0xcc, 0x85, 0x94, 0x81, 0x92, 0x99, 0x80, 0xa4, 0xb3, 0xb0, 0x80, 0xa3, 0x8f, 0x8e, 0x83, 0x85, 
	0xd9, 0xda, 0xc7, 0x80, 0xa9, 0x8e, 0x86, 0x8f, 0x8a, 0x8a, 0x8a, 0x0d, 0x0a, 0xa4, 0xb3, 0xb0, 
	0x80, 0xe4, 0xce, 0xc7, 0xcd, 0x85, 0x90, 0x94, 0x93, 0x80, 0xa9, 0x8e, 0x86, 0x8f, 0x8a, 0x8a, 
	0x8a, 0xb0, 0x92, 0x8f, 0x90, 0x92, 0x89, 0x85, 0x94, 0xc8, 0xdc, 0xcd, 0x80, 0xa4, 0xb3, 0xb0, 
	0x80, 0xa3, 0x8f, 0x8e, 0x0d, 0x0a, 0x8a, 0xb0, 0x92, 0x8f, 0xe3, 0xe6, 0xe8, 0xf1, 0xf1, 0x81, 
	0x92, 0x99, 0x80, 0xa4, 0xb3, 0xb0, 0x80, 0xa3, 0x8f, 0x8e, 0x83, 0x85, 0x90, 0x94, 0x93, 0x80, 
	0xa9, 0x8e, 0xcf, 0xc1, 0xde, 0x8a, 0x8a, 0xb0, 0x92, 0x8f, 0x90, 0x92, 0x89, 0x0d, 0x0a, 0x8e, 
	0x83, 0x85, 0x90, 0xd8, 0xdc, 0xc7, 0xca, 0xfa, 0xf4, 0xe3, 0x8a, 0x8a, 0x8a, 0xb0, 0x92, 0x8f, 
	0x90, 0x92, 0x89, 0x85, 0x94, 0x81, 0x92, 0x99, 0x80, 0xa4, 0xb3, 0xf9, 0xce, 0xf7, 0x8f, 0x8e, 
	0x83, 0x85, 0x90, 0x94, 0x93, 0x80, 0x0d, 0x0a, 0x89, 0x85, 0x94, 0x81, 0xf1, 0xf6, 0xee, 0xd0, 
	0xc1, 0xdf, 0xec, 0xdc, 0xfd, 0xeb, 0xf2, 0xf0, 0xf5, 0xe7, 0xe7, 0x80, 0xa9, 0x8e, 0x86, 0x8f, 
	0x8a, 0x8a, 0x8a, 0xb0, 0x98, 0xda, 0xd9, 0xdc, 0xdd, 0x85, 0x94, 0x81, 0x92, 0x99, 0x80, 0x0d, 
	0x0a, 0x80, 0xa9, 0x8e, 0x86, 0xec, 0xe5, 0xe4, 0xfe, 0xc2, 0xfd, 0xe3, 0xef, 0xe0, 0xec, 0xf6, 
	0xe4, 0xee, 0xfc, 0xea, 0xe5, 0xa4, 0xb3, 0xb0, 0x80, 0xa3, 0x8f, 0x8e, 0x83, 0x8f, 0xc5, 0xdd, 
	0xdd, 0xd4, 0xa9, 0x8e, 0x86, 0x8f, 0x8a, 0x8a, 0x0d, 0x0a, 0x80, 0xa4, 0xb3, 0xb0, 0xc3, 0xec, 
	0xca, 0xc8, 0xc5, 0x85, 0x90, 0x94, 0x93, 0x80, 0xa9, 0x8e, 0x86, 0x8f, 0x8a, 0x8a, 0x8a, 0xb0, 
	0x92, 0x8f, 0x90, 0x92, 0x89, 0x85, 0x9e, 0xc7, 0xde, 0xd6, 0xc1, 0xf0, 0xb3, 0xb0, 0x80, 0xa3, 
	0x8f, 0x0d, 0x0a, 0x8a, 0x8a, 0xb0, 0x92, 0xdf, 0xf8, 0xd3, 0xc7, 0xc1, 0xd8, 0xc4, 0x92, 0x99, 
	0x80, 0xa4, 0xb3, 0xb0, 0x80, 0xa3, 0x8f, 0x8e, 0x83, 0x85, 0x90, 0x94, 0x93, 0x80, 0xa9, 0xc7, 
	0xc8, 0xdb, 0x8a, 0x8a, 0x8a, 0xb0, 0x92, 0x8f, 0x90, 0x92, 0x0d, 0x0a, 0xd2, 0x0d, 0x0a, 
	0x00
};

const char *FIXNAME(GetSchema)()
{
	return (const char *)s_schema;
}

#ifdef __cplusplus
}
#endif


/* End of file. */
