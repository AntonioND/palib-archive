//Background converted using Mollusk's PAImageConverter
//This Background uses newfont_Pal

int newfont_Width = 256;
int newfont_Height = 72;

const unsigned short newfont_Map[288] __attribute__ ((aligned (4))) = {
0, 1, 2, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 
36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 
52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 
4, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 
52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

const unsigned char newfont_Tiles[4352] __attribute__ ((aligned (4))) = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 4, 2, 2, 
0, 0, 1, 4, 2, 3, 3, 3, 0, 1, 4, 2, 3, 3, 3, 3, 
0, 1, 2, 3, 3, 3, 3, 3, 1, 4, 3, 3, 3, 3, 3, 3, 
1, 2, 3, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 
1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
1, 2, 3, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 
1, 2, 3, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 
1, 2, 3, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 
1, 2, 3, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 5, 6, 5, 3, 3, 3, 3, 3, 8, 11, 8, 3, 3, 3, 
3, 3, 6, 9, 5, 3, 3, 3, 3, 3, 8, 9, 7, 3, 3, 3, 
3, 3, 5, 5, 8, 3, 3, 3, 3, 3, 6, 11, 5, 3, 3, 3, 
3, 3, 10, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 5, 7, 5, 3, 3, 3, 6, 9, 8, 11, 8, 3, 3, 
3, 5, 11, 8, 9, 10, 3, 3, 3, 8, 6, 5, 5, 6, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 8, 5, 6, 5, 6, 3, 3, 3, 8, 9, 7, 13, 8, 3, 3, 
5, 9, 11, 9, 9, 9, 8, 3, 6, 8, 15, 5, 9, 8, 5, 3, 
5, 9, 9, 9, 9, 11, 5, 3, 3, 5, 9, 8, 9, 8, 3, 3, 
3, 6, 5, 6, 5, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 8, 6, 9, 8, 10, 3, 3, 5, 7, 13, 9, 11, 5, 3, 3, 
8, 9, 8, 9, 5, 8, 3, 3, 5, 6, 11, 13, 11, 8, 6, 3, 
3, 8, 5, 9, 8, 9, 5, 3, 3, 6, 9, 15, 13, 5, 8, 3, 
3, 5, 7, 9, 6, 5, 3, 3, 3, 3, 8, 5, 8, 3, 3, 3, 
3, 5, 6, 5, 8, 6, 8, 3, 6, 9, 9, 8, 6, 13, 6, 3, 
8, 9, 9, 8, 9, 8, 12, 3, 3, 8, 6, 13, 8, 6, 3, 3, 
6, 10, 9, 5, 9, 13, 8, 3, 5, 9, 6, 5, 15, 11, 5, 3, 
5, 8, 8, 3, 6, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 11, 8, 6, 3, 3, 3, 8, 13, 6, 9, 5, 3, 3, 
3, 5, 6, 15, 5, 0, 7, 3, 10, 6, 9, 8, 9, 5, 13, 6, 
6, 15, 12, 5, 6, 9, 10, 6, 8, 6, 9, 15, 11, 8, 13, 8, 
3, 6, 8, 5, 5, 6, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 5, 5, 6, 8, 3, 3, 3, 3, 14, 9, 11, 5, 3, 3, 
3, 3, 5, 8, 9, 10, 3, 3, 3, 3, 3, 6, 5, 8, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 8, 8, 9, 6, 3, 3, 3, 6, 5, 11, 5, 8, 3, 3, 
3, 6, 15, 10, 6, 3, 3, 3, 3, 8, 11, 12, 3, 3, 3, 3, 
3, 5, 9, 8, 8, 3, 3, 3, 3, 6, 6, 9, 6, 5, 3, 3, 
3, 3, 5, 8, 9, 7, 3, 3, 3, 3, 3, 6, 8, 5, 3, 3, 
3, 3, 5, 9, 6, 5, 3, 3, 3, 3, 7, 5, 9, 8, 6, 3, 
3, 3, 3, 8, 6, 13, 14, 3, 3, 3, 3, 3, 8, 9, 6, 3, 
3, 3, 3, 5, 6, 9, 8, 3, 3, 3, 6, 8, 13, 6, 5, 3, 
3, 3, 5, 9, 6, 8, 3, 3, 3, 3, 7, 8, 5, 3, 3, 3, 
3, 3, 6, 8, 6, 3, 3, 3, 6, 6, 8, 13, 6, 8, 5, 3, 
8, 13, 5, 9, 8, 13, 6, 3, 6, 8, 11, 9, 9, 6, 8, 3, 
6, 9, 8, 9, 5, 9, 6, 3, 8, 5, 6, 13, 8, 6, 5, 3, 
3, 3, 10, 6, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 5, 12, 5, 3, 3, 3, 3, 3, 8, 9, 6, 3, 3, 3, 
5, 8, 6, 9, 7, 8, 5, 3, 8, 9, 13, 9, 9, 9, 8, 3, 
8, 6, 5, 11, 12, 8, 12, 3, 3, 3, 8, 15, 5, 3, 3, 3, 
3, 3, 6, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 12, 6, 5, 8, 3, 3, 3, 
3, 8, 13, 15, 6, 3, 3, 3, 3, 6, 6, 9, 8, 3, 3, 3, 
3, 8, 13, 6, 5, 3, 3, 3, 3, 8, 6, 5, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
6, 8, 6, 8, 5, 12, 3, 3, 5, 9, 9, 9, 9, 6, 3, 3, 
6, 8, 5, 5, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 8, 5, 3, 3, 3, 3, 3, 5, 9, 8, 3, 3, 3, 3, 
3, 7, 5, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 6, 5, 3, 3, 3, 3, 3, 7, 13, 8, 3, 
3, 3, 3, 8, 9, 8, 3, 3, 3, 3, 6, 13, 6, 3, 3, 3, 
3, 5, 9, 8, 3, 3, 3, 3, 8, 15, 5, 3, 3, 3, 3, 3, 
6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 8, 5, 6, 3, 3, 3, 6, 9, 11, 9, 5, 3, 3, 
3, 5, 9, 8, 9, 6, 3, 3, 3, 6, 11, 5, 15, 6, 3, 3, 
3, 5, 9, 6, 9, 8, 3, 3, 3, 8, 9, 9, 9, 6, 3, 3, 
3, 6, 8, 5, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 6, 6, 5, 3, 3, 3, 3, 3, 8, 9, 5, 3, 3, 
3, 3, 3, 5, 9, 6, 3, 3, 3, 3, 3, 10, 9, 8, 3, 3, 
3, 3, 3, 6, 15, 12, 3, 3, 3, 3, 3, 8, 9, 5, 3, 3, 
3, 3, 3, 5, 6, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 6, 8, 8, 5, 3, 3, 3, 8, 9, 13, 11, 14, 3, 3, 
3, 7, 8, 5, 9, 5, 3, 3, 3, 5, 9, 9, 15, 5, 3, 3, 
3, 10, 11, 12, 5, 8, 3, 3, 3, 5, 15, 9, 9, 6, 3, 3, 
3, 6, 5, 5, 5, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 12, 8, 5, 8, 3, 3, 3, 6, 9, 9, 9, 5, 3, 3, 
3, 6, 8, 5, 9, 8, 3, 3, 3, 5, 11, 9, 9, 6, 3, 3, 
3, 5, 6, 8, 13, 8, 3, 3, 3, 8, 9, 9, 11, 6, 3, 3, 
3, 5, 8, 7, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 8, 5, 6, 5, 3, 3, 3, 10, 9, 6, 9, 7, 3, 3, 
3, 12, 11, 8, 9, 5, 3, 3, 3, 8, 13, 9, 13, 6, 3, 3, 
3, 8, 6, 8, 9, 8, 3, 3, 3, 3, 3, 8, 11, 6, 3, 3, 
3, 3, 3, 5, 7, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 8, 5, 10, 6, 3, 3, 3, 6, 9, 11, 9, 8, 3, 3, 
3, 14, 9, 8, 5, 5, 3, 3, 3, 6, 13, 9, 9, 6, 3, 3, 
3, 8, 8, 6, 9, 8, 3, 3, 3, 5, 9, 9, 13, 6, 3, 3, 
3, 5, 6, 5, 8, 10, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 8, 5, 8, 5, 3, 3, 3, 5, 9, 11, 9, 8, 3, 3, 
3, 6, 9, 5, 8, 6, 3, 3, 3, 8, 9, 9, 9, 5, 3, 3, 
3, 6, 11, 8, 9, 5, 3, 3, 3, 5, 13, 9, 9, 8, 3, 3, 
3, 8, 8, 6, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 5, 6, 8, 3, 3, 3, 6, 9, 9, 11, 5, 3, 3, 
3, 5, 5, 6, 9, 8, 3, 3, 3, 3, 3, 8, 9, 6, 3, 3, 
3, 3, 3, 8, 9, 5, 3, 3, 3, 3, 3, 6, 9, 6, 3, 3, 
3, 3, 3, 5, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 8, 5, 8, 3, 3, 3, 6, 9, 9, 9, 5, 3, 3, 
3, 5, 9, 6, 9, 6, 3, 3, 3, 6, 9, 13, 9, 7, 3, 3, 
3, 5, 15, 6, 15, 5, 3, 3, 3, 6, 13, 9, 9, 7, 3, 3, 
3, 8, 8, 6, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 7, 5, 5, 6, 3, 3, 3, 8, 11, 15, 13, 6, 3, 3, 
3, 8, 9, 10, 9, 8, 3, 3, 3, 5, 13, 11, 9, 5, 3, 3, 
3, 7, 8, 5, 9, 8, 3, 3, 3, 5, 9, 15, 9, 6, 3, 3, 
3, 5, 6, 5, 7, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 6, 5, 3, 3, 3, 3, 7, 13, 9, 6, 3, 3, 3, 
3, 5, 9, 9, 8, 3, 3, 3, 3, 5, 6, 8, 5, 3, 3, 3, 
3, 6, 9, 11, 5, 3, 3, 3, 3, 8, 9, 9, 8, 3, 3, 3, 
3, 6, 5, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 8, 10, 6, 3, 3, 3, 3, 8, 11, 13, 6, 3, 3, 3, 
3, 6, 9, 9, 5, 3, 3, 3, 3, 5, 6, 8, 6, 3, 3, 3, 
3, 7, 9, 9, 5, 3, 3, 3, 3, 8, 5, 9, 8, 3, 3, 3, 
3, 6, 11, 5, 6, 3, 3, 3, 3, 5, 8, 7, 3, 3, 3, 3, 
3, 3, 3, 3, 8, 6, 3, 3, 3, 3, 3, 5, 9, 5, 3, 3, 
3, 3, 5, 9, 12, 3, 3, 3, 3, 12, 9, 6, 3, 3, 3, 3, 
3, 3, 6, 13, 8, 3, 3, 3, 3, 3, 3, 6, 9, 8, 3, 3, 
3, 3, 3, 3, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 5, 6, 8, 5, 8, 6, 3, 3, 
6, 15, 13, 11, 15, 8, 3, 3, 8, 8, 6, 5, 5, 6, 3, 3, 
12, 13, 9, 9, 9, 6, 3, 3, 5, 8, 6, 8, 5, 8, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 6, 8, 3, 3, 3, 3, 3, 3, 6, 9, 6, 3, 3, 3, 
3, 3, 3, 10, 9, 8, 3, 3, 3, 3, 3, 3, 5, 11, 5, 3, 
3, 3, 3, 6, 9, 6, 3, 3, 3, 3, 5, 15, 8, 3, 3, 3, 
3, 3, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 6, 6, 6, 8, 3, 3, 3, 6, 5, 9, 13, 5, 6, 3, 
3, 10, 9, 8, 12, 11, 8, 3, 3, 12, 5, 6, 8, 13, 6, 3, 
3, 3, 3, 5, 9, 6, 8, 3, 3, 3, 3, 5, 6, 5, 3, 3, 
3, 3, 3, 7, 15, 6, 3, 3, 3, 3, 3, 5, 5, 6, 3, 3, 
3, 3, 8, 3, 15, 13, 6, 3, 3, 6, 9, 8, 5, 6, 9, 8, 
6, 15, 5, 6, 9, 9, 12, 3, 10, 9, 6, 15, 5, 9, 5, 3, 
5, 11, 12, 9, 11, 9, 8, 3, 3, 8, 13, 8, 5, 5, 9, 8, 
3, 3, 6, 9, 9, 15, 8, 3, 3, 3, 3, 8, 8, 6, 3, 3, 
3, 3, 6, 8, 12, 5, 3, 3, 3, 8, 5, 15, 9, 8, 6, 3, 
3, 5, 11, 8, 6, 11, 8, 3, 3, 8, 9, 9, 13, 9, 5, 3, 
3, 5, 11, 8, 12, 15, 6, 3, 3, 6, 9, 5, 8, 11, 5, 3, 
3, 8, 8, 6, 12, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 8, 5, 6, 3, 3, 3, 3, 8, 9, 9, 9, 10, 3, 3, 
3, 6, 9, 8, 8, 11, 5, 3, 3, 8, 9, 13, 9, 8, 8, 3, 
3, 5, 9, 5, 6, 13, 7, 3, 3, 6, 9, 15, 9, 9, 6, 3, 
3, 10, 5, 6, 6, 5, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 10, 8, 6, 12, 3, 3, 3, 5, 5, 11, 9, 8, 5, 3, 
3, 6, 9, 6, 8, 9, 5, 3, 3, 8, 11, 10, 5, 6, 8, 3, 
3, 5, 9, 5, 7, 9, 5, 3, 3, 6, 8, 9, 11, 6, 5, 3, 
3, 3, 6, 5, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 12, 8, 6, 5, 8, 3, 3, 3, 5, 9, 11, 9, 10, 6, 3, 
3, 8, 9, 5, 8, 9, 8, 3, 3, 6, 9, 9, 6, 9, 6, 3, 
3, 8, 13, 6, 5, 9, 8, 3, 3, 6, 9, 15, 9, 12, 5, 3, 
3, 8, 5, 5, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 5, 6, 5, 3, 3, 3, 3, 6, 9, 9, 15, 5, 3, 3, 
3, 6, 13, 8, 12, 8, 3, 3, 3, 5, 15, 9, 11, 8, 3, 3, 
3, 7, 9, 10, 5, 5, 3, 3, 3, 8, 11, 9, 9, 8, 3, 3, 
3, 5, 5, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 8, 5, 8, 12, 5, 3, 3, 3, 6, 11, 13, 9, 6, 3, 3, 
3, 8, 9, 8, 8, 8, 3, 3, 3, 6, 13, 9, 9, 12, 3, 3, 
3, 8, 9, 8, 6, 5, 3, 3, 3, 5, 15, 5, 3, 3, 3, 3, 
3, 6, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 8, 5, 8, 6, 3, 3, 3, 8, 12, 11, 9, 8, 3, 3, 
3, 5, 9, 6, 8, 5, 6, 3, 3, 6, 9, 8, 9, 11, 8, 3, 
3, 8, 13, 6, 8, 9, 5, 3, 3, 6, 8, 9, 13, 5, 6, 3, 
3, 3, 10, 6, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 6, 8, 5, 6, 7, 3, 3, 6, 9, 5, 6, 15, 6, 3, 
3, 8, 13, 8, 8, 9, 5, 3, 3, 6, 15, 11, 9, 11, 8, 3, 
3, 5, 9, 5, 8, 9, 8, 3, 3, 7, 11, 5, 6, 13, 6, 3, 
3, 6, 5, 8, 8, 6, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 5, 6, 7, 3, 3, 3, 3, 3, 8, 13, 5, 3, 3, 3, 
3, 3, 8, 11, 8, 3, 3, 3, 3, 3, 5, 13, 6, 3, 3, 3, 
3, 3, 7, 11, 8, 3, 3, 3, 3, 3, 5, 9, 10, 3, 3, 3, 
3, 3, 8, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 8, 6, 5, 3, 3, 3, 3, 3, 5, 9, 6, 3, 3, 
3, 3, 3, 8, 9, 6, 3, 3, 3, 8, 5, 6, 15, 12, 3, 3, 
3, 12, 9, 5, 9, 5, 3, 3, 3, 6, 5, 9, 8, 6, 3, 3, 
3, 3, 6, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 6, 5, 7, 8, 6, 3, 3, 8, 9, 5, 6, 9, 8, 3, 
3, 5, 9, 6, 15, 5, 6, 3, 3, 8, 11, 9, 8, 6, 3, 3, 
3, 6, 15, 5, 11, 12, 8, 3, 3, 5, 9, 6, 8, 13, 8, 3, 
3, 7, 6, 8, 10, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 8, 3, 3, 3, 3, 3, 8, 15, 5, 3, 3, 3, 3, 
3, 12, 9, 6, 3, 3, 3, 3, 3, 5, 9, 5, 3, 3, 3, 3, 
3, 8, 9, 7, 5, 6, 3, 3, 3, 6, 9, 13, 9, 5, 3, 3, 
3, 8, 6, 12, 8, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
5, 6, 8, 3, 5, 8, 5, 3, 8, 9, 5, 6, 8, 9, 5, 3, 
6, 9, 15, 5, 11, 9, 8, 3, 10, 9, 6, 9, 8, 9, 5, 3, 
8, 9, 8, 5, 5, 9, 5, 3, 6, 13, 8, 6, 6, 9, 8, 3, 
8, 6, 8, 3, 10, 5, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 8, 12, 8, 6, 10, 8, 3, 3, 6, 15, 6, 5, 9, 6, 3, 
3, 5, 9, 9, 6, 9, 6, 3, 3, 8, 9, 5, 15, 11, 10, 3, 
3, 7, 9, 7, 5, 9, 5, 3, 3, 6, 13, 8, 6, 9, 8, 3, 
3, 8, 6, 8, 5, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 8, 6, 12, 8, 3, 3, 3, 5, 8, 11, 9, 8, 6, 3, 
3, 5, 9, 5, 8, 9, 5, 3, 3, 8, 9, 5, 6, 9, 8, 3, 
3, 5, 9, 5, 8, 13, 6, 3, 3, 5, 6, 15, 9, 6, 8, 3, 
3, 3, 6, 5, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 8, 5, 5, 3, 3, 3, 6, 9, 15, 9, 8, 6, 3, 
3, 8, 9, 6, 6, 13, 8, 3, 3, 6, 13, 9, 9, 8, 5, 3, 
3, 8, 9, 8, 7, 6, 3, 3, 3, 5, 11, 5, 3, 3, 3, 3, 
3, 8, 10, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 6, 5, 8, 5, 3, 3, 3, 6, 8, 11, 9, 6, 5, 3, 
3, 5, 9, 5, 8, 9, 8, 3, 3, 6, 9, 5, 6, 13, 6, 3, 
3, 6, 15, 6, 9, 6, 8, 3, 3, 5, 12, 9, 5, 15, 5, 3, 
3, 3, 8, 5, 6, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 5, 8, 5, 5, 3, 3, 3, 3, 8, 9, 11, 9, 12, 3, 3, 
3, 5, 13, 8, 5, 9, 6, 3, 3, 6, 11, 9, 9, 5, 8, 3, 
3, 8, 9, 5, 8, 13, 6, 3, 3, 5, 9, 6, 6, 15, 5, 3, 
3, 7, 8, 6, 8, 10, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 6, 5, 6, 6, 8, 3, 3, 6, 10, 15, 11, 9, 5, 3, 
3, 6, 15, 5, 8, 12, 8, 3, 3, 8, 5, 11, 9, 8, 3, 3, 
3, 3, 6, 5, 5, 9, 5, 3, 3, 6, 15, 9, 9, 6, 5, 3, 
3, 6, 10, 5, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
6, 7, 5, 8, 6, 5, 6, 3, 8, 9, 15, 9, 9, 15, 5, 3, 
5, 5, 5, 13, 6, 8, 6, 3, 3, 3, 8, 9, 8, 3, 3, 3, 
3, 3, 12, 11, 5, 3, 3, 3, 3, 3, 5, 15, 5, 3, 3, 3, 
3, 3, 5, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 8, 6, 8, 5, 8, 3, 3, 8, 13, 5, 8, 9, 5, 3, 
3, 6, 9, 6, 5, 9, 8, 3, 3, 8, 9, 7, 8, 9, 6, 3, 
3, 5, 9, 8, 12, 9, 5, 3, 3, 6, 8, 9, 9, 5, 6, 3, 
3, 3, 12, 6, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
5, 8, 6, 3, 6, 6, 5, 3, 6, 9, 5, 3, 8, 9, 5, 3, 
6, 9, 8, 3, 5, 9, 6, 3, 12, 9, 6, 8, 8, 11, 10, 3, 
5, 8, 9, 5, 9, 8, 5, 3, 3, 8, 5, 15, 5, 5, 3, 3, 
3, 3, 5, 8, 7, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
8, 6, 8, 3, 6, 5, 8, 3, 5, 9, 5, 3, 8, 15, 5, 3, 
5, 9, 12, 8, 12, 9, 6, 3, 6, 15, 5, 9, 5, 9, 5, 3, 
12, 9, 6, 9, 8, 9, 7, 3, 5, 8, 9, 5, 9, 8, 5, 3, 
3, 6, 5, 7, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
6, 8, 12, 3, 6, 8, 8, 3, 5, 15, 5, 8, 6, 9, 6, 3, 
12, 8, 9, 6, 9, 8, 5, 3, 3, 6, 5, 9, 5, 6, 3, 3, 
6, 8, 9, 8, 9, 8, 5, 3, 5, 13, 8, 6, 5, 9, 6, 3, 
8, 6, 12, 3, 6, 8, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
8, 5, 6, 3, 8, 12, 8, 3, 6, 9, 5, 10, 6, 15, 6, 3, 
12, 8, 15, 6, 9, 5, 8, 3, 3, 5, 6, 9, 8, 6, 3, 3, 
3, 3, 6, 13, 7, 3, 3, 3, 3, 3, 8, 9, 6, 3, 3, 3, 
3, 3, 6, 5, 8, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 8, 6, 12, 6, 8, 6, 3, 3, 6, 9, 9, 13, 15, 6, 3, 
3, 6, 8, 6, 5, 9, 5, 3, 3, 3, 3, 6, 9, 6, 3, 3, 
3, 3, 7, 9, 5, 8, 5, 3, 3, 8, 11, 9, 9, 11, 8, 3, 
3, 5, 5, 5, 8, 6, 5, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 8, 6, 5, 3, 3, 3, 6, 9, 9, 11, 5, 3, 3, 
3, 5, 15, 6, 8, 8, 3, 3, 3, 8, 9, 5, 3, 3, 3, 3, 
3, 6, 9, 8, 8, 6, 3, 3, 3, 8, 9, 9, 13, 6, 3, 3, 
3, 5, 6, 6, 8, 10, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 6, 5, 3, 3, 3, 3, 3, 3, 6, 15, 5, 3, 3, 3, 3, 
3, 3, 12, 9, 5, 3, 3, 3, 3, 3, 3, 8, 11, 5, 3, 3, 
3, 3, 3, 3, 6, 15, 6, 3, 3, 3, 3, 3, 3, 12, 9, 5, 
3, 3, 3, 3, 3, 3, 5, 6, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 12, 5, 5, 8, 5, 3, 3, 3, 6, 11, 9, 9, 6, 3, 
3, 3, 8, 8, 5, 13, 8, 3, 3, 3, 3, 3, 6, 11, 5, 3, 
3, 3, 6, 5, 8, 9, 5, 3, 3, 3, 8, 9, 9, 9, 6, 3, 
3, 3, 7, 5, 7, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 8, 6, 5, 3, 3, 3, 3, 6, 5, 9, 6, 10, 3, 3, 
3, 6, 15, 5, 9, 8, 3, 3, 3, 8, 5, 6, 8, 5, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
8, 5, 8, 5, 8, 12, 6, 5, 6, 9, 9, 13, 11, 13, 15, 5, 
5, 8, 8, 6, 8, 5, 8, 6, 3, 3, 3, 3, 3, 3, 3, 3
};

