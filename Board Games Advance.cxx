#include <iostream>
#include <ctime>
#include "conio.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#define clearscr system("cls") //use this to change between clrscr and system("cls")

using namespace std;
ifstream fi;					//for file reading
ofstream fo;					//for file writing
class game2048
{
	struct highscore			//highscore structure
	{
		char nick[10];
		unsigned int score;
		unsigned int tile;
		unsigned int board[4][4];
	}h[10];
	void savegame()				//saves the game to continue later
	{
		fo.open("save",ios::binary);
		fo.write((char*) board,16*sizeof(int));
		fo.write((char*) &score, sizeof(int));
		fo.close();
	}

	void drawb(unsigned int board[4][4]);		//draws the board

	void savehigh(unsigned int b[4][4]);		//saves highscore

	void dispb()			//draws board by calling drawb and more
	{
		clearscr;
		cout<<"\t+---------------+\n\t| The 2048 Game |\n\t+---------------+\n";
		cout<<"\t[W]=UP [A]=LEFT [S]=DOWN [D]=RIGHT\n";
		drawb(board);
		cout<<'\t'<<"Score: "<<score<<"\tQ to save and quit\n\tPress H for help";
	}

	short moved;			//whether the move moved the board or not
	short blocked;			//whether the board is blocked or not
	short loaded;			//whether loaded a saved game or not

	void put24();			//randomly inserts 2 or 4 into the board
	void shift(char);		//shifting mechanism for numbers
	unsigned int board[4][4];		//array that stores the board
	short canmove();		//checks whether the player can move or not

	void showhigh();		//displays highscores saved in the file

	void help()				//instructions
	{
		clearscr;
		cout<<"\t+---------------+\n\t| The 2048 Game |\n\t+---------------+\n\n\n";
		cout<<"\tCONTROLS:\n\t---------\n\n\t1) 'W' for UP\n\n\t2) 'A' for LEFT\n\n\t3) 'S' for DOWN\n\n\t4) 'D' for RIGHT";
		cout<<"\n\nPress any key to continue";getch();
		cout<<"\n\n\tINSTRUCTIONS:\n\t-------------\n\n1) The objective of the game to make the 2048 tile by adding the powers of 2.\n2) You have a board of 16 tiles. The game begins with 2 numbers, either of which is 2 or 4.\n3) At the end of each move, a random tile of 2 or 4 is added to the board\n4) Move UP / DOWN / LEFT / RIGHT, trying to join two equal numbers.\n5) When two equal numbers are moving into each other, they will add up to give one tile with their addition.\n6) If there are no moves possible, the game ends.\n7) This process eventually leads you to a 2048 tile.\nTake the right decisions to win the game. All the best!";
		cout<<"\n\nPress any key to go back";getch();
	}

	void game();			//generates a new game for the player

	unsigned int score;		//stores the score of the user

	void loadsave()			//loads previously saved game
	{
		fi.open("save",ios::binary);
		if (fi.good()&&fi.peek()!=ifstream::traits_type::eof()) 
		{
			loaded =1;
			fi.read((char*) board,16*sizeof(int));
			fi.read((char*) &score,sizeof(int));
		}
		else 
			loaded=-1;
		fi.close();
	}
	void setboard();	//sets initial value
public:
	void main();
};

void game2048::main()
{

	while (1)
	{
		clearscr;
		cout<<"\t+---------------+\n\t| The 2048 Game |\n\t+---------------+\n\n\n";
		cout<<"\t1. New Game\n\t2. Continue\n\t3. High Scores\n\t4. Instructions\n\t6. Return to main menu\n";
	start:
		char ch=getch();

		if (ch=='1')
		{
			setboard();
			game();
		}

		else if (ch=='6')
		{
			do
			{
				cout<<"\nAre you sure you want to return to the main menu? (Y/N)";
				cin>>ch;
				ch&=0x5f;
				if (ch!='Y'&&ch!='N')
				{
					cout<<"Try again\n";
					cin.ignore(255,'\n');
				}
			}while (ch!='Y'&&ch!='N');
			if (ch=='Y') return;
			else continue;
		}
		else if (ch=='2')
		{
			loadsave();
			game();
		}
		else if (ch=='3')
			showhigh();
		else if (ch=='4')
			help();
		else goto start;
	}
}

void game2048::setboard()
{
	score=0;
	moved=1;
	loaded=0;
	int i;
	for (i=0;i<4;i++)
		for (int j=0;j<4;j++)
			board[i][j]=0;
	for (i=0;i<10;i++)
	{
		h[i].tile=0;
	}
}

