#ifndef GAME_H
#define GAME_H

#include "main.h"

#define GRID_SIZE 8 /*Tam kullan�labilir de�il!*/
#define PLAYER_COUNT 2 /*Tam kullan�labilir de�il!*/

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef char BYTE;

enum PieceTypes
{
	PIECE_EMPTY,
	PIECE_PAWN,
	PIECE_ROCK,
	PIECE_KNIGHT,
	PIECE_BISHOP,
	PIECE_QUEEN,
	PIECE_KING,
};

enum PlayerTypes
{
	PLAYER_NONE,
	PLAYER_1, // alttaki 1
	PLAYER_2,
};

typedef struct Piece
{
	enum PieceTypes type;
	enum PlayerTypes owner;
	bool isMoved;
} Piece;

typedef struct Coor
{
	BYTE x, y;
} Coor;

typedef struct Area
{
	Coor coor;
	Piece piece;
} Area;

typedef struct Board
{
	Area board[GRID_SIZE][GRID_SIZE];
} Board;

typedef struct Players
{
	enum PlayerTypes players[PLAYER_COUNT];
} Players;


/**
 * Ileriki versiyonlarda bu sisteme gecis yapilabilir.
 */
typedef struct Player
{
	enum PlayerTypes type;
	Piece* pieces[16];
} Player;

enum MoveTypes
{
	MOVE_NONE,
	MOVE_R,
	MOVE_L,
	MOVE_U,
	MOVE_D,
	MOVE_RU,
	MOVE_RD,
	MOVE_LU,
	MOVE_LD,
	MOVE_SPECIAL, // At Hareketi
};

enum SpecialMove
{
	SPECIAL_MOVE_0, // x2 y1
	SPECIAL_MOVE_1, // 
	SPECIAL_MOVE_2, // 
	SPECIAL_MOVE_3, // 
	SPECIAL_MOVE_4, // 
	SPECIAL_MOVE_5, // 
	SPECIAL_MOVE_6, // 
	SPECIAL_MOVE_7, // 
};

typedef struct CoorNood
{
	Coor data;
	struct CoorNood* next;
} CoorNood;

typedef struct MoveNood
{
	Coor coor;
	CoorNood* data;
	struct MoveNood* next;
} MoveNood;

enum MoveReturn
{
	SAME_COOR, // Ayn� koordinatlar
	NOT_VALID_COOR, // Ge�ersiz koordinatlar
	NOT_OWNER, // Kendi ta�� de�il
	ALREADY_OWNER, // Gitti�i yerde kendi ta�� var
	MOVE_BLOCKED, // Arada ta� var
	SAH_TEHDIT_ALTINDA,
	GECERSIZ_HAMLE_TIPI,
	SUCCES,
};

enum BoardStates
{
	STATE_NORMAL,
	STATE_CHECK,
	CHECK_BY_PLAYER_1,
	MATE_BY_PLAYER_1,
	CHECK_BY_PLAYER_2,
	MATE_BY_PLAYER_2,
	STATE_UNKNOWN = 99,
};

/* Board */
void ClearBoard(Board* board);
bool CreateBoard(const char* fileName, Board** board);
void PrintBoard(Board* board);
bool IsReelCoor(Coor* coor);
Area* CoorToArea(Board* board, Coor* coor);
enum BoardStates GetBoardStateForPlayer(Board* board, enum PlayerTypes player);
void MakeMovement(Area* area1, Area* area2);
void MakeMovementBoard(Board* board, Coor* coor1, Coor* coor2);
bool UserInputToCoor(char* userInput, Coor* coor);
void CoorToUserInput(char* userInput, Coor* coor);
void MakeBoardMovements(const char* line, Board* mainBoard);
int GetCoorDistance(Coor* coor1, Coor* coor2);

/* Player */
void GetMovementList(Board* board, Area* area, CoorNood* list, bool withControl, bool forState);
bool IsEnemyArea(enum PlayerTypes player, Area* area);
bool IsMyArea(enum PlayerTypes player, Area* area);
bool ControlMovement(Board* scrBoard, Area* area1, Area* area2, enum PlayerTypes player);
void ChangePlayer();
void PlayerGetAllMovements(Board* board, enum PlayerTypes player, MoveNood* moveList);
void GetValidArea(MoveNood* allMovements, Area** area, CoorNood** moveList);
bool GetValidMovement(CoorNood* coorNood, Area** toArea);

/* Piece */
void GetPieceName(Piece* piece, char* name);
bool IsDefaultPos(Area* area);
int GetMaxMoveLen(Board* board, Coor* coor, enum MoveTypes type);

/* CoorNood */
CoorNood* CreateNood();
void AddNood(CoorNood* headNoodP, Coor value);
size_t GetNoodSize(CoorNood* headNoodP);
CoorNood* GetNoodLast(CoorNood* headNoodP);
Coor NoodGetValueByIndex(CoorNood* headCont, size_t index);
bool IsInNood(CoorNood* headNoodP, Coor* coor);
void ClearNood(CoorNood* headNood);

/* MoveNood */
MoveNood* CreateMoveNood();
void AddMoveNood(MoveNood* headNoodP, Coor coor, CoorNood* data);
size_t GetMoveNoodSize(MoveNood* headNoodP);
MoveNood* GetMoveNoodLast(MoveNood* headNoodP);
CoorNood* MoveNoodGetValueByIndex(MoveNood* headCont, size_t index);
Coor* MoveNoodGetCoorByIndex(MoveNood* headCont, size_t index);
void ClearMoveNood(MoveNood* headNood);
CoorNood* FindCoorNoodByCoor(MoveNood* headNoodP, Coor coor);

extern enum PlayerTypes currentPlayer;
extern Board* mainBoard;

#endif