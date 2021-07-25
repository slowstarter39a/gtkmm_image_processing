CC=g++ -std=c++11
#TARGET=magnolia
CFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`  -ldl -ltinyxml2 -Wall -I$(PRJ_ROOT)/$(SRC)/$(COMMON) -I$(PRJ_ROOT)/$(SRC)/$(LIB)

ifneq (,$(findstring $(MAKECMDGOALS), "eng"))
CFLAGS+=-g
endif

SRCS := $(wildcard *.cc)
SRCS += $(wildcard ../$(COMMON)/*.cc)
INCLUDES := $(wildcard *.h)
INCLUDES += $(wildcard ../$(COMMON)/*.h)
OBJS := $(SRCS:.cc=.o)
OUT_PATH := $(PRJ_ROOT)/$(OUT)/$(SRC)/$(GUI)

Q=@echo

eng:all

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(PRJ_ROOT)/$@ $(addprefix $(OUT_PATH)/,$^) $(CFLAGS)

$(OBJS): %.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $(OUT_PATH)/$@
