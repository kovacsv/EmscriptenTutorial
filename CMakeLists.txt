cmake_minimum_required (VERSION 3.6)

function (InitProject projectName projectFolder)
	set (SourcesFolder Sources/${projectFolder})
	file (GLOB
		SourceFiles
		${SourcesFolder}/*.hpp
		${SourcesFolder}/*.cpp
	)
	set (
		ProjectSourceFiles
		${SourceFiles}
	)

	add_executable (${projectName} ${ProjectSourceFiles})
	if (WIN32)
		target_compile_options (${projectName} PUBLIC /W4 /WX)
	else ()
		target_compile_options (${projectName} PUBLIC -Wall -Wextra -Werror)
	endif ()
	target_include_directories (${projectName} PUBLIC ${SourcesFolder})

	if (${EMSCRIPTEN})
		target_compile_options (${projectName} PUBLIC "$<$<CONFIG:Debug>:-g>")
		set_target_properties (${projectName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
	else ()
		target_compile_options (${projectName} PUBLIC "$<$<CONFIG:Debug>:-DDEBUG>")
		set_target_properties (${projectName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
	endif ()

	get_filename_component (SourcesFolderFullPath "${SourcesFolder}" ABSOLUTE)
	add_custom_command (TARGET ${projectName} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${SourcesFolderFullPath}/web" $<TARGET_FILE_DIR:${projectName}>
	)
endfunction ()

function (AddSDLDependency projectName sdlLibFolder sdlTTFLibFolder exportedFunctions)
	if (${EMSCRIPTEN})
		set (SDLUsedLibraries -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2)
		target_compile_options (${projectName} PUBLIC "$<$<CONFIG:Debug>:-gsource-map>")
		target_link_options (${projectName} PUBLIC "-sEXPORTED_FUNCTIONS=[${exportedFunctions}]")
		target_link_options (${projectName} PUBLIC "-sEXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]")
		target_compile_options (${projectName} PUBLIC ${SDLUsedLibraries})
		target_link_options (${projectName} PUBLIC ${SDLUsedLibraries})
	else ()
		set_target_properties (${projectName} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
		target_include_directories (
			${projectName} PUBLIC
			${sdlLibFolder}/include
			${sdlTTFLibFolder}/include
		)
		target_link_libraries (${projectName} SDL2 SDL2main SDL2_ttf)
		get_filename_component (SDLLibFolderFullPath "${sdlLibFolder}" ABSOLUTE)
		get_filename_component (SDLTTFLibFolderFullPath "${sdlTTFLibFolder}" ABSOLUTE)
		add_custom_command (TARGET ${projectName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${SDLLibFolderFullPath}/lib/x86/SDL2.dll" $<TARGET_FILE_DIR:${projectName}>
			COMMAND ${CMAKE_COMMAND} -E copy "${SDLTTFLibFolderFullPath}/lib/x86/SDL2_ttf.dll" $<TARGET_FILE_DIR:${projectName}>
			COMMAND ${CMAKE_COMMAND} -E copy "${SDLTTFLibFolderFullPath}/lib/x86/libfreetype-6.dll" $<TARGET_FILE_DIR:${projectName}>
		)
	endif ()
endfunction ()

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)
set (CMAKE_DEBUG_POSTFIX "Debug")

add_definitions (-DUNICODE -D_UNICODE)

project (EmscriptenTutorial)

set (LibSourcesFolder Libs)
set (SDLLibFolder ${LibSourcesFolder}/SDL2-2.0.12)
set (SDLTTFLibFolder ${LibSourcesFolder}/SDL2_ttf-2.0.15)
link_directories (${SDLLibFolder}/lib/x86)
link_directories (${SDLTTFLibFolder}/lib/x86)

InitProject (01_hello_world 01_hello_world)

InitProject (02_draw_line 02_draw_line)
AddSDLDependency (02_draw_line ${SDLLibFolder} ${SDLTTFLibFolder} "\"_main\"")

InitProject (03_draw_line_loop 03_draw_line_loop)
AddSDLDependency (03_draw_line_loop ${SDLLibFolder} ${SDLTTFLibFolder} "\"_main\"")

InitProject (04_draw_line_interaction 04_draw_line_interaction)
AddSDLDependency (04_draw_line_interaction ${SDLLibFolder} ${SDLTTFLibFolder} "\"_main\"")

InitProject (05_clear_lines 05_clear_lines)
AddSDLDependency (05_clear_lines ${SDLLibFolder} ${SDLTTFLibFolder} "\"_main\", \"_ClearLines\"")

InitProject (06_line_congrat 06_line_congrat)
AddSDLDependency (06_line_congrat ${SDLLibFolder} ${SDLTTFLibFolder} "\"_main\", \"_ClearLines\"")

InitProject (07_load_file 07_load_file)
AddSDLDependency (07_load_file ${SDLLibFolder} ${SDLTTFLibFolder} "\"_main\", \"_ClearLines\"")
get_filename_component (07_load_file_sources Sources/07_load_file ABSOLUTE)
add_custom_command (TARGET 07_load_file PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${07_load_file_sources}/07_load_file_assets" ${CMAKE_BINARY_DIR}/07_load_file_assets
)
if (${EMSCRIPTEN})
	target_link_options (07_load_file PUBLIC --use-preload-plugins --preload-file 07_load_file_assets)
endif ()
