#pragma once

#include <time.h>
#include <random>
#include <conio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

void cls()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}
void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

enum eDir
{
	STOP,
	LEFT,
	UPLEFT,
	DOWNLEFT,
	RIGHT,
	UPRIGHT,
	DOWNRIGHT
};

enum BonusType
{
	AddBall,

};

class Ball
{
private:
	int x, y;
	int origX, origY;
	int oldX, oldY;
	eDir directional;
public:
	Ball(int posX, int posY)
	{
		origX = posX;
		origY = posY;
		x = posX;
		y = posY;
		oldX = posX;
		oldY = posY;
		directional = STOP;
	}
	void Reset()
	{
		setCursorPosition(x, y);
		cout << " ";
		x = origX;
		y = origY;
		oldX = origX;
		oldY = origY;
	}
	void changeDirectional(eDir d)
	{
		directional = d;
	}
	void randomDirectional(bool random)
	{
		if (random == false)
		{
			switch (directional)
			{
			case UPLEFT:
				directional = DOWNLEFT;
				break;
			case UPRIGHT:
				directional = DOWNRIGHT;
				break;
			case DOWNLEFT:
				directional = UPLEFT;
				break;
			case DOWNRIGHT:
				directional = UPRIGHT;
				break;
			default:
				directional = (eDir)((rand() % 6) + 1);
				break;
			}
		}
		else directional = (eDir)((rand() % 6) + 1);
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline int getOldX() { return oldX; }
	inline int getOldY() { return oldY; }
	inline eDir getDirectional() { return directional; }
	void Move()
	{/*
		setCursorPosition(x, y);
		cout << " ";
		*/

		oldX = x;
		oldY = y;

		switch (directional)
		{

		case STOP:
			break;
		case LEFT:
			x--;
			break;
		case UPLEFT:
			x--; y--;
			break;
		case DOWNLEFT:
			x--; y++;
			break;
		case RIGHT:
			x++;
			break;
		case UPRIGHT:
			x++; y--;
			break;
		case DOWNRIGHT:
			x++; y++;
			break;
		default:
			break;
		}
	}
};

class Paddle
{
private:
	int x, y;
	int origX, origY;
public:
	Paddle()
	{
		x = y = 0;
	}
	Paddle(int posX, int posY) :Paddle()
	{
		origX = posX;
		origY = posY;
		x = posX;
		y = posY;
	}
	inline void Reset()
	{
		setCursorPosition(x + 1, y + 1);
		cout << " ";
		setCursorPosition(x + 1, y + 2);
		cout << " ";
		setCursorPosition(x + 1, y + 3);
		cout << " ";

		x = origX;
		y = origY;
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUP()
	{
		setCursorPosition(x + 1, y + 3);
		cout << " ";
		setCursorPosition(x + 1, y + 2);
		cout << " ";
		y--;
		y--;
	}
	inline void moveDOWN()
	{
		setCursorPosition(x + 1, y + 1);
		cout << " ";
		setCursorPosition(x + 1, y + 2);
		cout << " ";
		y++;
		y++;
	}
};

class Obstacles
{
private:
	int x, y, length;
	bool vertical = true;
public:
	Obstacles()
	{
		x = rand();
		y = rand();
		length = rand();
		vertical = rand() % 2;
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline int getLength() { return length; }
	inline bool getVertical() { return vertical; }
};

class GameManager
{
private:
	int width, height;
	int score1, score2;
	char up1, up2, down1, down2;
	bool quit;
	bool boolObs;
	bool addBall;
	Ball* ball1;
	Ball* ball2;
	Paddle* player1;
	Paddle* player2;
	Obstacles* Obstacle;

