library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.vhpi_procs.all;

entity top_module is
end entity;

architecture Behavioral of top_module is
    signal clk_internal   : std_logic := '0';
    signal reset_internal : std_logic := '1';
    signal quit_request : boolean := false;
    signal h_cnt, v_cnt     : std_logic_vector(11 downto 0);
    signal r_sig, g_sig, b_sig : std_logic_vector(7 downto 0);
    signal h_sync_sig, v_sync_sig, video_en : std_logic;
    signal h_sync_d, v_sync_d : std_logic;

    signal w, h : integer;

    component vga_signal_generator
        Port (
            clk     : in  std_logic;
            reset   : in  std_logic;
            h_count : out std_logic_vector(11 downto 0);
            v_count : out std_logic_vector(11 downto 0);
            h_sync  : out std_logic;
            v_sync  : out std_logic;
            pixel_enable : out std_logic;
            width   : integer;
            height  : integer
        );
    end component;

    component image_generator
        Port (
            clk      : in  std_logic;
            reset    : in  std_logic;
            pixel_x  : in  std_logic_vector(11 downto 0);
            pixel_y  : in  std_logic_vector(11 downto 0);
            video_on : in  std_logic;
            red      : out std_logic_vector(7 downto 0);
            green    : out std_logic_vector(7 downto 0);
            blue     : out std_logic_vector(7 downto 0);
            width    : out integer;
            height   : out integer
        );
    end component;

begin

    clk_process : process
    begin
        while true loop
            clk_internal <= '0'; wait for 1 ns;
            clk_internal <= '1'; wait for 1 ns;
        end loop;
    end process;

    reset_process : process
    begin
        wait for 4 ns;
        reset_internal <= '0';
        wait;
    end process;

    vga_inst : vga_signal_generator
        port map (
            clk     => clk_internal,
            reset   => reset_internal,
            h_count => h_cnt,
            v_count => v_cnt,
            h_sync  => h_sync_sig,
            v_sync  => v_sync_sig,
            pixel_enable => video_en,
            width   => w,
            height  => h
        );

    image_inst : image_generator
        port map (
            clk      => clk_internal,
            reset    => reset_internal,
            pixel_x  => h_cnt,
            pixel_y  => v_cnt,
            video_on => video_en,
            red      => r_sig,
            green    => g_sig,
            blue     => b_sig,
            width    => w,
            height   => h
        );

    sync_delay_proc : process(clk_internal)
    begin
        if rising_edge(clk_internal) then
            h_sync_d <= h_sync_sig;
            v_sync_d <= v_sync_sig;
        end if;
    end process;

    draw_proc : process(clk_internal)
        variable h_val, v_val : integer;
    begin
        if rising_edge(clk_internal) then
            if reset_internal = '0' then
                h_val := 1 when h_sync_d = '1' else 0;
                v_val := 1 when v_sync_d = '1' else 0;

                vhpi_draw_pixel(
                    to_integer(unsigned(r_sig)),
                    to_integer(unsigned(g_sig)),
                    to_integer(unsigned(b_sig)),
                    h_val,
                    v_val
                );
            end if;
        end if;
        
    end process;
quit_proc : process
begin
    wait;  -- Wait forever, never wakes up
end process;

   

end Behavioral;
