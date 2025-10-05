library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.vhpi_procs.all;

entity image800_test is
    Port (
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
end entity;

architecture Behavioral of image800_test is
    signal init_done : boolean := false;

    -- Resolution constants set by this test
    constant w_const : integer := 800;
    constant h_const : integer := 600;
begin

    -- Provide resolution to top_module
    width  <= w_const;
    height <= h_const;

    process(clk, reset)
    begin
        if reset = '1' then
            red       <= (others => '0');
            green     <= (others => '0');
            blue      <= (others => '0');
            init_done <= false;

        elsif rising_edge(clk) then
            -- Send resolution to VHPI just once
            if not init_done then
                vhpi_set_resolution(w_const, h_const);
                init_done <= true;
            end if;

            if video_on = '1' then
                -- Checker size = 64 pixels
                if (pixel_x(5) xor pixel_y(5)) = '1' then
                    red   <= x"FF";
                    green <= x"FF";
                    blue  <= x"FF";
                else
                    red   <= x"00";
                    green <= x"00";
                    blue  <= x"00";
                end if;
            else
                red   <= (others => '0');
                green <= (others => '0');
                blue  <= (others => '0');
            end if;
        end if;
    end process;

end Behavioral;
