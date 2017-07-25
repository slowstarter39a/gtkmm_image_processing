CC=g++ -std=c++11
#TARGET=magnolia
CFLAGS=`pkg-config gtkmm-3.0 --cflags --libs` -Wall -I$(SRC)/$(COMMON) -I$(SRC)/$(LIB)

ifneq (,$(findstring $(MAKECMDGOALS), "eng"))
CFLAGS+=-g
endif

SRCS := $(wildcard $(SRC)/$(GUI)/*.cc)
INCLUDES := $(wildcard $(SRC)/$(GUI)/*.h)
OBJS := $(SRCS:.cc=.o)

Q=@echo

eng:all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(addprefix $(OUT)/,$^) $(CFLAGS)
	@echo "GUI Compilation complete!"

$(OBJS): %.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $(OUT)/$@