void game2048::game()
{
	if (loaded ==-1)
	{
		clearscr;
		cout<<"\t+---------------+\n\t| The 2048 Game |\n\t+---------------+\n";
		cout<<"\nNo saved game found. Choose New Game on the main menu to start a new game.\n\nPress any key to go back to the main menu";
		setboard();
		getch();
		return;
	}
	else if (loaded==0) put24();
	int load=loaded;
	while (1)
		{
		if (moved&&!loaded) put24();
		dispb();
		char move;
		do
		{
			move=getch();
			move&=0x5f;
		}while (move!='W'&&move!='A'&&move!='S'&&move!='D'&&move!='Q'&&move!='H');
		if (move=='Q')
		{
			do
			{
				cout<<"\nAre you sure you want to quit? Your progress will be saved. (Y/N) ";
				cin>>move;
				move&=0x5f;
				if (move!='Y'&&move!='N')
				{
					cout<<"Try again\n";
					cin.ignore(255,'\n');
				}
			}while (move!='Y'&&move!='N');
			if (move=='Y')
			{
				savegame();
				return;
			}
			else 
			{
				moved=0;
				continue;
			}
		}
		else if (move=='H')
		{
			help();
			moved=0;
			continue;
		}
		shift(move);
		loaded=0;
		if (canmove()==0) break;
	}
	cout<<"\n\aGame Over!\nPress enter to continue";
	cin.ignore(255,'\n');
	if (load) 
	{
		remove("save");
		setboard();
	}
	savehigh(board);
}

void game2048::put24()
{
	srand(time(0));int i,j;
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			if (board[i][j]==0)
			{
				int x,y;
				do
				{
					x=rand()%4;
					y=rand()%4;
				}while (board[x][y]);
				if(rand()%10) board[x][y]=2;
				else board[x][y]=4;
				return;
			}
}

void game2048::savehigh(unsigned int b[4][4])
{
	fi.open("high",ios::binary);
	int i,j;
	for (i=0;i<10&&fi.read((char*) &h[i],sizeof(highscore));i++);
	fi.close();
	int lim=i;
	highscore cur;cur.score=score;cur.tile=0;
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
		{
			cur.board[i][j]=b[i][j];
			if (b[i][j]>cur.tile) cur.tile= b[i][j];
		}

	for (i=0;cur.score<h[i].score&&i<lim;i++);
	if (lim!=10)
	{

		char ch;
		do
		{
			cout<<"Would you like to save your high score (Y/N)?";
			ch=cin.get();
			cin.ignore(255,'\n');
			ch&=0x5f;
			if (ch!='Y'&&ch!='N')
				cout<<"Try again\n";
		}while (ch!='Y'&&ch!='N');
		if (ch=='N') return;
		else
		{
			cout<<"Enter your nickname: ";
			cin.getline(cur.nick,10,'\n');
			if (lim==i||lim==1)
			{
				fo.open("high",ios::binary|ios::app);
				fo.write((char *) &cur,sizeof(highscore));
				fo.close();
				return;
			}
			for (int k=lim;k>=i;k--) h[k+1]=h[k];
			h[i]=cur;
			fo.open("high",ios::binary|ios::ate);fo.seekp(0);
			for (i=0;i<lim;i++) fo.write((char*) &h[i],sizeof(highscore));
			fo.close();
			return;
		}
	}


	if (i!=10)
	{

		char ch;
		do
		{
			cout<<"Would you like to save your high score (Y/N)?";
			ch=cin.get();
			cin.ignore(255,'\n');
			ch&=0x5f;
			if (ch!='Y'&&ch!='N')
				cout<<"Try again\n";
		}while (ch!='Y'&&ch!='N');
		if (ch=='N') return;
		else
		{
			for (int k=i;k>=i;k--) h[k+1]=h[k];
			cout<<"Enter your nickname: ";
			cin.ignore(255,'\n');
			cin.getline(cur.nick,10);
			h[i]=cur;
			fo.open("high",ios::binary);
			for (i=0;i<10;i++) fo.write((char*) &h[i],sizeof(highscore));
			fo.close();
		}
	}
}


