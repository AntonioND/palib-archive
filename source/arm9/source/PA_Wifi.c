#include <PA9.h>

#include <arm9/PA_Wifi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define IS_DIGIT_OR_DOT(c) (isdigit(c)||((c)=='.'))
#define IS_INETADDR(s) (IS_DIGIT_OR_DOT(s[0])&&IS_DIGIT_OR_DOT(s[1])&&IS_DIGIT_OR_DOT(s[2])&&IS_DIGIT_OR_DOT(s[3])&&IS_DIGIT_OR_DOT(s[4])&&IS_DIGIT_OR_DOT(s[5])&&IS_DIGIT_OR_DOT(s[6]))

bool PA_ConnectWifiWFC() {
	int wifiStatus = ASSOCSTATUS_DISCONNECTED;
	Wifi_AutoConnect(); // request connect

	while (wifiStatus != ASSOCSTATUS_ASSOCIATED) {
		wifiStatus = Wifi_AssocStatus(); // check status

		if (wifiStatus == ASSOCSTATUS_CANNOTCONNECT) return false;
	}

	return true;
}

unsigned long makelong(unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4) {
	return (data4 << 24) | (data3 << 16) | (data2 << 8) | data1;
}

unsigned long PA_chartoip(char *message) {
	int i, j;
	unsigned char data[4];
	int debut = 0;

	for (j = 0; j < 4; j++) {
		char buffer[256];

		for (i = debut; message[i] != '.' && message[i] != '\0'; i++) {
			buffer[i-debut] = message[i];
		}

		buffer[i-debut] = '\0';
		debut = i + 1;
		data[j] = atoi(buffer);
	}

	return makelong(data[0], data[1], data[2], data[3]);
}

int PA_InitSocket(int* sock, char *host, int port, int mode) {
	unsigned long ip;
	struct sockaddr_in servaddr;
	*sock = socket(AF_INET, SOCK_STREAM, 0);

	if (IS_INETADDR(host))
		ip = PA_chartoip(host);
	else
		ip = *(unsigned long *)gethostbyname(host)->h_addr_list[0];

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = ip;

	if (mode == PA_NORMAL_TCP) {
		if (connect(*sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) == 0)
			return 1;
	} else if (mode == PA_NONBLOCKING_TCP) {
		if (connect(*sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) == 0) {
			int i = 1;
			ioctl(*sock, FIONBIO, &i);
			return 1;
		}
	}

	return 0;
}

int PA_InitServer(int* sock, int port, int mode, int num_connect) {
	int i;
	struct sockaddr_in servaddr;
	*sock = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = 0;

	if (mode == PA_NORMAL_TCP) {
		bind(*sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
		listen(*sock, num_connect);
		return 1;
	} else if (mode == PA_NONBLOCKING_TCP) {
		bind(*sock, (struct sockaddr *)&servaddr, sizeof(servaddr));
		i = 1;
		ioctl(*sock, FIONBIO, &i);
		listen(*sock, num_connect);
		return 1;
	}

	return 0;
}

int search_word(char *mot1, char *mot2, int depart) {
	int i, j, erreur = 1;

	for (i = depart; i < strlen(mot1); i++) {
		if (mot1[i] == mot2[0]) {
			erreur = 0;

			for (j = 0; j < strlen(mot2); j++)
				if (mot2[j] != mot1[i+j])
					erreur = 1;

			if (erreur == 0)
				return i;
		}
	}

	return -1;
}

int get_HTTP_serveur(char *buffer, char *buffer2) {
	int i, depart = 0;

	if (search_word(buffer2, "http://", 0) != -1)
		depart += 7;

	for (i = depart; buffer2[i] != '\0' && buffer2[i] != '/' && buffer2[i] != '\\'; i++) {
		buffer[i-depart] = buffer2[i];
	}

	buffer[i-depart] = '\0';
	return i;
}

// LeaderBoard functions
// I had to put it in here because it gave me some weird errors bcus of redefinitions

int pa_transferScore(char *buffer, char *adress);
int pa_httpServer(char *buffer, char *buffer2);
int pa_wordSearch(char *mot1, char *mot2, int depart);

//Ideally - you shouldn't need to change any of this code.
bool PA_SubmitScore(char *response, char *gamekey, char *playername, int score, char *level, u8 responsetype) {
	bool returnval = true;

	if (PA_ConnectWifiWFC()) {
		PA_WaitForVBL();
		char url[256];
		sprintf(url, "http://leaderboard.palib.info/submitscore.php?gamekey=%s&playername=%s&levelname=%s&score=%d&responsetype=%d&end", gamekey, playername, level, score, responsetype);
		pa_transferScore(response, url);
	} else {
		returnval = false;
	}

	PA_WaitForVBL();
	Wifi_DisconnectAP();
	PA_WaitForVBL();
	Wifi_DisableWifi();
	return returnval;
}

//modified version of: int PA_GetHTTP(char *buffer, char *adress);
int pa_transferScore(char *buffer, char *adress) {
	int sock;
	char serveur[256];
	char buffer3[256];
	int pos = pa_httpServer(serveur, adress);
	PA_InitSocket(&sock, serveur, 80, PA_NONBLOCKING_TCP);
	char buffer2[256];
	sprintf(buffer2, "GET %s HTTP/1.0\r\nhost: %s\r\nAccept: */*\r\n\r\n", adress + pos, serveur);
	send(sock, buffer2, 256, 0);
	strcpy(buffer, "");

	while (pa_wordSearch(buffer3, "\r\n\r\n", 0) == -1) {
		if (Pad.Newpress.Start) break;

		memset(buffer3, 0, sizeof(buffer3));
		recv(sock, buffer3, 256, 0);
	}

	int poshtml = pa_wordSearch(buffer3, "\r\n\r\n", 0) + 4;
	strcat(buffer, buffer3 + poshtml);

	while (1) { //wordSearch(buffer,"-----",0) == -1)
		if (Pad.Newpress.Start) break;

		memset(buffer3, 0, sizeof(buffer3));

		if (recv(sock, buffer3, 256, 0) < 1) break;

		strcat(buffer, buffer3);
	}

	if (sock) closesocket(sock);

	return 1;
}

//modified version of: int get_HTTP_serveurx(char *buffer, char *buffer2);
int pa_httpServer(char *buffer, char *buffer2) {
	int i, depart = 0;

	if (pa_wordSearch(buffer2, "http://", 0) != -1)
		depart += 7;

	for (i = depart; buffer2[i] != '\0' && buffer2[i] != '/' && buffer2[i] != '\\'; i++) {
		buffer[i-depart] = buffer2[i];
	}

	buffer[i-depart] = '\0';
	return i;
}

//modified version of: int search_word(char *mot1, char *mot2, int depart);
int pa_wordSearch(char *mot1, char *mot2, int depart) {
	int i, j, erreur = 1;

	for (i = depart; i < strlen(mot1); i++) {
		if (mot1[i] == mot2[0]) {
			erreur = 0;

			for (j = 0; j < strlen(mot2); j++)
				if (mot2[j] != mot1[i+j])
					erreur = 1;

			if (erreur == 0)
				return i;
		}
	}

	return -1;
}
