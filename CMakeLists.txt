cmake_minimum_required(VERSION 3.16)

project(MDPIV)

add_executable(MDPIV 
        resources.rc
        src/glad/src/glad.c
        src/LoadImage.c
        src/Window.cpp
        src/OGL/Renderer_GL.cpp
        src/D3D9/Renderer_D3D9.cpp
        src/Main.cpp)

target_compile_options(MDPIV PRIVATE -O3)

include_directories(${CMAKE_SOURCE_DIR}/src/glfw/include
                        ${CMAKE_SOURCE_DIR}/src/glad/include)

target_link_libraries(MDPIV 
            ${CMAKE_SOURCE_DIR}/src/glfw/libglfw3.a
            opengl32
            d3d9)

function(copy_dll_to_install target dst)
    set(SRC $<TARGET_FILE_DIR:${target}>/${target}.dll)
    set(DST ${CMAKE_SOURCE_DIR}/${dst}.dll)
    add_custom_command(TARGET ${target} POST_BUILD VERBATIM
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${target} output file \"${SRC}\" to \"${DST}\""
            COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
endfunction()
        
function(copy_exe_to_install target dst)
    set(SRC $<TARGET_FILE_DIR:${target}>/${target}.exe)
    set(DST ${CMAKE_SOURCE_DIR}/${dst}.exe)
    add_custom_command(TARGET ${target} POST_BUILD VERBATIM
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${target} output file \"${SRC}\" to \"${DST}\""
            COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST})
endfunction()  

copy_exe_to_install(MDPIV MDPIV)