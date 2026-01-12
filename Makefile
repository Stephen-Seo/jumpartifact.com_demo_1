ifdef RELEASE
	COMMON_FLAGS := -O2 -DNDEBUG
else
	COMMON_FLAGS := -sASSERTIONS
endif

INCLUDE_FLAGS := -Ithird_party/raylib_out/include -Ithird_party/imgui_git -Ithird_party/rlImGui_git -Ithird_party/lua_out/include -Ithird_party/lpeg_out/include -Ithird_party/box2d_git/include

CURRENT_WORKING_DIR != pwd
EMSDK_REPO_PATH ?= https://github.com/emscripten-core/emsdk.git
EMSDK_VERSION ?= 4.0.23
EMSDK_SHELL ?= ${CURRENT_WORKING_DIR}/third_party/emsdk_git/emsdk_env.sh
EMSDK_SHELL_DIR != dirname ${EMSDK_SHELL}

RAYLIB_REPO_PATH ?= https://github.com/raysan5/raylib.git
RAYLIB_VERSION_TAG ?= 5.5

IMGUI_REPO_PATH ?= https://github.com/ocornut/imgui.git
IMGUI_VERSION_TAG ?= v1.92.5

RLIMGUI_REPO_PATH ?= https://github.com/raylib-extras/rlImGui.git
RLIMGUI_COMMIT ?= dc7f97679a024eee8f5f009e77cc311748200415

LUA_VERSION ?= 5.5.0
LUA_DL_LINK ?= https://lua.org/ftp/lua-${LUA_VERSION}.tar.gz
LUA_TAR_SHA256SUM ?= 57ccc32bbbd005cab75bcc52444052535af691789dba2b9016d5c50640d68b3d

LPEG_DL_LINK ?= https://www.inf.puc-rio.br/~roberto/lpeg/lpeg-1.1.0.tar.gz
LPEG_TAR_SHA256SUM ?= 4b155d67d2246c1ffa7ad7bc466c1ea899bbc40fef0257cc9c03cecbaed4352a

MOONSCRIPT_VERSION_TAG ?= v0.6.0
MOONSCRIPT_VER_NUM := $(subst v0,0,${MOONSCRIPT_VERSION_TAG})
MOONSCRIPT_DL_LINK ?= https://github.com/leafo/moonscript/archive/refs/tags/${MOONSCRIPT_VERSION_TAG}.tar.gz
MOONSCRIPT_TAR_SHA256SUM ?= b98e58f4657ffc2e730904da0b4034796ff16f08e4e6c47c681905fd56509037

BOX2D_VERSION_TAG ?= v3.1.1
BOX2D_REPO_PATH ?= https://github.com/erincatto/box2d.git

SOURCES != find src -regex '.*\.cc$$'
HEADERS != find src -regex '.*\.h$$'

OBJDIR := objdir
OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cc,.cc.o,${SOURCES}))

all: dist/index.html

dist/index.html: third_party/raylib_out/lib/libraylib.a third_party/rlImGui_out/rlImGui.cpp.o third_party/imgui_out/libimgui.a ${OBJECTS} custom_shell.html third_party/lua_out/lib/liblua.a third_party/lpeg_out/lib/liblpeg.a assets_embed/moonscript third_party/box2d_out/lib/libbox2d.a
	@mkdir -p dist
	pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && em++ -std=c++23 -o dist/ja_demo1.html \
		-s USE_GLFW=3 ${INCLUDE_FLAGS} \
		-Lthird_party/raylib_out/lib -lraylib \
		-Lthird_party/imgui_out -limgui \
		third_party/rlImGui_out/rlImGui.cpp.o \
		-Lthird_party/lua_out/lib -llua \
		-Lthird_party/lpeg_out/lib -llpeg \
		-Lthird_party/box2d_out/lib -lbox2d \
		--embed-file assets_embed \
		--shell-file custom_shell.html \
		-sEXPORTED_FUNCTIONS=_main,_upload_script_to_test_lua \
		-sEXPORTED_RUNTIME_METHODS=ccall \
		${COMMON_FLAGS} \
		${OBJECTS}
	ln -sf ja_demo1.html dist/index.html

third_party/raylib_out/lib/libraylib.a: third_party/emsdk_git/emsdk_env.sh third_party/raylib_git
	cd third_party/raylib_git && git clean -xfd && git restore . && patch -N -p1 < ${CURRENT_WORKING_DIR}/third_party/raylib_noF12.patch
	pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && ${MAKE} PLATFORM=PLATFORM_WEB -C third_party/raylib_git/src
	install -D -m444 third_party/raylib_git/src/libraylib.a third_party/raylib_out/lib/libraylib.a
	cd third_party/raylib_git && git clean -xfd && git restore .

third_party/raylib_out/include/raylib.h third_party/raylib_out/include/raymath.h third_party/raylib_out/include/rlgl.h: third_party/raylib_git
	install -D -m444 third_party/raylib_git/src/raylib.h third_party/raylib_out/include/raylib.h
	install -D -m444 third_party/raylib_git/src/raymath.h third_party/raylib_out/include/raymath.h
	install -D -m444 third_party/raylib_git/src/rlgl.h third_party/raylib_out/include/rlgl.h

