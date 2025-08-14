ifdef RELEASE
	COMMON_FLAGS := -O2 -DNDEBUG
else
	COMMON_FLAGS := -sASSERTIONS
endif

INCLUDE_FLAGS := -Ithird_party/raylib_out/include -Ithird_party/imgui_git -Ithird_party/rlImGui_git

CURRENT_WORKING_DIR != pwd
EMSDK_REPO_PATH ?= https://github.com/emscripten-core/emsdk.git
EMSDK_VERSION ?= 4.0.12
EMSDK_SHELL ?= ${CURRENT_WORKING_DIR}/third_party/emsdk_git/emsdk_env.sh

RAYLIB_REPO_PATH ?= https://github.com/raysan5/raylib.git
RAYLIB_VERSION_TAG ?= 5.5

IMGUI_REPO_PATH ?= https://github.com/ocornut/imgui.git
IMGUI_VERSION_TAG ?= v1.92.2

RLIMGUI_REPO_PATH ?= https://github.com/raylib-extras/rlImGui.git
RLIMGUI_COMMIT ?= 4d8a61842903978bc42adf3347cd34f4e6524efc

SOURCES != find src -regex '.*\.cc$$'
HEADERS != find src -regex '.*\.h$$'

OBJDIR := objdir
OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cc,.cc.o,${SOURCES}))

all: dist/index.html

dist/index.html: third_party/raylib_out/lib/libraylib.a third_party/rlImGui_out/rlImGui.cpp.o third_party/imgui_out/libimgui.a ${OBJECTS} custom_shell.html
	@mkdir -p dist
	source ${EMSDK_SHELL} >&/dev/null && em++ -std=c++23 -o dist/ja_demo1.html \
		-s USE_GLFW=3 ${INCLUDE_FLAGS} \
		-Lthird_party/raylib_out/lib -lraylib \
		-Lthird_party/imgui_out -limgui \
		third_party/rlImGui_out/rlImGui.cpp.o \
		--shell-file custom_shell.html \
		-sEXPORTED_FUNCTIONS=_main \
		${COMMON_FLAGS} \
		${OBJECTS}
	ln -sf ja_demo1.html dist/index.html

third_party/raylib_out/lib/libraylib.a: third_party/emsdk_git/emsdk_env.sh third_party/raylib_git/
	cd third_party/raylib_git && git clean -xfd && git restore . && patch -N -p1 < ${CURRENT_WORKING_DIR}/third_party/raylib_noF12.patch
	source ${EMSDK_SHELL} >&/dev/null && ${MAKE} PLATFORM=PLATFORM_WEB -C third_party/raylib_git/src
	install -D -m444 third_party/raylib_git/src/libraylib.a third_party/raylib_out/lib/libraylib.a
	cd third_party/raylib_git && git clean -xfd && git restore .

third_party/raylib_out/include/raylib.h third_party/raylib_out/include/raymath.h third_party/raylib_out/include/rlgl.h: third_party/raylib_git/
	install -D -m444 third_party/raylib_git/src/raylib.h third_party/raylib_out/include/raylib.h
	install -D -m444 third_party/raylib_git/src/raymath.h third_party/raylib_out/include/raymath.h
	install -D -m444 third_party/raylib_git/src/rlgl.h third_party/raylib_out/include/rlgl.h

third_party/raylib_git/:
	test -d ./third_party/raylib_git || git clone --depth 1 --no-single-branch ${RAYLIB_REPO_PATH} third_party/raylib_git
	cd third_party/raylib_git && git fetch && git clean -xfd && git restore . && git checkout ${RAYLIB_VERSION_TAG}

third_party/rlImGui_out/rlImGui.cpp.o: third_party/emsdk_git/emsdk_env.sh third_party/raylib_out/include/raylib.h third_party/imgui_git/
	@mkdir -p third_party/rlImGui_out
	test -d third_party/rlImGui_git || git clone ${RLIMGUI_REPO_PATH} third_party/rlImGui_git && cd third_party/rlImGui_git && git fetch && git checkout "${RLIMGUI_COMMIT}"
	source ${EMSDK_SHELL} >&/dev/null && em++ ${COMMON_FLAGS} -c -o third_party/rlImGui_out/rlImGui.cpp.o ${INCLUDE_FLAGS} third_party/rlImGui_git/rlImGui.cpp

