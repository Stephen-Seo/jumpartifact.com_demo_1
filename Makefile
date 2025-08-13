ifdef RELEASE
	COMMON_FLAGS := -O2 -DNDEBUG
else
	COMMON_FLAGS := -sASSERTIONS
endif

INCLUDE_FLAGS := -Ithird_party/raylib_out/include -Ithird_party/imgui -Ithird_party/rlImGui_git

CURRENT_WORKING_DIR != pwd
EMSDK_VERSION ?= 4.0.12
EMSDK_SHELL ?= ${CURRENT_WORKING_DIR}/third_party/emsdk_git/emsdk_env.sh

RAYLIB_VERSION_TAG ?= 5.5

IMGUI_VERSION_TAG ?= v1.92.2

SOURCES != find src -regex '.*\.cc$$'
HEADERS != find src -regex '.*\.h$$'

OBJDIR := objdir
OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cc,.cc.o,${SOURCES}))

all: dist/index.html

dist/index.html: third_party/raylib_out/lib/libraylib.a third_party/rlImGui_out/rlImGui.cpp.o third_party/imgui_out/libimgui.a ${OBJECTS}
	@mkdir -p dist
	source ${EMSDK_SHELL} >&/dev/null && em++ -std=c++23 -o dist/ja_demo1.html \
		-s USE_GLFW=3 ${INCLUDE_FLAGS} \
		-Lthird_party/raylib_out/lib -lraylib \
		-Lthird_party/imgui_out -limgui \
		third_party/rlImGui_out/rlImGui.cpp.o \
		--shell-file custom_shell.html \
		-sEXPORTED_FUNCTIONS=_main \
		-SEXPORTED_RUNTIME_METHODS=ccall \
		${COMMON_FLAGS} \
		${OBJECTS}
	ln -sf ja_demo1.html dist/index.html

third_party/raylib_out/lib/libraylib.a: third_party/emsdk_git/emsdk_env.sh
	/usr/bin/env EMSDK_ENV_SCRIPT="${EMSDK_SHELL}" ./third_party/wasm_make_raylib.sh -o ./third_party/raylib_out -c ./third_party/raylib_git

third_party/rlImGui_out/rlImGui.cpp.o: third_party/emsdk_git/emsdk_env.sh third_party/raylib_out/lib/libraylib.a third_party/imgui/
	@mkdir -p third_party/rlImGui_out
	test -d third_party/rlImGui_git || git clone https://github.com/raylib-extras/rlImGui.git third_party/rlImGui_git
	source ${EMSDK_SHELL} >&/dev/null && em++ ${COMMON_FLAGS} -c -o third_party/rlImGui_out/rlImGui.cpp.o ${INCLUDE_FLAGS} third_party/rlImGui_git/rlImGui.cpp

third_party/imgui/:
	test -d third_party/imgui || git clone https://github.com/ocornut/imgui.git third_party/imgui
	cd third_party/imgui && git checkout ${IMGUI_VERSION_TAG}

IMGUI_SOURCES := \
	third_party/imgui/imgui.cpp \
	third_party/imgui/imgui_demo.cpp \
	third_party/imgui/imgui_draw.cpp \
	third_party/imgui/imgui_tables.cpp \
	third_party/imgui/imgui_widgets.cpp
IMGUI_OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cpp,.cpp.o,${IMGUI_SOURCES}))

third_party/imgui_out/libimgui.a: third_party/imgui/ third_party/emsdk_git/emsdk_env.sh ${IMGUI_OBJECTS}
	@mkdir -p third_party/imgui_out
	source ${EMSDK_SHELL} >&/dev/null && emar rcs third_party/imgui_out/libimgui.a ${IMGUI_OBJECTS}

${OBJDIR}/third_party/imgui/%.cpp.o: third_party/imgui/%.cpp third_party/emsdk_git/emsdk_env.sh
	@mkdir -p ${OBJDIR}/third_party/imgui
	source ${EMSDK_SHELL} >&/dev/null && em++ -c -o $@ -std=c++23 ${COMMON_FLAGS} $<

third_party/emsdk_git/emsdk_env.sh:
	/usr/bin/env EMSDK_CLONE_DIR=./third_party/emsdk_git EMSDK_TAG_VERSION="${EMSDK_VERSION}" ./third_party/setup_emsdk.sh

${OBJDIR}/src/%.cc.o: src/%.cc ${HEADERS} third_party/raylib_out/lib/libraylib.a third_party/rlImGui_out/rlImGui.cpp.o third_party/imgui_out/libimgui.a
	@mkdir -p "$(dir $@)"
	source ${EMSDK_SHELL} >&/dev/null && em++ -c -o $@ -std=c++23 ${COMMON_FLAGS} ${INCLUDE_FLAGS} $<

.PHONY: clean update

clean:
	rm -rf dist
	rm -rf ${OBJDIR}
	rm -rf third_party/raylib_out
	rm -rf third_party/rlImGui_out
	rm -rf third_party/imgui_out

update: third_party/raylib_out/lib/libraylib.a third_party/rlImGui_out/rlImGui.cpp.o third_party/imgui_out/libimgui.a
	@rm -rf third_party/raylib_out
	@rm -rf third_party/rlImGui_out
	@rm -rf third_party/imgui_out
	/usr/bin/env EMSDK_CLONE_DIR=./third_party/emsdk_git EMSDK_TAG_VERSION="${EMSDK_VERSION}" ./third_party/setup_emsdk.sh
	cd third_party/raylib_git && git fetch && git checkout "${RAYLIB_VERSION_TAG}"
	cd third_party/imgui && git fetch && git checkout "${IMGUI_VERSION_TAG}"
	cd third_party/rlImGui_git && git pull