third_party/raylib_git:
	test -d ./third_party/raylib_git || git clone --depth 1 --no-single-branch ${RAYLIB_REPO_PATH} third_party/raylib_git
	cd third_party/raylib_git && git fetch && git clean -xfd && git restore . && git checkout ${RAYLIB_VERSION_TAG}

third_party/rlImGui_git:
	test -d third_party/rlImGui_git || git clone ${RLIMGUI_REPO_PATH} third_party/rlImGui_git && cd third_party/rlImGui_git && git fetch && git checkout "${RLIMGUI_COMMIT}"

third_party/rlImGui_out/rlImGui.cpp.o: third_party/emsdk_git/emsdk_env.sh third_party/raylib_out/include/raylib.h third_party/imgui_git third_party/rlImGui_git
	@mkdir -p third_party/rlImGui_out
	pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && em++ ${COMMON_FLAGS} -c -o third_party/rlImGui_out/rlImGui.cpp.o ${INCLUDE_FLAGS} third_party/rlImGui_git/rlImGui.cpp

IMGUI_SOURCES := \
	third_party/imgui_git/imgui.cpp \
	third_party/imgui_git/imgui_demo.cpp \
	third_party/imgui_git/imgui_draw.cpp \
	third_party/imgui_git/imgui_tables.cpp \
	third_party/imgui_git/imgui_widgets.cpp

${IMGUI_SOURCES}: third_party/imgui_git

third_party/imgui_git:
	test -d third_party/imgui_git || git clone ${IMGUI_REPO_PATH} third_party/imgui_git
	cd third_party/imgui_git && git fetch && git checkout ${IMGUI_VERSION_TAG}

IMGUI_OBJECTS := $(addprefix ${OBJDIR}/,$(subst .cpp,.cpp.o,${IMGUI_SOURCES}))

third_party/imgui_out/libimgui.a: third_party/imgui_git third_party/emsdk_git/emsdk_env.sh ${IMGUI_OBJECTS}
	@mkdir -p third_party/imgui_out
	pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && emar rcs third_party/imgui_out/libimgui.a ${IMGUI_OBJECTS}

${OBJDIR}/third_party/imgui_git/%.cpp.o: third_party/imgui_git/%.cpp third_party/imgui_git third_party/emsdk_git/emsdk_env.sh
	@mkdir -p ${OBJDIR}/third_party/imgui_git
	pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && em++ -c -o $@ -std=c++23 ${COMMON_FLAGS} $<

third_party/emsdk_git/emsdk_env.sh:
	test -d ./third_party/emsdk_git || git clone ${EMSDK_REPO_PATH} ./third_party/emsdk_git
	cd ./third_party/emsdk_git && git pull
	cd ./third_party/emsdk_git && ./emsdk install "${EMSDK_VERSION}" && ./emsdk activate "${EMSDK_VERSION}"

${OBJDIR}/src/%.cc.o: src/%.cc ${HEADERS} third_party/raylib_out/include/raylib.h third_party/rlImGui_git third_party/imgui_git third_party/emsdk_git/emsdk_env.sh third_party/lua_out/include/lua.h third_party/lpeg_out/include/lpeg_exported.h third_party/box2d_git | format
	@mkdir -p "$(dir $@)"
	pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && em++ -c -o $@ -std=c++23 ${COMMON_FLAGS} ${INCLUDE_FLAGS} $<

third_party/lua-${LUA_VERSION}.tar.gz:
	curl -L -o third_party/lua-${LUA_VERSION}.tar.gz ${LUA_DL_LINK}
	sha256sum third_party/lua-${LUA_VERSION}.tar.gz | grep ${LUA_TAR_SHA256SUM}

third_party/lua-${LUA_VERSION}: third_party/lua-${LUA_VERSION}.tar.gz
	cd third_party && tar -xf lua-${LUA_VERSION}.tar.gz && touch lua-${LUA_VERSION}

third_party/lua_out/include/lua.h third_party/lua_out/include/lualib.h third_party/lua_out/include/luaconf.h third_party/lua_out/include/lauxlib.h: third_party/lua-${LUA_VERSION}
	install -D -m644 third_party/lua-${LUA_VERSION}/src/lua.h third_party/lua_out/include/lua.h
	install -D -m644 third_party/lua-${LUA_VERSION}/src/lualib.h third_party/lua_out/include/lualib.h
	install -D -m644 third_party/lua-${LUA_VERSION}/src/luaconf.h third_party/lua_out/include/luaconf.h
	install -D -m644 third_party/lua-${LUA_VERSION}/src/lauxlib.h third_party/lua_out/include/lauxlib.h

third_party/lua_out/lib/liblua.a: third_party/lua-${LUA_VERSION} third_party/emsdk_git/emsdk_env.sh
	cd third_party \
		&& cd lua-${LUA_VERSION} \
		&& patch -p1 < ${CURRENT_WORKING_DIR}/third_party/lua_src_Makefile_wasm.patch \
		&& ${MAKE} EMSDK_SHELL=${EMSDK_SHELL} -C src \
		&& install -D -m644 src/liblua.a ${CURRENT_WORKING_DIR}/third_party/lua_out/lib/liblua.a

