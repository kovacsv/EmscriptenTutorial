pushd %~dp0
mkdir BuildEm

pushd BuildEm
call %1\emsdk_env.bat || goto :error
call emcmake cmake -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=%3 .. || goto :error
call emmake mingw32-make || goto :error
popd
echo Build Succeeded.

xcopy Sources\01_hello_world\index.html %2\01_hello_world\index.html* /s /i /Y > NUL
xcopy BuildEm\01_hello_world.js %2\01_hello_world\01_hello_world.js* /s /i /Y > NUL
xcopy BuildEm\01_hello_world.wasm %2\01_hello_world\01_hello_world.wasm* /s /i /Y > NUL

xcopy Sources\02_draw_line\index.html %2\02_draw_line\index.html* /s /i /Y > NUL
xcopy BuildEm\02_draw_line.js %2\02_draw_line\02_draw_line.js* /s /i /Y > NUL
xcopy BuildEm\02_draw_line.wasm %2\02_draw_line\02_draw_line.wasm* /s /i /Y > NUL

xcopy Sources\03_draw_line_loop\index.html %2\03_draw_line_loop\index.html* /s /i /Y > NUL
xcopy BuildEm\03_draw_line_loop.js %2\03_draw_line_loop\03_draw_line_loop.js* /s /i /Y > NUL
xcopy BuildEm\03_draw_line_loop.wasm %2\03_draw_line_loop\03_draw_line_loop.wasm* /s /i /Y > NUL

xcopy Sources\04_draw_line_interaction\index.html %2\04_draw_line_interaction\index.html* /s /i /Y > NUL
xcopy BuildEm\04_draw_line_interaction.js %2\04_draw_line_interaction\04_draw_line_interaction.js* /s /i /Y > NUL
xcopy BuildEm\04_draw_line_interaction.wasm %2\04_draw_line_interaction\04_draw_line_interaction.wasm* /s /i /Y > NUL

xcopy Sources\05_clear_lines\index.html %2\05_clear_lines\index.html* /s /i /Y > NUL
xcopy BuildEm\05_clear_lines.js %2\05_clear_lines\05_clear_lines.js* /s /i /Y > NUL
xcopy BuildEm\05_clear_lines.wasm %2\05_clear_lines\05_clear_lines.wasm* /s /i /Y > NUL

xcopy Sources\06_line_congrat\index.html %2\06_line_congrat\index.html* /s /i /Y > NUL
xcopy Sources\06_line_congrat\congrat.png %2\06_line_congrat\congrat.png* /s /i /Y > NUL
xcopy BuildEm\06_line_congrat.js %2\06_line_congrat\06_line_congrat.js* /s /i /Y > NUL
xcopy BuildEm\06_line_congrat.wasm %2\06_line_congrat\06_line_congrat.wasm* /s /i /Y > NUL

xcopy Sources\07_load_file\index.html %2\07_load_file\index.html* /s /i /Y > NUL
xcopy BuildEm\07_load_file.js %2\07_load_file\07_load_file.js* /s /i /Y > NUL
xcopy BuildEm\07_load_file.wasm %2\07_load_file\07_load_file.wasm* /s /i /Y > NUL
xcopy BuildEm\07_load_file.data %2\07_load_file\07_load_file.data* /s /i /Y > NUL

popd
exit /b 0

:error
echo Build Failed with Error %errorlevel%.
popd
popd
exit /b %errorlevel%
