cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)
project(ltp_predictor)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(../include/)

set(tinyxml_SRC
        tinyxml2.h
        tinyxml2.cpp
        )

set(ltp_lib
        ../include/ltp/ner_dll.h
        ../include/ltp/parser_dll.h
        ../include/ltp/postag_dll.h
        ../include/ltp/segment_dll.h
        ../include/ltp/SRL_DLL.h)

set(gen_feature_lib
        genfeature.h
        genfeature.cpp)

set(predictor_lib predictor.h predictor.cpp)

link_directories(../lib/)
add_library(tinyxml ${tinyxml_SRC})

add_executable(genfeature ${gen_feature_lib})
target_link_libraries(genfeature ner parser postagger segmentor boost_regex srl)

add_executable(predictor ${predictor_lib})
target_link_libraries(predictor tinyxml ner parser postagger segmentor boost_regex srl)

add_executable(compare compare.cpp)
target_link_libraries(compare tinyxml)