void game2048::showhigh()
{
	clearscr;
	cout<<"\t+---------------+\n\t| The 2048 Game |\n\t+---------------+\n\n";
	
	fi.open("high",ios::binary);
	if (fi.bad()||fi.peek()==ifstream::traits_type::eof())
	{
		fi.close();
		cout<<"No highscores yet. Press any key to return to main menu";
		getch();
		return;
	}
	int i,j;
	for (i=0;i<10&&fi.read((char*) &h[i],sizeof(highscore));i++);
	fi.close();
	
	cout<<"\t\t+------------+\n\t\t| Highscores |\n\t\t+------------+\n\n";
	cout<<"\tRank |Nickname\t|Score\t|Highest tile\n\t-----+----------+-------+------------\n";
	for (j=0;j<i;j++) cout<<'\t'<<j+1<<".   |"<<h[j].nick<<"\t|"<<h[j].score<<"\t|"<<h[j].tile<<'\n';
	unsigned short rank;
	short chop;
	do
	{
		cout<<"\tEnter a rank to see more details: ";
		cin>>rank;
		chop=cin.fail();
		if (chop||rank>10)
		{
			cout<<"Try again\n";
			if (cin.fail())
			{
				cin.clear();
				cin.sync();
				cin.ignore(255,'\n');
			}
		}
	}while (chop||rank>10);cin.ignore(255,'\n');
	cout<<'\t'<<"Score: "<<h[rank-1].score<<"\n\tBoard: \n";
	drawb(h[rank-1].board);
	getch();
}

short game2048::canmove()
{
	int i,j;
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			if (board[i][j]==0) return 1;
	for (i=1;i<3;i++)
		for (j=1;j<3;j++)
		{
			if (board[i][j]==board[i-1][j]||board[i][j]==board[i][j-1]||board[i][j]==board[i+1][j]||board[i][j]==board[i][j+1]) return 1;
		}
	if (board[0][0]==board[0][1]||board[0][3]==board[0][2]||board[3][0]==board[3][1]||board[3][3]==board[3][2]||board[0][0]==board[1][0]||board[0][3]==board[1][3]||board[3][0]==board[2][0]||board[3][3]==board[2][3]||board[0][1]==board[0][2]||board[3][1]==board[3][2]||board[1][0]==board[2][0]||board[1][3]==board[2][3]) return 1;

	return 0;
}

void game2048::drawb(unsigned int board[4][4])
{
	cout<<'\t'<<"+--------------------------------+\n";
	for (int i=0;i<4;i++)
	{
		cout<<"\t|\t |\t |\t |\t |\n";
		cout<<'\t';
		cout<<"|";
		for (int j=0;j<4;j++)
		{
			if (board[i][j])
			{
				cout<<board[i][j]<<"\t |";
			}
			else
			{
				cout<<"\t |";
			}
;			}
		cout<<"\n";
		cout<<'\t'<<"+--------------------------------+";
		cout<<"\n";
	}

	}

