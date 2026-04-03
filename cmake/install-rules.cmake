install(
    TARGETS tcp_socket_exe
    RUNTIME COMPONENT tcp_socket_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
