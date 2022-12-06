#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<fstream>
#include<graphics.h>
#include<string>
#include<utility>
#include<windows.h>
#include<utility>

using namespace std;
const int black=1,white=-1,obstacle=2,pad=7;;
const int dx[10]={0,1,-1,0,0,1,1,-1,-1};
const int dy[10]={0,0,0,1,-1,1,-1,1,-1};
const bool TRY=1;
bool able[10][10];//��¼��ǰĳ���ӿ���λ�� 
const char graph[10]={'X','.','O','#'};
int map[12][12],mousePos[2];
int step,col=white,playerCol,canRegret=0;
bool isBot,reset;
string botName="null";
fstream save,boti,boto;
PIMAGE title,help1,mapImg,titlePic,longButton1,longButton2,button1,button2;
mouse_msg msg={0};

void put_map();

void init_map(){//��ʼ������ 
	for(int i=1;i<=8;i++)
	for(int j=1;j<=8;j++)
	map[i][j]=0;
	map[1][3]=map[1][6]=map[3][1]=map[3][8]=1;
	map[6][1]=map[6][8]=map[8][3]=map[8][6]=-1;
}

void prt_map(){//��ӡ����
	for(int i=0;i<=8;i++)cout<<i<<' ';
	cout<<endl; 
	for(int i=1;i<=8;i++){
		cout<<i<<' ';
		for(int j=1;j<=8;j++)
		cout<<graph[map[i][j]+1]<<' ';
		cout<<endl;
	}
}

void save_map(int step){//�������� ��һ����step isBot playerCol���������� 
	system("type presave.txt>prepresave.txt");
	system("type save.txt>presave.txt");
	save.open("save.txt");
	save<<step<<' '<<isBot<<' '<<playerCol<<' '<<botName<<endl;
	for(int i=1;i<=8;i++){
		for(int j=1;j<=8;j++)
		save<<map[i][j]<<' ';
		save<<endl;
	}
	save.close();
	//cout<<"map saved"<<endl;
}

bool get_save(){//��Ϸ��ʼʱ������޴浵������ 
	system("type handsave.txt>save.txt");
	system("type botisave.txt>boti.txt");
	save.open("save.txt");
	save>>step;
	char yn;
	if(step){//���� 
		save>>isBot>>playerCol>>botName;
		for(int i=1;i<=8;i++)
		for(int j=1;j<=8;j++)
		save>>map[i][j];
		col=white;
		for(int i=1;i<=step;i++)col*=-1;
	}
	save.close();
	if(step)return 1;
	return 0;
}

bool out_map(int x,int y){//�жϳ��� 
	if(x<1||x>8||y<1||y>8)return 1;
	return 0;
}

bool check_queen(int x1,int y1,int x2,int y2){//�жϷ��ϻʺ��߷� 
	if(!(x1==x2||y1==y2||x1+y1==x2+y2||x1-y1==x2-y2))return 0;
	int dx=(x2-x1) / max(abs(x2-x1),1), dy=(y2-y1) / max(abs(y2-y1),1);//��max��ֹ��0 
	for(int i=x1+dx; i!=x2; i+=dx)
	for(int j=y1+dy; j!=y2; j+=dy){
		if(map[i][j]!=0)return 0;
	}
	return 1;
}

bool calc_able(int x1,int y1){//��������x1,y1�Ŀɶ���Χ 
	memset(able,0,sizeof(able));
	bool ret=0;
	for(int i=1; i<=8; i++){
		for(int k=1; k<=8; k++){
			int nx = x1 + dx[i]*k;
			int ny = y1 + dy[i]*k;
			if(out_map(nx,ny) || map[nx][ny])break;
			able[nx][ny]=1;
			ret=1;
		}
	}
	return ret;
}

pair<int,int> calc_pos(int i,int j){//�����������������Ļ���� 
	int y=i*pad + i*56 - 28, x=j*pad + j*56 - 28;
	return make_pair(x,y);	
}

