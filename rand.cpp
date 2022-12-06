// ����ѷ�壨Amazons���򵥽�����������
// ������ԣ���zhouhy�޸ģ�
// ���ߣ�dgf123/syys
// ��Ϸ��Ϣ��http://www.botzone.org/games#Amazons


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>


#define GRIDSIZE 8
#define OBSTACLE 2
#define judge_black 0
#define judge_white 1
#define grid_black 1
#define grid_white -1

using namespace std;

const int INF=1e9;
int currBotColor; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // ��x��y����¼����״̬
int dx[] = { -1,-1,-1,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,1,-1,0,1 };
double value[3000];

// �ж��Ƿ��ڵ�ͼ��
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}


// �����괦���ӣ�����Ƿ�Ϸ���ģ������
bool ProcStep(int x0, int y0, int x1, int y1, int x2, int y2, int color, bool check_only)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2)))
		return false;
	if (gridInfo[x0][y0] != color || gridInfo[x1][y1] != 0)
		return false;
	if ((gridInfo[x2][y2] != 0) && !(x2 == x0 && y2 == y0))
		return false;
	if (!check_only)
	{
		gridInfo[x0][y0] = 0;
		gridInfo[x1][y1] = color;
		gridInfo[x2][y2] = OBSTACLE;
	}
	return true;
}

double evaluate(int x0,int y0,int x1,int y1,int x2,int y2){
	return rand();
}
int main()
{
	cout<<"rand"<<endl;
	//system("pause");
	freopen("save.txt","r",stdin);
	freopen("boto.txt","w",stdout);
	int x0, y0, x1, y1, x2, y2;

	// ��ʼ������
	gridInfo[0][(GRIDSIZE - 1) / 3] = gridInfo[(GRIDSIZE - 1) / 3][0]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= gridInfo[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = grid_black;
	gridInfo[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = gridInfo[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= gridInfo[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = grid_white;


	int turnID;
	cin >> turnID;
	turnID=(turnID+1)/2;
	freopen("boti.txt","r",stdin);
	// ���뵽��ǰ�غ�Ϊֹ���Լ��Ͷ��ֵ������ж����Ӷ��Ѿ���ָ�����ǰ�غ�
	currBotColor = grid_white; // �ȼ����Լ��ǰ׷�
	for (int i = 0; i < turnID; i++)
	{
		// ������Щ��������𽥻ָ�״̬����ǰ�غ�

		// �����Ƕ����ж�
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			currBotColor = grid_black; // ��һ�غ��յ�������-1, -1��˵�����Ǻڷ�
		else
			ProcStep(x0, y0, x1, y1, x2, y2, -currBotColor, false); // ģ��Է�����

		// Ȼ�����Լ���ʱ���ж�
		// �����ж��ܱ��Լ��ж���һ��
		if (i < turnID - 1)
		{
			cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
			if (x0 >= 0)
				ProcStep(x0, y0, x1, y1, x2, y2, currBotColor, false); // ģ�⼺������
		}
	}

	// �������ߣ���ֻ���޸����²��֣�

	// ����ö�������п��ܵ��·����Ա�֮������á���

	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];
	int posCount = 0;
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if(gridInfo[i][j]!=currBotColor)continue;
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (gridInfo[xx][yy] != 0 || !inMap(xx, yy))
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))
								break;
							if (gridInfo[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (ProcStep(i, j, xx, yy, xxx, yyy, currBotColor, true))
							{
								beginPos[posCount][0] = i;
								beginPos[posCount][1] = j;
								possiblePos[posCount][0] = xx;
								possiblePos[posCount][1] = yy;
								obstaclePos[posCount][0] = xxx;
								obstaclePos[posCount++][1] = yyy;
							}
						}

					}
				}

			}
		}
	}

	int startX, startY, resultX, resultY, obstacleX, obstacleY;
	int choice=0;
	double maxVal=-INF;
	if (posCount > 0)
	{
		srand(time(0));
		for(int i=0;i<posCount;i++){
			value[i]=evaluate(beginPos[choice][0],beginPos[choice][1],possiblePos[choice][0],possiblePos[choice][1],obstaclePos[choice][0],obstaclePos[choice][1]);
			if(value[i]>maxVal){
				maxVal=value[i];
				choice=i;
			}
		}
		choice=rand()%posCount;
		startX = beginPos[choice][0];
		startY = beginPos[choice][1];
		resultX = possiblePos[choice][0];
		resultY = possiblePos[choice][1];
		obstacleX = obstaclePos[choice][0];
		obstacleY = obstaclePos[choice][1];
	}
	else
	{
		startX = -1;
		startY = -1;
		resultX = -1;
		resultY = -1;
		obstacleX = -1;
		obstacleY = -1;
	}
	
	// ���߽���������������ֻ���޸����ϲ��֣�
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
	//system("pause");
	return 0;
}
