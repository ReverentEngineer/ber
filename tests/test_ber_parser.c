#include "ber.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

static const char basic_form[7] = { 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05 };

static const char extended_tag[9] = { 0x1f, 0x81, 0x00,  0x05, 0x01, 0x02, 0x03, 0x04, 0x05 };

static const char indefinite[2] = { 0x30, 0x80 };

static const char extended_length[7] = { 0x30, 0x81, 0xff, 0x01, 0x02, 0x03, 0x04 };

int main() {
  struct ber_parser* parser = NULL;
  FILE* fp = NULL;
  struct ber_element element;
  parser = ber_parser_new();
  assert(parser != NULL);
  
  fp = fmemopen((void*)basic_form, sizeof(basic_form), "rb");
  assert(fp != NULL);
  ber_parser_set_input(parser, fp);
  assert(ber_parser_parse(&element, parser) == 0);
  assert(element.tag_class == BER_TAG_CLASS_UNIVERSAL);
  assert(!element.constructed);
  assert(element.definite);
  assert(element.tag == BER_TAG_OCTET_STRING);
  assert(element.length == 5);
  fclose(fp);

  fp = fmemopen((void*)extended_tag, sizeof(extended_tag), "rb");
  assert(fp != NULL);
  ber_parser_set_input(parser, fp);
  assert(ber_parser_parse(&element, parser) == 0);
  assert(element.tag_class == BER_TAG_CLASS_UNIVERSAL);
  assert(!element.constructed);
  assert(element.definite);
  assert(element.tag == 0x80);
  assert(element.length == 5);
  fclose(fp);

  fp = fmemopen((void*)indefinite, sizeof(indefinite), "rb");
  assert(fp != NULL);
  ber_parser_set_input(parser, fp);
  assert(ber_parser_parse(&element, parser) == 0);
  assert(element.tag_class == BER_TAG_CLASS_UNIVERSAL);
  assert(element.constructed);
  assert(!element.definite);
  assert(element.tag == BER_TAG_SEQUENCE);
  assert(element.length == 0);
  fclose(fp);

  fp = fmemopen((void*)extended_length, sizeof(extended_length), "rb");
  assert(fp != NULL);
  ber_parser_set_input(parser, fp);
  assert(ber_parser_parse(&element, parser) == -1);
  assert(errno == EAGAIN);
  assert(element.tag_class == BER_TAG_CLASS_UNIVERSAL);
  assert(element.constructed);
  assert(element.definite);
  assert(element.tag == BER_TAG_SEQUENCE);
  assert(element.length == 255);
  fclose(fp);

  ber_parser_free(parser);
}