	int ObstacleTimer = 0;
	int ObstacleX;
	int ObstacleY;
	int ObstacleLen;
	bool ObsVertical;

public:
	GameManager(int width, int height)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w'; down1 = 's';
		up2 = 'i'; down2 = 'k';
		score1 = score2 = 0;
		this->width = width;
		this->height = height;
		ball2 = new Ball(width / 2, height / 2);
		ball1 = new Ball(width / 2, height / 2);
		player1 = new Paddle(1, height / 2 - 3);
		player2 = new Paddle(width - 2, height / 2 - 3);
		boolObs = false;
		addBall = false;
	}
	~GameManager()
	{
		delete ball1, ball2, player1, player2;
	}
	void Reset()
	{
		setCursorPosition(0, 0);
		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";

		for (int i = 1; i < height; i++)
		{
			setCursorPosition(0, i);
			cout << "\xB2";
			setCursorPosition(width + 1, i);
			cout << "\xB2";
		}

		cout << endl;

		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";
	}
	inline int GetWidth() { return width; }
	inline int GetHeight() { return height; }
	void ScoreUP(Paddle* player)
	{
		if (player == player1)
			score1++;
		else if (player == player2)
			score2++;

		this->Reset();
		ball1->Reset();
		ball2->Reset();
		player1->Reset();
		player2->Reset();
	}
	void Draw()
	{
		int ballX = ball1->getX();
		int ballY = ball1->getY();


		int player1X = player1->getX();
		int player1Y = player1->getY();

		int player2X = player2->getX();
		int player2Y = player2->getY();


		if (boolObs == true)
		{
			int Obstacletimer = rand() % 1000;
			int ObstacleX = Obstacle->getX();
		}


		setCursorPosition(ballX, ballY);
		cout << "O";

		setCursorPosition(player1X + 1, player1Y + 1);
		cout << "\xDB";
		setCursorPosition(player1X + 1, player1Y + 2);
		cout << "\xDB";
		setCursorPosition(player1X + 1, player1Y + 3);
		cout << "\xDB";

		setCursorPosition(player2X + 1, player2Y + 1);
		cout << "\xDB";
		setCursorPosition(player2X + 1, player2Y + 2);
		cout << "\xDB";
		setCursorPosition(player2X + 1, player2Y + 3);
		cout << "\xDB";

		setCursorPosition(0, height + 1);
		cout << "Score 1:" << score1 << "\t\t\tScore 2:" << score2 << endl;

		Sleep(1);
	}
	void Input()
	{
		ball1->Move();
		int ballX = ball1->getX();
		int ballY = ball1->getY();
		int player1X = player1->getX();
		int player1Y = player1->getY();
		int player2X = player2->getX();
		int player2Y = player2->getY();

		if (_kbhit())
		{
			char current = _getch();
			if (current == up1)
				if (player1Y > 0)
					player1->moveUP();

			if (current == down1)
				if (player1Y + 4 < height)
					player1->moveDOWN();

			if (current == up2)
				if (player2Y > 0)
					player2->moveUP();
			if (current == down2)
				if (player2Y + 4 < height)
					player2->moveDOWN();
			if (ball1->getDirectional() == STOP)
				ball1->randomDirectional(false);

			if (current == 'q')
				quit = true;
		}
	}
	void Logic()
	{
		int ballX = ball1->getX();
		int ballY = ball1->getY();
		int ballOldX = ball1->getOldX();
		int ballOldY = ball1->getOldY();
		int player1X = player1->getX();
		int player1Y = player1->getY();
		int player2X = player2->getX();
		int player2Y = player2->getY();

		if (boolObs == false)
		{
			int randomaz = rand() % 40;
			if (randomaz == rand() % 40)
			{
				boolObs = true;
				ObstacleTimer = rand() % 1000;
				Obstacle = new Obstacles();
				ObstacleX = Obstacle->getX() % width - 7;
				ObstacleY = Obstacle->getY() % height - 5;
				width > height ? ObstacleLen = Obstacle->getLength() % height : ObstacleLen = Obstacle->getLength() % width;
				ObsVertical = Obstacle->getVertical();
				if (ObstacleX <= 0) ObstacleX = height / 2;
				if (ObstacleY <= 0) ObstacleY = height / 2;
			}
		}
		if (ObstacleTimer != 0)
		{
			ObstacleTimer--;
			if (ObsVertical == true)
			{
				for (int i = 0; i < ObstacleLen; i++)
				{
					if (ObstacleY + i == height)break;
					setCursorPosition(ObstacleX, ObstacleY + i);
					cout << "\xB2";

					if (ballX == ObstacleX)
						if (ballY == ObstacleY + i)
							if (ball1->getDirectional() == LEFT) ball1->changeDirectional(RIGHT);
							else ball1->randomDirectional(true);
				}

				if (ObstacleTimer == 0)
				{
					for (int i = 0; i < ObstacleLen; i++)
					{
						if (ObstacleY + i == height)break;
						setCursorPosition(ObstacleX, ObstacleY + i);
						cout << " ";

					}
				}
			}
			if (ObsVertical == false)
			{
				for (int i = 0; i < ObstacleLen; i++)
				{
					if (ObstacleX + i == width)break;
					setCursorPosition(ObstacleX + i, ObstacleY);
					cout << "\xB2";

					if (ballX == ObstacleX + i)
						if (ballY == ObstacleY)
							if (ball1->getDirectional() == LEFT) ball1->changeDirectional(RIGHT);
							else ball1->randomDirectional(true);
				}

				if (ObstacleTimer == 0)
				{
					for (int i = 0; i < ObstacleLen; i++)
					{
						if (ObstacleX + i == width)break;
						setCursorPosition(ObstacleX + i, ObstacleY);
						cout << " ";

					}
					boolObs = false;
				}
			}
		}
		else boolObs = false;

		//left
		for (int i = 0; i < 4; i++)
			if (ballX == player1X + 1)
				if (ballY == player1Y + i)
				{
					//if (ball->getDirectional() == LEFT) ball->changeDirectional(RIGHT);
					ball1->changeDirectional(ball1->getDirectional() == UPLEFT ? UPRIGHT : DOWNRIGHT);
					bool leftRand = rand() % 15;
					if (leftRand == false) ball1->changeDirectional(RIGHT);
				}

		//right
		for (int i = 0; i < 4; i++)
			if (ballX == player2X)
				if (ballY == player2Y + i)
				{
					//if (ball->getDirectional() == RIGHT) ball1->changeDirectional(LEFT);
					ball1->changeDirectional(ball1->getDirectional() == UPRIGHT ? UPLEFT : DOWNLEFT);
					bool leftRand = rand() % 15;
					if (leftRand == false) ball1->changeDirectional(LEFT);
				}

		//bott
		if (ballY == height - 1)
			ball1->randomDirectional(false);

		//top
		if (ballY == 1)
			ball1->randomDirectional(false);

		//right
		if (ballX == width)
			ScoreUP(player1);

		//left
		if (ballX == 1)
			ScoreUP(player2);

		setCursorPosition(ballOldX, ballOldY);
		cout << " ";
	}
	void Run()
	{
		try
		{
			if (this->width < 6) throw exception("Error the field length is too small");
			if (this->height < 7) throw exception("Error The field height is too small");

			if (this->width > 190) throw exception("Error the field length is too big");
			if (this->height > 40) throw exception("Error The field height is too big");
		}
		catch (const exception& err)
		{
			cout << "\n\t" << err.what() << "\n\t";
			return;
		}

		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";

		for (int i = 1; i < height; i++)
		{
			setCursorPosition(0, i);
			cout << "\xB2";
			setCursorPosition(width + 1, i);
			cout << "\xB2";
		}

		cout << endl;

		for (int i = 0; i < width + 2; i++)
			cout << "\xB2";

		while (!quit)
		{
			int speed;
			score1 <= 30 ? speed = 33 - score1 + score2 : speed = 0;
			score2 <= 50 ? speed += score2 : speed = speed;
			Draw();
			Input();
			Logic();
			Sleep(speed);
		}
		setCursorPosition(0, height + 1);
	}
};

int main()
{
	GameManager g(100, 20);
	g.Run();
}