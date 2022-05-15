#include <time.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>

void cls()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };
	std::cout.flush();
	if (!GetConsoleScreenBufferInfo(hOut, &csbi))
		abort();
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD written;
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
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
	void Reset(bool first)
	{
		if (first == true)
		{
			setCursorPosition(x, y);
			std::cout << " ";
			x = origX;
			y = origY;
			oldX = origX;
			oldY = origY;
		}
		else
		{
			setCursorPosition(x, y);
			std::cout << " ";
		}
	}
	void changeDirectional(eDir d)
	{
		directional = d;
	}
	void randomDirectional(bool random) // true - real random, false - ricoshet
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
	{
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
		std::cout << " ";
		setCursorPosition(x + 1, y + 2);
		std::cout << " ";
		setCursorPosition(x + 1, y + 3);
		std::cout << " ";

		x = origX;
		y = origY;
	}
	inline int getX() { return x; }
	inline int getY() { return y; }
	inline void moveUP()
	{
		setCursorPosition(x + 1, y + 3);
		std::cout << " ";
		setCursorPosition(x + 1, y + 2);
		std::cout << " ";
		y--;
		y--;
	}
	inline void moveDOWN()
	{
		setCursorPosition(x + 1, y + 1);
		std::cout << " ";
		setCursorPosition(x + 1, y + 2);
		std::cout << " ";
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
	int width, height; // размеры игрового пол€
	int score1, score2; // счЄт игроков 
	int speedAI, currentSpeedAI; // задерка перемещени€ правой платформы под управлением »»
	int PlayTime; //  ол-во отрисованых кадров 
	int ball2Timer; // ¬рем€ жизни второго м€чика
	char up1, up2, down1, down2; // кнопки дл€ управлени€ движением платформы
	char OnAI, OffAI; // кнопки дл€ управлени€ состо€нием »»
	bool quit; // активность кнопки выхода
	bool boolObs; // активность преп€тстви€
	bool addBall; // активность второго м€чика
	bool PaddleAI; // активность управлени€ правой платформой с помощью »»
	bool colize; // пересекал ли первый м€чик область второго м€чика 
	std::string username; // никнейм пользовател€
	Ball* ball1; // первый м€чик
	Ball* ball2; // второй м€чик
	Paddle* player1; // перва€ платформа 
	Paddle* player2; // втара€ платформа
	Obstacles* Obstacle; // преп€тствие

	std::ofstream fileOutput; // 
	std::ifstream fileInput;

	int ObstacleTimer = 0;
	int ObstacleX;
	int ObstacleY;
	int ObstacleLen;
	bool ObsVertical;
	int progressiya;

public:
	GameManager(int width, int height)
	{
		srand(time(NULL));
		quit = false;
		up1 = 'w'; down1 = 's';
		up2 = 'i'; down2 = 'k';
		OnAI = 'v'; OffAI = 'b';
		score1 = score2 = 0;
		this->width = width;
		this->height = height;
		ball2 = new Ball(width / 2, height / 2);
		ball1 = new Ball(width / 2, height / 2);
		player1 = new Paddle(1, height / 2 - 3);
		player2 = new Paddle(width - 2, height / 2 - 3);
		boolObs = false;
		addBall = true;
		PaddleAI = false;
		colize = true;
		speedAI = 9;
		PlayTime = 0;
		currentSpeedAI = 0;
		ball2Timer = 150;
		progressiya = 500;
	}
	~GameManager()
	{
		delete ball1, ball2, player1, player2;
	}
	void Reset()
	{
		setCursorPosition(0, 0);
		for (int i = 0; i < width + 2; i++)
			std::cout << "\xB2";

		for (int i = 1; i < height; i++)
		{
			setCursorPosition(0, i);
			std::cout << "\xB2";
			setCursorPosition(width + 1, i);
			std::cout << "\xB2";
		}

		std::cout << std::endl;

		for (int i = 0; i < width + 2; i++)
			std::cout << "\xB2";
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
		ball1->Reset(true);
		if (ball2->getX() >= width || ball2->getY() >= height)
			ball2->Reset(true);
		else
			ball2->Reset(false);

		ball1->randomDirectional(true);
	}
	void AI()
	{
		if (PaddleAI == false) return;

		int ball1X = ball1->getX();
		int ball1Y = ball1->getY();
		
		int ball2X = ball2->getX();
		int ball2Y = ball2->getY();
		
		int player2Y = player2->getY();
		
		if (PlayTime % progressiya == 0 && speedAI > 1)
		{
			progressiya += progressiya*2;
			speedAI--;
		}

		if (colize == false)
		{
			if (currentSpeedAI <= 0 && speedAI > 0) currentSpeedAI = speedAI;
			else
			{
				currentSpeedAI--;
				if (currentSpeedAI == 0)
				{
					if (ball1Y == player2Y + 1 || ball1Y == player2Y + 2 || ball1Y == player2Y + 3)
						return;
					else if (ball1Y < player2Y + 2)
						player2->moveUP();
					else if (ball1Y > player2Y + 2)
						player2->moveDOWN();
				}
			}
			return;

		}
			
		if (ball1X > ball2X)
		{
			if (currentSpeedAI <= 0 && speedAI > 0) currentSpeedAI = speedAI;
			else
			{
				currentSpeedAI--;
				if (currentSpeedAI == 0)
				{
					if (ball1Y == player2Y + 1 || ball1Y == player2Y + 2 || ball1Y == player2Y + 3)
						return;
					else if (ball1Y < player2Y + 2)
						player2->moveUP();
					else if (ball1Y > player2Y + 2)
						player2->moveDOWN();
				}
			}
			return;
		}
		else
		{
			if (currentSpeedAI <= 0 && speedAI > 0) currentSpeedAI = speedAI;
			else
			{
				currentSpeedAI--;
				if (currentSpeedAI == 0)
				{
					if (ball2Y == player2Y + 1 || ball2Y == player2Y + 2 || ball2Y == player2Y + 3)
						return;
					else if (ball2Y < player2Y + 2)
						player2->moveUP();
					else if (ball2Y > player2Y + 2)
						player2->moveDOWN();
				}
			}
			return;
		}
	}
	void Draw()
	{
		int ballX = ball1->getX();
		int ballY = ball1->getY();

		int player1X = player1->getX();
		int player1Y = player1->getY();

		int player2X = player2->getX();
		int player2Y = player2->getY();

		if (addBall == true && colize == true && PlayTime % 2 == 0)
		{
			int ball2X = ball2->getX();
			int ball2Y = ball2->getY();
			setCursorPosition(ball2X, ball2Y);
			std::cout << "O";
		}

		if (boolObs == true)
		{
			int Obstacletimer = rand() % 1000;
			int ObstacleX = Obstacle->getX();
		}


		setCursorPosition(ballX, ballY);
		std::cout << "O";

		setCursorPosition(player1X + 1, player1Y + 1);
		std::cout << "\xDB";
		setCursorPosition(player1X + 1, player1Y + 2);
		std::cout << "\xDB";
		setCursorPosition(player1X + 1, player1Y + 3);
		std::cout << "\xDB";

		setCursorPosition(player2X + 1, player2Y + 1);
		std::cout << "\xDB";
		setCursorPosition(player2X + 1, player2Y + 2);
		std::cout << "\xDB";
		setCursorPosition(player2X + 1, player2Y + 3);
		std::cout << "\xDB";

		setCursorPosition(0, height + 1);
		std::cout << "\nScore 1:" << score1 << "\t\t\tScore 2:" << score2
			<< "\n\ncurrentSpeedAI " << currentSpeedAI << "\tspeedAI " << speedAI << "\tPlayTime " << PlayTime
			<< "\nAddBall " << addBall << "\t\tBallColize " << colize << "\tObstacle " << boolObs
			<< "\n\nw / s - to control the left platform"
			<< "\ni / k - to control the right platform"
			<< "\nv / b - on/off to control the right platform using AI";

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

			if (current == OnAI)
				if (player2Y > 0)
					PaddleAI = true;
			if (current == OffAI)
				if (player2Y + 4 < height)
					PaddleAI = false;

			if (ball1->getDirectional() == STOP)
				ball1->randomDirectional(false);

			if (ball2->getDirectional() == STOP)
				ball2->randomDirectional(false);

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

		int ball2X = ball2->getX();
		int ball2Y = ball2->getY();

		if (PaddleAI == true) AI();

		#pragma region Obstacle
		if (boolObs == false)
		{
			int randomaz = rand() % 40;
			if (randomaz == rand() % 40)
			{
				boolObs = true;
				ObstacleTimer = rand() % 1000;
				Obstacle = new Obstacles();
				ObstacleX = Obstacle->getX() % (width - 7)+3;
				ObstacleY = Obstacle->getY() % (height - 5)+2;
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
					if (ObstacleY + i >= height - 2 || ObstacleY <= 2)break;
					setCursorPosition(ObstacleX, ObstacleY + i);
					std::cout << "\xB2";

					if (ballX == ObstacleX)
						if (ballY == ObstacleY + i)
							if (ball1->getDirectional() == LEFT) ball1->changeDirectional(RIGHT);
							else ball1->randomDirectional(true);

					if (addBall == true && colize == true && PlayTime % 2 == 0)
						if (ball2X == ObstacleX)
							if (ball2Y == ObstacleY + i)
								if (ball1->getDirectional() == LEFT) ball1->changeDirectional(RIGHT);
								else ball1->randomDirectional(true);
				}

				if (ObstacleTimer == 0)
				{
					for (int i = 0; i < ObstacleLen; i++)
					{
						if (ObstacleY + i == height)break;
						setCursorPosition(ObstacleX, ObstacleY + i);
						std::cout << " ";

					}
				}
			}
			if (ObsVertical == false)
			{
				for (int i = 0; i < ObstacleLen; i++)
				{
					if (ObstacleX + i >= width - 3 || ObstacleX <= 3)break;
					setCursorPosition(ObstacleX + i, ObstacleY);
					std::cout << "\xB2";

					if (ballX == ObstacleX + i)
						if (ballY == ObstacleY)
							if (ball1->getDirectional() == LEFT) ball1->changeDirectional(RIGHT);
							else ball1->randomDirectional(true);

					if (addBall == true && colize == true && PlayTime % 2 == 0)
						if (ball2X == ObstacleX)
							if (ball2Y == ObstacleY + i)
								if (ball1->getDirectional() == LEFT) ball1->changeDirectional(RIGHT);
								else ball1->randomDirectional(true);
				}

				if (ObstacleTimer == 0)
				{
					for (int i = 0; i < ObstacleLen; i++)
					{
						if (ObstacleX + i == width)break;
						setCursorPosition(ObstacleX + i, ObstacleY);
						std::cout << " ";

					}
					boolObs = false;
				}
			}
		}
		else boolObs = false;

		#pragma endregion

		#pragma region AddBall

		if (addBall == false && PlayTime%150 == rand()%150)
		{
			addBall = true;
			ball2Timer = rand() % 1000 + 100;
			colize = false;
		}
		
		if (addBall == true && colize == false)
		{
			if (PlayTime % 20 >= 10)
			{
				setCursorPosition(ball2X, ball2Y);
				std::cout << " ";
			}
			else
			{
				setCursorPosition(ball2X, ball2Y);
				std::cout << "0";
			}
			if (ballX == ball2X - 1 || ballX == ball2X || ballX == ball2X + 1)
			{
				if (ballY == ball2Y - 1 || ballY == ball2Y || ballY == ball2Y + 1)
				{
					setCursorPosition(ball2X, ball2Y);
					std::cout << " ";
					colize = true;
				}
			}
		}

		if (ball2Timer <= 0) addBall = false;

		if (addBall == true && colize == true && PlayTime % 2 == 0)
		{
			ball2Timer--;

			//left
			for (int i = 0; i < 4; i++)
				if (ball2X == player1X + 1)
					if (ball2Y == player1Y + i)
					{
						ball2->changeDirectional(ball2->getDirectional() == UPLEFT ? UPRIGHT : DOWNRIGHT);
						bool leftRand = rand() % 15;
						if (leftRand == false) ball2->changeDirectional(RIGHT);
					}

			//right
			for (int i = 0; i < 4; i++)
				if (ball2X == player2X)
					if (ball2Y == player2Y + i)
					{
						ball2->changeDirectional(ball2->getDirectional() == UPRIGHT ? UPLEFT : DOWNLEFT);
						bool leftRand = rand() % 15;
						if (leftRand == false) ball2->changeDirectional(LEFT);
					}
			//bott
			if (ball2Y >= height - 1)
				ball2->randomDirectional(false);

			//top
			if (ball2Y <= 1)
				ball2->randomDirectional(false);

			//right
			if (ball2X >= width)
			{
				//clenOldBall();
				ball2->Reset(true);
				ScoreUP(player1);
			}

			//left
			if (ball2X <= 1)
			{
				//clenOldBall();
				ball2->Reset(true);
				ScoreUP(player2);
			}

			ball2->Move();
		}
		
		if (ball2Timer <= 0) { addBall == false; colize = false; }
		#pragma endregion

		//left
		for (int i = 0; i < 4; i++)
			if (ballX == player1X + 1)
				if (ballY == player1Y + i)
				{
					ball1->changeDirectional(ball1->getDirectional() == UPLEFT ? UPRIGHT : DOWNRIGHT);
					bool leftRand = rand() % 15;
					if (leftRand == false) ball1->changeDirectional(RIGHT);
				}

		//right
		for (int i = 0; i < 4; i++)
			if (ballX == player2X)
				if (ballY == player2Y + i)
				{
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

		if (addBall == true && PlayTime % 2 == 0)
		{
			int ballOld2X = ball2->getOldX();
			int ballOld2Y = ball2->getOldY();
			setCursorPosition(ballOld2X, ballOld2Y);
			std::cout << " ";
		}
		setCursorPosition(ballOldX, ballOldY);
		std::cout << " ";
	}
	void Run()
	{
		try
		{
			if (this->width < 90) throw std::exception("Error the field length is too small");
			if (this->height < 20) throw std::exception("Error The field height is too small");

			if (this->width > 190) throw std::exception("Error the field length is too big");
			if (this->height > 40) throw std::exception("Error The field height is too big");
		}
		catch (const std::exception& err)
		{
			std::cout << "\n\t" << err.what() << "\n\t";
			return;
		}

		std::cout << "\n\tInput username: ";
		std::cin >> username;

		std::system("cls");
		
		const int cou = 10;
		int Icou = 1;

		fileInput.open("Rates.txt");
		std::string topUsernames[cou];
		int topScore[cou]{0};
		
		setCursorPosition(0, height + 11);
		std::cout << "Top Usernames";
		setCursorPosition(32, height + 11); 
		std::cout << "Top Score";

		for (int i = 0; i < cou; i++)
			if (fileInput.eof() || fileInput.fail() || fileInput.bad()) break;
			else
			{
				fileInput >> topUsernames[i];
				fileInput >> topScore[i];
				if (topUsernames[i] != "" && topScore[i] != 0)
				{
					setCursorPosition(0, height + 12 + i);
					std::cout << topUsernames[i];
					setCursorPosition(32, height + 12 + i);
					std::cout << topScore[i];
					Icou = i + 1;
				}
			}

		fileInput.close();

		
		setCursorPosition(0, 0);
		for (int i = 0; i < width + 2; i++)
			std::cout << "\xB2";

		for (int i = 1; i < height; i++)
		{
			setCursorPosition(0, i);
			std::cout << "\xB2";
			setCursorPosition(width + 1, i);
			std::cout << "\xB2";
		}

		std::cout << std::endl;

		for (int i = 0; i < width + 2; i++)
			std::cout << "\xB2";

		while (!quit)
		{
			PlayTime++;
			int speed;
			score1 <= 30 ? speed = 33 - score1 + score2 : speed = 0;
			score2 <= 50 ? speed += score2 : speed = speed;
			Draw();
			Input();
			Logic();
			Sleep(speed);
		}

		fileOutput.open("Rates.txt");

		int stopCou = 0;

		for (int i = 0; i < Icou; i++)
		{
			if (topScore[i] < score1)
			{
				stopCou = i;
				if (Icou < 10)
					Icou++;
				break;
			}

			if (topUsernames[i] == "" || topScore[i] == 0) continue;

			fileOutput << topUsernames[i] << " " << topScore[i] << std::endl;
		}

		if(stopCou<=10) fileOutput << username << " " << score1 << std::endl;

		for (int i = stopCou; i < Icou; i++)
		{
			if (topUsernames[i] == "" || topScore[i] == 0) continue;
			fileOutput << topUsernames[i] << " " << topScore[i] << std::endl;
		}

		fileOutput.close();

		setCursorPosition(0, height + 1);
	}
};

int main()
{
	GameManager g(100, 20);
	g.Run();
}