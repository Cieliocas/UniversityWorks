cmake_minimum_required(VERSION 3.15)

project(ColoracaoEmGrafos
    VERSION 1.0
    DESCRIPTION "Heuristicas de coloracao de grafos e benchmark"
    LANGUAGES CXX)

# --- Padrao da linguagem ---------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Tipo de build padrao: Release (otimizado) -----------------------------
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# --- Executavel ------------------------------------------------------------
add_executable(coloracao
    src/main.cpp
    src/Graph.cpp
    src/Coloring.cpp)

target_include_directories(coloracao PRIVATE ${CMAKE_SOURCE_DIR}/include)

# --- Avisos de compilacao (GCC/Clang) --------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(coloracao PRIVATE -Wall -Wextra -O2)
endif()

# Mensagem util apos a configuracao.
message(STATUS "Configurado: alvo 'coloracao' (C++${CMAKE_CXX_STANDARD}, ${CMAKE_BUILD_TYPE})")
