add_library(ber ber.c)
install(TARGETS ber LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES ber.h DESTINATION include)