void show_move(int x1,int y1,int x2,int y2,int col){//�ƶ����Ӷ��� 
	pair <int,int> pos=calc_pos(x1,y1);
	int xx1=pos.first,yy1=pos.second;
	pos=calc_pos(x2,y2);
	int xx2=pos.first,yy2=pos.second;
	
	int dx,dy;
	if(xx1==xx2)dx=0;
	else if(xx1>xx2)dx=-6;
	else dx=6;
	if(yy1==yy2)dy=0;
	else if(yy1>yy2)dy=-6;
	else dy=6;
	
	int x=xx1,y=yy1;
	for(;is_run();delay_fps(60)){
		put_map();
		setcolor(BLACK);
		if(col==white)setfillcolor(WHITE);
		else if(col==black)setfillcolor(BLACK);
		else if(col==obstacle)setfillcolor(BLUE);
		circle(x, y, 25);
		floodfill(x, y, BLACK);
		x+=dx,y+=dy;
		if(abs(x-xx2)<=6 && abs(y-yy2)<=6)break;
	}
}

void move_piece(int x1,int y1,int x2,int y2,int col){//�ƶ����� 
	map[x1][y1]=0;
	show_move(x1,y1,x2,y2,col);
	map[x2][y2]=col;
	put_map();
	//Sleep(100);
	return;
}

void obstacle_appear(int x,int y){//�ϰ����ֶ��� 
	pair<int,int> pos=calc_pos(x,y);
	int xx=pos.first,yy=pos.second;
	int r=2;
	for(;is_run();delay_fps(60)){
		put_map();
		setcolor(BLACK);
	    setfillcolor(BLUE);
		circle(xx,yy,r);
		floodfill(xx,yy,BLACK);
		r++;
		if(r==25)break;
	}
}

void put_obstacle(int x1,int y1){//�ϰ����� 
	obstacle_appear(x1,y1);
	map[x1][y1]=obstacle;
	put_map();
	//Sleep(100);
	return;
}

void make_move(int col,int x0,int y0,int x1,int y1,int x2,int y2){//���������� 
	Sleep(500);
	move_piece(x0,y0,x1,y1,col);
	Sleep(500);
	put_obstacle(x2,y2);
	boti.close();
	system("type preboti.txt>prepreboti.txt");
	system("type boti.txt>preboti.txt");
	boti.open("boti.txt",ios::app);
	boti<<y0-1<<' '<<x0-1<<' '<<y1-1<<' '<<x1-1<<' '<<y2-1<<' '<<x2-1<<endl; 
	return;
}

bool check_lose(int col){
	for(int i=1; i<=8; i++)
	for(int j=1; j<=8; j++){
		if(map[i][j]==col){
			if(calc_able(i,j))return 0;
		}
	}
	return 1;
}

void calc_mouse_pos(int x,int y){//���������λ���������ϵĶ�Ӧ���꣬����mousePos�� 
	for(int i=1;i<=8;i++)
	for(int j=1;j<=8;j++){
		int ci=i*pad + i*56 - 28, cj=j*pad + j*56 - 28;
		if(x>cj-25 && x<cj+25 && y>ci-25 && y<ci+25){
			mousePos[0]=i;
			mousePos[1]=j;
			cout<<i<<' '<<j<<endl;
			return;
		}
	}
	mousePos[0]=-1,mousePos[1]=-1;
	return;
}

void put_able(){
	put_map();
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		if(able[j+1][i+1]){
			setfillcolor(EGERGB(129,214,124));
			bar(i*56+(i+1)*pad, j*56+(j+1)*pad, (i+1)*56+(i+1)*pad, (j+1)*56+(j+1)*pad);
		}
	}
	return;
}

void hand_save(){
	system("type save.txt>handsave.txt");
	system("type boti.txt>botisave.txt");
	setcolor(BLACK);
	setfont(60,0,"����");
	setbkmode(OPAQUE);
	outtextxy(256,200,"��Ϸ�ѱ���");
	Sleep(1000); 	
	put_map();
}

void regret(){
	boti.close();
	system("type prepresave.txt>save.txt");
	system("type prepreboti.txt>boti.txt");
	boti.open("boti.txt",ios::app);
	save.open("save.txt");
	save>>step>>isBot>>playerCol>>botName;
	for(int i=1;i<=8;i++){
		for(int j=1;j<=8;j++)
		save>>map[i][j];
	}
	save.close();
	canRegret=0;
	put_map();
	//step-=2;
	return;
}

