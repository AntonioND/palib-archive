gbfs.exe temp.gbfs *.txt
padbin 256 GBFS.nds
cat GBFS.nds temp.gbfs >boot.nds
del temp.gbfs
cat ndsmall.bin boot.nds >GBFS2.ds.gba
pause