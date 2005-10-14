gbfs.exe temp.gbfs *.raw
padbin 256 GBFS_Sound.nds
cat GBFS_Sound.nds temp.gbfs >boot.nds
del temp.gbfs
cat ndsmall.bin boot.nds >GBFSSound.ds.gba
pause