void player_play(int col){//����һ�غ� 
	int x,y,x0,y0,x1,y1,x2,y2;
	int subStep=0;//��¼״̬��ѡ�塢���塢���ϰ��� 
	bool onSave=0,onReturn=0,onBack=0;
	for(; is_run(); delay_fps(60)){
		if(subStep==3){//������
			boti.close();
			system("type preboti.txt>prepreboti.txt");
			system("type boti.txt>preboti.txt");
			boti.open("boti.txt",ios::app);
			boti<<y0-1<<' '<<x0-1<<' '<<y1-1<<' '<<x1-1<<' '<<y2-1<<' '<<x2-1<<endl; 
			cout<<"subStep = 3"<<endl; 
			return;
		}
		
		msg={0};
		while(mousemsg())msg=getmouse();
		
		if(msg.x>152 && msg.x<262 && msg.y>520 && msg.y<560){
			onSave=1;
			onReturn=onBack=0;
		}
		else if(msg.x>392 && msg.x<502 && msg.y>520 && msg.y<560){
			onReturn=1;
			onSave=onBack=0;
		}
		else if(msg.x>272 && msg.x<382 && msg.y>520 && msg.y<560){
			onBack=1;
			onSave=onReturn=0;
		}
		else if(msg.is_move())onSave=onReturn=onBack=0;
		
		if(onSave)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy(207,525,"�� ��");
		if(onReturn)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy(447,525,"�� ��");
		if(canRegret<2)setcolor(EGERGB(100,100,100));
		else if(onBack)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy(327,525,"�� ��");
		if(msg.is_up() && msg.is_left()){
			calc_mouse_pos(msg.x,msg.y);
			x=mousePos[0],y=mousePos[1];
			
			//rectangle(20,520,140,560);
	        //rectangle(372,520,492,560);
	        
			if(msg.x>152 && msg.x<262 && msg.y>520 && msg.y<560){
				hand_save();
			}
			else if(msg.x>392 && msg.x<502 && msg.y>520 && msg.y<560){
				reset=1;
				return;
			}
			else if(canRegret>=2 && msg.x>272 && msg.x<382 && msg.y>520 && msg.y<560){
				regret();
			}
			if(x>=1 && x<=8 && y>=1 && y<=8)//����������� 
			switch(subStep){
				case 0:{//ѡ�� 
					x0=mousePos[0];
					y0=mousePos[1];
					cout<<"map[x0][y0] = "<<map[x0][y0]<<"  col = "<<col<<endl;
					if(map[x0][y0] == col){
						calc_able(x0,y0);
						put_able();
						subStep = 1;
					}
					break;
				}
				case 1:{//���� 
					cout<<"subStep = 1"<<endl;
					x1=mousePos[0];
					y1=mousePos[1];
					if(able[x1][y1]){
						move_piece(x0,y0,x1,y1,col);
						calc_able(x1,y1);
						put_map();
						//Sleep(100);
						put_able();
						subStep = 2;
					}
					else if(map[x1][y1]==col){
						x0=x1,y0=y1;
						calc_able(x1,y1);
						put_able();
					}
					break;
				}
				case 2:{//���ϰ� 
					cout<<"subStep = 2"<<endl;
					x2=mousePos[0];
					y2=mousePos[1];
					if(able[x2][y2]){
						put_obstacle(x2,y2);
						subStep = 3;
					}
					break;
				}
			}
		}
	}
}
void bot_play(int col){
	char cBotName[20];
	for(int i=0;i<=botName.length();i++)cBotName[i]=botName[i];
	system(cBotName);
	Sleep(600);
	
	boto.open("boto.txt");
	int x0,y0,x1,y1,x2,y2;
	boto>>y0>>x0>>y1>>x1>>y2>>x2;
	make_move(col,x0+1,y0+1,x1+1,y1+1,x2+1,y2+1);
	cout<<col<<' '<<x0+1<<' '<<y0+1<<' '<<map[x0+1][y0+1]<<endl;
	boto.close();
}

