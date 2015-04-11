#cmake ver req
cmake_minimum_required(VERSION 2.8.3)

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
project(${project_name})

#Set source files.
set(source_files
    src/breeder.cpp
    src/genecodec.cpp
    src/chromosome.cpp
    src/cistron.cpp
    src/randomgene.cpp
)

set(header_files
    include/dna/assert.hpp
    include/dna/breeder.hpp
    include/dna/chromosome.hpp
    include/dna/cistron.hpp
    include/dna/gene.hpp
    include/dna/genecodec.hpp
    include/dna/genetable.hpp
    include/dna/randomgene.hpp
)

include_directories(include)

#C++11 required
if(NOT MSVC)
    set(CPP_11_FLAG -std=c++11)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPP_11_FLAG}")

#build
add_library(${project_name} ${SHARED_OR_STATIC} ${header_files} ${source_files})
target_link_libraries(${project_name})
    
#tests
if(BUILD_TESTS)
    include_directories(test)
    add_executable(tests "test/main.cpp"
        "test/genecodectest.cpp"
        "test/cistrontest.cpp"
        "test/chromosometest.cpp"
        )

    target_link_libraries(tests ${project_name})
endif(BUILD_TESTS)

#examples
if(BUILD_EXAMPLES)
    find_package(Featherkit COMPONENTS ui rendering sdl2 structure REQUIRED)
    if(${FEATHERKIT_FOUND})
        include_directories(${FEATHERKIT_INCLUDE_DIRS})
    endif()
    #find_package(OpenGL REQUIRED)
    #if(${OPENGL_FOUND})
    #    include_directories(${OPENGL_INCLUDE_DIRS})
    #endif()
    #find_package(SDL2 REQUIRED)
    #if(SDL2_FOUND)
    #    include_directories(${SDL2_INCLUDE_DIR})
    #endif()

    set(cells_src "examples/cells/src")
    add_executable(cells
        ${cells_src}/main.cpp
        ${cells_src}/cells.cpp
        ${cells_src}/cell.cpp
        ${cells_src}/lodepng.cpp
        )

    target_link_libraries(cells ${project_name} ${FEATHERKIT_LIBRARIES})
endif()

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

