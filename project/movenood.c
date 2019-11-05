#include "game.h"

/*
*====*====*====*====*====* --> Main Nood (MoveNood)
|    |    |    |    |    |
*    *    *    *    *    *
|    |    |    |    |    |
*    *    *    *    *    *
*/

MoveNood* CreateMoveNood(Coor coor)
{
	MoveNood* headNood;
	headNood = (MoveNood*)malloc(sizeof(MoveNood));

	CoorNood* emptyCoorNood = CreateNood();

	headNood->coor = coor;
	headNood->data = emptyCoorNood;
	headNood->next = NULL;
	return headNood;
}

/***
 * Kaçýncý sýraya eklendiyse onu döndürüyor.
 * 0 dan baþlar
 * -1: Eklenemedi NOT: Þimdilik bu özellik devredýþý býrakýldý.
***/
void AddMoveNood(MoveNood* headNoodP, Coor coor, CoorNood* data)
{

	if (headNoodP->data->data.x == -1) // if is head nood
	{
		headNoodP->coor = coor;
		headNoodP->data = data;
		return;
	}

	MoveNood* newContP;

	newContP = (MoveNood*)malloc(sizeof(MoveNood));
	newContP->next = NULL;
	newContP->coor = coor;
	newContP->data = data;

	GetMoveNoodLast(headNoodP)->next = newContP;
}

size_t GetMoveNoodSize(MoveNood* headNoodP)
{
	size_t size;

	if (headNoodP->data->data.x == -1)
		size = 0;
	else
		size = 1;

	MoveNood* tempNood;
	tempNood = headNoodP;

	while (tempNood->next != NULL)
	{
		tempNood = tempNood->next;
		size++;
	}

	return size;
}

/***
 * Ýlkine bakmýyor!!
 */
MoveNood* GetMoveNoodLast(MoveNood* headNoodP)
{
	MoveNood* tempNood;
	tempNood = headNoodP;

	while (tempNood->next != NULL)
	{
		tempNood = tempNood->next;
	}

	return tempNood;
}

/***
 * Verilen indexe ulaþamazsa null döndürür.
***/
CoorNood* MoveNoodGetValueByIndex(MoveNood* headCont, size_t index)
{
	MoveNood* tempNood = headCont;
	for (size_t i = 0; i < index; i++)
	{
		if (tempNood == NULL)
		{
			// For fixing uplimit
			return NULL;
		}
		// return NoodDefaultValue;
		tempNood = tempNood->next;
	}
	
	return tempNood->data;
}

/***
 * Verilen indexe ulaþamazsa null döndürür.
***/
Coor* MoveNoodGetCoorByIndex(MoveNood* headCont, size_t index)
{
	MoveNood* tempNood = headCont;
	for (size_t i = 0; i < index; i++)
	{
		if (tempNood == NULL)
		{
			// For fixing uplimit
			return NULL;
		}
		// return NoodDefaultValue;
		tempNood = tempNood->next;
	}
	
	return &(tempNood->coor);
}

/***
 * Kendisine geleni ve sonrasýný temizler.
***/
void ClearMoveNood(MoveNood* headNood)
{
	MoveNood* tempNood = headNood;
	MoveNood* deletingNood = tempNood;
	while(tempNood != NULL)
	{
		deletingNood = tempNood;
		tempNood = tempNood->next;
		ClearNood(deletingNood->data);
		free(deletingNood);
	}
	free(headNood);
}

/***
 * Eðer bulamazsa NULL döndürür.
 */
CoorNood* FindCoorNoodByCoor(MoveNood* headNoodP, Coor coor)
{
	MoveNood* tempNood;
	tempNood = headNoodP;

	while (true)
	{
		if (tempNood->coor.x == coor.x && tempNood->coor.y == coor.y)
			return tempNood->data;

		if (tempNood->next == NULL)
			break;

		tempNood = tempNood->next;
	}

	return NULL;
}
