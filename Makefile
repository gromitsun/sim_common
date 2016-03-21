CC = clang++
CFLAGS = -Wall -O3 -std=c++11 -I../cl_common/
LFLAGS = -framework OpenCL -shared -L../cl_common/ -lclcommon
EXE = libsimcommon.so
SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

$(EXE): $(OBJ_FILES)
	$(CC) -o $(EXE) $(OBJ_FILES) $(LFLAGS)
$(OBJ_FILES): $(SRC_FILES)
	$(CC) -c $(SRC_FILES) $(CFLAGS)
clean:
	rm -fv $(EXE)
	rm -fv *.o
	rm -fv *.gch
