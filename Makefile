all: wrapper-test

LuaJIT/src/libluajit.a:
	$(MAKE) $(MAKEOPTS) -C LuaJIT/src amalg CC=$(CC) BUILDMODE=static XCFLAGS=-DLUAJIT_ENABLE_GC64

wrapper-test: LuaJIT/src/libluajit.a wrapper-test.cc
	$(CXX) -o wrapper-test -O3 -g $(CXXFLAGS) wrapper-test.cc LuaJIT/src/libluajit.a -ILuaJIT/src -ldl
