include src/gui/Makefile
include src/lib/Makefile

all:

clean:
	rm -rf $(OUT)
	rm -rf $(TARGET)
	rm -rf $(TARGET_LIB).so*
