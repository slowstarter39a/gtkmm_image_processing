CC=g++ -std=c++11
TARGET=magnolia
CFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`

SRCS := $(wildcard src/*.cc)
INCLUDES := $(wildcard src/*.h)
OBJS := $(SRCS:.cc=.o)

Q=@echo

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(addprefix $(OUT)/,$^) $(CFLAGS)
	@echo "Compilation complete!"

$(OBJS): %.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $(OUT)/$@


