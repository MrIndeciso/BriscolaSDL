#OBJS specifies which files to compile as part of the project
OBJS = briscola.c Modes/Briscola.c Render/RenderUtil.c Render/GUIUtil.c

#CC specifies which compiler we're using
LCC = gcc
WCC = i686-w64-mingw32-gcc

#COMPILER FLAGS specifies the additional compilation options we're using
# -w suppresses all warning
COMPILER_FLAGS = -w

#LINKER FLAGS specifies the libraries we're linking against
LINUX_LINKER_FLAGS = -lSDL2
WIN_LINKER_FLAGS = Lib/SDL/lib/win32/SDL2main.lib Lib/SDL/lib/win32/SDL2.lib

#OBJ NAME specifies the name of our executable
LINUX_OBJ_NAME = Output/Linux/briscola
WIN_OBJ_NAME = Output/Windows/briscola.exe

#This is the target that compiles our executable
linux : $(OBJS)
	$(LCC) $(OBJS) $(COMPILER_FLAGS) $(LINUX_LINKER_FLAGS) -o $(LINUX_OBJ_NAME)
	
win : $(OBJS)
	$(WCC) $(OBJS) $(COMPILER_FLAGS) $(WIN_LINKER_FLAGS) -o $(WIN_OBJ_NAME) 
