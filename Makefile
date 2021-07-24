
all:
	make -C src/gui all -j$(CPU_CORE_NUM)
	make -C src/lib all -j$(CPU_CORE_NUM)

clean:
	rm -rf $(OUT)
	rm -rf $(TARGET)
	rm -rf $(TARGET_LIB).so*
