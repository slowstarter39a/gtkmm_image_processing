CC=g++ -std=c++11
#TARGET_LIB=lib_magnolia_image_process
CFLAGS=`pkg-config gtkmm-3.0 --cflags --libs` -Wall -ldl -I$(SRC)/$(COMMON) -I$(SRC)/$(LIB) -I/usr/include/opencv4

ifneq (,$(findstring $(MAKECMDGOALS), "eng"))
CFLAGS+=-g
endif

SRCS := $(wildcard $(SRC)/$(LIB)/*.cc)
INCLUDES := $(wildcard $(SRC)/$(LIB)/*.h)
OBJS := $(SRCS:.cc=.o)
LIB_OUT=$(OUT)/$(SRC)/$(LIB)

Q=@echo

eng:all

all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJS)
	$(CC)  $(CFLAGS) -shared -Wl,-soname,$(TARGET_LIB).so.0 -o  $(TARGET_LIB).so.0.0 $(addprefix $(OUT)/,$^) 
	ln -sf $(TARGET_LIB).so.0.0 $(TARGET_LIB).so
	@echo "LIB Compilation complete!"

$(OBJS): %.o : %.cc
	$(CC) $(CFLAGS) -c $< -o $(OUT)/$@ -fPIC


