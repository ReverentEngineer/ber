if (UNIX)
  find_program(POD2MAN_EXECUTABLE pod2man)

  if (POD2MAN_EXECUTABLE)
    message(STATUS "Found pod2man. Manpages enabled.")
    
    add_custom_command(OUTPUT ber_parser_parse.3 COMMAND ${POD2MAN_EXECUTABLE} --center="Library Functions"
      --release=${PROJECT_VERSION} --name=BER ${CMAKE_CURRENT_SOURCE_DIR}/man3/ber_parser_parse.pod ./ber_parser_parse.3)
  
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ber_parser_parse.3 DESTINATION share/man/man3)

    macro(install_symlink path link)
      install(CODE "execute_process(COMMAND \${CMAKE_COMMAND} -E create_symlink ${path} \${CMAKE_INSTALL_PREFIX}/${link})")
      install(CODE "message(STATUS \"Created symlink: ${link} -> ${path}\")")
    endmacro(install_symlink)

    foreach (LINK ber_parser_new.3 ber_parser_set_input.3 ber_parser_free.3)
        install_symlink(ber_parser_parse.3 share/man/man3/${LINK})
    endforeach()


    add_custom_target(man ALL DEPENDS ber_parser_parse.3)
  endif(POD2MAN_EXECUTABLE)
endif(UNIX)

find_program(SPHINX_EXECUTABLE sphinx-build)
if(SPHINX_EXECUTABLE)
  message(STATUS "Found sphinx-build. Documentation enabled.")

  add_custom_target(docs COMMAND ${SPHINX_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/sphinx html)
endif(SPHINX_EXECUTABLE)