void init_img(){
	title=newimage();
	titlePic=newimage();
	getimage(title,"title.jpg",0,0);
	getimage(titlePic,"title.jpg",0,0);
	putimage(0,0,title);
	
	setcolor(BLACK);
	setfont(50,0,"����");
	setbkmode(TRANSPARENT);
	settextjustify(CENTER_TEXT,TOP_TEXT);
	outtextxy (250,70,"����ѷ��");
	
	setfont(30,0,"����");
	setbkmode(OPAQUE);
	setfontbkcolor(EGERGB(0xA0,0xC0,0xA0));
	outtextxy (250,300,"��ʼ��Ϸ"); 
	outtextxy (250,360,"��ȡ�浵");
	outtextxy (250,420,"��Ϸ����");
	
	getimage(title,0,0,512,580);
	
	help1=newimage();
	getimage(help1,"help1.jpg",0,0);
	
	mapImg=newimage();
	getimage(mapImg,0,0,512,580);
	cleardevice(mapImg);
	setbkcolor(EGERGB(200,200,200),mapImg);
	setcolor(BLACK,mapImg);
	setfillcolor(WHITE,mapImg);
	for(int i=0;i<8;i++)
	for(int j=0;j<8;j++){
		bar(i*56+(i+1)*pad, j*56+(j+1)*pad, (i+1)*56+(i+1)*pad, (j+1)*56+(j+1)*pad, mapImg);
	}
	
	/*for(int i=0;i<3;i+=2){
		posImg[i]=newimage();
		getimage(posImg[i],0,0,56,56);
		cleardevice(posImg[i]);
		setbkcolor(OBSTACLE,posImg[i]);
		setcolor(BLACK,posImg[i]);
		circle(28,28,25,posImg[i]);
	}
	setfillcolor(WHITE,posImg[0]);
	floodfill(28,28,BLACK,posImg[0]);
	setfillcolor(BLACK,posImg[2]);
	floodfill(28,28,BLACK,posImg[2]);*/
	//getch();
	//cout<<100000;
}

void show_help(){
	putimage(0,0,help1);
	setbkmode(OPAQUE);
	bool onReturn=0;
	for(; is_run(); delay_fps(60)){
		msg={0};
		while(mousemsg()){
			msg=getmouse();
		}
		if(msg.x>350 && msg.x<450 && msg.y>515 && msg.y<545)onReturn=1;
		else if(msg.is_move())onReturn=0;
		if(onReturn)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy(400,515,"���ز˵�");
		if(msg.is_up() && msg.is_left()){
			if(msg.x>350 && msg.x<450 && msg.y>515 && msg.y<545){
				putimage(0,0,title);
				break;
			}
		}
	}
}

void put_pos(int i,int j,int col){
	setcolor(BLACK);
	if(col==white)setfillcolor(WHITE);
	else if(col==black)setfillcolor(BLACK);
	else if(col==obstacle)setfillcolor(BLUE);
	int y=i*pad + i*56 - 28, x=j*pad + j*56 - 28;
	circle(x, y, 25);
	floodfill(x, y, BLACK);
}

void put_map(){
	cleardevice();
	putimage(0,0,mapImg);
	
	for(int i=1; i<=8; i++)
	for(int j=1; j<=8; j++){
		if(map[i][j]!=0){
			//putimage((i+1)*pad + i*56, (j+1)*pad + j*56, posImg[map[i][j]]);
			put_pos(i,j,map[i][j]);
		}
	}
	
	setcolor(BLACK);
	setfillcolor(GREEN);
	//bar(20,520,140,560);
	//bar(372,520,492,560);
	bar(152,520,262,560);
	bar(272,520,382,560);
	bar(392,520,502,560);
	//rectangle(20,520,140,560);
	//rectangle(372,520,492,560);
	rectangle(152,520,262,560);
	rectangle(272,520,382,560);
	rectangle(392,520,502,560);
	
	setfont(30,0,"����");
	setbkmode(TRANSPARENT);
	//outtextxy(80,525,"�� ��");
	//outtextxy(432,525,"�� ��");
	outtextxy(207,525,"�� ��");
	if(canRegret<2)setcolor(EGERGB(100,100,100));
	outtextxy(327,525,"�� ��");
	setcolor(BLACK);
	outtextxy(447,525,"�� ��");
	if(step%2){
		//outtextxy(256,525,"�ڷ��غ�");
		outtextxy(80,525,"�ڷ��غ�"); 
	}
	else{
		setcolor(WHITE);
		//outtextxy(256,525,"�׷��غ�");
		outtextxy(80,525,"�׷��غ�");
	}
}

