SRC = $(wildcard game/*.c)

default:mac
.PHONY: assets
assets:
	@if [ -d "bin/pack" ]; then rm -r "bin/pack"; fi
	@echo "#include \"sep/Pat.h\"" > pack.c
	@clang -g -DPACK_PROGRAM pack.c -o bin/pack
	@bin/pack -p assets assets.abp
	@rm pack.c
	@echo "Assets Updated"

mac:
	@clang -g -O0 -DDEBUG -ObjC -std=c17 -I. lib/libopenal.a main.c $(SRC) -framework Cocoa -framework Quartz -framework OpenGL -framework CoreAudio -framework AudioToolbox -framework AVFoundation -lc++ -o bin/main
	@bin/main
	@echo "Built a debug build for MacOS"
win:
	@zig cc -target x86_64-windows-gnu -I. -O3 -static lib/OpenAL32.lib main.c $(SRC) -o bin/main.exe -lopengl32 -lshell32 -luser32 -lgdi32 -lkernel32 -lpdh -lole32 -ldsound -lxaudio2_9
	@wine64 bin/main.exe
	@echo "Built a debug build for Windows"
release:
	@echo "Building the MacOS build"
	@if [ -d "bin/publish/Specters.app" ]; then rm -r "bin/publish/Specters.app"; fi
	@mkdir bin/publish/Specters.app
	@mkdir bin/publish/Specters.app/Contents
	@mkdir bin/publish/Specters.app/Contents/MacOS
	@mkdir bin/publish/Specters.app/Contents/Resources
	@echo '<?xml version="1.0" encoding="UTF-8"?>\n<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">\n<plist version="1.0">\n<dict>\n\t<key>CFBundleDevelopmentRegion</key>\n\t<string>en</string>\n\t<key>CFBundleExecutable</key>\n\t<string>Specters</string>\n\t<key>CFBundleIconFile</key>\n\t<string>Icon</string>\n\t<key>CFBundleIdentifier</key>\n\t<string>net.Sepano.Specters</string>\n\t<key>CFBundleInfoDictionaryVersion</key>\n\t<string>6.0</string>\n\t<key>CFBundleName</key>\n\t<string>Specters</string>\n\t<key>CFBundlePackageType</key>\n\t<string>APPL</string>\n\t<key>CFBundleShortVersionString</key>\n\t<string>1.0</string>\n\t<key>CFBundleSignature</key>\n\t<string>Specters</string>\n\t<key>CFBundleVersion</key>\n\t<string>Release</string>\n\t<key>LSApplicationCategoryType</key>\n\t<string>public.app-category.games</string>\n\t<key>LSMinimumSystemVersion</key>\n\t<string>11</string>\n\t<key>NSHumanReadableCopyright</key>\n\t<string>Copyright © 2024-2025 Sepano Darbandi</string>\n\t<key>NSPrincipalClass</key>\n\t<string>NSApplication</string>\n\t<key>NSHighResolutionCapable</key>\n\t<string>True</string>\n</dict>\n</plist>' > bin/publish/Specters.app/Contents/Info.plist
	@cp -r assets.abp bin/publish/Specters.app/Contents/Resources
	@cp -r Icon.icns bin/publish/Specters.app/Contents/Resources
	@clang -ObjC -std=c17 -I.  lib/libopenal.a main.c $(SRC) -framework Cocoa -framework Quartz -framework OpenGL -framework CoreAudio -framework AudioToolbox -framework AVFoundation -lc++ -arch x86_64 -arch arm64 -O3 -o bin/publish/Specters.app/Contents/MacOS/Specters
	@echo "published for MacOS-Universal2 in bin/publish/Rhine.app"

	@echo "Building the Windows x64 build"
	@if [ -d "bin/publish/Win64" ]; then rm -r "bin/publish/Win64"; fi
	@mkdir bin/publish/Win64
	zig cc -target x86_64-windows-gnu -I. -O3 -static lib/libOpenAL32.a lib/OpenAL32.lib app_icon.o main.c $(SRC) -o bin/publish/Win64/Specters.exe -lopengl32 -lshell32 -luser32 -lgdi32 -lkernel32 -lpdh
	@rm -f bin/publish/Win64/*.pdb
	@cp -r assets.abp bin/publish/Win64
	@cp -r lib/OpenAL32.dll bin/publish/Win64
	@cp -r Icon.ico bin/publish/Win64