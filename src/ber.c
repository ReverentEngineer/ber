#include "ber.h"

#include <errno.h>
#include <stdlib.h>

#define IS_MAX_TAG(x) ((size_t) 0x7f << ((sizeof(size_t) * 8) - 7) & x)

enum parser_state {
  PARSE_TAG,
  PARSE_EXTENDED_TAG,
  PARSE_LENGTH,
  PARSE_EXTENDED_LENGTH,
  PARSE_VALUE
};

struct ber_parser {
  struct {
    char* data;
    size_t index;
  } buffer;
  struct {
    uint8_t index;
    uint8_t length;
  } extended_length;

  FILE* fp;
  enum parser_state state;
};

struct ber_parser* ber_parser_new(void) {
  struct ber_parser* parser = calloc(1, sizeof(struct ber_parser));
  parser->state = PARSE_TAG;
  return parser;
}

void ber_parser_set_input(struct ber_parser* parser, FILE* file) {
  parser->fp = file;
}

int ber_parser_parse(struct ber_element* element, struct ber_parser* parser) {
  uint8_t buffer = 0;
  int rc = -1;
  int keep_parsing = 1;
  do {
    switch (parser->state) {
      case PARSE_TAG:
        // Read the tag (or beginning if extended tag)
        if (fread(&buffer, 1, 1, parser->fp) == 1) {
          element->tag_class = (buffer & 0xc0) >> 6; 
          element->constructed = buffer & 0x20 ? 1 : 0;
          if ((buffer & 0x1f) != 0x1f) {
            element->tag = buffer & 0x1f;
            parser->state = PARSE_LENGTH;
          } else {
            element->tag = 0;
            parser->state = PARSE_EXTENDED_TAG;
          }
        } else {
          keep_parsing = 0;
        }
        break;
      case PARSE_EXTENDED_TAG:
        // Check if we're maxing out on tag length
        if (!IS_MAX_TAG(element->tag)) {
          if (fread(&buffer, 1, 1, parser->fp) == 1) {
            element->tag = element->tag << 7;
            element->tag |= (buffer & 0x7f);
            if (!(buffer & 0x80)) {
              parser->state = PARSE_LENGTH;
            } 
          } else {
            keep_parsing = 0;
          } 
        } else {
            rc = -1;
            keep_parsing = -1;
        }      
        break;
      case PARSE_LENGTH:
        // Read the length (or beginning if extended)
        if (fread(&buffer, 1, 1, parser->fp) == 1) {
  
          if (buffer & 0x80 && !(buffer & 0x7f)) {
            element->definite = 0;
            element->length = 0;
            parser->state = PARSE_TAG;
            keep_parsing = 0;
            rc = 0;
          } else if (!(buffer & 0x80)) {
            element->length = buffer & 0x7f;
            element->definite = 1;
            parser->state = PARSE_VALUE;
            parser->buffer.index = 0;
          } else if (buffer & 0x80 && (buffer & 0x7f)) {
            element->definite = 1;
            parser->state = PARSE_EXTENDED_LENGTH;
            // If length of length is greated than uint64_t, fail.
            parser->extended_length.index = 0;
            parser->extended_length.length = (buffer & 0x7f);
            if (parser->extended_length.length > 8) {
              rc = -1;
              keep_parsing = 0;
            }
          } else {
            rc = -1;
            keep_parsing = 0;
          }
        }
        break;
      case PARSE_EXTENDED_LENGTH:
        if (parser->extended_length.index < parser->extended_length.length) {
          if (fread(&buffer, 1, 1, parser->fp) == 1) {
            element->length = element->length << 8;
            element->length |= buffer;
            parser->extended_length.index++;
          } else if(!ferror(parser->fp)) {
            rc = -1;
            errno = EAGAIN;
            keep_parsing = 0;
          } else {
            rc = -1;
            keep_parsing = 0;
          }
        } else {
          parser->state = PARSE_VALUE;
        }
        break;
      case PARSE_VALUE:
        parser->buffer.data = realloc(parser->buffer.data, element->length);
        size_t read = fread(parser->buffer.data + parser->buffer.index, 1, element->length, parser->fp);
        parser->buffer.index += read;
        if (parser->buffer.index == element->length) {
          parser->state = PARSE_TAG;
          keep_parsing= 0;
          rc = 0;
          element->value = parser->buffer.data;
        } else if (!ferror(parser->fp)) {
          parser->buffer.index = read;
          rc = -1;
          errno = EAGAIN;
        } else {
          rc = -1;
        }
        keep_parsing = 0;
        break;
    } 
  } while(keep_parsing);

  return rc;
}

void ber_parser_free(struct ber_parser* parser) {
  if (parser) {
    free(parser);
  }
}
