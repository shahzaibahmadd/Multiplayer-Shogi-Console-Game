#include <iostream>
#include <iomanip>
#include<conio.h>
#include <fstream>
#include<Windows.h>
using namespace std;
struct position
{
	int ri;
	int ci;
};
enum { WHITE, BLACK };

void init(char**& b, char**& a, int& dim, string pnames[], int& turn)
{

	ifstream rdr("hehe.txt");
	rdr >> dim;
	b = new char* [dim];
	for (int ri = 0; ri < dim; ri++)
	{
		b[ri] = new char[dim];
	}
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			rdr >> b[ri][ci];
		}
	}
	turn = WHITE;
	a = new char* [dim];
	for (int ri = 0; ri < dim; ri++)
	{
		a[ri] = new char[dim];
	}
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}

void selectposition(position& p, int dim)
{
	getRowColbyLeftClick(p.ri, p.ci);
	p.ri /= 3;
	p.ci /= 3;
}
bool iswhitepiece(char sym)
{

	return sym >= 'A' && sym <= 'Z';



}
bool isblackpiece(char sym)
{

	return sym >= 'a' && sym <= 'z';
}
bool ismypiece(char sym, int& turn)
{
	bool a;
	if (turn == BLACK)
		return isblackpiece(sym);
	if (turn == WHITE)
		return iswhitepiece(sym);
	return false;

}

bool isvalidsrc(char**& b, int& dim, position sc, int& turn)
{
	if (sc.ri < 0 || sc.ri >= dim || sc.ci < 0 || sc.ci >= dim)
		return false;

	return ismypiece(b[sc.ri][sc.ci], turn);


}
bool isvaliddst(char**& b, int& dim, position dc, int& turn)
{
	if (dc.ri < 0 || dc.ri >= dim || dc.ci < 0 || dc.ci >= dim)
		return false;

	return !ismypiece(b[dc.ri][dc.ci], turn);
}
void turnf(int& turn)
{
	turn = (turn + 1) % 2;
}
void  turnmsg(string pnames)
{
	cout << pnames << "'s Turn.." << endl;
}

void drawbox(int sr, int sc, int  brows, int bcols, int col)
{
	for (int r = 0; r < brows; r++)
	{
		for (int c = 0; c < bcols; c++)
		{
			SetClr(col);
			gotoRowCol(sr + r, sc + c);
			cout << char(-37);
		}
	}

}

bool ishorizontalmove(position sc, position dc)
{
	return sc.ri == dc.ri;

}
bool isverticalmove(position sc, position dc)
{
	return sc.ci == dc.ci;

}
bool isdiagnolmove(position sc, position dc)
{
	int r = abs(sc.ri - dc.ri);
	int c = abs(sc.ci - dc.ci);
	return r == c;
}
bool isHorizontalPathClear(char** b, position sc, position dc)
{
	if (dc.ci > sc.ci)
	{
		for (int i = sc.ci + 1; i <= dc.ci; i++)
		{
			if (b[sc.ri][i] != '-')
				return false;
		}
		return true;
	}
	if (dc.ci < sc.ci)
	{
		for (int i = sc.ci - 1; i >= dc.ci; i--)
		{
			if (b[sc.ci][i] != '-')
				return false;
		}
	}
}
bool isverticalpathclear(char**& b, position sc, position dc)
{
	if (sc.ri < dc.ri)
	{
		for (int i = sc.ri + 1; i < dc.ri; i++)
		{
			if (b[i][sc.ci] != '-')
			{
				return false;
			}
		}
		return true;
	}
	if (sc.ri > dc.ri)
	{
		for (int i = sc.ri - 1; i > dc.ri; i--)
		{
			if (b[i][sc.ci] != '-')
			{
				return false;
			}
		}
	}   return true;
}

