#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

#include "XmlType.h"
#include "Ucst/CstType.h"
#include "Ugio/GioType.h"


#ifdef __cplusplus
extern "C" {
#endif


//xml_type * XML_parse_from_file( char *filename);
//xml_type * XML_parse_from_data( char *stream, int length);
xml_type * XML_parse_from_cst(cst_type * cst);
xml_type * XML_parse_from_gio(gio_type * gio);



int S0(gio_type *gio, char* buffer, xmlTag_type *t);

// normal tags states
int P1(gio_type *gio, char* buffer, xmlTag_type *t);
int P2(gio_type *gio, char* buffer, xmlTag_type *t);
int P3(gio_type *gio, char* buffer, xmlTag_type *t);
int P4(gio_type *gio, char* buffer, xmlTag_type *t);
int P5(gio_type *gio, char* buffer, xmlTag_type *t);
int P6(gio_type *gio, char* buffer, xmlTag_type *t);
int P7(gio_type *gio, char* buffer, xmlTag_type *t);
int P8(gio_type *gio, char* buffer, xmlTag_type *t);
int P9(gio_type *gio, char* buffer, xmlTag_type *t);
int P10(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att);
int P11(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att);
int P12(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att, int seperator);
int P13(gio_type *gio, char* buffer, xmlTag_type *t);


//header tags
int H1(gio_type *gio, char* buffer, xmlTag_type *t, int delimiter);

// comment state
int C1(gio_type *gio, char *buffer, xmlTag_type *t);

int Q0(gio_type *gio, char* buffer, xmlTag_type *t, int err);
int Q1(gio_type *gio, char* buffer, xmlTag_type* t, int stat);
int Q3(gio_type *gio, char* buffer, xmlTag_type *t, xmlAtt_type *att, int err);


#ifdef __cplusplus
}
#endif

#endif //__XMLPARSER_H__
