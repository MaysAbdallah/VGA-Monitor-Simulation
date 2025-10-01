
module top_module (
    input logic clk, reset,
    output  int height ,width,
    output logic hsync, vsync,
    output logic [11:0] pixel_x, pixel_y,
    output logic [7:0] red ,green ,blue ,
    output logic video_on
);
 import "DPI-C" function void render_pixel(input byte r, input byte g, input byte b, input int hsync, input int vsync);
    vga_signal_generator vga_gen (
        .clk(clk),
        .reset(reset),
        .h_count(pixel_x),
        .v_count(pixel_y),
        .h_sync(hsync),
        .v_sync(vsync),
        .pixel_enable(video_on),
        .width(width),
        .height(height)
    );
    user_image test(
        .clk(clk),
        .reset(reset),
        .pixel_x(pixel_x),
        .pixel_y(pixel_y),
        .video_on(video_on),
        //.hsync(hsync),
        //.vsync(vsync),
        .width(width),
        .height(height),
        .red(red),
        .green(green),
        .blue(blue)
    );
    always_ff @(posedge clk or reset ) begin
        render_pixel(red, green, blue, int'(hsync), int'(vsync));
    end 
endmodule
