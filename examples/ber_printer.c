#include "ber.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static char* bytes_to_hex(char* data, size_t size) {
  char* result = malloc((size * 2) + 1);
  for (int i = 0; i < size; i++) {
    sprintf(result + (i*2), "%02x", data[i]);
  }
  return result;
}

static const char* tag_class_text[4] = {
  "Universal",
  "Application",
  "Context-Specific",
  "Private"
};

static const char* constructed_text[2] = {
  "Primitive",
  "Constructed"
};


int main(int argc, char* argv[]) {
  struct ber_parser* parser = NULL;
  FILE* input = NULL;
  struct ber_element element;
  int res = 0;
  char* hex = NULL;
  if (argc > 2) {
    printf("usage: ber_printer [<file>]\n");
    return EXIT_FAILURE;
  }

  if (argc == 2) {
    input = fopen(argv[1], "rb");
  } else {
    input = stdin;
  }

  parser = ber_parser_new();
  ber_parser_set_input(parser, input);

  while ((res = ber_parser_parse(&element, parser)) == 0 || errno == EAGAIN) {
    if (res == 0) {
      printf("Class: %s\n", tag_class_text[element.tag_class]);
      printf("P/C: %s\n", constructed_text[element.constructed]);
      printf("Tag: %zx\n", element.tag);
      if (element.length > 0) {
        hex = bytes_to_hex(element.value, element.length);
        printf("Value: 0x%s\n", hex);
        free(hex);
      }
      printf("\n");
    }
  }

  ber_parser_free(parser);

  fclose(input);

  return EXIT_SUCCESS;
}