void clear_files(){
	system("type clear.txt>save.txt");
	system("type clear.txt>boti.txt");
}

int main(){
	HWND hwnd;
	hwnd=FindWindow("ConsoleWindowClass",NULL);	//���������ڵ������ʹ�������ƥ��ָ�����ַ���,�������Ӵ��ڡ�	
	if(hwnd){
		ShowWindow(hwnd,SW_HIDE);				//����ָ�����ڵ���ʾ״̬	
	}
	initgraph(512,580);
	init_img();
	
	start:putimage(0,0,title);
	
	init_map();
	//cout<<check_queen(3,5,3,2);
	//put_map();
	bool onNew=0,onSave=0,onHelp=0;
	for(; is_run();delay_fps(60)){//��ʼ�˵� 
		msg={0};
		while(mousemsg()){
			msg=getmouse();
		}
		if(msg.x>190 && msg.x<310 && msg.y>300 && msg.y<330){
			onNew=1;
			onSave=onHelp=0;
		}
		else if(msg.x>190 && msg.x<310 && msg.y>360 && msg.y<390){
			onSave=1;
			onNew=onHelp=0;
		}
		else if(msg.x>190 && msg.x<310 && msg.y>420 && msg.y<450){
			onHelp=1;
			onNew=onSave=0;
		}
		else if(msg.is_move()){
			onNew=onSave=onHelp=0;
		}
		setbkmode(OPAQUE);
		if(onNew)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy (250,300,"��ʼ��Ϸ"); 
		if(onSave)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy (250,360,"��ȡ�浵");
		if(onHelp)setcolor(YELLOW);
		else setcolor(BLACK);
		outtextxy (250,420,"��Ϸ����");
		if(msg.is_up() && msg.is_left()){
			if(msg.x>190 && msg.x<310 && msg.y>300 && msg.y<330){//�µĿ�ʼ 
				clear_files();
				col=black;
				step=1;
				boti.open("boti.txt");
				
				putimage(0,0,titlePic);//����/�˻� 
				setbkmode(OPAQUE);
				setcolor(BLACK);
				outtextxy (250,160,"���˶�ս");
				outtextxy (250,260,"�˻���");
				outtextxy (250,290,"�˻���ͨ");
				outtextxy (250,320,"�˻�����");
				bool onHuman=0,onSimple=0,onNormal=0,onHard=0;
				for(; is_run(); delay_fps(60)){
					msg={0};
					while(mousemsg()){
						msg=getmouse();
					}
					if(msg.x>190 && msg.x<310 && msg.y>160 && msg.y<190){
						onHuman=1;
						onSimple=onNormal=onHard=0;
					}
					else if(msg.x>190 && msg.x<310 && msg.y>260 && msg.y<290){
						onSimple=1;
						onHuman=onNormal=onHard=0;
					}
					else if(msg.x>190 && msg.x<310 && msg.y>290 && msg.y<320){
						onNormal=1;
						onHuman=onSimple=onHard=0;
					}
					else if(msg.x>190 && msg.x<310 && msg.y>320 && msg.y<350){
						onHard=1;
						onHuman=onSimple=onNormal=0;
					}
					else{
						if(msg.is_move())
						onHuman=onSimple=onNormal=onHard=0;
					}
					putimage(0,0,titlePic);//����/�˻� 
					setbkmode(OPAQUE);
					setcolor(BLACK);
					if(onHuman)setcolor(YELLOW);//ʵ�������ͣ�����ɫ 
					else setcolor(BLACK);
					outtextxy (250,160,"���˶�ս");
					if(onSimple)setcolor(YELLOW);
					else setcolor(BLACK);
					outtextxy (250,260,"�˻���");
					if(onNormal)setcolor(YELLOW);
					else setcolor(BLACK);
					outtextxy (250,290,"�˻���ͨ");
					if(onHard)setcolor(YELLOW);
					else setcolor(BLACK);
					outtextxy (250,320,"�˻�����");
					if(msg.is_up() && msg.is_left()){
						if(msg.x>190 && msg.x<310 && msg.y>160 && msg.y<190){
							isBot=0;
							break;
						}
						else if(msg.x>190 && msg.x<310 && msg.y>260 && msg.y<350){
							isBot=1;
							if(msg.y<290)botName="rand.exe";
							else if(msg.y<320)botName="Aqua.exe";
							else botName="minmax.exe"; 
							
							
							putimage(0,0,titlePic);//�Ⱥ��� 
							outtextxy(250,260,"���ֺ���");
							outtextxy(250,360,"���ְ���");
							//Sleep(100);
							bool onBlack=0,onWhite=0;
							for(; is_run(); delay_fps(60)){
								msg={0};
								while(mousemsg()){
									msg=getmouse();
								}
								if(msg.x>190 && msg.x<310 && msg.y>260 && msg.y<290){
									onBlack=1;
									onWhite=0;
								}
								else if(msg.x>190 && msg.x<310 && msg.y>360 && msg.y<390){
									onWhite=1;
									onBlack=0;
								}
								else if(msg.is_move()){
									onWhite=onBlack=0;
								}
								putimage(0,0,titlePic);//�Ⱥ��� 
								if(onBlack){
									setcolor(YELLOW);
								}
								else{
									setcolor(BLACK);
								}
								outtextxy(250,260,"���ֺ���");
								if(onWhite){
									setcolor(YELLOW);
								}
								else{
									setcolor(BLACK);
								}
								outtextxy(250,360,"���ְ���");
								if(msg.is_down() && msg.is_left()){
									if(msg.x>190 && msg.x<310 && msg.y>260 && msg.y<290){
										playerCol=black;
										break;
									}
									else if(msg.x>190 && msg.x<310 && msg.y>360 && msg.y<390){
										playerCol=white;
										break;
									}
								}
							}
							if(playerCol==white){
								for(int i=1;i<=6;i++)boti<<"-1"<<' ';
								boti<<endl;
							}
							break;
						}
					}
				}
				//getch();
				break;
			}
			else if(msg.x>190 && msg.x<310 && msg.y>360 && msg.y<390){//���� 
				if(get_save()){
					boti.open("boti.txt",ios::app);
					break;
				}
				else outtextxy (250,360,"û�д浵");
			}
			else if(msg.x>190 && msg.x<310 && msg.y>420 && msg.y<450){//���� 
				onHelp=0;
				show_help();
			}
		}
	}
	
	canRegret=0;
	put_map();
	Sleep(10);
	for(step;step<1000;step++){
		put_map();
		save_map(step);
		canRegret++;
		if(check_lose(col)){
			setfont(60,0,"����");
			setcolor(BLACK);
			setbkmode(OPAQUE);
			if(col==black){
				outtextxy(256,200,"����ʤ��"); 	
			}
			else{
				outtextxy(256,200,"����ʤ��"); 
			}
			Sleep(2000);
			break;
		}
		//if(col==black)cout<<"�ڷ����ӣ�"<<endl;
		//else cout<<"�׷����ӣ�"<<endl; 
		if(isBot==true && col!=playerCol) bot_play(col);
		else player_play(col);
		
		if(reset){
			reset=0;
			boti.close();
			save.close();
			goto start;
		}
		
		put_map();
		Sleep(10);
		col*=-1;
	}
	
	setfont(30,0,"����");
	outtextxy(256,300,"�������λ�÷������˵�");
	for(; is_run(); delay_fps(60)){
		msg={0};
		while(mousemsg())msg=getmouse();
		if(msg.is_down() && msg.is_left())
		boti.close();
		save.open("save.txt");//���save 
		save<<0;
		save.close();
		goto start;
	}
	
	boti.close();
	save.open("save.txt");//���save 
	save<<0;
	save.close();
	
	delimage(title);
	delimage(help1);
	delimage(titlePic);
	closegraph();
	return 0;
}
