
cmake_minimum_required(VERSION 3.16)

project(MDPIV)

add_executable(MDPIV WIN32
        src/MD_Math.c
        src/OpenGL3.3/src/glad.c
		src/Shader.c
        src/Graphics.cpp
		src/SimpleInput.c
		src/Texture.c
		src/Camera.cpp
        src/Scene.cpp
        src/Window.cpp 
        src/Main.cpp
		MDPIV.rc)

target_link_libraries(MDPIV 
                winmm
                opengl32)

target_include_directories(MDPIV PUBLIC
            ${CMAKE_SOURCE_DIR}/src/OpenGL3.3/include)

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

