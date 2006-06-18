gbfs.exe temp.gbfs images/*.bmp
padbin 256 GBFS_Bmp.nds
cat GBFS_Bmp.nds temp.gbfs >boot.nds
del temp.gbfs
cat ndsmall.bin boot.nds >GBFSBmp.ds.gba
pause