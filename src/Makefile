CC = $(CROSS_COMPILE)g++ 
objcopy= objcopy
objdump= objdump

SRCS = $(wildcard ./*.cpp) \
	$(wildcard ./xml_tool/*.cpp)

INCLUDES = -I include/ 

OBJDIR = .

OBJS = $(basename $(SRCS))

vpath %.c / 
vpath %.o $(OBJDIR)

all: start build end

start:
	@echo "Starting build process...." 

build:
	$(CC) $(INCLUDES) $(SRCS) -std=gnu++11 -w -o $(OBJDIR)/dump_analyzer 

end:
	@echo "Completed" 

clean:
	@echo "Cleaning..."
	rm -rf $(OBJDIR)/*.o ./mem_dump_tool 

