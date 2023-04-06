TARGET = bin/winfetch.exe
SRCS = $(wildcard src/*.cpp)
LDLIBS = -ld3d9

all: $(TARGET)
	$(TARGET)

$(TARGET): $(SRCS)
	g++ $(SRCS) $(LDLIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)