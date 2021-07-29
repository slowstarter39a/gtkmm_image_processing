CC=g++ -std=c++11
#TARGET_LIB=lib_magnolia_image_process
CFLAGS=`pkg-config gtkmm-3.0 --cflags --libs` `pkg-config --cflags --libs opencv4` -Wall -ldl -I$(PRJ_ROOT)/$(SRC)/$(COMMON) -I$(PRJ_ROOT)/$(SRC)/$(LIB)

ifneq (,$(findstring $(MAKECMDGOALS), "eng"))
CFLAGS+=-g
endif

SRCS := $(wildcard *.cc)
SRCS += $(wildcard ../$(COMMON)/*.cc)
INCLUDES := $(wildcard *.h)
INCLUDES += $(wildcard ../$(COMMON)/*.h)
OBJS := $(SRCS:.cc=.o)
LIB_OUT_PATH=$(PRJ_ROOT)/$(OUT)/$(SRC)/$(LIB)

Q=@echo

eng:all

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
	$(CC)  $(CFLAGS) -shared -Wl,-soname,$(PRJ_ROOT)/$(TARGET_LIB).so.0 -o $(PRJ_ROOT)/$(TARGET_LIB).so.0.0 $(addprefix $(LIB_OUT_PATH)/,$^)
	ln -sf $(PRJ_ROOT)/$(TARGET_LIB).so.0.0 $(PRJ_ROOT)/$(TARGET_LIB).so

$(OBJS): %.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $(LIB_OUT_PATH)/$@ -fPIC


