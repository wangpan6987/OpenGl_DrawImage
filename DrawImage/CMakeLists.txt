# CMakeList.txt: DrawImage 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(GL_Dir ${CMAKE_SOURCE_DIR}/../../ThirdPart)

set(SRC 
	DrawImage.cpp
)

set(HRC
	stb_image.h
	Shader.h
)

project(DrawImage)

# 将源代码添加到此项目的可执行文件。
add_executable (DrawImage ${HRC} ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DrawImage PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
set(GL_INC
	${GL_Dir}/glew-2.2.0/include
	${GL_Dir}/freeglut_build/install/include
	${GL_Dir}/glfw-3.4.bin.WIN64/include
)

include_directories(${GL_INC})

if(MSVC_VERSION EQUAL 1800)
  # Visual Studio 2013
  set(LIBRARY_PATH "${GL_Dir}/glfw-3.4.bin.WIN64/lib-vc2013")
elseif(MSVC_VERSION EQUAL 1900)
  # Visual Studio 2015
  set(LIBRARY_PATH "${GL_Dir}/glfw-3.4.bin.WIN64/lib-vc2015")
elseif(MSVC_VERSION GREATER_EQUAL 1910 AND MSVC_VERSION LESS 1920)
  # Visual Studio 2017
  set(LIBRARY_PATH "${GL_Dir}/glfw-3.4.bin.WIN64/lib-vc2017")
elseif(MSVC_VERSION GREATER_EQUAL 1920 AND MSVC_VERSION LESS 1930)
  # Visual Studio 2019
  set(LIBRARY_PATH "${GL_Dir}/glfw-3.4.bin.WIN64/lib-vc2019")
elseif(MSVC_VERSION GREATER_EQUAL 1930)
  # Visual Studio 2022
  set(LIBRARY_PATH "${GL_Dir}/glfw-3.4.bin.WIN64/lib-vc2022")
else()
  message(FATAL_ERROR "不支持的Visual Studio版本。")
endif()

target_link_libraries(DrawImage
	PRIVATE
		$<$<CONFIG:Debug>:${GL_Dir}/freeglut_build/install/lib/freeglutd.lib>
		$<$<CONFIG:Release>:${GL_Dir}/freeglut_build/install/lib/freeglut.lib>
		${LIBRARY_PATH}/glfw3.lib
		${GL_Dir}/glew-2.2.0/lib/Release/x64/glew32.lib
		opengl32.lib
)

# Debug版本
set(DLLS_DEBUG
    ${GL_Dir}/glew-2.2.0/bin/Release/x64/glew32.dll
    ${GL_Dir}/freeglut_build/install/bin/freeglutd.dll
)

# Release版本
set(DLLS_RELEASE
    ${GL_Dir}/glew-2.2.0/bin/Release/x64/glew32.dll
    ${GL_Dir}/freeglut_build/install/bin/freeglut.dll
)

add_custom_command (TARGET DrawImage POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${GL_Dir}/glew-2.2.0/bin/Release/x64/glew32.dll
		$<TARGET_FILE_DIR:DrawImage>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<$<CONFIG:Debug>:${GL_Dir}/freeglut_build/install/bin/freeglutd.dll>
		$<$<CONFIG:Release>:${GL_Dir}/freeglut_build/install/bin/freeglut.dll>
		$<TARGET_FILE_DIR:DrawImage>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${PROJECT_SOURCE_DIR}/shader_f.txt
		$<TARGET_FILE_DIR:DrawImage>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${PROJECT_SOURCE_DIR}/shader_v.txt
		$<TARGET_FILE_DIR:DrawImage>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${PROJECT_SOURCE_DIR}/test.jpg
		$<TARGET_FILE_DIR:DrawImage>
)


install(TARGETS DrawImage DESTINATION bin)

# 安装Debug版本DLL
install(FILES ${DLLS_DEBUG}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    CONFIGURATIONS Debug
)

# 安装Release版本DLL
install(FILES ${DLLS_RELEASE}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    CONFIGURATIONS Release
)

install(FILES ${PROJECT_SOURCE_DIR}/shader_f.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${PROJECT_SOURCE_DIR}/shader_v.txt DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES ${PROJECT_SOURCE_DIR}/test.jpg DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)