void game2048::shift(char move)
{
	unsigned int oldb[4][4];int i,j;
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			oldb[i][j]=board[i][j];

	if (move =='W')
	{
		for(j=0;j<4;j++)
		{
			for (i=3;i>=0;i--)
			{
				if (board[i][j]==0)
				{
					int k;
					for (k=i;k<3;k++)
					{
						board[k][j]=board[k+1][j];
					}
					board[3][j]=0;
				}
			}
			if (board[0][j]==board[1][j]&&board[0][j])
			{
				if (board[2][j]==board[3][j])
				{
					board[0][j]*=2;
					board[1][j]=2*board[2][j];
					board[2][j]=0;
					board[3][j]=0;
					score+=board[0][j]+board[1][j];
				}
				else
				{
					board[0][j]*=2;
					board[1][j]=board[2][j];
					board[2][j]=board[3][j];
					board[3][j]=0;
					score+=board[0][j];
				}
			}
			else if (board[1][j]==board[2][j]&&board[1][j])
			{
				board[1][j]*=2;
				board[2][j]=board[3][j];
				board[3][j]=0;
				score+=board[1][j];
			}
			else if (board[2][j]==board[3][j]&&board[3][j])
			{
				board[2][j]*=2;
				board[3][j]=0;
				score+=board[2][j];
			}
		}
	}
	if (move =='A')
	{
		for(j=0;j<4;j++)
		{
			for (i=3;i>=0;i--)
			{
				if (board[j][i]==0)
				{
					int k;
					for (k=i;k<3;k++)
					{
						board[j][k]=board[j][k+1];
					}
					board[j][3]=0;
				}
			}
			if (board[j][0]==board[j][1]&&board[j][0])
			{
				if (board[j][2]==board[j][3])
				{
					board[j][0]*=2;
					board[j][1]=2*board[j][2];
					board[j][2]=0;
					board[j][3]=0;
					score+=board[j][0]+board[j][1];
				}
				else
				{
					board[j][0]*=2;
					board[j][1]=board[j][2];
					board[j][2]=board[j][3];
					board[j][3]=0;
					score+=board[j][0];
				}
			}
			else if (board[j][1]==board[j][2]&&board[j][1])
			{
				board[j][1]*=2;
				board[j][2]=board[j][3];
				board[j][3]=0;
				score+=board[j][1];
			}
			else if (board[j][2]==board[j][3]&&board[j][3])
			{
				board[j][2]*=2;
				board[j][3]=0;
				score+=board[j][2];
			}
		}
	}
	if (move =='S')
	{
		for(j=0;j<4;j++)
		{
			for (i=0;i<4;i++)
			{
				if (board[i][j]==0)
				{
					int k;
					for (k=i;k>0;k--)
					{
						board[k][j]=board[k-1][j];
					}
					board[0][j]=0;
				}
			}
			if (board[3][j]==board[2][j]&&board[3][j])
			{
				if (board[1][j]==board[0][j])
				{
					board[3][j]*=2;
					board[2][j]=2*board[1][j];
					board[1][j]=0;
					board[0][j]=0;
					score+=board[3][j]+board[2][j];
				}
				else
				{
					board[3][j]*=2;
					board[2][j]=board[1][j];
					board[1][j]=board[0][j];
					board[0][j]=0;
					score+=board[3][j];
				}
			}
			else if (board[2][j]==board[1][j]&&board[2][j])
			{
				board[2][j]*=2;
				board[1][j]=board[0][j];
				board[0][j]=0;
				score+=board[2][j];
			}
			else if (board[1][j]==board[0][j]&&board[0][j])
			{
				board[1][j]*=2;
				board[0][j]=0;
				score+=board[1][j];
			}
		}
	}
	if (move =='D')
	{
		for(j=0;j<4;j++)
		{
			for (i=0;i<4;i++)
			{
				if (board[j][i]==0)
				{
					int k;
					for (k=i;k>0;k--)
					{
						board[j][k]=board[j][k-1];
					}
					board[j][0]=0;
				}
			}
			if (board[j][3]==board[j][2]&&board[j][3])
			{
				if (board[j][1]==board[j][0])
				{
					board[j][3]*=2;
					board[j][2]=2*board[j][1];
					board[j][1]=0;
					board[j][0]=0;
					score+=board[j][3]+board[j][2];
				}
				else
				{
					board[j][3]*=2;
					board[j][2]=board[j][1];
					board[j][1]=board[j][0];
					board[j][0]=0;
					score+=board[j][3];
				}
			}
			else if (board[j][2]==board[j][1]&&board[j][2])
			{
				board[j][2]*=2;
				board[j][1]=board[j][0];
				board[j][0]=0;
				score+=board[j][2];
			}
			else if (board[j][1]==board[j][0]&&board[j][0])
			{
				board[j][1]*=2;
				board[j][0]=0;
				score+=board[j][1];
			}
		}
	}
	moved=0;
	for (i=0;i<4;i++)
		for (j=0;j<4;j++)
			if (oldb[i][j]!=board[i][j])
			{
				moved=1;
				return;
			}
}

class snakesandladders
{
	int player1_pos, player2_pos;	//player positions
	int lastposition;				//last position of player
	char name1[20], name2[20];		//names of the players
	void draw_line(int n, char c);	//for repeatedly couting character
	void showboard();				//displays the board
	void board()			//displays the snake and ladder positions
	{
		clearscr;
		cout << "\n\n";
		draw_line(50, '-');
		cout << "\n\t\tSNAKE POSITIONS" << endl;
		draw_line(50, '-');
		cout << "\n\tFrom 98 to 28 \n\tFrom 95 to 24\n\tFrom 92 to 51\n\tFrom 83 to 19\n\tFrom 73 to 1\n\tFrom 69 to 33\n\tFrom 64 to 36\n\tFrom 59 to 17\n\tFrom 55 to 7\n\tFrom 52 to 11\n\tFrom 48 to 9\n\tFrom 46 to 5\n\tFrom 44 to 22\n\n";
		draw_line(50, '-');
		cout<<"\n\nPress any key to continue";getch();
		cout << "\n\t\tLADDER POSITIONS" << endl;
		draw_line(50, '-');
		cout << "\n\tFrom 8 to 26\n\tFrom 21 to 82\n\tFrom 43 to 77\n\tFrom 50 to 91\n\tFrom 62 to 96\n\tFrom 66 to 87\n\tFrom 80 to 100\n";
		draw_line(50, '-');
		cout<<"\n\nPress any key to continue\n";getch();
	}

