-- vhpi_procs.vhdl
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

package vhpi_procs is
    -- VHPI procedures callable from VHDL
    procedure vhpi_startup;
    attribute foreign of vhpi_startup : procedure is "VHPIDIRECT vhpi_startup";
    procedure vhpi_set_resolution(width, height : in integer);
    attribute foreign of vhpi_set_resolution : procedure is "VHPIDIRECT vhpi_set_resolution";

    procedure vhpi_draw_pixel(r, g, b : in integer; hsync, vsync : in integer);
    attribute foreign of vhpi_draw_pixel : procedure is "VHPIDIRECT vhpi_draw_pixel";
end vhpi_procs;

package body vhpi_procs is
    -- Empty bodies
    procedure vhpi_startup is
    begin
        null;
    end procedure;
    procedure vhpi_set_resolution(width, height : in integer) is
    begin
        null;
    end procedure;

    procedure vhpi_draw_pixel(r, g, b : in integer; hsync, vsync : in integer) is
    begin
        null;
    end procedure;

end vhpi_procs;
