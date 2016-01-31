#cmake ver req
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

if(NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Select if you want to build Debug or Release" FORCE )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(SHARED_BUILD TRUE CACHE BOOL "Set this to false if you want a static build")

set(BUILD_TESTS TRUE CACHE BOOL "build the tests")
set(BUILD_EXAMPLES TRUE CACHE BOOL "build the examples")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  #for shared libs 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  #for static libs 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  #for static libs 
 
SET(CMAKE_DEBUG_POSTFIX "-d") 

if(SHARED_BUILD)
    set(SHARED_OR_STATIC SHARED)
else(SHARED_BUILD)
    set(SHARED_OR_STATIC STATIC)
endif(SHARED_BUILD)

set(project_name genes)
project(${project_name} CXX)

#Set source files.
set(source_files
    src/stub.cpp
)

set(header_files
    include/dna/assert.hpp
    include/dna/allele.hpp
    include/dna/allelechromosomegenerator.hpp
    include/dna/gene.hpp
    include/dna/genecodec.hpp
    include/dna/genemanipulator.hpp
    include/dna/makeprotein.hpp
    include/dna/protein.hpp
    include/dna/randomgenegenerator.hpp
    include/dna/chromosome.hpp
)

include_directories(include)

#build
add_library(${project_name} ${SHARED_OR_STATIC} ${header_files} ${source_files})
set_property(TARGET ${project_name} PROPERTY CXX_STANDARD 14)
set_property(TARGET ${project_name} PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(${project_name})
    
#tests
if(BUILD_TESTS)
    include_directories(test)
    add_executable(tests "test/main.cpp"
        "test/alleletest.cpp"
        "test/genecodectest.cpp"
        "test/proteintest.cpp"
        "test/proteinextractortest.cpp"
        "test/randomgenegeneratortest.cpp"
        )

    set_property(TARGET tests PROPERTY CXX_STANDARD 14)
    set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

    target_link_libraries(tests ${project_name})
endif(BUILD_TESTS)

#installation
if(WIN32)
    set(GENES_MISC_DIR .)
    set(WINFIX SYSTEM)
else()
    set(GENES_MISC_DIR share/genes)
endif()

install(
    DIRECTORY include
    DESTINATION .
    )

install(
    TARGETS ${project_name}
    RUNTIME DESTINATION bin COMPONENT bin
    LIBRARY DESTINATION lib COMPONENT bin
    ARCHIVE DESTINATION lib COMPONENT dev
    )

install(
    FILES README.md LICENSE
    DESTINATION ${GENES_MISC_DIR}
    )

