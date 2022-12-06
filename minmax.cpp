// ����ѷ�壨Amazons������С����AI 
// ��Ϸ��Ϣ��http://www.botzone.org/games#Amazons


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>
#include <memory.h> 
#include <algorithm>

#define GRIDSIZE 8
#define obstacle 2
#define black 1
#define white -1

using namespace std;

const int INF=1e9;
const double K=0.2;//�������� 
int botCol; // ����ִ����ɫ��1Ϊ�ڣ�-1Ϊ�ף�����״̬��ͬ��
int map[GRIDSIZE][GRIDSIZE] = { 0 }; // ��x��y����¼����״̬
int kingMove[2][8][8],queenMove[2][8][8],map_flex[8][8];
int dx[] = { -1,-1,-1,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,1,-1,0,1 };
int turnID=-1,maxDepth=1;
const int max_time=950;
bool finish=0;//��û����������һ�� 

struct Move{
	int x0,y0,x1,y1,x2,y2;
	double value;
};

bool cmp1(Move x,Move y){
	return x.value>y.value;
}

bool cmp2(Move x,Move y){
	return x.value<y.value;
}

struct Point{
	int x;
	int y;
	Point(int a,int b){
		x=a,y=b;
	}
};

void prt_map(){
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++)cout<<map[j][i]<<' ';
		cout<<endl;
	}
}

// �ж��Ƿ��ڵ�ͼ��
inline bool inMap(int x, int y)
{
	if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
		return false;
	return true;
}


// �����괦���ӣ�����Ƿ�Ϸ���ģ������
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

void cancel_move(int x0,int y0,int x1,int y1,int x2,int y2,int color){
	map[x2][y2]=0;
	map[x1][y1]=0;
	map[x0][y0]=color;
	return;
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
//���������̵����� 
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
//������������ 
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
 
double tiao_can(double t1,double t2,double p1,double p2,double f,int turnID){//���� 
	double ret=0;
	double kt1=0,kt2=0,kp1=0,kp2=0,kf=0;
	turnID*=2;
	if(turnID<=20)		kt1=0.14,kt2=0.37,kp1=0.13,kp2=0.13,kf=0.20;
	else if(turnID<50)  kt1=0.30,kt2=0.25,kp1=0.20,kp2=0.20,kf=0.05;
	else 				kt1=0.80,kt2=0.10,kp1=0.05,kp2=0.05,kf=0.00;
	ret=kt1*t1 + kt2*t2 + kp1*p1 + kp2*p2 + kf*f;
	return ret;
}

double evaluate(){
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
	return ret;	
}
//**********************************************************************���� 
Move minMax(int depth,double alpha,double beta){
	int curCol=botCol;
	for(int i=2;i<=depth;i++)curCol*=-1;
	Move ret;
	int posCount = 0;
	Move move[3000];
	
	for (int i = 0; i < GRIDSIZE; ++i) {//ö�����п����·� 
		for (int j = 0; j < GRIDSIZE; ++j) {
			if(map[i][j]!=curCol)continue;
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
							if (make_move(i, j, xx, yy, xxx, yyy, curCol, true))
							{
								move[posCount].x0=i;
								move[posCount].y0=j;
								move[posCount].x1=xx;
								move[posCount].y1=yy;
								move[posCount].x2=xxx;
								move[posCount++].y2=yyy;
							}
						}

					}
				}

			}
		}
	}
	
	int i=0;
	if(posCount>0){
		for(i=0; i<posCount; i++){
			//cout<<i<<endl;
			if(1000*clock()/(double)CLOCKS_PER_SEC>=max_time)
			break;
			//cout<<111<<endl;
			//cout<<curCol<<endl;
			make_move(move[i].x0,move[i].y0,move[i].x1,move[i].y1,move[i].x2,move[i].y2,curCol,false);
			if(depth==maxDepth){
				move[i].value=evaluate();
				if(curCol!=botCol)move[i].value*=-1;
			}
			else move[i].value=-minMax(depth+1,-beta,-alpha).value;
			cancel_move(move[i].x0,move[i].y0,move[i].x1,move[i].y1,move[i].x2,move[i].y2,curCol);
			//cout<<move[i].x0<<' '<<move[i].y0<<' '<<move[i].x1<<' '<<move[i].y1<<' '<<move[i].x2<<' '<<move[i].y2<<' '<<move[i].value<<endl;
			if(move[i].value>=beta){
				ret=move[i];
				return ret;
			}
			if(move[i].value>alpha){
				alpha=move[i].value;
				ret=move[i];
			}
		}
		if(i==posCount && depth==1)finish=1;
	}
	else{
		ret.value=evaluate();
		if(curCol!=botCol)ret.value*=-1;
	}
	return ret;
}

int main()
{
	cout<<"minmax"<<endl;
	//system("pause");
	freopen("save.txt","r",stdin);
	freopen("boto.txt","w",stdout);
	int x0, y0, x1, y1, x2, y2;

	// ��ʼ������
	map[0][(GRIDSIZE - 1) / 3] = map[(GRIDSIZE - 1) / 3][0]
		= map[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][0]
		= map[GRIDSIZE - 1][(GRIDSIZE - 1) / 3] = black;
	map[0][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = map[(GRIDSIZE - 1) / 3][GRIDSIZE - 1]
		= map[GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)][GRIDSIZE - 1]
		= map[GRIDSIZE - 1][GRIDSIZE - 1 - ((GRIDSIZE - 1) / 3)] = white;

	cin >> turnID;
	turnID=(turnID+1)/2;
	freopen("boti.txt","r",stdin);
	// ���뵽��ǰ�غ�Ϊֹ���Լ��Ͷ��ֵ������ж����Ӷ��Ѿ���ָ�����ǰ�غ�
	botCol = white; // �ȼ����Լ��ǰ׷�
	for (int i = 0; i < turnID; i++)
	{
		// ������Щ��������𽥻ָ�״̬����ǰ�غ�

		// �����Ƕ����ж�
		cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
		if (x0 == -1)
			botCol = black; // ��һ�غ��յ�������-1, -1��˵�����Ǻڷ�
		else
			make_move(x0, y0, x1, y1, x2, y2, -botCol, false); // ģ��Է�����

		// Ȼ�����Լ���ʱ���ж�
		// �����ж��ܱ��Լ��ж���һ��
		if (i < turnID - 1)
		{
			cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
			if (x0 >= 0)
				make_move(x0, y0, x1, y1, x2, y2, botCol, false); // ģ�⼺������
		}
	}

//prt_map();
	// �������ߣ���ֻ���޸����²��֣�
	Move ans;
	for(maxDepth=1;maxDepth<=2;maxDepth++){//������������ 
		Move curAns=minMax(1,-INF,INF);
		if(finish){
			//cout<<1<<endl;
			ans=curAns;
			finish=0;
		}
		else break;
	}
	cout<<ans.x0<<' '<<ans.y0<<' '<<ans.x1<<' '<<ans.y1<<' '<<ans.x2<<' '<<ans.y2;
	
	return 0;
}
