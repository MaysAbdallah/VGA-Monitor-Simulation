module user_image(
    input logic clk,
    input logic reset,
    input logic video_on , 
    input logic [11:0] pixel_x,pixel_y,
    output logic [7:0] red ,green ,blue,
    int height ,width
);
 import "DPI-C" function void set_resolution(input int w, input int h);
 //here add you image , before drawing the image use set_resolution
/*for example :
   initial begin
        width  = 1280;
        height = 720;
        set_resolution(width, height);
    end

    always_ff @(posedge clk or reset) begin
        if (video_on) begin
            red   = pixel_x[7:0];         
            green = 8'h00;
            blue  = 8'hFF - pixel_x[7:0];  
        end else begin
            red   = 8'h00;
            green = 8'h00;
            blue  = 8'h00;
        end
    end
*/
endmodule 
