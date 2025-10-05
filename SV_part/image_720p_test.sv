module image_720p_test (
    input  logic clk,           // Pixel clock
    input  logic reset,         // Reset signal
    input  logic [11:0] pixel_x, pixel_y,
    input  logic video_on,
    output logic [7:0] red,
    output logic [7:0] green,
    output logic [7:0] blue,
    output int height, width
);

    import "DPI-C" function void set_resolution(input int w, input int h);

    initial begin
        width  = 1280;
        height = 960;
        $display("Verilog 1280x720 test is executing!");
        set_resolution(width, height);
    end

    always_ff @(posedge clk or reset) begin
        if (video_on) begin
            red   = pixel_x[7:0];           // Horizontal gradient
            green = 8'h00;
            blue  = 8'hFF - pixel_x[7:0];   // Inverse gradient
        end else begin
            red   = 8'h00;
            green = 8'h00;
            blue  = 8'h00;
        end
    end

endmodule
