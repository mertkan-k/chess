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
 * Ka��nc� s�raya eklendiyse onu d�nd�r�yor.
 * 0 dan ba�lar
 * -1: Eklenemedi NOT: �imdilik bu �zellik devred��� b�rak�ld�.
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
 * �lkine bakm�yor!!
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
 * Verilen indexe ula�amazsa null d�nd�r�r.
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
 * Verilen indexe ula�amazsa null d�nd�r�r.
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
 * Kendisine geleni ve sonras�n� temizler.
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
 * E�er bulamazsa NULL d�nd�r�r.
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
