set( GENES_FOUND false)

find_path(
    GENES_INCLUDE_DIR
    dna/assert.hpp
    PATH_SUFFIXES
    include
    PATHS
    ${GENES_ROOT}
    ${GENESDIR}
    $ENV{GENESDIR}
    $ENV{GENES_ROOT}
    /usr
    /usr/local
    ~/Library/Frameworks
    /Library/Frameworks
    /sw
    /opt/local
    /opt/csw
    /opt
    "C:/Program Files (x86)"
    "C:/Program Files (x86)/genes"
)

set(FIND_Genes_LIB_PATHS
    ${GENES_ROOT}
    ${GENESDIR}
    $ENV{GENESDIR}
    $ENV{GENES_ROOT}
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt
    "C:/Program Files (x86)"
    "C:/Program Files (x86)/genes"
)

#list(FIND Genes_FIND_COMPONENTS "rendering" Genes_NEEDS_GLM)

#if(Genes_NEEDS_GLM EQUAL -1)
#    message("i don't need glm!")
#else()
#    message("i need glm!")
#endif()

set(GENES_FOUND)
set(GENES_LIBRARY)
set(GENES_INCLUDE_DIR)

set(GENES_PRESERVE_LIBS false CACHE "" INTERNAL)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(GENES_LIBRARY_SUFFIX "-d")
    set(GENES_LIBRARY_ANTISUFFIX "")
    set(BUILD_TYPE "debug")
    set(ANTI_BUILD_TYPE "release")
else()
    set(GENES_LIBRARY_SUFFIX "")
    set(GENES_LIBRARY_ANTISUFFIX "-d")
    set(BUILD_TYPE "release")
    set(ANTI_BUILD_TYPE "debug")
endif()

if(NOT (${GENES_LAST_MODE} MATCHES ${BUILD_TYPE}))
    unset(GENES_LAST_MODE CACHE)
    set(GENES_LAST_MODE ${BUILD_TYPE} CACHE "" INTERNAL)
    set(GENES_RESET_LIBS true)
else()
    set(GENES_RESET_LIBS false)
endif()

set(FIND_Genes_NAME genes)

if(${GENES_RESET_LIBS})
    unset(Genes_LIBRARY CACHE)
endif()

find_library(Genes_LIBRARY
    NAMES ${FIND_Genes_NAME}
    PATH_SUFFIXES lib64 lib
    PATHS ${FIND_Genes_LIB_PATHS})

#second pass with anti suffix
if(NOT Genes_LIBRARY)
    #let the user know
    if(NOT Genes_FIND_QUIETLY)
        message(WARNING "Failed to find ${BUILD_TYPE} version of Genes. Will try to find ${ANTI_BUILD_TYPE} version instead.")
    endif()

    if(${GENES_RESET_LIBS})
        unset(Genes_LIBRARY CACHE)
    endif()

    find_library(Genes_LIBRARY
        NAMES ${FIND_Genes_COMPONENT_NAME}${GENES_LIBRARY_ANTISUFFIX}
        PATH_SUFFIXES lib64 lib
        PATHS ${FIND_Genes_LIB_PATHS})
endif()

if(NOT Genes_LIBRARY)
    set(FIND_Genes_MISSING "${FIND_Genes_MISSING} Genes_LIBRARY")
else()
    set(GENES_LIBRARY ${GENES_LIBRARY} ${Genes_LIBRARY})
endif()

find_library(
    GENES_LIBRARY
    genes
    PATH_SUFFIXES
    lib
    lib64
    PATHS
    /usr
    /usr/local
    ${GENES_ROOT}
    ${GENESDIR}
    $ENV{GENESDIR}
    $ENV{GENES_ROOT}
)

if(NOT GENES_INCLUDE_DIR OR FIND_Genes_MISSING)
    set(GENES_FOUND)
    set(GENES_INCLUDE_DIRS)
    set(GENES_LIBRARIES)
    if(Genes_FIND_REQUIRED)
        message(FATAL_ERROR "Genes not found.")
    elseif(NOT Genes_FIND_QUIETLY)
        message(STATUS "Genes not found.")
    endif()
else()
    set(GENES_FOUND true)
    if(NOT Genes_FIND_QUIETLY)
        message(STATUS "Genes found: ${GENES_INCLUDE_DIR}")
    endif()
    set(GENES_INCLUDE_DIRS ${GENES_INCLUDE_DIR})
    set(GENES_LIBRARIES ${GENES_LIBRARY})
endif()
