Basic Encoding Rules (BER)
============================

Welcome to the BER library documentation. This is a simple library for parsing
data encoed using the Basic Encoding Rules (BER) of ASN.1. Since the Distinguished
Encoding Rules (DER) is a restricted subset of BER, it should also work on
DER-encoded data.

Example
--------------

This is a basic example of how to use it:

.. code-block:: c

  FILE* fp = fopen("myfile.ber", "rb");
  struct ber_element element;
  struct ber_event event;
  struct ber_parser* parser = ber_parser_new();
  
  while (ber_parser_parser(&event, &element, parser) == 0 && event != BER_NO_EVENT) {
    printf("Tag: %d\n", element.tag);
  
    printf("Data: ");
    for (int i = 0; i < element.length; i++) {
      printf("%02x", element.data[i]);
    }
    printf("\n");
  }



API
--------------
.. c:enum:: ber_event

   .. c:enumerator:: \
      BER_NO_EVENT
      BER_PRIMITIVE_EVENT
      BER_CONSTRUCTED_EVENT
      BER_EOC_EVENT

.. c:enum:: ber_tag_class

   .. c:enumerator:: \
      BER_TAG_CLASS_UNIVERSAL
      BER_TAG_CLASS_APPLICATION
      BER_TAG_CLASS_CONTEXT_SPECIFIC
      BER_TAG_CLASS_PRIVATE


.. c:struct:: ber_element

   .. c:var:: enum tag_class tag_class
      
      The class of the tag
  
   .. c:var:: int constructed
   
      Whether the element is constructed

   .. c:var:: size_t tag

      The tag of the element

   .. c:var:: int definite

      Whether the element has a definite (has a pre-defined length)

   .. c:var:: size_t length

      The length of the value

   .. c:var:: char* value

      The value encoded


.. c:struct:: ber_parser

   A BER parser object that holds the state of the parsing.

.. c:function:: const char* ber_get_version()
  
    Gets the version string

    :returns: A pointer to the static version string

.. c:function:: struct ber_parser* ber_parser_new()

    Creates a new BER parser.

    :returns: An instance of :c:struct:`ber_parser`

.. c:function:: int ber_parser_parse(enum ber_event *event, ber_element *element, struct ber_parser* parser)

   :returns: 0 on success, or -1 on error

   .. warning::
   
      This is not thread-safe.

.. c:function:: void ber_parser_free(struct ber_parser* parser)

   Frees the parsero bject
