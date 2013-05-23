.PHONY: all clean

TARGET := output

all: 
	nanoc compile
	rsync -raz examples/ $(TARGET)/tutorial/

clean:
	rm -rf $(TARGET)
