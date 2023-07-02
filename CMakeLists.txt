cmake_minimum_required(VERSION 3.24)

project(duplicate)

set (CMAKE_CXX_STANDARD 17)
set(OPENSSL_USE_STATIC_LIBS TRUE)

find_package(OpenSSL REQUIRED)

add_executable(duplicator
	main.cpp
	searchHandler.cpp
)

target_include_directories(duplicator PRIVATE . ./cxxopts/include)
target_link_libraries(duplicator OpenSSL::Crypto)