bool isdiagonalpathclear(char**& b, position sc, position dc)
{
	int x, y;
	x = abs(sc.ri - dc.ri);
	y = abs(sc.ci - dc.ci);

	//top left
	if (sc.ri > dc.ri && sc.ci > dc.ci)
	{
		for (int a = 1; a < x; a++)
		{
			if (b[sc.ri - a][sc.ci - a] == '-')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	//bottom right
	if (sc.ri < dc.ri && sc.ci < dc.ci)
	{
		for (int a = 1; a < x; a++)
		{
			if (b[sc.ri + a][sc.ci + a] == '-')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	//bottom left
	if (sc.ri<dc.ri && sc.ci>dc.ci)
	{
		for (int a = 1; a < x; a++)
		{
			if (b[sc.ri + a][sc.ci - a] == '-')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	//top right
	if (sc.ri > dc.ri && sc.ci < dc.ci)
	{
		for (int a = 1; a < x; a++)
		{
			if (b[sc.ri - a][sc.ci + a] == '-')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}


}
bool pawnlegality(char** B, position sp, position dp)
{
	int dltar = dp.ri - sp.ri;
	if (isverticalmove(sp, dp) && isverticalpathclear(B, sp, dp) && dltar == -1)
	{
		return true;
	}
	return false;
}
bool Cpawnlegality(char** B, position sp, position dp)
{
	int dltar = dp.ri - sp.ri;
	if (isverticalmove(sp, dp) && isverticalpathclear(B, sp, dp) && dltar == 1)
	{
		return true;
	}
	return false;
}
bool rooklegality(char**& b, position sc, position dc)
{
	return ((ishorizontalmove(sc, dc) && isHorizontalPathClear(b, sc, dc)) ||
		(isverticalmove(sc, dc) && isverticalpathclear(b, sc, dc)));

}
bool bishoplegality(char**& b, position sc, position dc)
{
	return ((isdiagnolmove(sc, dc) && isdiagonalpathclear(b, sc, dc)));

}
bool oshu(char**& b, position sc, position dc)
{
	int r = abs(sc.ri - dc.ri);
	int c = abs(sc.ci - dc.ci);
	return (rooklegality(b, sc, dc) || bishoplegality(b, sc, dc)) && ((r <= 1) && (c <= 1));

}
bool knight(char**& b, position sc, position dc)
{
	int r = abs(sc.ri - dc.ri);
	int c = abs(sc.ci - dc.ci);

	return ((r == 2 && c == 1) || (r == 1 && c == 2));

}
bool cknight(char** B, position sp, position dp)
{
	int dltar = abs(sp.ri - dp.ri);
	int dltac = abs(sp.ci - dp.ci);

	if (dp.ri == sp.ri + 2 && (dp.ci == sp.ci - 1 || dp.ci == sp.ci + 1))
	{
		return true;
	}
	return false;
}
bool lance(char** B, position sp, position dp)
{
	int dltar = sp.ri - dp.ri;
	if (isverticalmove(sp, dp) && isverticalpathclear(B, sp, dp) && dltar > 0)
	{
		return true;
	}
	return false;
}
bool clance(char** B, position sp, position dp)
{
	int dltar = dp.ri - sp.ri;
	if (isverticalmove(sp, dp) && isverticalpathclear(B, sp, dp) && dltar > 0)
	{
		return true;
	}
	return false;
}
bool gold(char** B, position sp, position dp)
{
	int dltar = abs(sp.ri - dp.ri);
	int dltac = abs(sp.ci - dp.ci);

	if (rooklegality(B, sp, dp) && (dltar <= 1 && dltac <= 1) || isdiagnolmove(sp, dp) && dp.ri < sp.ri && dltar <= 1 && dltac <= 1)
	{
		return true;
	}
	return false;
}
bool cgold(char** B, position sp, position dp)
{
	int dltar = abs(sp.ri - dp.ri);
	int dltac = abs(sp.ci - dp.ci);

	if ((rooklegality(B, sp, dp) && (dltar <= 1 && dltac <= 1)) || (isdiagnolmove(sp, dp) && dp.ri > sp.ri && (dltar <= 1 && dltac <= 1)))
	{
		return true;
	}
	return false;
}
bool csilver(char** B, position sp, position dp)
{
	int dltar = abs(sp.ri - dp.ri);
	int dltac = abs(sp.ci - dp.ci);

	if (bishoplegality(B, sp, dp) && (dltar <= 1 && dltac <= 1) || isverticalmove(sp, dp) && dp.ri > sp.ri && (dltar <= 1 && dltac <= 1))
	{
		return true;
	}
	return false;
}


bool silver(char** B, position sp, position dp)
{
	int dltar = abs(sp.ri - dp.ri);
	int dltac = abs(sp.ci - dp.ci);

	if (bishoplegality(B, sp, dp) && (dltar <= 1 && dltac <= 1) || isverticalmove(sp, dp) && dp.ri < sp.ri && (dltar <= 1 && dltac <= 1))
	{
		return true;
	}
	return false;
}




bool Islegalmove(char** B, position sp, position dp)
{
	switch (B[sp.ri][sp.ci])
	{
	case 'O':
	{
		return oshu(B, sp, dp);
	}

	case 'o':
	{
		return oshu(B, sp, dp);
	}

	case 'R':
	{
		return rooklegality(B, sp, dp);
	}

	case 'r':
	{
		return rooklegality(B, sp, dp);
	}

	case 'b':
	{
		return bishoplegality(B, sp, dp);
	}

	case 'B':
	{
		return bishoplegality(B, sp, dp);
	}

	case 'G':
	{
		return cgold(B, sp, dp);
	}

	case 'g':
	{
		return gold(B, sp, dp);
	}


	case 's':
	{
		return silver(B, sp, dp);
	}

	case 'S':
	{
		return csilver(B, sp, dp);
	}

	case 'l':
	{
		return lance(B, sp, dp);
	}

	case 'L':
	{
		return clance(B, sp, dp);
	}


	case 'k':
	{
		return knight(B, sp, dp);
	}

	case 'K':
	{
		return cknight(B, sp, dp);
	}

	case 'p':
	{
		return pawnlegality(B, sp, dp);
	}

	case 'P':
	{
		return Cpawnlegality(B, sp, dp);
	}


	}
}
bool** checkhighlight(char** B, position sc, int dim, int Turn)
{
	bool** bMap = new bool* [dim];
	for (int b = 0; b < dim; b++)
	{
		bMap[b] = new bool[dim] {};
	}
	position dc;
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			dc.ri = ri;
			dc.ci = ci;
			if (isvaliddst(B, dim, dc, Turn) && Islegalmove(B, sc, dc))
			{
				bMap[ri][ci] = true;
			}
			else
			{
				bMap[ri][ci] = false;
			}
		}
	}
	return bMap;
}
void boxhigh(int sri, int sci, int brows, int bcols, char sym)
{
	int clr = 3;
	SetClr(3);
	for (int r = 0; r < brows; r++)
	{
		for (int c = 0; c < bcols; c++)
		{
			if (r == 0 || c == 0 || r == brows - 1 || c == bcols - 1)
			{
				gotoRowCol(sri + r, sci + c);
				cout << sym;
			}
		}
	}
}
void highlight(char** B, bool**& bmap, position Sp, int brows, int bcoloumns, int dimension, int turn)
{
	bmap = checkhighlight(B, Sp, dimension, turn);
	for (int i = 0; i < dimension; i++)
	{
		for (int j = 0; j < dimension; j++)
		{
			if (bmap[i][j] == true)
			{
				boxhigh(i * brows, j * bcoloumns, brows, bcoloumns, -37);
			}


		}
	}
}


void unhighlight(char** b, bool**& bmap, position Sp, int brows, int bcols, int dim, int turn)
{
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (bmap[i][j] == true)
			{
				if (i + j % 2 == 0)
				{
					drawbox(i * brows, i * bcols, brows, bcols, 15);
					gotoRowCol(i * brows + bcols / 2, i * bcols + brows / 2);

					cout << b[i][i];

				}
				else
				{
					drawbox(i * brows, i * bcols, brows, bcols, 15);
					gotoRowCol(i * brows + bcols / 2, i * bcols + brows / 2);

					cout << b[i][i];
				}
			}


		}
	}
}
position findking(char**& b, int dim, int turn)
{
	position dc;


	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			if (turn == WHITE)
			{
				if (b[i][j] == 'o')
					dc.ri = i, dc.ci = j;

			}
			if (turn == BLACK)
			{
				if (b[i][j] == 'O')
					dc.ri = i, dc.ci = j;
			}
		}

	}
	return dc;

}
bool check(char**& b, int dim, int turn)
{
	position dc;
	position sc;
	/*turnf(turn);*/
	dc = findking(b, dim, turn);
	/*turnf(turn);*/
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			sc.ri = ri, sc.ci = ci;
			if (isvalidsrc(b, dim, sc, turn) && Islegalmove(b, sc, dc))
				return true;


		}
	}
	return false;
}
bool selfcheck(char**& b, int dim, int turn)
{
	turnf(turn);
	return check(b, dim, turn);
}
void copyboard(char** b, char**& a, int dim)
{
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			a[i][j] = b[i][j];
		}

	}

}

