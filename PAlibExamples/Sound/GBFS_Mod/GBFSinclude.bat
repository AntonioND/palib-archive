gbfs.exe temp.gbfs *.mod
padbin 256 GBFS_Mod.nds
cat GBFS_Mod.nds temp.gbfs >boot.nds
del temp.gbfs
cat ndsmall.bin boot.nds >GBFSMod.ds.gba
pause