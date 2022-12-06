// 亚马逊棋（Amazons）贪心AI 
// 游戏信息：http://www.botzone.org/games#Amazons


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>
#include <memory.h> 

#define GRIDSIZE 8
#define obstacle 2
#define black 1
#define white -1

using namespace std;

const int INF=1e9;
const double K=0.2;//先手优势 
int botCol; // 我所执子颜色（1为黑，-1为白，棋盘状态亦同）
int map[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int kingMove[2][8][8],queenMove[2][8][8],map_flex[8][8];
int dx[] = { -1,-1,-1,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,1,-1,0,1 };
double value[3000];

struct Point{
	int x;
	int y;
	Point(int a,int b){
		x=a,y=b;
	}
};

// 判断是否在地图内
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}


// 在坐标处落子，检查是否合法或模拟落子
bool make_move(int x0, int y0, int x1, int y1, int x2, int y2, int color, bool check_only)
{
	if ((!inMap(x0, y0)) || (!inMap(x1, y1)) || (!inMap(x2, y2)))
		return false;
	if (map[x0][y0] != color || map[x1][y1] != 0)
		return false;
	if ((map[x2][y2] != 0) && !(x2 == x0 && y2 == y0))
		return false;
	if (!check_only)
	{
		map[x0][y0] = 0;
		map[x1][y1] = color;
		map[x2][y2] = obstacle;
	}
	return true;
}

void calc_king_move(int col,int kingMove[8][8]){
	queue<Point> que;
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++)
	kingMove[i][j]=INF;
	
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(map[i][j]==col){
			que.push(Point(i,j));
			kingMove[i][j]=0;
		}
	}
	while(!que.empty()){
		Point u=que.front();
		que.pop();
		for(int i=0;i<8;i++){
			int vx=u.x+dx[i],vy=u.y+dy[i];
			if(inMap(vx,vy) && map[vx][vy]==0 && kingMove[vx][vy]>kingMove[u.x][u.y]+1){
				kingMove[vx][vy]=kingMove[u.x][u.y]+1;
				que.push(Point(vx,vy));
			}
		}
	}
}

void calc_queen_move(int col,int queenMove[8][8]){
	queue<Point> que;
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++)
	queenMove[i][j]=INF;
	
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(map[i][j]==col){
			que.push(Point(i,j));
			queenMove[i][j]=0;
		}
	}
	while(!que.empty()){
		Point u=que.front();
		que.pop();
		for(int i=0;i<8;i++){
			for(int j=1;j<8;j++){
				int vx=u.x+dx[i]*j;
				int vy=u.y+dy[i]*j;
				if(!inMap(vx,vy) || map[vx][vy]!=0)break;
				if(queenMove[vx][vy]>queenMove[u.x][u.y]+1){
					queenMove[vx][vy]=queenMove[u.x][u.y]+1;
					que.push(Point(vx,vy));
				}
			}
		}
	}
}

void calc_map_flex(){
	memset(map_flex,0,sizeof(map_flex));
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(map[i][j]==0){
			for(int k=0;k<8;k++){
				if(inMap(i+dx[k],j+dy[k]) && map[i+dx[k]][j+dy[k]]==0)
					map_flex[i][j]++;
			}
		}
	}
}

double calc_pos_flex(int x,int y,int col){
	double ret=0;
	for(int i=0;i<8;i++)
	for(int j=1;j<8;j++){
		int nx=x+dx[i]*j;
		int ny=y+dy[i]*j;
		if(!inMap(nx,ny) || map[nx][ny]!=0)break;
		ret+=map_flex[nx][ny]/(double)j;
	}
	return ret;
}

double tiao_can(double t1,double t2,double p1,double p2,double f,int turnID){
	double ret=0;
	double kt1=0,kt2=0,kp1=0,kp2=0,kf=0;
	turnID*=2;
	if(turnID<=20)		kt1=0.14,kt2=0.37,kp1=0.13,kp2=0.13,kf=0.20;
	else if(turnID<50)  kt1=0.30,kt2=0.25,kp1=0.20,kp2=0.20,kf=0.05;
	else 				kt1=0.80,kt2=0.10,kp1=0.05,kp2=0.05,kf=0.00;
	ret=kt1*t1 + kt2*t2 + kp1*p1 + kp2*p2 + kf*f;
	return ret;
}

