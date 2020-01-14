# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

add_executable(tcbinfo_test tcbinfoparser.cpp)
add_executable(sgxextensions_test parcesgxextensions.cpp)
add_executable(qeidentity_test qeidentityparser.cpp)
add_executable(remote_verify remote_verify.cpp)

if(USE_LIBSGX)
    target_compile_definitions(tcbinfo_test PRIVATE OE_USE_LIBSGX )
	target_compile_definitions(sgxextensions_test PRIVATE OE_USE_LIBSGX )
    target_compile_definitions(qeidentity_test PRIVATE OE_USE_LIBSGX )
    target_compile_definitions(remote_verify PRIVATE OE_USE_LIBSGX )
endif()

add_custom_command( TARGET tcbinfo_test
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/seeds/tcbinfoparser ${CMAKE_CURRENT_BINARY_DIR}/seeds/tcbinfoparser )
add_custom_command( TARGET sgxextensions_test
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/seeds/parcesgxextensions ${CMAKE_CURRENT_BINARY_DIR}/seeds/parcesgxextensions )
add_custom_command( TARGET qeidentity_test
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/seeds/qeidentity ${CMAKE_CURRENT_BINARY_DIR}/seeds/qeidentity )
add_custom_command( TARGET remote_verify
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/seeds/remoteverify ${CMAKE_CURRENT_BINARY_DIR}/seeds/remote_verify )

target_link_libraries(tcbinfo_test PRIVATE oehost oe_includes)
target_link_libraries(sgxextensions_test PRIVATE oehostapp )
target_link_libraries(qeidentity_test PRIVATE oehost oe_includes)
target_link_libraries(remote_verify PRIVATE oehost oehostapp oe_includes)

add_test(NAME tests/tcbinfo_test COMMAND ./tcbinfo_test ${CMAKE_CURRENT_BINARY_DIR}/seeds/tcbinfoparser/tcbInfo.json)
add_test(NAME tests/sgxextensions_test COMMAND ./sgxextensions_test ${CMAKE_CURRENT_BINARY_DIR}/seeds/parcesgxextensions/generate_report.txt)
add_test(NAME tests/qeidentity_test COMMAND ./qeidentity_test ${CMAKE_CURRENT_BINARY_DIR}/seeds/qeidentity/qeidentity.json)
add_test(NAME tests/remote_verify COMMAND ./remote_verify ${CMAKE_CURRENT_BINARY_DIR}/seeds/remoteverify/remoteverify.bytes)
