/**
 * @file ber.h
 *
 * @brief Public interface for libber
 */

#ifndef __BER_H__
#define __BER_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdio.h>

/** BER parser */
struct ber_parser;

/** Tag class values */
enum ber_tag_class {
  BER_TAG_CLASS_UNIVERSAL,
  BER_TAG_CLASS_APPLICATION,
  BER_TAG_CLASS_CONTEXT_SPECIFIC,
  BER_TAG_CLASS_PRIVATE
};

/** ASN.1 native tags */
#define BER_TAG_EOC               0
#define BER_TAG_BOOLEAN           1
#define BER_TAG_INTEGER           2
#define BER_TAG_BIT_STRING        3
#define BER_TAG_OCTET_STRING      4
#define BER_TAG_NULL              5
#define BER_TAG_OBJECT_IDENTIFER  6
#define BER_TAG_OBJECT_DESCRIPTOR 7
#define BER_TAG_EXTERNAL          8
#define BER_TAG_REAL              9
#define BER_TAG_ENUMERATED       10
#define BER_TAG_EMBEDDED_PDV     11
#define BER_TAG_UTF8STRING       12
#define BER_TAG_RELATIVE_OID     13
#define BER_TAG_TIME             14
// Left blank for reserved
#define BER_TAG_SEQUENCE         16
#define BER_TAG_SET              17
#define BER_TAG_NUMERIC_STRING   18
#define BER_TAG_PRINTABLE_STRING 19
#define BER_TAG_T61_STRING       20
#define BER_TAG_VIDEOTEX_STRING  21
#define BER_TAG_IA5_STRING       22
#define BER_TAG_UTCTIME          23
#define BER_TAG_GENERALIZED_TIME 24
#define BER_TAG_GRAPHIC_STRING   25
#define BER_TAG_VISIBLE_STRING   26
#define BER_TAG_GENERAL_STRING   27
#define BER_TAG_UNIVERSAL_STRING 28
#define BER_TAG_CHARACTER_STRING 29
#define BER_TAG_BMP_STRING       30
#define BER_TAG_DATE             31
#define BER_TAG_TIME_OF_DATA     32
#define BER_TAG_DATETIME         33
#define BER_TAG_DURATION         34
#define BER_TAG_OID_IRI          35
#define BER_TAG_RELATIVE_OID_IRI 26

/** BER element */
struct ber_element {
    
    /** Tag class */
    enum ber_tag_class tag_class;

    /** Primitive/constructed */
    int constructed;

    /** Tag */
    size_t tag;

    /** Definite Form (has a defined length ) */
    int definite;

    /** Length of value */
    size_t length;

    /** Value */
    char* value; 

};

/**
 * @brief Allocates a new parser
 *
 * @return A parser object
 */
struct ber_parser* ber_parser_new(void);

/**
 * @brief Sets the input of the paser
 *
 * @param[in,out] parser Parser object
 * @param[in] file Input file
 */
void ber_parser_set_input(struct ber_parser* parser, FILE* file);

/**
 * @brief Parse the input stream
 *
 * @param event[out] event An event object to be populated
 * @param parser[in,out] parser Parser to use
 *
 * @retval  0 success
 * @retval -1 error
 */
int ber_parser_parse(struct ber_element* element, struct ber_parser* parser);

/**
 * @brief Frees the parser
 *
 * @param [in,out] parser Parse to free
 */
void ber_parser_free(struct ber_parser* parser);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BER_H__ */
