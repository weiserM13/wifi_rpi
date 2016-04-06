#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EOL '\n'
#define SC '"'

int countLines(FILE *fp);
void formatEssid(int lines, char essid[lines][30]);
void formatWPAtext(char essid[30], char pass[30], char wpa_text[100]);

int main(int argc, char *argv[]){
	system("iwlist wlan0 scan | grep ESSID: > wifi.txt");

	FILE *wifi_file;
	if((wifi_file = fopen("wifi.txt", "r")) < 0){
		printf("Error al abrir wifi.txt\n");
	}

	int lines = countLines(wifi_file);
	if(lines == 0){
		printf("No WiFi founded.\n");
		exit(-1);
	}

	char essid[lines][30];

	int i;
	for(i = 0; i < lines; i++){
		fscanf(wifi_file, "%s", essid[i]);
	}
	formatEssid(lines, essid);

	printf("Founded WiFi:\n");
	for(i = 0; i < lines; i++){
		printf("%d. %s\n", i+1, essid[i]);
	}

	int opt;
	printf("\nChoose Wifi to connect: (1-%d)\n", lines);
	scanf("%d", &opt);

	if(opt > lines){
		printf("Wrong number\n");
		exit(-1);
	}

	char pass[30];
	printf("\nInsert the password:\n");
	scanf("%s", pass);
	
	char wpa_text[100];
	formatWPAtext(essid[opt-1], pass, wpa_text);
	printf("\n\n%s\n", wpa_text);

	return 0;
}

int countLines(FILE *fp){
	int lines = 0;
	char c;
	while(!feof(fp)){
		c = fgetc(fp);
		if(c == '\n'){
			lines++;
		}
	}
	fseek(fp, 0, SEEK_SET);
	return lines;
}

void formatEssid(int lines, char essid[lines][30]){
	int i, j, l;

	for(i = 0; i < lines; i++){
		l = strlen(essid[i]);
		for(j = 0; j < l; j++){
			if(j < (l-6)){
				essid[i][j] = essid[i][j+6];
			}
			else{
				essid[i][j] = '\0';
			}	
		}
	}
}

void formatWPAtext(char essid[30], char pass[30], char wpa_text[100]){
	strcpy(wpa_text, "network={\n\tssid=");
	strcat(wpa_text, essid);
	strcat(wpa_text, "\n\tpsk=\"");
	strcat(wpa_text, pass);
	strcat(wpa_text, "\"\n}");
}