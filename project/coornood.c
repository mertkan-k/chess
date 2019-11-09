#include "game.h"

CoorNood* CreateNood()
{
	CoorNood* headNood;
	headNood = (CoorNood*)malloc(sizeof(CoorNood));

	Coor coor;
	coor.x = -1;
	coor.y = -1;

	headNood->data = coor;
	headNood->next = NULL;
	return headNood;
}

/***
 * Kaçýncý sýraya eklendiyse onu döndürüyor.
 * 0 dan baþlar
 * -1: Eklenemedi NOT: Þimdilik bu özellik devredýþý býrakýldý.
***/
void AddNood(CoorNood* headNoodP, Coor data)
{
	if (headNoodP->data.x == -1) // if is head nood
	{
		headNoodP->data = data;
		return;
	}

	CoorNood* newContP;

	newContP = (CoorNood*)malloc(sizeof(CoorNood));
	newContP->next = NULL;
	newContP->data = data;

	GetNoodLast(headNoodP)->next = newContP;
}

size_t GetNoodSize(CoorNood* headNoodP)
{
	size_t size;

	if (headNoodP->data.x == -1)
		size = 0;
	else
		size = 1;

	CoorNood* tempNood;
	tempNood = headNoodP;

	while (tempNood->next != NULL)
	{
		tempNood = tempNood->next;
		size++;
	}

	return size;
}

CoorNood* GetNoodLast(CoorNood* headNoodP)
{
	CoorNood* tempNood;
	tempNood = headNoodP;

	while (tempNood->next != NULL)
		tempNood = tempNood->next;

	return tempNood;
}

/***
 * Verilen indexe ulaþamazsa default döndürür.
 ** Ileride pointer dondurecek sekle getirilebilir.
***/
Coor NoodGetValueByIndex(CoorNood* headCont, size_t index)
{
	CoorNood* tempNood = headCont;
	for (size_t i = 0; i < index; i++)
	{
		if (tempNood == NULL)
		{
			// For fixing uplimit
			Coor coor;
			coor.x = -1;
			coor.y = -1;
			return coor;
		}
			// return NoodDefaultValue;
		tempNood = tempNood->next;
	}
	
	return tempNood->data;
}

bool IsInNood(CoorNood* headNoodP, Coor* coor)
{
	CoorNood* tempNood;
	tempNood = headNoodP;

	do
	{
		if (tempNood->data.x == coor->x && tempNood->data.y == coor->y)
			return true;
		tempNood = tempNood->next;
	} while (tempNood != NULL);

	return false;
}

/***
 * Kendisine geleni ve sonrasýný temizler.
***/
void ClearNood(CoorNood* headNood)
{
	CoorNood* tempNood = headNood;
	CoorNood* deletingNood = tempNood;
	while(tempNood != NULL)
	{
		deletingNood = tempNood;
		tempNood = tempNood->next;
		free(deletingNood);
	}
}