	void game_pos()			//displays game status
	{
		clearscr;
		draw_line(50, '~');
		cout << "\n                    GAME STATUS" << endl;
		draw_line(50, '~');
		cout << "\n\t---> " << name1 << " is at position " << player1_pos;
		cout << "\n\t---> " << name2 << " is at position " << player2_pos << endl;
		draw_line(50, '~');
	}

	void play_dice(int &pos);	//pseudorandomly generates 1 to 6 dice rolls
public:
	void main();		//main function
	snakesandladders()	//constructor function
	{
		player1_pos = 0;
		player2_pos = 0;
	}
};

void snakesandladders::play_dice(int &pos)
{
	srand(time(0));
	int dice=rand()%6+1;
	if (pos+dice<=100)
	{
		if (dice!=1)
			cout << "\nYou got " << dice << " points!" << endl;
		else
			cout << "\nYou got 1 point!" << endl;
		pos += dice;
	}
	else
	{
		cout<<"\nUh oh! You got "<< dice <<" points but you can only move if you get "<< 100-pos<<" or less\n";
	}
	cout << "Now you are at position " << pos;
	switch (pos)
	{
		case 98: pos = 28; break;
		case 95: pos = 24; break;
		case 92: pos = 51; break;
		case 83: pos = 19; break;
		case 73: pos = 1;  break;
		case 69: pos = 33; break;
		case 64: pos = 36; break;
		case 59: pos = 17; break;
		case 55: pos = 7;  break;
		case 52: pos = 11; break;
		case 48: pos = 9;  break;
		case 46: pos = 5;  break;
		case 44: pos = 22; break;
		case 8:  pos = 26; break;
		case 21: pos = 82; break;
		case 43: pos = 77; break;
		case 50: pos = 91; break;
		case 54: pos = 93; break;
		case 62: pos = 96; break;
		case 66: pos = 87; break;
		case 80: pos = 100;
	}
}

void snakesandladders::showboard()
{
	cout<<"     +---+---+---+---+---+---+---+---+---+---+\n     |";
	for (int i=0;i<10;i++)
	{
		for (int j=0;j<10;j++)
		{
			short pos,k=0;
			if (i%2)
				pos=10*(9-i)+j+1;
			else
				pos=10*(9-i)+(9-j)+1;
			if (pos==player1_pos&&player1_pos==player2_pos)
			{
				cout<<name1[0]<<"&"<<name2[0];
				k=1;
			}
			else if (pos==player1_pos)
			{
				cout<<'"'<<name1[0]<<'"';
				k=1;
			}
			else if (pos==player2_pos)
			{
				cout<<'"'<<name2[0]<<'"';
				k=1;
			}
			else
				switch(pos)
				{
					case 98: cout<<"{1}";k=1;break;
					case 28: cout<<"{1}";k=1;break;
					case 95: cout<<"{2}";k=1;break;
					case 24: cout<<"{2}";k=1;break;
					case 92: cout<<"{3}";k=1;break;
					case 51: cout<<"{3}";k=1;break;
					case 83: cout<<"{4}";k=1;break;
					case 19: cout<<"{4}";k=1;break;
					case 73: cout<<"{5}";k=1;break;
					case 1:  cout<<"{5}";k=1;break;
					case 69: cout<<"{6}";k=1;break;
					case 33: cout<<"{6}";k=1;break;
					case 64: cout<<"{7}";k=1;break;
					case 36: cout<<"{7}";k=1;break;
					case 59: cout<<"{8}";k=1;break;
					case 17: cout<<"{8}";k=1;break;
					case 55: cout<<"{9}";k=1;break;
					case 7:  cout<<"{9}";k=1;break;
					case 52: cout<<"{A}";k=1;break;
					case 11: cout<<"{A}";k=1;break;
					case 48: cout<<"{B}";k=1;break;
					case 9:  cout<<"{B}";k=1;break;
					case 46: cout<<"{C}";k=1;break;
					case 5:  cout<<"{C}";k=1;break;
					case 44: cout<<"{D}";k=1;break;
					case 22: cout<<"{D}";k=1;break;
					case 8:  cout<<"[1]";k=1;break;
					case 26: cout<<"[1]";k=1;break;
					case 21: cout<<"[2]";k=1;break;
					case 82: cout<<"[2]";k=1;break;
					case 43: cout<<"[3]";k=1;break;
					case 77: cout<<"[3]";k=1;break;
					case 50: cout<<"[4]";k=1;break;
					case 91: cout<<"[4]";k=1;break;
					case 54: cout<<"[5]";k=1;break;
					case 93: cout<<"[5]";k=1;break;
					case 62: cout<<"[6]";k=1;break;
					case 96: cout<<"[6]";k=1;break;
					case 66: cout<<"[7]";k=1;break;
					case 87: cout<<"[7]";k=1;break;
					case 80: cout<<"[8]";k=1;break;
					case 100:cout<<"[8]";k=1;break;
				}
			if (k==0) cout<<"   ";
			cout<<'|';
		}
		cout<<'\n'<<"     +---+---+---+---+---+---+---+---+---+---+\n";
		if (i!=9) cout<<"     |";
	}
	cout<<"The tiles with {} represent snakes and those with [] represent ladders.\nThe players are represented by their initials. Press a key to start next round";getch();
}

