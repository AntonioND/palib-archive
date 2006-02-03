mkdir bmp
mkdir frm
rm frm/*.frm
viDeoconverterS MyVideo.avi -f 16 -x 256 -y 192 -c GOOD -n MyVideo
rm bmp/*.bmp
pause