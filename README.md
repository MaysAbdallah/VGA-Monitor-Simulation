# VGA-Monitor-Simulation
to run the vhdl part use:
python3 build.py name_of_the_test.vhdl

to run the system verilog part use:
make -f Makefile USER_MODULE=test_name

to stop the image display on the screen press Q

to use verilator /dpi and vhpi you need to install:
sudo apt update 
sudo apt install build-essential
sudo apt install verilator libsdl2-dev

for python : 
python :sudo apt install python3


for using GHDL :
GHDL version :GHDL 6.0.0-dev (5.1.1.r10.g282f20413) [Dunoon edition]
(used this to install this version 
# 1. Install required dependencies
sudo apt update
sudo apt install -y git make gnat zlib1g-dev libreadline-dev \
                    libffi-dev libgmp-dev libboost-all-dev \
                    gcc g++ python3-pip llvm clang cmake

# 2. Clone the GHDL repository
git clone https://github.com/ghdl/ghdl.git
cd ghdl

# 3. Configure GHDL to use LLVM backend (or GCC if you chose it)
./configure --prefix=/usr/local --with-llvm-config

# 4. Build and install
make -j$(nproc)
sudo make install


            shell=True, text=True
        ) 
and use this command :
 run(f"g++ -Wall -fPIC -c VGA_Monitor_VHDL.cpp -o main_vhdl.o `sdl2-config --cflags --libs` -I{ghdl_location}")
instead of {ghdl_location} put the output that he/she got when running the previous command in terminal.
e.g.  run(f"g++ -Wall -fPIC -c VGA_Monitor_VHDL.cpp -o main_vhdl.o `sdl2-config --cflags --libs` -I/usr/local/include/ghdl") 