void snakesandladders::main()
{
	clearscr;
	srand((unsigned)time(0));
	draw_line(50, '=');
	cout << "\n\n\n\t\t SNAKES & LADDERS \n\n\n";
	draw_line(50, '=');
	cout << "\nEnter name of player 1: ";
	cin >> name1;
	cout << "\nEnter name of player 2: ";
	cin >> name2;
	board();
	while (player1_pos < 100 && player2_pos < 100)
	{
		game_pos();
		cout<<endl;
		showboard();
		cout << "\n\n---> " << name1 << ", now it's your turn ==>\tpress any key to roll the dice";
	player1_repeat:
		getch();
		lastposition = player1_pos;
		play_dice(player1_pos);
		if (player1_pos < lastposition)
			cout << "\n\a Oops! A snake found!! Now you are at position " << player1_pos << endl;
		else if (player1_pos > lastposition + 6)
			cout << "\n\a Great! You found a ladder!! Now you are at position " << player1_pos << endl;
		if (player1_pos - lastposition == 6 && player1_pos!=100)
		{
			cout << "\n\n---> Wow " << name1 << "! You get to play again because you got 6 ==> press any key to roll the dice";
			goto player1_repeat;
		}
		cout << "\n\n---> " << name2 << ", now it's your turn ==>\tpress any key to roll the dice";
	player2_repeat:
		getch();
		lastposition = player2_pos;
		play_dice(player2_pos);
		if (player2_pos < lastposition)
			cout << "\n\a Oops! A snake found!! Now you are at position " << player2_pos << endl;
		else if (player2_pos > lastposition + 6)
			cout << "\n\a Great! You found a ladder!! Now you are at position " << player2_pos << endl;
		if (player2_pos - lastposition == 6&&player2_pos!=100)
		{
			cout << "\n\n---> Wow " << name2 << "! You get to play again because you got 6 ==> press any key to roll the dice";
			goto player2_repeat;
		}
		getch();
	}
	clearscr;
	cout << "\n\n\t\t RESULT \n\n";
	draw_line(50, '+');
	cout << "\n";
	game_pos();
	cout << "\n\n\n";
	if (player1_pos > player2_pos)
		cout << "Congrats " << name1 << "!! You are the winner of this game!! Press enter to return to the main menu\n";
	else
		cout << "Congrats " << name2 << "!! You are the winner of this game!! Press enter to return to the main menu\n";
	cout << "\n\n";
	draw_line(50, '+');
    cin.ignore(255,'\n');
	cin.get();
	cin.ignore(255,'\n');
}

void snakesandladders::draw_line(int n, char c)
{
	for (int i = 0; i <= n; i++)
		cout << c;
}

class TicTac
{
	int nCheck(int,int);		//checks whether slot on board is empty
	int rCheck(const char&);	//checks for repeated symbols on row
	int cCheck(const char&);	//checks for repeated symbols on column
	int dCheck(const char&);	//checks for repeated symbols on diagonal
	void playC(int&);			//function that plays for computer
	char square[3][3];			//3x3 board
	int input(const char &ch,const char &mark,int &player); //accept input
	int checkwin(const char&);	//checks for winner

	void board(const char &ch)	//displays the board
	{
		clearscr;
		cout<<"===========================================";
		cout<<"\n\n\n\t\tTIC TAC TOE\n\n\n";
		cout<<"===========================================\n\n\n";
		if (ch=='P')
			cout<<"\tPlayer 1 (X) V/S Player 2 (O)"<<endl<<endl;
		else cout<<"\tPlayer (X)  V/S  Computer (O)"<<endl<<endl;
		cout<<endl;
		cout<<"\t           |     |     "<<endl;
		cout<<"\t        "<<square[0][0]<<"  |  "<<square[0][1]<<"  |  "<<square[0][2]<<endl;
		cout<<"\t      -----|-----|-----"<<endl;
		cout<<"\t        "<<square[1][0]<<"  |  "<<square[1][1]<<"  |  "<<square[1][2]<<endl;
		cout<<"\t      -----|-----|-----"<<endl;
		cout<<"\t        "<<square[2][0]<<"  |  "<<square[2][1]<<"  |  "<<square[2][2]<<endl;
		cout<<"\t           |     |     "<<endl<<endl<<endl;
	}
	void vs_comp();		//human vs computer mode
	void vs_human();	//human vs human mode
	void setboard();	//sets initial board values
public:
	void main();		//main menu
};
void TicTac::setboard()
{
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			square[i][j]=(char)3*i+j+49;
}

