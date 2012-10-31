.PHONY: all clean

TARGET := output

all: 
	nanoc compile
	cp -dpr examples/ $(TARGET)/tutorial/

clean:
	rm -rf $(TARGET)
