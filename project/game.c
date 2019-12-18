#include "game.h"

/* Board */
void ClearBoard(Board* board)
{
	Area tempArea;
	tempArea.piece.type = PIECE_EMPTY;
	tempArea.piece.owner = PLAYER_NONE;
	tempArea.piece.moveCount = 0;
	for (int i=0; i < GRID_SIZE; i++)
	{
		for (int j=0; j < GRID_SIZE; j++)
		{
			tempArea.coor.x = i;
			tempArea.coor.y = j;

			board->board[i][j] = tempArea;
		}
	}
}
bool CreateBoard(const char* fileName, Board** board)
{
	Board* tempBoard = malloc(sizeof(Board));

	FILE* fptr;

	ClearBoard(tempBoard);

	if ((fptr = fopen(fileName,"r")) == NULL)
		return false;

	Area tempArea;
	tempArea.piece.moveCount = 0;

	char line[7+1];
	int i = 1;
	while (fgets(line, 9, fptr) != NULL)
	{
		// printTEST(">%s<", line);
		if (sscanf(line, "%d %d %d %d\n", &tempArea.coor.x, &tempArea.coor.y, &tempArea.piece.type, &tempArea.piece.owner) != 4)
		{
			printTEST("File importing (conversation) failed (line: %d)", i++);
			continue;
		}

		if (!IsReelCoor(&(tempArea.coor)))
		{
			printTEST("File importing (not reel coor) failed (line: %d)", i++);
			continue;
		}

		tempBoard->board[tempArea.coor.x][tempArea.coor.y] = tempArea;

		i++;
	}

	*board = tempBoard;

	printTEST("File importing succes (total line: %d)", i);

	fclose(fptr);

	return true;
}
void PrintBoard(Board* board)
{
	Area tempArea;
	for (int j=GRID_SIZE-1; j > -1; j--)
	{
		printf("(%d)	", (j+1));
		for (int i=0; i < GRID_SIZE; i++)
		{
			tempArea = board->board[i][j];
			char name[2];
			GetPieceName(&((&tempArea)->piece), name);
			printf("(%c%c) ", name[0], name[1]);
		}
		printf("\n\n\n");
	}
	printf("\n	(%c)  (%c)  (%c)  (%c)  (%c)  (%c)  (%c)  (%c)\n\n\n\n", 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
}
bool IsReelCoor(Coor* coor)
{
	return (coor->x >= 0 && coor->x < GRID_SIZE && coor->y >= 0 && coor->y < GRID_SIZE);
}
Area* CoorToArea(Board* board, Coor* coor)
{
	return &(board->board[coor->x][coor->y]);
}
bool UserInputToCoor(char* userInput, Coor* coor)
{
	// Doðrudan ascii tablosundan da yapýlabilir.
	// 00 is special
	switch (userInput[0])
	{
		case 'a': case 'A': coor->x = 0; break;
		case 'b': case 'B': coor->x = 1; break;
		case 'c': case 'C': coor->x = 2; break;
		case 'd': case 'D': coor->x = 3; break;
		case 'e': case 'E': coor->x = 4; break;
		case 'f': case 'F': coor->x = 5; break;
		case 'g': case 'G': coor->x = 6; break;
		case 'h': case 'H': coor->x = 7; break;
		case '0': coor->x = GRID_SIZE; break;
		
		default: return false;
	}
	switch (userInput[1])
	{
		case '1': coor->y = 0; break;
		case '2': coor->y = 1; break;
		case '3': coor->y = 2; break;
		case '4': coor->y = 3; break;
		case '5': coor->y = 4; break;
		case '6': coor->y = 5; break;
		case '7': coor->y = 6; break;
		case '8': coor->y = 7; break;
		case '0': coor->y = GRID_SIZE; break;
		
		default: return false;
	}

	return true;
}
void CoorToUserInput(char* userInput, Coor* coor)
{
	switch (coor->x)
	{
		case 0: userInput[0] = 'a'; break;
		case 1: userInput[0] = 'b'; break;
		case 2: userInput[0] = 'c'; break;
		case 3: userInput[0] = 'd'; break;
		case 4: userInput[0] = 'e'; break;
		case 5: userInput[0] = 'f'; break;
		case 6: userInput[0] = 'g'; break;
		case 7: userInput[0] = 'h'; break;
		
		default:
			break;
	}
	switch (coor->y)
	{
		case 0: userInput[1] = '1'; break;
		case 1: userInput[1] = '2'; break;
		case 2: userInput[1] = '3'; break;
		case 3: userInput[1] = '4'; break;
		case 4: userInput[1] = '5'; break;
		case 5: userInput[1] = '6'; break;
		case 6: userInput[1] = '7'; break;
		case 7: userInput[1] = '8'; break;
		
		default: break;
	}
}
void MakeBoardMovements(const char* line, Board* mainBoard)
{
	FILE *fptr;

	if ((fptr = fopen(line, "r")) == NULL)
		return;

	Coor coor1, coor2;

	while (fscanf(fptr, "%d %d %d %d\n", &coor1.x, &coor1.y, &coor2.x, &coor2.y)==4)
	{
		MakeMovementBoard(mainBoard, &coor1, &coor2);
		ChangePlayer();
	}
}

/* Player */
void GetMovementList(Board* board, Area* area, CoorNood* list, bool withControl, bool forState)
{
	switch (area->piece.type)
	{
		case PIECE_PAWN:
		{
			/* Ýki birim ilerleme */
			if (IsDefaultPos(area))
			{
				Coor addCoor = area->coor;
				int max;
				if (area->piece.owner == PLAYER_1)
				{
					addCoor.y += 2;
					max = GetMaxMoveLen(board, &(area->coor), MOVE_U);
				}
				else
				{
					addCoor.y -= 2;
					max = GetMaxMoveLen(board, &(area->coor), MOVE_D);
				}

				if (max >= 2 && CoorToArea(board, &addCoor)->piece.owner == PLAYER_NONE)
				{
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &addCoor), area->piece.owner))
							AddNood(list, addCoor);
					}
					else
						AddNood(list, addCoor);
				}
			}
			/* Bir birim ilerleme */
			{
				Coor addCoor = area->coor;
				int max;
				if (area->piece.owner == PLAYER_1)
					addCoor.y += 1;
				else
					addCoor.y -= 1;

				if (CoorToArea(board, &addCoor)->piece.owner == PLAYER_NONE)
				{
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &addCoor), area->piece.owner))
							AddNood(list, addCoor);
					}
					else
						AddNood(list, addCoor);
				}
			}
			/* Saldýrý Hamleleri */
			{
				Coor tempCoors[2];
				switch (area->piece.owner)
				{
					case PLAYER_1:
					{
						/* Sað üste saldýrý */
						tempCoors[0].x = (area->coor.x)+1; tempCoors[0].y = (area->coor.y)+1;
						/* Sol üste saldýrý */
						tempCoors[1].x = (area->coor.x)-1; tempCoors[1].y = (area->coor.y)+1;
						break;
					}
					case PLAYER_2:
					{
						/* Sað alta saldýrý */
						tempCoors[0].x = (area->coor.x)+1; tempCoors[0].y = (area->coor.y)-1;
						/* Sol alta saldýrý */
						tempCoors[1].x = (area->coor.x)-1; tempCoors[1].y = (area->coor.y)-1;
						break;
					}
				
					default:
					{
						printTEST("Unknow owner type (%d)", area->piece.owner);
						break;
					}
				}
				FOR_ARRAY(tempCoors)
				{
					Coor tempCoor = tempCoors[i];
					if (IsReelCoor(&tempCoor))
					{
						if (IsEnemyArea(area->piece.owner, &(board->board[tempCoor.x][tempCoor.y])))
						{
							if (withControl)
							{
								if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
									AddNood(list, tempCoor);
							}
							else
								AddNood(list, tempCoor);
						}
						else if(!IsMyArea(area->piece.owner, &(board->board[tempCoor.x][tempCoor.y])))
						{
							Coor nearCoor;
							nearCoor.x = tempCoor.x;
							bool canAttack = true;
							switch (area->piece.owner)
							{
								case PLAYER_1:
								{
									if (area->coor.y != 4)
									{
										canAttack = false;
										break;
									}
									nearCoor.y = tempCoor.y-1;
									break;
								}
								case PLAYER_2:
								{
									if (area->coor.y != 3)
									{
										canAttack = false;
										break;
									}
									nearCoor.y = tempCoor.y+1;
									break;
								}
								default:
								{
									canAttack = false;
									break;
								}
							}

							if (!canAttack)
								continue;

							// Piyonun sadece bir kere oynanmis oynanmis olmasi gerek.
							if (CoorToArea(board, &nearCoor)->piece.moveCount != 1)
								continue;

							// Piyon, en son oynanan tas olmali
							Coor lastCoor = players.lastMove[(CoorToArea(board, &nearCoor)->piece.owner)-1];
							if (nearCoor.x != lastCoor.x || nearCoor.y != lastCoor.y)
								continue;

							if (withControl)
							{
								if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
									AddNood(list, tempCoor);
							}
							else
								AddNood(list, tempCoor);
						}
					}
				}
			}
			break;
		}
		case PIECE_QUEEN:
		case PIECE_ROCK:
		{
			Coor tempCoor;

			{ // Move up
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				size_t moveSize = GetMaxMoveLen(board, &(area->coor), MOVE_U);
				for (int i=0; i<moveSize; i++)
				{
					tempCoor.y += 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			{ // Move down
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				size_t moveSize = GetMaxMoveLen(board, &(area->coor), MOVE_D);
				for (int i=0; i<moveSize; i++)
				{
					tempCoor.y -= 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			{ // Move right
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				size_t moveSize = GetMaxMoveLen(board, &(area->coor), MOVE_R);
				for (int i=0; i<moveSize; i++)
				{
					tempCoor.x += 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			{ // Move left
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				size_t moveSize = GetMaxMoveLen(board, &(area->coor), MOVE_L);
				for (int i=0; i<moveSize; i++)
				{
					tempCoor.x -= 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			if (area->piece.type == PIECE_ROCK)
				break;
		}
		case PIECE_BISHOP:
		{
			Coor tempCoor;

			{ // Move right up
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				for (int i=0; i<GetMaxMoveLen(board, &(area->coor), MOVE_RU); i++)
				{
					tempCoor.x += 1;
					tempCoor.y += 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			{ // Move left up
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				for (int i=0; i<GetMaxMoveLen(board, &(area->coor), MOVE_LU); i++)
				{
					tempCoor.x -= 1;
					tempCoor.y += 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			{ // Move right down
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				for (int i=0; i<GetMaxMoveLen(board, &(area->coor), MOVE_RD); i++)
				{
					tempCoor.x += 1;
					tempCoor.y -= 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			{ // Move left down
				tempCoor.x = area->coor.x;
				tempCoor.y = area->coor.y;
				for (int i=0; i<GetMaxMoveLen(board, &(area->coor), MOVE_LD); i++)
				{
					tempCoor.x -= 1;
					tempCoor.y -= 1;
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			break;
		}
		case PIECE_KNIGHT:
		{
			Coor tempCoor;

			Coor allCoors[8];
			tempCoor.x = area->coor.x+2; tempCoor.y = area->coor.y+1; allCoors[0] = tempCoor;
			tempCoor.x = area->coor.x+1; tempCoor.y = area->coor.y+2; allCoors[1] = tempCoor;
			tempCoor.x = area->coor.x-1; tempCoor.y = area->coor.y+2; allCoors[2] = tempCoor;
			tempCoor.x = area->coor.x-2; tempCoor.y = area->coor.y+1; allCoors[3] = tempCoor;
			tempCoor.x = area->coor.x-2; tempCoor.y = area->coor.y-1; allCoors[4] = tempCoor;
			tempCoor.x = area->coor.x-1; tempCoor.y = area->coor.y-2; allCoors[5] = tempCoor;
			tempCoor.x = area->coor.x+1; tempCoor.y = area->coor.y-2; allCoors[6] = tempCoor;
			tempCoor.x = area->coor.x+2; tempCoor.y = area->coor.y-1; allCoors[7] = tempCoor;

			for (size_t i = 0; i < sizeof(allCoors)/sizeof(allCoors[0]); i++)
			{
				tempCoor = allCoors[i];
				if (IsReelCoor(&tempCoor) && !IsMyArea(area->piece.owner, &(board->board[tempCoor.x][tempCoor.y])))
				{
					if (withControl)
					{
						if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
							AddNood(list, tempCoor);
					}
					else
						AddNood(list, tempCoor);
				}
			}
			break;
		}
		case PIECE_KING:
		{
			Coor tempCoor;

			// Her yöne bir birim hareket
			{
				Coor allCoors[8];
				tempCoor.x = area->coor.x+1; tempCoor.y = area->coor.y; allCoors[0] = tempCoor;
				tempCoor.x = area->coor.x+1; tempCoor.y = area->coor.y+1; allCoors[1] = tempCoor;
				tempCoor.x = area->coor.x; tempCoor.y = area->coor.y+1; allCoors[2] = tempCoor;
				tempCoor.x = area->coor.x-1; tempCoor.y = area->coor.y+1; allCoors[3] = tempCoor;
				tempCoor.x = area->coor.x-1; tempCoor.y = area->coor.y; allCoors[4] = tempCoor;
				tempCoor.x = area->coor.x-1; tempCoor.y = area->coor.y-1; allCoors[5] = tempCoor;
				tempCoor.x = area->coor.x; tempCoor.y = area->coor.y-1; allCoors[6] = tempCoor;
				tempCoor.x = area->coor.x+1; tempCoor.y = area->coor.y-1; allCoors[7] = tempCoor;

				for (size_t i = 0; i < sizeof(allCoors)/sizeof(allCoors[0]); i++)
				{
					tempCoor = allCoors[i];
					if (IsReelCoor(&tempCoor) && !IsMyArea(area->piece.owner, &(board->board[tempCoor.x][tempCoor.y])))
					{
						if (withControl)
						{
							if (ControlMovement(board, area, CoorToArea(board, &tempCoor), area->piece.owner))
								AddNood(list, tempCoor);
						}
						else
							AddNood(list, tempCoor);
					}
				}
			}
			// Rok hareketleri
			if (!forState)
			{
				/*
				 ** Sah ve kale hareket etmemis olacak
				 ** O anda Sah cekilmis olmayacak
				 ** Sah cekilen karelerden gecilmeyecek
				 ** Sah ve kale arasýnda tas olmayacak
				 */

				if (area->piece.moveCount != 0)
					break;

				if (GetBoardStateForPlayer(board, area->piece.owner) != STATE_NORMAL)
					break;

				Coor allCoors[2];
				switch (area->piece.owner)
				{
					case PLAYER_1:
						allCoors[0].x = 0; allCoors[0].y = 0;
						allCoors[1].x = 7; allCoors[1].y = 0;
						break;
					case PLAYER_2:
						allCoors[0].x = 0; allCoors[0].y = 7;
						allCoors[1].x = 7; allCoors[1].y = 7;
						break;
					
					default:
						break;
				}

				for (size_t i = 0; i < (sizeof(allCoors)/sizeof(Coor)); i++)
				{
					tempCoor = allCoors[i];
					Area* tempArea = CoorToArea(board, &tempCoor);

					if (tempArea->piece.type != PIECE_ROCK)
						continue;

					if (tempArea->piece.moveCount != 0)
						continue;

					enum MoveTypes moveType;
					if (area->coor.x > tempCoor.x)
						moveType = MOVE_L;
					else
						moveType = MOVE_R;

					int mmax = GetMaxMoveLen(board, &(area->coor), moveType);

					// Arada taþ yoksa
					if (mmax != GetCoorDistance(&(area->coor), &tempCoor))
						continue;

					Coor iiPrevCoor = area->coor;
					Board tempBoard = *board;
					Coor iiLastCoor = iiPrevCoor;

					bool ret = true;

					for (size_t ii = 1; ii <= 2; ii++)
					{
						/**
						 * Tempboard olusturup sahý aradaki karalere alarak sah durumu var mý
						 * yok mu diye bakýyoruz.
						 * Sah her zaman 2 birim ilerliyor.
						 */

						if (moveType == MOVE_L)
							iiLastCoor.x -= 1;
						else
							iiLastCoor.x += 1;

						MakeMovementBoard(&tempBoard, &iiPrevCoor, &iiLastCoor);

						if (GetBoardStateForPlayer(&tempBoard, area->piece.owner) != STATE_NORMAL)
						{
							ret = false;
							break;
						}
					}

					if (!ret)
						continue;

					AddNood(list, iiLastCoor);

				}
				
			}
			break;
		}
		default:
			printTEST("Unknow piece type (%d)", area->piece.type);
			break;
	}
}
int GetCoorDistance(Coor* coor1, Coor* coor2)
{
	if (coor1->y == coor2->y)
	{
		int ret = abs((coor1->x) - (coor2->x)) - 1;
		return ret;
	}

	return 0;
}
bool ControlMovement(Board* scrBoard, Area* area1, Area* area2, enum PlayerTypes player)
/* Hamle sonrasýnda tahtanýn normal durumda olup olmadýðýna bakýyor. */
{
	Board tempBoard = *scrBoard;
	// memcpy(&tempBoard, board, sizeof(board));

	MakeMovementBoard(&tempBoard, &(area1->coor), &(area2->coor));

	enum BoardStates boardState = GetBoardStateForPlayer(&tempBoard, player);

	return (boardState == STATE_NORMAL);

	/*switch (boardState)
	{
	case CHECK_BY_PLAYER_1:
		if (player == PLAYER_1)
			return false;
		break;
	case CHECK_BY_PLAYER_2:
		if (player == PLAYER_2)
			return false;
		break;
	
	default:
		break;
	}

	return true;*/
}
void MakeMovement(Area* area1, Area* area2)
/* Tahtadan baðýmsýz, doðrudan içerik deðiþtiriyor. */
{

	if (area1 == &(mainBoard->board[area1->coor.x][area1->coor.y]))
	{
		if (g_exportingFile != NULL)
		{
			FILE *tempFile = fopen(g_exportingFile, "a");
			fprintf(tempFile, "%d %d %d %d\n", area1->coor.x, area1->coor.y, area2->coor.x, area2->coor.y);
			fclose(tempFile);
		}
	#ifdef DEBUG
		printf("MainBoard changing!\n");
	#endif
	}

	area1->piece.moveCount++;

	Piece emptyPiece;
	emptyPiece.type = PIECE_EMPTY;
	emptyPiece.owner = PLAYER_NONE;
	emptyPiece.moveCount = 0;

	area2->piece = area1->piece;
	area1->piece = emptyPiece;
}
void MakeMovementBoard(Board* board, Coor* coor1, Coor* coor2)
{
	Area *area1 = CoorToArea(board, coor1), *area2 = CoorToArea(board, coor2);
	if (area1 == &(mainBoard->board[area1->coor.x][area1->coor.y]))
	{
		players.lastMove[(area1->piece.owner)-1] = *coor2;
	}

	if (area2->piece.type == PIECE_EMPTY && area1->coor.x != area2->coor.x)
	/**
	 * En passant:
	 * eger gidecegi yer bossa ve x degeri kendi x ine esit degilse:
	*/
	{
		/**
		 * yandaki tasi bul ve kaldir.
		*/
		Piece emptyPiece;
		emptyPiece.type = PIECE_EMPTY;
		emptyPiece.owner = PLAYER_NONE;
		emptyPiece.moveCount = 0;

		board->board[area2->coor.x][area1->coor.y].piece = emptyPiece;
	}

	MakeMovement(area1, area2);

	if (area2->piece.type == PIECE_KING && GetCoorDistance(&(area1->coor), &(area2->coor)) > 0)
	// Rok ise:
	{
		Coor tempCoorPrev; // Rock's previous coor
		tempCoorPrev.y = area1->coor.y; // area1 or area2, same

		if (area2->coor.x == 2) // Sola rok
			tempCoorPrev.x = 0;
		else
			tempCoorPrev.x = 7;

		Coor tempCoorNew; // Rock's new coor
		tempCoorNew.y = area1->coor.y; // area1 or area2, same

		if (area2->coor.x == 2) // Sola rok
			tempCoorNew.x = area1->coor.x - 1;
		else
			tempCoorNew.x = area1->coor.x + 1;

		MakeMovement(CoorToArea(board, &tempCoorPrev), CoorToArea(board, &tempCoorNew));
	}
	else if (area2->piece.type == PIECE_PAWN)
	// Piyonun son kareye ulasmasý
	{
		if ((area2->piece.owner == PLAYER_1 && coor2->y == GRID_SIZE-1) || (area2->piece.owner == PLAYER_2 && coor2->y == 0))
		{
			area2->piece.type = PIECE_QUEEN;
		}
	}
	
}
enum BoardStates GetBoardStateForPlayer(Board* board, enum PlayerTypes player)
/* Sadece þah çekilip çelimediðine bakýyor. */
/**
 * Tahtadaki tum taslarda diger oyuncularýn taslarýný alýyoruz.
 * Bu taslarýn her birinin hareket listesini alýp icerisinde kendi sah ýmýzýn olup olmadýgýna bakýyoruz.
 */
{
	Area tempArea;

	for (int i=0; i<GRID_SIZE; i++)
	{
		for (int j=0; j<GRID_SIZE; j++)
		{
			tempArea = board->board[i][j];

			if (tempArea.piece.owner == PLAYER_NONE || tempArea.piece.owner == player)
				continue;

			// Gerçi bu satýr olmasa da olur çünkü tahta bu pozisyona hiçbir zaman gelmez
			/*if (tempArea.piece.type == PIECE_KING) // Þahla þah çekilmez
				continue;*/

			CoorNood* tempNood = CreateNood();
			GetMovementList(board, &tempArea, tempNood, false, true);

			for (int ii=0; ii<GetNoodSize(tempNood); ii++)
			{
				Coor tempCoor = NoodGetValueByIndex(tempNood, ii);

				if ((board->board[tempCoor.x][tempCoor.y]).piece.type == PIECE_KING && (board->board[tempCoor.x][tempCoor.y]).piece.owner == player)
				{
					/*switch (tempArea.piece.owner)
					{
					case PLAYER_1:
						ClearNood(tempNood);
						return CHECK_BY_PLAYER_1;
					case PLAYER_2:
						ClearNood(tempNood);
						return CHECK_BY_PLAYER_2;
					default:
						ClearNood(tempNood);
						return STATE_UNKNOWN;
					}*/
					ClearNood(tempNood);
					return STATE_CHECK;
				}
			}

			ClearNood(tempNood);
		}
	}

	return STATE_NORMAL;
}
bool IsEnemyArea(enum PlayerTypes player, Area* area)
{
	enum PlayerTypes other = area->piece.owner;

	return (other != player && other != PLAYER_NONE);
}
bool IsMyArea(enum PlayerTypes player, Area* area)
{
	enum PlayerTypes other = area->piece.owner;

	return (other == player);
}
void ChangePlayer()
{
	switch (players.currentPlayer)
	{
	case PLAYER_1:
		players.currentPlayer = PLAYER_2;
		break;
	case PLAYER_2:
		players.currentPlayer = PLAYER_1;
		break;
	
	default:
		break;
	}
}
void PlayerGetAllMovements(Board* board, enum PlayerTypes player, MoveNood* moveList)
{
	Area tempArea;

	for (int i=0; i<GRID_SIZE; i++)
	{
		for (int j=0; j<GRID_SIZE; j++)
		{
			tempArea = board->board[i][j];

			if (tempArea.piece.owner != player)
				continue;

			CoorNood* tempCoorNood = CreateNood();
			GetMovementList(board, &tempArea, tempCoorNood, true, false);

			if (GetNoodSize(tempCoorNood) > 0)
				AddMoveNood(moveList, tempArea.coor, tempCoorNood);
		}
	}
}
void GetValidArea(MoveNood* allMovements, Area** area, CoorNood** moveList)
{
	char userInput[2+1];
	Coor userCoor; 
	Area* tempArea1;
	CoorNood* tempMoveList;

	while (true)
	{
		system("cls");

		PrintBoard(mainBoard);

		printf("Oynamak istediginiz tasin koordinatlarini giriniz. (OR: a1)");

		if (g_PRINTMOVES)
		{ // Printing.. // Fonksiyon pointer bilseydim burasý gibi yerler çok daha iyi olabilirdi.
			printf("\n\n		Oynanabilecek Taslar:\n			");
			size_t noodLen = GetMoveNoodSize(allMovements);
			for (int i=0; i<noodLen; i++)
			{
				char userInput[2+1]; CoorToUserInput(userInput, MoveNoodGetCoorByIndex(allMovements, i));
				printf("%s  ", userInput);
			}
			printf("\n\n\n");
		}

		#ifndef DEBUG
		scanf("%s", &userInput);
		#endif

		if (!UserInputToCoor(userInput, &userCoor))
		{
			continue;
		}

		tempArea1 = CoorToArea(mainBoard, &userCoor);
		if (tempArea1->piece.owner != players.currentPlayer)
		{
			printf("Bu tasi oynayamazsiniz.\n");
			continue;
		}

		tempMoveList = FindCoorNoodByCoor(allMovements, userCoor);
		if (tempMoveList == NULL || GetNoodSize(tempMoveList)==0)
		{
			printf("Bu tasi oynayabileceginiz yer yok!\n");
			continue;
		}

		break;
	}

	*area = tempArea1;
	*moveList = tempMoveList;
}
bool GetValidMovement(CoorNood* coorNood, Area** area)
{
	/***
	 * Kendisine gelen listeyi satranc tipi coor a dönüþtürüp ekrana bastýrýyouzr.
	 * Kullanýcýdan bunlardan birisini girmesini itiyoruz.
	 *	Ve taþý deðiþtirmesi için de bir seçeneke veriyoruz.
	 *	Eðer bu seçeneði seçerse false döndürüyoruz.
	 *	Else return true
	 * Daha sonra bu girdiðini area'ya atýyoruz.
	 */

	do
	{
		size_t noodLen = GetNoodSize(coorNood);

		if (noodLen == 1)
		{
			Coor tempCoor = NoodGetValueByIndex(coorNood, 0);
			*area = CoorToArea(mainBoard, &(tempCoor));
			break;
		}

		if (g_PRINTPIECES)
		{ // Printing.. // Fonksiyon pointer bilseydim burasý gibi yerler çok daha iyi olabilirdi.
			printf("\n\n		Yapilabilecek Hamleler:\n			");
			for (int i=0; i<noodLen; i++)
			{
				Coor tempCoor = NoodGetValueByIndex(coorNood, i);

				char userInput[2+1]; CoorToUserInput(userInput, &tempCoor);
				printf("%s  ", userInput);
			}
			printf("\n\n\n");
		}

		char userInput[2+1];
		printf("Belirlediginiz tasi oynamak istediginiz koordinatlarini giriniz. (OR: a3)\n");
		printf("Eger hareket ettirmek istediginiz tasi degistirmek istiyorsaniz 00 giriniz.\n");
		#ifndef DEBUG
		scanf("%s", &userInput);
		#endif

		Coor coor;

		if (!UserInputToCoor(userInput, &coor))
		{
			continue;
		}

		if (coor.x == GRID_SIZE && coor.y == GRID_SIZE)
		{
			return false;
		}

		if (!IsInNood(coorNood, &coor))
		{
			continue;
		}

		*area = CoorToArea(mainBoard, &coor);
		break;

	} while (true);

	return true;
}

/* Piece */
void GetPieceName(Piece* piece, char* name)
{
	switch (piece->owner)
	{
		default:
			printTEST("Unknown piece owner type %d", piece->owner);
		case PLAYER_NONE: name[0] = '*'; break;
		case PLAYER_1: name[0] = '1'; break;
		case PLAYER_2: name[0] = '2'; break;
	}
	switch (piece->type)
	{
		default:
			printTEST("Unknown piece type %d", piece->type);
		case PIECE_EMPTY:	name[1] = '*'; break;
		case PIECE_PAWN:	name[1] = 'P'; break;
		case PIECE_ROCK:	name[1] = 'K'; break;
		case PIECE_KNIGHT:	name[1] = 'A'; break;
		case PIECE_BISHOP:	name[1] = 'F'; break;
		case PIECE_QUEEN:	name[1] = 'V'; break;
		case PIECE_KING:	name[1] = 'S'; break;
	}
}
bool IsDefaultPos(Area* area)
{
	switch (area->piece.type)
	{
		case PIECE_PAWN:
		{
			if ((area->coor.y == 1 && area->piece.owner==PLAYER_1) || (area->coor.y == 6 && area->piece.owner==PLAYER_2))
				return true;
			else
				return false;
		}
		
		default:
			return false;
	}
}
int GetMaxMoveLen(Board* board, Coor* coor, enum MoveTypes type)
{
	int ret = 0;

	Coor tempCoor = *coor;
	while (true)
	{
		/* Yeni koordinatý hazýrlýyoruz. */
		switch (type)
		{
			case MOVE_RD:
			case MOVE_RU:
			case MOVE_R:
			{
				tempCoor.x += 1;
				break;
			}
			case MOVE_LD:
			case MOVE_LU:
			case MOVE_L:
			{
				tempCoor.x -= 1;
				break;
			}
		}
		switch (type)
		{
			case MOVE_U:
			case MOVE_LU:
			case MOVE_RU:
			{
				tempCoor.y += 1;
				break;
			}
			case MOVE_D:
			case MOVE_LD:
			case MOVE_RD:
			{
				tempCoor.y -= 1;
				break;
			}
		}

		/* Tahta dýþýna çýkarsak bitirelim. */
		if (!IsReelCoor(&tempCoor))
			break;

		Area *tempArea = CoorToArea(board, &tempCoor);

		/* Kendi taþýmýza gelirsek bitirelim. */
		if (IsMyArea(CoorToArea(board, coor)->piece.owner, tempArea))
			break;

		if (IsEnemyArea(CoorToArea(board, coor)->piece.owner, tempArea))
		{
			ret++;
			break;
		}

		ret++;
	}

	return ret;
}
