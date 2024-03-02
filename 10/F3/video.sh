ffmpeg -framerate 15 -pattern_type sequence -start_number 11 -i 'Output_%d.bmp' -c:v libx264 -pix_fmt yuv420p out.mp4