third_party/lpeg-1.1.0.tar.gz:
	curl -o third_party/lpeg-1.1.0.tar.gz ${LPEG_DL_LINK}
	sha256sum third_party/lpeg-1.1.0.tar.gz | grep ${LPEG_TAR_SHA256SUM} || (rm -f third_party/lpeg-1.1.0.tar.gz && /usr/bin/false)

third_party/lpeg-1.1.0: third_party/lpeg-1.1.0.tar.gz
	cd third_party/ && tar -xf lpeg-1.1.0.tar.gz
	cd third_party/lpeg-1.1.0 && patch -p1 < ${CURRENT_WORKING_DIR}/third_party/lpeg_emsdk_wasm.patch

third_party/lpeg_out/lib/liblpeg.a: third_party/lpeg-1.1.0 third_party/emsdk_git/emsdk_env.sh third_party/lua_out/include/lua.h
	${MAKE} EMSDK_SHELL=${EMSDK_SHELL} LUADIR=${CURRENT_WORKING_DIR}/third_party/lua_out/include -C third_party/lpeg-1.1.0 liblpeg.a
	install -D -m644 third_party/lpeg-1.1.0/liblpeg.a third_party/lpeg_out/lib/liblpeg.a

third_party/lpeg_out/include/lpeg_exported.h: third_party/lpeg_out/lib/liblpeg.a
	install -D -m644 third_party/lpeg-1.1.0/lpeg_exported.h third_party/lpeg_out/include/lpeg_exported.h

third_party/moonscript_${MOONSCRIPT_VERSION_TAG}.tar.gz:
	curl -L -o third_party/moonscript_${MOONSCRIPT_VERSION_TAG}.tar.gz ${MOONSCRIPT_DL_LINK}
	sha256sum third_party/moonscript_${MOONSCRIPT_VERSION_TAG}.tar.gz | grep ${MOONSCRIPT_TAR_SHA256SUM} || (rm -f third_party/moonscript_${MOONSCRIPT_VERSION_TAG}.tar.gz && /usr/bin/false)

assets_embed/moonscript: third_party/moonscript_${MOONSCRIPT_VERSION_TAG}.tar.gz
	@mkdir -p assets_embed
	mkdir -p /tmp/${USER}_JADEMO1_TEMP/
	cd /tmp/${USER}_JADEMO1_TEMP && tar -xf ${CURRENT_WORKING_DIR}/third_party/moonscript_${MOONSCRIPT_VERSION_TAG}.tar.gz moonscript-${MOONSCRIPT_VER_NUM}/moonscript
	cp -r /tmp/${USER}_JADEMO1_TEMP/moonscript-${MOONSCRIPT_VER_NUM}/moonscript ./assets_embed/
	rm -rf /tmp/${USER}_JADEMO1_TEMP/

third_party/box2d_git:
	cd third_party && git clone ${BOX2D_REPO_PATH} box2d_git && cd box2d_git && git checkout ${BOX2D_VERSION_TAG}

third_party/box2d_out/lib/libbox2d.a: third_party/box2d_git third_party/emsdk_git/emsdk_env.sh
	cd third_party/box2d_git && git clean -xfd && git restore .
	cd third_party/box2d_git && pushd ${EMSDK_SHELL_DIR} >&/dev/null && source ${EMSDK_SHELL} >&/dev/null && popd >&/dev/null && emcmake cmake -S . -B BUILD -DBOX2D_VALIDATE=Off -DBOX2D_UNIT_TESTS=Off -DBOX2D_SAMPLES=Off -DCMAKE_BUILD_TYPE=Release && ${MAKE} -C BUILD
	install -D -m644 third_party/box2d_git/BUILD/src/libbox2d.a third_party/box2d_out/lib/libbox2d.a

.PHONY: clean update format

clean:
	rm -rf dist
	rm -rf ${OBJDIR}
	${MAKE} PLATFORM=PLATFORM_WEB -C third_party/raylib_git/src clean || /usr/bin/true
	rm -rf third_party/raylib_out
	rm -rf third_party/rlImGui_out
	rm -rf third_party/imgui_out
	rm -rf third_party/lua_out
	rm -rf third_party/lua-${LUA_VERSION}
	rm -rf third_party/lpeg_out
	rm -rf third_party/lpeg-1.1.0
	rm -rf assets_embed/moonscript
	rm -rf third_party/box2d_out
	(cd third_party/box2d_git && git clean -xfd && git restore .) || /usr/bin/true

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
	test -d third_party/box2d_git || git clone ${BOX2D_REPO_PATH} third_party/box2d_git
	cd third_party/box2d_git && git fetch && git clean -xfd && git restore . && git checkout "${BOX2D_VERSION_TAG}"

format:
	test -x /usr/bin/clang-format && clang-format -i --style=file ${SOURCES} ${HEADERS} || /usr/bin/true