IMGUI_SOURCES := \
	third_party/imgui_git/imgui.cpp \
	third_party/imgui_git/imgui_demo.cpp \
	third_party/imgui_git/imgui_draw.cpp \
	third_party/imgui_git/imgui_tables.cpp \
	third_party/imgui_git/imgui_widgets.cpp

${IMGUI_SOURCES}: third_party/imgui_git/

third_party/imgui_git/:
	test -d third_party/imgui_git || git clone ${IMGUI_REPO_PATH} third_party/imgui_git
	cd third_party/imgui_git && git fetch && git checkout ${IMGUI_VERSION_TAG}

IMGUI_OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cpp,.cpp.o,${IMGUI_SOURCES}))

third_party/imgui_out/libimgui.a: third_party/imgui_git/ third_party/emsdk_git/emsdk_env.sh ${IMGUI_OBJECTS}
	@mkdir -p third_party/imgui_out
	source ${EMSDK_SHELL} >&/dev/null && emar rcs third_party/imgui_out/libimgui.a ${IMGUI_OBJECTS}

${OBJDIR}/third_party/imgui_git/%.cpp.o: third_party/imgui_git/%.cpp third_party/imgui_git/ third_party/emsdk_git/emsdk_env.sh
	@mkdir -p ${OBJDIR}/third_party/imgui_git
	source ${EMSDK_SHELL} >&/dev/null && em++ -c -o $@ -std=c++23 ${COMMON_FLAGS} $<

third_party/emsdk_git/emsdk_env.sh:
	test -d ./third_party/emsdk_git || git clone ${EMSDK_REPO_PATH} ./third_party/emsdk_git
	cd ./third_party/emsdk_git && git pull
	cd ./third_party/emsdk_git && ./emsdk install "${EMSDK_VERSION}" && ./emsdk activate "${EMSDK_VERSION}"

${OBJDIR}/src/%.cc.o: src/%.cc ${HEADERS} third_party/raylib_out/include/raylib.h third_party/rlImGui_out/rlImGui.cpp.o third_party/imgui_out/libimgui.a | format
	@mkdir -p "$(dir $@)"
	source ${EMSDK_SHELL} >&/dev/null && em++ -c -o $@ -std=c++23 ${COMMON_FLAGS} ${INCLUDE_FLAGS} $<

.PHONY: clean update format

clean:
	rm -rf dist
	rm -rf ${OBJDIR}
	${MAKE} PLATFORM=PLATFORM_WEB -C third_party/raylib_git/src clean || /usr/bin/true
	rm -rf third_party/raylib_out
	rm -rf third_party/rlImGui_out
	rm -rf third_party/imgui_out

update:
	test -d ./third_party/emsdk_git || git clone ${EMSDK_REPO_PATH} ./third_party/emsdk_git
	cd ./third_party/emsdk_git && git pull
	cd ./third_party/emsdk_git && ./emsdk install "${EMSDK_VERSION}" && ./emsdk activate "${EMSDK_VERSION}"
	test -d ./third_party/raylib_git || git clone ${RAYLIB_REPO_PATH} ./third_party/raylib_git
	cd third_party/raylib_git && git clean -xfd && git restore . && git fetch && git checkout "${RAYLIB_VERSION_TAG}"
	test -d ./third_party/imgui_git || git clone ${IMGUI_REPO_PATH} ./third_party/imgui_git
	cd third_party/imgui_git && git fetch && git checkout "${IMGUI_VERSION_TAG}"
	test -d ./third_party/rlImGui_git || git clone ${RLIMGUI_REPO_PATH} ./third_party/rlImGui_git
	cd third_party/rlImGui_git && git fetch && git checkout "${RLIMGUI_COMMIT}"

format:
	test -x /usr/bin/clang-format && clang-format -i --style=Google ${SOURCES} ${HEADERS} || /usr/bin/true
