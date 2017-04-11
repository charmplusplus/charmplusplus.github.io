.PHONY: all clean

TARGET := output

all: 
	nanoc compile

clean:
	rm -rf $(TARGET)
