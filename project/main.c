#include "main.h"
#include "game.h"

Players players;
Board* mainBoard = NULL;
bool g_TESTMODE = false;
bool g_PRINTPIECES = false;
bool g_PRINTMOVES = false;
bool g_AUTOPLAY = false;
char* g_exportingFile = NULL;

int main(int argc, char *argv[])
{
	players.currentPlayer = PLAYER_1;
	memset(players.remainTimes, 0, sizeof(TremainTime));

	LoadSettings("settings.txt");

	if (!mainBoard)
		return 0;

	while (true)
	{
		/***
		 ** Nood pointerleri tutuan
		 **	farklý bir nood tipi oluþturalým.
		 **	/*
		 **	Coor --- Hareket bilgileri tutulan taþýn coordinatlarý
		 **	NoodP -- Hareket bilgileri bulunan Nood Head'ý
		 **	Next --- Sonrakinin pointeri
		 ** Tahta üzerinde kendimize ait bir taþ olana kadar tarayalým.
		 ** Her bir taþýmýz için:
		 **	Yapýlabilecek hamleleri alalým (control = true)
		 **	Eðer hamle sayýsý 0 dan büyükse bu listeyi nood'a ekleyelim.
		 ** Eðer nood'un büyüklüðü 0 ise mat olmuþ oluyoruz.
		 ** Diðer türlü kullanýcýdan satranct tipi koordinat alýp
		 **	kullanabileceðimiz Coor tipine dönüþtürelim.
		 * Nood üzerinden verilen coor'u bulup hareket listesini alalým.
		 **	Eðer listede yoksa
		 **	   Hamle yapýlamayacaðýný söylesin
		 **	   continue;
		 * Hareket listesini satranct tipi koor'a dönüþtürüp ekrana bastýralým.
		 * Kullanýcýdan satranc tipi koor isteyelim.
		 * Seçilen taþý deðiþtirme seçeneði de ekleyelim.
		 * Bu koordinatý kendi tipimize çevirelim.
		 *	Eðer listemizde bu koordinat var ise:
		 *	   Hareketi gerçekleþtirip, sýrayý devredelim.
		 *	Yoksa:
		 *	   Tekrar aynýsýný isteyelim.
		 *	   (Sonuçta hareket ettirmek istediði taþý deðiþtirmesi için bir seçenek ekleyeceðiz.)
		 * 
		 */

		Coor tempCoor; tempCoor.x = -1; tempCoor.y = -1;
		MoveNood* moveList = CreateMoveNood(tempCoor);
		PlayerGetAllMovements(mainBoard, players.currentPlayer, moveList);

		if (GetMoveNoodSize(moveList) <= 0)
		{
			PrintBoard(mainBoard);
			printf("You are mate!\n");
			break;
		}

		Area* fromArea = NULL;
		CoorNood* coorNood = NULL;
		Area* toArea = NULL;

		do
		{
			GetValidArea(moveList, &fromArea, &coorNood);
		} while (!GetValidMovement(coorNood, &toArea));

		MakeMovementBoard(mainBoard, &(fromArea->coor), &(toArea->coor));
		ChangePlayer();

		// Ekraný temizleyelim.
		system("cls");
	}

	system("pause");
	return 1;
}

void LoadSettings(const char* fileName)
{
	FILE *fptr;

	if ((fptr = fopen(fileName,"r")) == NULL)
		return;

	char line[255+1];
	enum SettingTypes type;
	while (fscanf(fptr, "%s ", &line)==1)
	{
		type = GetSettingType(line);
		switch (type)
		{
			case SETTING_TEST:
				fscanf(fptr, "%d\n", &g_TESTMODE);
				break;
	
			case SETTING_EXPORTING:
				fscanf(fptr, "%s\n", &line);
				g_exportingFile = malloc(sizeof(line));
				strcpy(g_exportingFile, line);
				FILE *tempFile = fopen(g_exportingFile, "w");
				fclose(tempFile);
				break;
			case SETTING_LOAD_FILE:
				fscanf(fptr, "%s\n", line);
				CreateBoard(line, &mainBoard);
				break;
			case SETTING_MAKE_FILE:
				fscanf(fptr, "%s\n", line);
				if (strcmp(g_exportingFile, line) != 0)
					MakeBoardMovements(line, mainBoard);
				break;

			case SETTING_PRINT_AVAIBLE_PIECES:
				fscanf(fptr, "%d\n", &g_PRINTPIECES);
				break;
			case SETTING_PRINT_AVAIBLE_MOVE:
				fscanf(fptr, "%d\n", &g_PRINTMOVES);
				break;
			case SETTING_AUTO_PLAY_1_MOMENT:
				fscanf(fptr, "%d\n", &g_AUTOPLAY);
				break;
			case SETTING_REMAIN_TIME:
				{
				TremainTime tempTime;
				fscanf(fptr, "%d %d\n", &tempTime.minute, &tempTime.second);

				for (size_t i = 0; i < (sizeof(players.remainTimes)/sizeof(players.remainTimes[0])); i++)
					players.remainTimes[i] = tempTime;

				break;
				}
			default:
				break;
		}
	}
}

enum SettingTypes GetSettingType(const char* buf)
{
	char * settingArray[] = { // Must be same as enum SettingTypes
		"TESTMODE",
		"EXPORTING",
		"LOAD_FILE",
		"MAKE_FILE",
		"PRINT_AVAIBLE_PIECES",
		"PRINT_AVAIBLE_MOVE",
		"AUTO_PLAY_1_MOMENT"
	};

	for (size_t i = 0; i < SETTING_MAX; i++)
		if (strcmp(settingArray[i], buf) == 0)
			return i;

	return SETTING_MAX;
}

