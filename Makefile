
all:
	@echo -e "\e[1;36mStart GUI build!\e[0m"
	make -C src/gui all -j$(CPU_CORE_NUM)
	@echo -e "\e[1;32mGUI build completed!\e[0m\n"

	@echo -e "\e[1;36mStart LIB build!\e[0m"
	make -C src/lib all -j$(CPU_CORE_NUM)
	@echo -e "\e[1;32mLIB build completed!\e[0m\n"

clean:
	rm -rf $(OUT)
	rm -rf $(TARGET)
	rm -rf $(TARGET_LIB).so*
