# VGA-Monitor-Simulation
The project aims to design and implement a VGA display simulation system using hardware description languages (VHDL and SystemVerilog) integrated with a C++ environment. Instead of relying on a physical VGA connector and monitor, this project replaces the hardware interface with software-driven simulation, enabling images to be displayed directly on a computer screen. VHDL is simulated with GHDL and connected through VHPI, while SystemVerilog is simulated with Verilator using DPI, with both approaches interfacing to a C++ program that renders the display using the SDL2 library.
<vid width="1430" height="714" alt="video" src="https://github.com/user-attachments/assets/6cd69eb3-f9c9-41e9-98ab-4183e9248dc1" />


to run the vhdl part use:

**python3 build.py name_of_the_test.vhdl**

to run the system verilog part use:

**make -f Makefile USER_MODULE=test_name**

to stop the image display on the screen press Q

# 1.to use verilator /dpi and vhpi you need to install:
sudo apt update 
sudo apt install build-essential
sudo apt install verilator libsdl2-dev

for python : 
python :sudo apt install python3


for using GHDL :
GHDL version :GHDL 6.0.0-dev (5.1.1.r10.g282f20413) [Dunoon edition]
(used this to install this version) 
# 2. Install required dependencies
sudo apt update
sudo apt install -y git make gnat zlib1g-dev libreadline-dev \
                    libffi-dev libgmp-dev libboost-all-dev \
                    gcc g++ python3-pip llvm clang cmake

# 3. Clone the GHDL repository
git clone https://github.com/ghdl/ghdl.git
cd ghdl

# 4. Configure GHDL to use LLVM backend (or GCC if you chose it)
./configure --prefix=/usr/local --with-llvm-config

# 5. Build and install
make -j$(nproc)
sudo make install
