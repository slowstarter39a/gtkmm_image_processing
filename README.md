# gtkmm_image_processing
## image processing based on gtkmm, opencv

1. Coding convention
  * Linux Kernel style. Tabs with a length of 8 characters are used for the indentation
  * method name of class is like below
    * Gtk::Window get_current_image_window();
  * member variable of class is like below
    * int image_id_;
    * int image_count_;

2. Open sources libraries used in this project
  * gtkmm
  * opencv
  * tinyxml2

3. packages need to be installed to compile
   ```bash
   $ sudo apt-get update
   $ sudo apt-get install libgtkmm-3.0-dev
   $ sudo apt-get install libtinyxml2-dev
   $ sudo apt-get install libopencv-dev
   ```

4. Compilation
  * Debug mode compilation
    ```bash
    $ ./build eng
    ```
  * Release mode compilation
    ```bash
    $ ./build
     ```
     or
     ```bash
     $ ./build all
     ```
5. Clean
    ```bash
    $ ./build clean
    ```

6. Execution
    ```bash
   $ export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH    
      #add path to LD_LIBRARY_PATH to search image_processing_lib.so in current directory
   $ ./magnolia
   ```
   Or simply run run.sh
   ```bash
   $ ./run.sh
   ```

 
