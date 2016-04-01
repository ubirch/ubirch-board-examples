target extended-remote localhost:2331

monitor speed 1000
monitor reset

# Setup GDB FOR FASTER DOWNLOADS
set remote memory-write-packet-size 1024
set remote memory-write-packet-size fixed
