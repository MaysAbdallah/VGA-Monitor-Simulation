#!/usr/bin/env python3
import sys
import subprocess
import os

GHDL_STD = "--std=08"

def generate_image_wrapper(entity_name):
    wrapper = f"""\
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity image_generator is
  port (
    clk      : in  std_logic;
    reset    : in  std_logic;
    pixel_x  : in  std_logic_vector(11 downto 0);
    pixel_y  : in  std_logic_vector(11 downto 0);
    video_on : in  std_logic;
    red      : out std_logic_vector(7 downto 0);
    green    : out std_logic_vector(7 downto 0);
    blue     : out std_logic_vector(7 downto 0);
    height   : out integer;
    width    : out integer
  );
end image_generator;

architecture Structural of image_generator is
  component {entity_name}
    port (
      clk      : in  std_logic;
      reset    : in  std_logic;
      pixel_x  : in  std_logic_vector(11 downto 0);
      pixel_y  : in  std_logic_vector(11 downto 0);
      video_on : in  std_logic;
      red      : out std_logic_vector(7 downto 0);
      green    : out std_logic_vector(7 downto 0);
      blue     : out std_logic_vector(7 downto 0);
      height   : out integer;
      width    : out integer
    );
  end component;
begin
  inst : {entity_name} port map (
    clk      => clk,
    reset    => reset,
    pixel_x  => pixel_x,
    pixel_y  => pixel_y,
    video_on => video_on,
    red      => red,
    green    => green,
    blue     => blue,
    height   => height,
    width    => width
  );
end Structural;

"""
    with open("image_wrapper.vhdl", "w") as f:
        f.write(wrapper)

def run(cmd):
    result = subprocess.run(cmd, shell=True)
    if result.returncode != 0:
        print("\033[91m Command failed.\033[0m")
        sys.exit(1)

def build_cpp():
    ghdl_bin_path = subprocess.check_output(
        "which ghdl", shell=True, text=True
    ).strip()
    ghdl_include_path = ghdl_bin_path.replace("/bin/ghdl", "/include/ghdl")
    print(f"Using GHDL include path: {ghdl_include_path}")
    run(f"g++ -Wall -fPIC -c VGA_Monitor_VHDL.cpp -o main_vhdl.o `sdl2-config --cflags --libs` -I{ghdl_include_path}")



def analyze(sources):
    for src in sources:
        run(f"ghdl -a {GHDL_STD} {src}")

def elaborate(exe_name):
    run(f"ghdl -e {GHDL_STD} -o {exe_name} -Wl,main_vhdl.o -Wl,-lSDL2 -Wl,-lstdc++ top_module")

def run_sim(exe_name):
    run(f"./{exe_name}")

def clean():
    for f in os.listdir("."):
        if f.endswith(".o") or f.endswith(".cf") or f in ["top_module", "image_wrapper.vhdl"]:
            os.remove(f)
    print("\033[92m Cleaned.\033[0m")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(" Usage: python3 build.py <image_test.vhdl> or python3 build.py clean")
        sys.exit(1)

    if sys.argv[1] == "clean":
        clean()
        sys.exit(0)

    test_file = sys.argv[1]  # e.g., checkerboard_image.vhdl (name of a test )
    test_entity = os.path.splitext(os.path.basename(test_file))[0]  # checkerboard_image
    exe_name = test_entity

    generate_image_wrapper(test_entity)

    VHDL_SOURCES = [
        "vhpi_procs.vhdl",
        "vga_signal_generator.vhdl",
        test_file,
        "image_wrapper.vhdl",
        "top_module.vhdl"
    ]

    build_cpp()
    analyze(VHDL_SOURCES)
    elaborate(exe_name)
    run_sim(exe_name)
