cmake_minimum_required(VERSION 3.15)

set(BINARY casper_cpp_sdk_tests)

include_directories(
    ${SDK_ROOT}/lib/cryptopp-pem
)

add_executable(${BINARY}
    ClientTest.cpp
    RpcTest.cpp
    CLValueByteSerializerTest.cpp
    DeployItemByteSerializerTest.cpp
)

target_link_libraries(${BINARY} 
    PRIVATE
    spdlog::spdlog
    ${LIB_NAME}
    ${CRYPTOPP_LIBRARIES})

add_test(NAME ${BINARY} COMMAND ./${BINARY})

set(DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

set(PEM_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/data/cpp_sdk_test_secret_key.pem)
set(PEM_FILE
        ${CMAKE_CURRENT_SOURCE_DIR}/data/Piotrek1_secret_key.pem)

file(COPY ${PEM_FILE} DESTINATION ${DESTINATION})