void TicTac::vs_comp()
{
	int player,c;
	//char ch;
	srand(time(0));
	player = rand()%2+1;
	clearscr;
	cout<<"===========================================";
	cout<<"\n\n\n\t\tTIC TAC TOE\n\n\n";
	cout<<"===========================================\n\n\n";
	cout<<"By pseudorandom chance, ";

	if (player==0)
		cout<<"you get to play first !";
	else cout<<"Computer gets to play first !";
	cout<<endl;
	getch();
	do
	{
		board('C');
		if (player%2==0)
		{
			do
			{
				char m;
				cout<<"Player, enter a number: ";
				m=getch();
				input(m,'X',player);
				player++;
				board('C');
			} while(player%2==0);
		}
		else
			playC(player);
	} while(!(c=checkwin('X')));

	if (c==1) cout<<"===>\aYou win! Amazing!";
	else if (c==2) cout<<"===>\aThe computer wins! Too bad!";
	else cout<<"===>\aIt's a draw!";
	getch();
}

void TicTac::main()
{
	while (1)
	{
		clearscr;
		setboard();
		cout<<"===========================================";
		cout<<"\n\n\n\t\tTIC TAC TOE\n\n\n";
		cout<<"===========================================\n\n\n";
		cout<<"\tChoose a mode: \n\t1. Human vs Human\n\t2. Human vs Computer\n\t3. Exit";
	start:
		char ch=getch();
		if (ch=='1')
			vs_human();
		else if (ch=='2')
			vs_comp();
		else if (ch=='3')
			return;
		else goto start;
	}
}

void TicTac::playC(int &player)
{
	cout<<endl<<endl<<"Computer is thinking . . ."<<endl;
	int k=0;
		if (rCheck('O')!=1)
			if (cCheck('O')!=1)
				if (dCheck('O')!=1)
					k=1;
	if (k==1)
		if (rCheck('X')!=1)
			if (cCheck('X')!=1)
				if (dCheck('X')!=1)
					k=2;
	if (k==2)
	{
		if (nCheck(1,1))
			square[1][1]='O';
		else
			while (1)
			{
				if (player==9)
				{
					for (int i=0;i<3;i++)
						for (int j=0;j<3;j++)
						{
							if (square[i][j]!='O'&&square[i][j]!='X')
							{
								square[i][j]='O';
								break;
							}
						}
				}
				srand(time(0));
				int i=rand()%3,j=rand()%3;
				if(nCheck(i,j)==1)
				{
					square[i][j]='O';
					break;
				}
		}
	}
	board('C');
	player++;
}

int TicTac::nCheck(int i,int j)
{
	if (square[i][j]!='X'&&square[i][j]!='O')
		return 1;
	return 0;
}

int TicTac::rCheck(const char &c)
{
	int valid=0;
	for(int i=0;i<3;i++)
	{
		if((square[i][0]==c&&square[i][1]==c)&&(nCheck(i,2)==1))
		{
			square[i][2]='O';
			valid=1;
			break;
		}
		else if((square[i][0]==c&&square[i][2]==c)&&(nCheck(i,1)==1))
		{
			square[i][1]='O';
			valid=1;
			break;
		}
		else if((square[i][1]==c&&square[i][2]==c)&&(nCheck(i,0)==1))
		{
			square[i][0]='O';
			valid=1;
			break;
		}
	}
	return valid;
}


int TicTac::cCheck(const char &c)
{
	int valid=0;
	for(int i=0;i<3;i++)
	{
		if((square[0][i]==c&&square[1][i]==c)&&(nCheck(2,i)==1))
		{
			square[2][i]='O';
			valid=1;
			break;
		}
		else if((square[0][i]==c&&square[2][i]==c)&&(nCheck(1,i)==1))
		{
			square[1][i]='O';
			valid=1;
			break;
		}
		if((square[1][i]==c&&square[2][i]==c)&&(nCheck(0,i)==1))
		{
			square[0][i]='O';
			valid=1;
			break;
		}
	}
	return valid;
}

