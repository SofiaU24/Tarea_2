CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g \
           -Iinclude -I. -Iimgui -Iimgui/backends

# Todos los .cpp de src/ y sus subcarpetas (src/*.cpp, src/pcg/*.cpp, ...) + ImGui
SRCS = $(wildcard src/*.cpp) \
       imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp \
       imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp
OBJS = $(SRCS:.cpp=.o)


ifeq ($(OS),Windows_NT)
  TARGET = Tarea2.exe
  LDLIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32
else
  TARGET = Tarea2
  LDLIBS = -lglfw -lGLEW -lGL
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe


.PHONY: all clean