double evaluate(int x0,int y0,int x1,int y1,int x2,int y2,int turnID){
	make_move(x0,y0,x1,y1,x2,y2,botCol,false);
	double ret=0;
	
	int myMove=(botCol+1)/2;
	calc_king_move(botCol,kingMove[myMove]);
	calc_king_move(-botCol,kingMove[myMove^1]);
	calc_queen_move(botCol,queenMove[myMove]);
	calc_queen_move(-botCol,queenMove[myMove^1]);
	double t1=0,t2=0;
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(kingMove[myMove][i][j]<kingMove[myMove^1][i][j])t1+=1;
		else if(kingMove[myMove][i][j]>kingMove[myMove^1][i][j])t1-=1;
		else if(kingMove[myMove][i][j]!=INF)t1-=K*1.0;
		
		if(queenMove[myMove][i][j]<queenMove[myMove^1][i][j])t2+=1;
		else if(queenMove[myMove][i][j]>queenMove[myMove^1][i][j])t2-=1;
		else if(queenMove[myMove][i][j]!=INF)t2-=K*1.0;
	}
	
	double p1=0,p2=0;
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(map[i][j]==0){
			p1+=2*(pow(2.0,-(double)queenMove[myMove][i][j]) - pow(2.0,-(double)queenMove[myMove^1][i][j]));
			p2+=min(1.0 , max(-1.0 , (kingMove[myMove^1][i][j] - kingMove[myMove][i][j]) / 6.0));
		}
	}
	
	double f=0;
	calc_map_flex();
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(map[i][j]==botCol)f+=calc_pos_flex(i,j,botCol);
		else if(map[i][j]==-botCol)f-=calc_pos_flex(i,j,-botCol);
	}
	
	ret=tiao_can(t2,t1,p1,p2,f,turnID);
	
	map[x0][y0]=botCol;
	map[x1][y1]=0;
	map[x2][y2]=0;
	return ret;	
}


int main()
{
	cout<<"Aqua"<<endl;
	//system("pause");
	freopen("save.txt","r",stdin);
	freopen("boto.txt","w",stdout);
	int x0, y0, x1, y1, x2, y2;

	// 初始化棋盘
	map[0][(GRIDSIZE - 1) / 3] = map[(GRIDSIZE - 1) / 3][0]
		= map[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= map[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = black;
	map[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = map[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= map[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= map[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = white;


	int turnID;
	cin >> turnID;
	turnID=(turnID+1)/2;
	freopen("boti.txt","r",stdin);
	// 读入到当前回合为止，自己和对手的所有行动，从而把局面恢复到当前回合
	botCol = white; // 先假设自己是白方
	for (int i = 0; i < turnID; i++)
	{
		// 根据这些输入输出逐渐恢复状态到当前回合

		// 首先是对手行动
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			botCol = black; // 第一回合收到坐标是-1, -1，说明我是黑方
		else
			make_move(x0, y0, x1, y1, x2, y2, -botCol, false); // 模拟对方落子

		// 然后是自己当时的行动
		// 对手行动总比自己行动多一个
		if (i < turnID - 1)
		{
			cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
			if (x0 >= 0)
				make_move(x0, y0, x1, y1, x2, y2, botCol, false); // 模拟己方落子
		}
	}

	// 做出决策（你只需修改以下部分）

	// 这里枚举了所有可能的下法，以便之后随机用……

	int beginPos[3000][2], possiblePos[3000][2], obstaclePos[3000][2];
	int posCount = 0;
	for (int i = 0; i < GRIDSIZE; ++i) {
		for (int j = 0; j < GRIDSIZE; ++j) {
			if(map[i][j]!=botCol)continue;
			for (int k = 0; k < 8; ++k) {
				for (int delta1 = 1; delta1 < GRIDSIZE; delta1++) {
					int xx = i + dx[k] * delta1;
					int yy = j + dy[k] * delta1;
					if (map[xx][yy] != 0 || !inMap(xx, yy))
						break;
					for (int l = 0; l < 8; ++l) {
						for (int delta2 = 1; delta2 < GRIDSIZE; delta2++) {
							int xxx = xx + dx[l] * delta2;
							int yyy = yy + dy[l] * delta2;
							if (!inMap(xxx, yyy))
								break;
							if (map[xxx][yyy] != 0 && !(i == xxx && j == yyy))
								break;
							if (make_move(i, j, xx, yy, xxx, yyy, botCol, true))
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
		for(int i=0;i<posCount;i++){//计算每种下法的估价 
			value[i]=evaluate(beginPos[i][0],beginPos[i][1],possiblePos[i][0],possiblePos[i][1],obstaclePos[i][0],obstaclePos[i][1],turnID);
			if(value[i]>maxVal){
				maxVal=value[i];
				choice=i;
			}
		}
		
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
	
	// 决策结束，输出结果（你只需修改以上部分）
	cout << startX << ' ' << startY << ' ' << resultX << ' ' << resultY << ' ' << obstacleX << ' ' << obstacleY << endl;
	
	/*调试代码 
	for(int i=0;i<8;i++){
	for(int j=0;j<8;j++){
		printf("%4d ",kingMove[][j][i]);
	}
	cout<<endl<<endl;
	}
	cout<<endl<<endl;
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			printf("%4d ",queenMove[j][i]);
		}
		cout<<endl<<endl;
	}*/
	//for(int i=0;i<posCount;i++)cout<<value[i]<<' ';
	//system("pause");
	return 0;
}