int TicTac::dCheck(const char &c)
{
	int valid=0;
	if((square[0][0]==c&&square[1][1]==c)&&(nCheck(2,2)==1))
	{
		square[2][2]='O';
		valid=1;
	}
	else if((square[0][0]==c&&square[2][2]==c)&&(nCheck(1,1)==1))
	{
		square[1][1]='O';
		valid=1;
	}
	else if((square[1][1]==c&&square[2][2]==c)&&(nCheck(0,0)==1))
	{
		square[0][0]='O';
		valid=1;
	}
	else if((square[0][2]==c&&square[1][1]==c)&&(nCheck(2,0)==1))
	{
		square[2][0]='O';
		valid=1;
	}
	else if((square[0][2]==c&&square[2][0]==c)&&(nCheck(1,1)==1))
	{
		square[1][1]='O';
		valid=1;
	}
	else if((square[1][1]==c&&square[2][0]==c)&&(nCheck(0,2)==1))
	{
		square[0][2]='O';
		valid=1;
	}
	return valid;
}

int TicTac::input(const char &ch,const char &mark,int &player)
{
	if (ch == '1' && square[0][0] == '1')
		square[0][0] = mark;
	else if (ch == '2' && square[0][1] == '2')
		square[0][1] = mark;
	else if (ch == '3' && square[0][2] == '3')
		square[0][2] = mark;
	else if (ch == '4' && square[1][0] == '4')
		square[1][0] = mark;
	else if (ch == '5' && square[1][1] == '5')
		square[1][1] = mark;
	else if (ch == '6' && square[1][2] == '6')
		square[1][2] = mark;
	else if (ch == '7' && square[2][0] == '7')
		square[2][0] = mark;
	else if (ch == '8' && square[2][1] == '8')
		square[2][1] = mark;
	else if (ch == '9' && square[2][2] == '9')
		square[2][2] = mark;
	else
	{
		cout<<"Invalid move !";
		player--;
		getch();
		return 0;
	}
	return 1;
}
void TicTac::vs_human()
{
	int player = 1,i=123;
	char mark,ch;
	do
	{
		board('P');
		player = (player % 2)? 1 : 2;
		cout<<"\tPlayer "<<player<<", press a number key: ";
		ch=getch();
		mark = (player == 1)? 'X' : 'O';
		input(ch,mark,player);
		player++;
	}
	while (!(i = checkwin('X')));
	board('P');
	if (i == 1)
		cout<<"===>\aPlayer 1 wins!";
	else if (i==2)
		cout<<"===>\aPlayer 2 wins!";
	else
		cout<<"===>\aIt's a draw!";
	getch();
}
int TicTac::checkwin(const char &c)
{
	for(int i=0;i<3;i++)
	{
		if((square[i][0]==square[i][1])&&(square[i][1]==square[i][2]))
		{
			if(square[i][0]==c)
				return 1;
			else
				return 2;
		}
		else if((square[0][i]==square[1][i])&&(square[0][i]==square[2][i]))
		{
			if(square[0][i]==c)
				return 1;
			else
				return 2;
		}
	}
	if((square[1][1]==square[2][2])&&(square[2][2]==square[0][0]))
	{
		if(square[1][1]==c)
			return 1;
		else
			return 2;
	}
	else if((square[0][2]==square[1][1])&&(square[1][1]==square[2][0]))
	{
		if(square[0][2]==c)
			return 1;
		else
			return 2;
	}
	else
	{
		for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)
			{
				if (square[i][j]!='O'&&square[i][j]!='X')
				{
					return 0;
				}
			}
		return -1;
	}
}

int main()
{

	while (1)
	{
		clearscr;
		cout<<"\t+---------------------+\n\t| Board Games Advance |\n\t+---------------------+\n\n\n";
		cout<<"\tChoose a game: \n\t1. 2048\n\t2. Snakes and ladders\n\t3. Tic-Tac-Toe\n\t4. Exit\n";
	start:
		char ch=getch();
		if (ch=='1')
		{
			game2048 g;
			g.main();
		}
		else if (ch=='4')
		{

			do
			{
				cout<<"\nAre you sure you want to quit? (Y/N)";
				cin>>ch;
				ch&=0x5f;
				if (ch!='Y'&&ch!='N')
				{
					cout<<"Try again\n";
					cin.ignore(255,'\n');
				}
			}while (ch!='Y'&&ch!='N');
			if (ch=='Y') return 0;
			else continue;
		}
		else if (ch=='2')
		{
			snakesandladders s;
			s.main();
		}
		else if (ch=='3')
		{
			TicTac t;
			t.main();
		}
		else goto start;
	}
}