void promotion(char**& b,int dim,position sc,position dc,int turn)
{
	if (turn == BLACK)
	{
		if (dc.ri == 1 || dc.ri == 0 || dc.ri == 2)
		{
			if (b[dc.ri][dc.ci] == 'p' || b[dc.ri][dc.ci] == 'l' || b[dc.ri][dc.ci] == 's' ||
				b[dc.ri][dc.ci] == 'o')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'g';
				}
				if (pro == 2)
				{
					return;
				}
			}
			if (b[dc.ri][dc.ci] == 'k')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'g';
				}
				if (pro == 2)
				{
					return;
				}

			}
			if (b[dc.ri][dc.ci] == 'b')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'y';
				}
				if (pro == 2)
				{
					return;
				}

			}
			if (b[dc.ri][dc.ci] == 'r')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'd';
				}
				if (pro == 2)
				{
					return;
				}

			}
		}


	}

	if (turn == WHITE)
	{
		if(dc.ri==6|| dc.ri == 7 || dc.ri == 8)
		{
			if (b[dc.ri][dc.ci] == 'P' || b[dc.ri][dc.ci] == 'L' || b[dc.ri][dc.ci] == 'S' ||
				b[dc.ri][dc.ci] == 'O')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'G';
				}
				if (pro == 2)
				{
					return;
				}
			}
			if (b[dc.ri][dc.ci] == 'K')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'G';
				}
				if (pro == 2)
				{
					return;
				}

			}
			if (b[dc.ri][dc.ci] == 'B')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'Y';
				}
				if (pro == 2)
				{
					return;
				}

			}
			if (b[dc.ri][dc.ci] == 'R')
			{
				int pro;
				gotoRowCol(4, 80);
				cout << "enter 1 to promote or 2 to continue" << endl;
				cin >> pro;
				if (pro == 1)
				{
					b[dc.ri][dc.ci] = 'D';
				}
				if (pro == 2)
				{
					return;
				}

			}
		}


	}


}
void save(char**& b, int dim, int turn)
{

	ofstream rdr("save.txt");
	rdr << dim;

	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			rdr << b[ri][ci];
		}

	}



}
void load(char**& b, char**& a, int& dim, string pnames[], int& turn)
{
	ifstream rdr("save.txt");
	rdr >> dim;
	b = new char* [dim];
	for (int ri = 0; ri < dim; ri++)
	{
		b[ri] = new char[dim];
	}
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			rdr >> b[ri][ci];
		}
	}
	turn = WHITE;
	a = new char* [dim];
	for (int ri = 0; ri < dim; ri++)
	{
		a[ri] = new char[dim];
	}

}
bool checkmate(char** b, int dim, int turn)
{
	position sc;
	position dc;
	turnf(turn);
	for (int r1 = 0; r1 < dim; r1++)
	{
		for (int c1 = 0; c1 < dim; c1++)
		{
			sc.ri = r1;
			sc.ci = c1;
			for (int r2 = 0; r2 < dim; r2++)
			{
				for (int c2 = 0; c2 < dim; c2++)
				{
					dc.ri = r2;
					dc.ci = c2;
					if (isvalidsrc(b,dim,sc,turn) && isvaliddst(b,dim,dc,turn) && Islegalmove(b,sc,dc))
					{
						turnf(turn);
						if (check(b, dim, turn) == false)
						{
							return false;
						}
						turnf(turn);
					}
				}
			}
		}
	}
	return true;
}

