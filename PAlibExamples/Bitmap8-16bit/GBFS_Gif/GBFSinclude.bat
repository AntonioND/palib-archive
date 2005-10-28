gbfs.exe temp.gbfs *.gif
padbin 256 GBFS_Gif.nds
cat GBFS_Gif.nds temp.gbfs >boot.nds
del temp.gbfs
cat ndsmall.bin boot.nds >GBFSGif.ds.gba
pause