EXE = ffmpeg_gui
SOURCES = src/main.cpp
SOURCES += ${wildcard imgui/*.cpp}
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -Iimgui/ -Iinclude/
CXXFLAGS += -g -Wall -Wformat
LIBS = -lGL -ldl  `sdl2-config --libs` -lGLEW -lglfw

CXXFLAGS += `sdl2-config --cflags`
CFLAGS = $(CXXFLAGS)

ODIR = build

OBJS := $(patsubst %, $(ODIR)/%, $(OBJS))

$(ODIR)/%.o:imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(ODIR)/%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)


clean:
	rm -f $(EXE) build/*.o 