void drawboard(char**& b, int dim, int brows, int bcols, int color)
{
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			if ((ri + ci) % 2 == 0)
			{
				drawbox(ri * brows, ci * bcols, brows, bcols, 15);
				gotoRowCol(ri * brows + bcols / 2, ci * bcols + brows / 2);
				SetClr(color);
				
				cout << b[ri][ci];
			}
			else
			{
				drawbox(ri * brows, ci * bcols, brows, bcols, 6);
				gotoRowCol(ri * brows + bcols / 2, ci * bcols + brows / 2);
				SetClr(color);
				
				cout << b[ri][ci];
			}
		}
	}
}
void updateboard(char**& b, int dim, int brows, int bcols, position sc, position dc, int color)
{
	b[dc.ri][dc.ci] = b[sc.ri][sc.ci];
	b[sc.ri][sc.ci] = '-';
	for (int ri = 0; ri < dim; ri++)
	{
		for (int ci = 0; ci < dim; ci++)
		{
			if ((ri + ci) % 2 == 0)
			{
				drawbox(ri * brows, ci * bcols, brows, bcols, 15);
				gotoRowCol(ri * brows + bcols / 2, ci * bcols + brows / 2);
				SetClr(color);
				cout << b[ri][ci];
			}
			else
			{
				drawbox(ri * brows, ci * bcols, brows, bcols, 6);
				gotoRowCol(ri * brows + bcols / 2, ci * bcols + brows / 2);
				SetClr(color);
				cout << b[ri][ci];
			}
		}
	}


}
int main()
{
	char** b;
	bool** bmap;
	char** a;
	int dim;
	
	string pnames[2] = {};
	cout << "enter player1 name:" << endl;
	cin >> pnames[0];
	cout << "enter player2 name:" << endl;
	cin >> pnames[1];
	int turn;
	position sp, ds;
	int choose;
	cout << "enter 1 for new game \n enter 2 for previous game" << endl;
	cin >> choose;
	if (choose == 1)
	{
		init(b, a, dim, pnames, turn);
	}
	if (choose == 2)
	{
		load(b, a, dim, pnames, turn);
	}
	drawboard(b, 9, 3, 3, 15);

	while (true)
	{
		do
		{
			do
			{
				gotoRowCol(0, 100);
				turnmsg(pnames[turn]);
				do
				{
					do
					{
						gotoRowCol(0, 50);
						cout << "Select from";
						selectposition(sp, dim);
					} while (!isvalidsrc(b, dim, sp, turn));
					highlight(b, bmap, sp, 3, 3, dim, turn);
					gotoRowCol(0, 50);
					cout << "Select to" << "   ";
					selectposition(ds, dim);

				} while (!isvaliddst(b, dim, ds, turn));
				unhighlight(b, bmap, sp, 3, 3, dim, turn);
			} while (!Islegalmove(b, sp, ds));
			copyboard(b, a, dim);
			updateboard(a, dim, 3, 3, sp, ds, 15);

		} while (selfcheck(a, dim, turn));

		
		updateboard(b, dim, 3, 3, sp, ds, 15);
		promotion(b, dim, sp, ds, turn);
		
		if (check(b, dim, turn) == true)
		{
			if (checkmate(b, dim, turn) == true)
			{
				gotoRowCol(8, 80);
				
				cout << pnames[turn] << "wins";
				return 0;
			}
			gotoRowCol(8, 83);
			cout << pnames[turn] << "has put in check";
		}

		drawboard(b, 9, 3, 3, 15);
		turnf(turn);
		save(b, dim, turn);
	}

	 return _getch();
}
