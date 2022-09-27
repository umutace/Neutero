Information for running the program on Lab PC with Qt Creator:



1. Open the file pse_ss22_nn/NeuteroAll/NeuteroAll.pro with Qt Creator

2. Right click the Neutero project and select "Run qmake"

3. Right click the Neutero project and select "Build Neutero"

4. Right click the Neutero project and select "Run". The application window should be opened after ~5 seconds.

Note: the git repository (pse_ss22_nn) is already cloned on Lap PC to the directory: /home/pse/Desktop/Neutero

Hints for GUI:

--The user has to double-click to choose the hardware.

--The user has to click one time on the listed neural network to select it.



=== Build Instructions (guaranteed for this computer only!) ===

  1. Navigate to the folder NeuteroAll/Neutero inside the git repository.
  2. Run "qmake && make install -j16 && make".
  3. Run "make clean" to clean up build files.
  4. Executable file "Neutero" is located at NeuteroAll/Neutero/build.

  Note: It takes around 10 seconds to open the application, it might look
        as if the application stopped responding, please be patient.

