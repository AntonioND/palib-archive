make clean
make
padbin 256 video.nds
cat video.nds 3dmoto.vds >video_tmp.nds
mv video_tmp.nds video.nds
pause