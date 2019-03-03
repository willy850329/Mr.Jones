#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define sw 1024
#define sh 768

typedef struct pic{
	HDC dc;
	HBITMAP hbm;
	int height;
	int width;
}picture;

void reg_class(char *classname);
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam);

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam){
	if(message == WM_DESTROY){
		PostQuitMessage(0);
		return 0;
	}
	else if(message==WM_CLOSE){
		DestroyWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd,message,wparam,lparam);
}

void reg_class(char *classname){
	WNDCLASS wnd;
	ZeroMemory(&wnd,sizeof(WNDCLASS)); //清空wnd
	
	 wnd.style=CS_HREDRAW|CS_VREDRAW; //水平重畫|垂直重畫
	 wnd.lpfnWndProc=WndProc; 
	 wnd.hbrBackground=(HBRUSH)COLOR_ACTIVECAPTION;
	 wnd.lpszClassName=classname;
	 
	 //註冊 
	 if(RegisterClass(&wnd)==0) {
	 	MessageBox(NULL,"無法註冊window class","在註冊時",MB_ICONERROR);
		exit(0);  
	 }	
}

picture readBMP(const char *filename,HDC dc1){
	picture p;
	
	//include <io.h>
	if(access(filename,F_OK)==-1){
		char error[1024];
		sprintf(error,"檔案%s不存在!!",filename);
		MessageBox(NULL,"檔案不存在","在讀檔時",MB_ICONERROR);
		return p;
	}
	
	p.dc=CreateCompatibleDC(dc1);
	p.hbm=(HBITMAP)LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	
	BITMAP temproary;
	GetObject(p.hbm,sizeof(BITMAP),&temproary);
	if((temproary.bmWidth<=0)||(temproary.bmHeight<=0)){
		char error[1024];
		sprintf(error,"讀取圖檔%s錯誤!!",filename);
		MessageBox(NULL,error,"在讀圖檔時",MB_ICONERROR);
		return p;
	}
	p.width=temproary.bmWidth;
	p.height=temproary.bmHeight;
	
	SelectObject(p.dc,p.hbm);
	
	return p;
}

main(){
	HWND hwnd;
	char classname[64]; 
	MSG msg;
	
	strcpy(classname, "test class"); 
	reg_class(classname); 
	hwnd=CreateWindow(classname,"第12組",WS_VISIBLE|WS_SYSMENU|WS_OVERLAPPED,0,0,sw,sh,NULL,NULL,NULL,NULL);
	if(hwnd==NULL){
		MessageBox(NULL,"無法創建此視窗","創建視窗時",MB_ICONERROR);
		return 0;
	}
	
	HDC dc1=GetDC(hwnd);
	HBITMAP hbm = CreateCompatibleBitmap(dc1,sw,sh);
	HDC dc2=CreateCompatibleDC(dc1);
	SelectObject(dc2,hbm);
	//---------------------------------------------------
	
	HFONT font;	

	font=CreateFont( 60, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Showcard Gothic") );
	if( font == NULL ) {
    	char error[1024];
    	sprintf( error, "無法創建字型...");
		MessageBox( NULL, error, "字型錯誤" , MB_ICONERROR );
    }
	HFONT font2;	

	font2=CreateFont( 30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, TEXT("Showcard Gothic") );
	if( font == NULL ) {
    	char error[1024];
    	sprintf( error, "無法創建字型...");
		MessageBox( NULL, error, "字型錯誤" , MB_ICONERROR );
    }
	//---------------------------------------------------
	
	MCI_OPEN_PARMS music_open;
	MCI_PLAY_PARMS music_play;
	MCIERROR music_err;
	
	
	music_open.lpstrDeviceType="mpegvideo";
	music_open.lpstrElementName="music.mp3";
	
	music_err=mciSendCommand(0,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,(DWORD_PTR)&music_open);
	if(music_err){
		char err[1024];
		mciGetErrorString(music_err,err,sizeof(err));
		printf("%s",err);
	}
	
	UINT dID=music_open.wDeviceID;
	
	music_err=mciSendCommand(dID,MCI_PLAY,0,(DWORD_PTR)&music_play);	
	
	if(music_err){
		char err[1024];
		mciGetErrorString(music_err,err,sizeof(err));
		printf("%s",err);
	}
	
	//---------------------------------------------------
		
	picture background;
	background = readBMP("pic//background.bmp",dc1);
	picture background2;
	background2 = readBMP("pic//background2.bmp",dc1);
	picture death;
	death = readBMP("pic//death.bmp",dc1);
	picture score;
	score = readBMP("pic//score.bmp",dc1);
	picture bonus1;
	bonus1 = readBMP("pic//bonus1.bmp",dc1);
	picture bonus2;
	bonus2 = readBMP("pic//bonus2.bmp",dc1);
	picture bonus3;
	bonus3 = readBMP("pic//bonus3.bmp",dc1);
	picture bonus4;
	bonus4 = readBMP("pic//bonus4.bmp",dc1);
	picture bonus5;
	bonus5 = readBMP("pic//bonus5.bmp",dc1);
	picture guy[2]; //人物 
	guy[0]=readBMP("pic//left.bmp",dc1);
	guy[1]=readBMP("pic//right.bmp",dc1);
	int LR = 0;//LR=0 for left, LR=1 for right
	
	//
	picture arrow[4]; //上下左右四種箭頭
	arrow[0]= readBMP("pic//arrowup.bmp",dc1);//上 
	arrow[1]= readBMP("pic//arrowdown.bmp",dc1);//下 
	arrow[2]= readBMP("pic//arrowleft.bmp",dc1);//左 
	arrow[3]= readBMP("pic//arrowright.bmp",dc1);//右 
	
	picture stone;
	stone=readBMP("pic//stone.bmp",dc1);
	
	picture stoneR[4];
	stoneR[0]=readBMP("pic//stone.bmp",dc1);
	stoneR[1]=readBMP("pic//stoneright.bmp",dc1);
	stoneR[2]=readBMP("pic//stoneright2.bmp",dc1);
	stoneR[3]=readBMP("pic//stoneright3.bmp",dc1);
	
	picture stoneL[4];
	stoneL[0]=readBMP("pic//stone.bmp",dc1);
	stoneL[1]=readBMP("pic//stoneL.bmp",dc1);
	stoneL[2]=readBMP("pic//stoneL2.bmp",dc1);
	stoneL[3]=readBMP("pic//stoneL3.bmp",dc1);
	
	
	
	picture die[4];
	die[0]=readBMP("pic//dieup.bmp",dc1);
	die[1]=readBMP("pic//diedown.bmp",dc1);
	die[2]=readBMP("pic//dieleft.bmp",dc1);
	die[3]=readBMP("pic//dieright.bmp",dc1);
	
	
	
	//出石頭箭頭位置
	int bomb[10]; //最多到10顆
	int out[20];//20個地方可以出石頭
	memset(bomb,0,sizeof(bomb));
	memset(out,0,sizeof(out));
	
	//計算第幾輪攻擊,每 3 round = 1 part,共 21 round,7 parts 
	int round=1; 
	int part=1;
	
	//石頭數量
	int number=3;
	 
	//迴圈用
	int i,j,k;
	
	 
	
	//鎖鍵盤
	int lock[4]; //up=0; down=1; left=2; right=3
	memset(lock,0,sizeof(lock)); //0是未上鎖; 1是上鎖 
	double lock_time=0.2; //上鎖時間 
	LARGE_INTEGER t1g[4],t2g[4],fr;//up=0; down=1; left=2; right=3
	QueryPerformanceFrequency(&fr);
	
	int screen_lock = 0;
	LARGE_INTEGER t1scr,t2scr;
	
	
	//LARGE_INTEGER t1u,t2u;
	
	//人物位置
	int xg,yg;
	xg=513;
	yg=352; 
	
	
	
	//石頭移動 
	double rolling_time=2.5; //花多少時間滾完石頭 
	double escape_time=1; //預告到石頭出來的時間差 
	double time_step=0.1; //每幾秒讓石頭推進一次 
	int roll=(550/rolling_time)*(time_step); //每推進一次走多少pixels 
	int total=550/roll; //總共推進幾次 
	int yr_u[total]; //石頭移動位置
	int yr_d[total];
	int xr_l[total];
	int xr_r[total]; 
	
	for(i=0;i<total;i++){
		yr_u[i]=670-roll*i;
	}
	for(i=0;i<total;i++){
		yr_d[i]=33+roll*i;
	}
	for(i=0;i<total;i++){
		xr_l[i]=815-roll*i;
	}
	for(i=0;i<total;i++){
		xr_r[i]=173+roll*i;
	}
	
	int xr[20];//記錄石頭位置 
	int yr[20];
	memset(xr,0,sizeof(xr));
	memset(yr,0,sizeof(yr));
	
	int attack=0; // 1 攻擊 
	
	int dead=0; //0活 1死 
	
	//出石頭時間 
	LARGE_INTEGER t1,t2,f;
	QueryPerformanceFrequency(&f);
	QueryPerformanceCounter(&t1);
	double round_time=escape_time+rolling_time+0.001; //每波攻擊的時間差 
	
	
	//預告時間過後開始滾石頭 預告時間為escape_time 
	LARGE_INTEGER t1es,t2es;
	
	LARGE_INTEGER t1s,t2s;
	
	QueryPerformanceCounter(&t1es);
	QueryPerformanceCounter(&t1s);
	
	//第一次亂數 
	srand((unsigned)time(NULL));
	for(i=0;i<20;i++){
		out[i]=i;
	}
	for(i=0;i<20;i++){
		j=rand()%20;
		k=out[j];
		out[j]=out[i];
		out[i]=k;
	}
	for(i=0;i<number;i++){
		bomb[i]=out[i];
	}
	
	int q=0,z=0;
	
	char howmany[64];
	
	int screen = 0; //0 主畫面; 1 遊戲說明; 2 遊戲畫面; 3死掉後再來一次 
	int start = 0;
	int info = 0;
	int bonus = 0;
	int msgboxID;
	//維持視窗
	while(1){
		
		if(screen==0){ //主畫面 
			StretchBlt(dc2,0,0,1024,768,background2.dc,0,0,background2.width,background2.height,SRCCOPY);
			
			if((GetAsyncKeyState(VK_RETURN)<0)&&(screen_lock==0)){
				screen_lock=1;
				QueryPerformanceCounter(&t1scr);
				
	    		screen=2;
	    		QueryPerformanceCounter(&t1es);
				QueryPerformanceCounter(&t1s);
				QueryPerformanceCounter(&t1);
				
				//第一次亂數 
				srand((unsigned)time(NULL));
				for(i=0;i<20;i++){
					out[i]=i;
				}
				for(i=0;i<20;i++){
					j=rand()%20;
					k=out[j];
					out[j]=out[i];
					out[i]=k;
				}
				for(i=0;i<number;i++){
					bomb[i]=out[i];
				}
				
				memset(xr,0,sizeof(xr));
				memset(yr,0,sizeof(yr));
				
				
				dead=0;
				screen=2;
				round=1;
				part=1;
				number=3;
				xg=513;
				yg=352;
				attack=0;
				q=0;
				z=0;
			}
			
			else if((GetAsyncKeyState(VK_SPACE)<0)&&(screen_lock==0)){ //是否需要遊戲說明 
				screen_lock=1;
				QueryPerformanceCounter(&t1scr);
				
				screen=1;
				StretchBlt(dc2,0,0,1024,768,score.dc,0,0,score.width,score.height,SRCCOPY);
				sprintf(howmany,"test");
		        TextOut(dc2,350,100,howmany,strlen(howmany));
					
			}
		}
		
		
		else if (screen==1){ //遊戲說明畫面 
			StretchBlt(dc2,0,0,1024,768,score.dc,0,0,score.width,score.height,SRCCOPY);
			
			if((GetAsyncKeyState(VK_RETURN)<0)&&(screen_lock==0)){
				screen_lock=1;
				QueryPerformanceCounter(&t1scr);
				
	    		screen=0;
			}
		}
		
		
		
		else if(screen==2){
		
		//貼背景 
		BitBlt(dc2,0,0,background.width,background.height,background.dc,0,0,SRCCOPY); //把圖貼到dc2 
		
		SelectObject( dc2, font);
		sprintf(howmany,"第 %d 波",round);
		TextOut(dc2,75,75,howmany,strlen(howmany));
		
		
		
		// 亂數出石頭的位置 
		QueryPerformanceCounter(&t2); //舊回合結束的時間 
		if((double)(t2.QuadPart-t1.QuadPart)/f.QuadPart>=round_time){//多久出一次石頭 
			QueryPerformanceCounter(&t1);//新回合開始的時間 
			QueryPerformanceCounter(&t1es);
			QueryPerformanceCounter(&t1s);
			
			srand((unsigned)time(NULL));
			for(i=0;i<20;i++){
				out[i]=i;
			}
			for(i=0;i<20;i++){
				j=rand()%20;
				k=out[j];
				out[j]=out[i];
				out[i]=k;
			}
			for(i=0;i<number;i++){
				bomb[i]=out[i];
			}
			
			memset(xr,0,sizeof(xr));
			memset(yr,0,sizeof(yr));
			
			if( ( ( round % 3 ) == 0 ) && ( round >= 1 ) && ( round <= 54 )){
			part++;
			}			
			
			if(part==1){
				number=3;
			}
			else if(part==2){
				number=4;
			}
			else if(part==3){
				number=5;
			}
			else if(part==4){
				number=6;
			}
			else if(part==5){
				number=7;
			}
			else if(part==6){
				number=8;
			}
			else if(part==7){
				number=9;
			}
			else if(part==8){
				number=10;
			}
			else if(part==9){
				number=11;
			}
			else if(part==10){
				number=12;
			}
			else if(part==11){
				number=13;
			}
			else if(part==12){
				number=14;
			}
			else if(part==13){
				number=15;
			}
			else if(part==14){
				number=16;
			}
			else if(part==15){
				number=17;
			}
			else if(part==16){
				number=18;
			}
			else if(part==17){
				number=19;
			}
			else if(part==18){
				number=20;
			}
				
				round++;
		}
		
		
		
		//貼箭頭預告 
		for(i=0;i<number;i++){
			if( bomb[i]==0 ){
				TransparentBlt(dc2,291,670,arrow[0].width,arrow[0].height,arrow[0].dc,0,0,arrow[0].width,arrow[0].height,RGB(255,255,255));
			}
			if( bomb[i]==1 ){
				TransparentBlt(dc2,401,670,arrow[0].width,arrow[0].height,arrow[0].dc,0,0,arrow[0].width,arrow[0].height,RGB(255,255,255));
			}
			if( bomb[i]==2 ){
				TransparentBlt(dc2,511,670,arrow[0].width,arrow[0].height,arrow[0].dc,0,0,arrow[0].width,arrow[0].height,RGB(255,255,255));
			}
			if( bomb[i]==3 ){
				TransparentBlt(dc2,621,670,arrow[0].width,arrow[0].height,arrow[0].dc,0,0,arrow[0].width,arrow[0].height,RGB(255,255,255));
			}
			if( bomb[i]==4 ){
				TransparentBlt(dc2,731,670,arrow[0].width,arrow[0].height,arrow[0].dc,0,0,arrow[0].width,arrow[0].height,RGB(255,255,255));
			}
			if( bomb[i]==5 ){
				TransparentBlt(dc2,291,58,arrow[1].width,arrow[1].height,arrow[1].dc,0,0,arrow[1].width,arrow[1].height,RGB(255,255,255));
			}
			if( bomb[i]==6 ){
				TransparentBlt(dc2,401,58,arrow[1].width,arrow[1].height,arrow[1].dc,0,0,arrow[1].width,arrow[1].height,RGB(255,255,255));
			}
			if( bomb[i]==7 ){
				TransparentBlt(dc2,511,58,arrow[1].width,arrow[1].height,arrow[1].dc,0,0,arrow[1].width,arrow[1].height,RGB(255,255,255));
			}
			if( bomb[i]==8 ){
				TransparentBlt(dc2,621,58,arrow[1].width,arrow[1].height,arrow[1].dc,0,0,arrow[1].width,arrow[1].height,RGB(255,255,255));
			}
			if( bomb[i]==9 ){
				TransparentBlt(dc2,731,58,arrow[1].width,arrow[1].height,arrow[1].dc,0,0,arrow[1].width,arrow[1].height,RGB(255,255,255));
			}
			if( bomb[i]==10 ){
				TransparentBlt(dc2,815,147,arrow[2].width,arrow[2].height,arrow[2].dc,0,0,arrow[2].width,arrow[2].height,RGB(255,255,255));
			}
			if( bomb[i]==11 ){
				TransparentBlt(dc2,815,257,arrow[2].width,arrow[2].height,arrow[2].dc,0,0,arrow[2].width,arrow[2].height,RGB(255,255,255));
			}
			if( bomb[i]==12 ){
				TransparentBlt(dc2,815,367,arrow[2].width,arrow[2].height,arrow[2].dc,0,0,arrow[2].width,arrow[2].height,RGB(255,255,255));
			}
			if( bomb[i]==13 ){
				TransparentBlt(dc2,815,477,arrow[2].width,arrow[2].height,arrow[2].dc,0,0,arrow[2].width,arrow[2].height,RGB(255,255,255));
			}
			if( bomb[i]==14 ){
				TransparentBlt(dc2,815,587,arrow[2].width,arrow[2].height,arrow[2].dc,0,0,arrow[2].width,arrow[2].height,RGB(255,255,255));
			}
			if( bomb[i]==15 ){
				TransparentBlt(dc2,198,147,arrow[3].width,arrow[3].height,arrow[3].dc,0,0,arrow[3].width,arrow[3].height,RGB(255,255,255));
			}
			if( bomb[i]==16 ){
				TransparentBlt(dc2,198,257,arrow[3].width,arrow[3].height,arrow[3].dc,0,0,arrow[3].width,arrow[3].height,RGB(255,255,255));
			}
			if( bomb[i]==17 ){
				TransparentBlt(dc2,198,367,arrow[3].width,arrow[3].height,arrow[3].dc,0,0,arrow[3].width,arrow[3].height,RGB(255,255,255));
			}
			if( bomb[i]==18 ){
				TransparentBlt(dc2,198,477,arrow[3].width,arrow[3].height,arrow[3].dc,0,0,arrow[3].width,arrow[3].height,RGB(255,255,255));
			}
			if( bomb[i]==19 ){
				TransparentBlt(dc2,198,587,arrow[3].width,arrow[3].height,arrow[3].dc,0,0,arrow[3].width,arrow[3].height,RGB(255,255,255));
			}			
		}
		
		//滾石頭 
		QueryPerformanceCounter(&t2es);
		if((double)(t2es.QuadPart-t1.QuadPart)/f.QuadPart >= escape_time){
			attack=1;
		}
		
		if(attack==1){
			QueryPerformanceCounter(&t2s);
				for(i=0;i<number;i++){//找出滾出石頭的位置 
					if( bomb[i]==0 ){
						TransparentBlt(dc2,274,yr_u[q],stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[0]=274;
						yr[0]=yr_u[q];
					}
					if( bomb[i]==1 ){
						TransparentBlt(dc2,384,yr_u[q],stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[1]=384;
						yr[1]=yr_u[q];
					}
					if( bomb[i]==2 ){
						TransparentBlt(dc2,494,yr_u[q],stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[2]=494;
						yr[2]=yr_u[q];
					}
					if( bomb[i]==3 ){
						TransparentBlt(dc2,604,yr_u[q],stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[3]=604;
						yr[3]=yr_u[q];
					}
					if( bomb[i]==4 ){
						TransparentBlt(dc2,714,yr_u[q],stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[4]=714;
						yr[4]=yr_u[q];
					}
					if( bomb[i]==5 ){
						TransparentBlt(dc2,274,yr_d[q],stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[5]=274;
						yr[5]=yr_d[q];
					}
					if( bomb[i]==6 ){
						TransparentBlt(dc2,384,yr_d[q],stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[6]=384;
						yr[6]=yr_d[q];
					}
					if( bomb[i]==7 ){
						TransparentBlt(dc2,494,yr_d[q],stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[7]=494;
						yr[7]=yr_d[q];
					}
					if( bomb[i]==8 ){
						TransparentBlt(dc2,604,yr_d[q],stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[8]=604;
						yr[8]=yr_d[q];
					}
					if( bomb[i]==9 ){
						TransparentBlt(dc2,714,yr_d[q],stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[9]=714;
						yr[9]=yr_d[q];
					}
					if( bomb[i]==10 ){
						TransparentBlt(dc2,xr_l[q],134,stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[10]=xr_l[q];
						yr[10]=134;
					}
					if( bomb[i]==11 ){
						TransparentBlt(dc2,xr_l[q],244,stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[11]=xr_l[q];
						yr[11]=244;
					}
					if( bomb[i]==12 ){
						TransparentBlt(dc2,xr_l[q],354,stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[12]=xr_l[q];
						yr[12]=354;
					}
					if( bomb[i]==13 ){
						TransparentBlt(dc2,xr_l[q],464,stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[13]=xr_l[q];
						yr[13]=464;
					}
					if( bomb[i]==14 ){
						TransparentBlt(dc2,xr_l[q],574,stoneL[z].width,stoneL[z].height,stoneL[z].dc,0,0,stoneL[z].width,stoneL[z].height,RGB(255,255,255));
						xr[14]=xr_l[q];
						yr[14]=574;
					}
					if( bomb[i]==15 ){
						TransparentBlt(dc2,xr_r[q],134,stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[15]=xr_r[q];
						yr[15]=134;
					}
					if( bomb[i]==16 ){
						TransparentBlt(dc2,xr_r[q],244,stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[16]=xr_r[q];
						yr[16]=244;
					}
					if( bomb[i]==17 ){
						TransparentBlt(dc2,xr_r[q],354,stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[17]=xr_r[q];
						yr[17]=354;
					}
					if( bomb[i]==18 ){
						TransparentBlt(dc2,xr_r[q],464,stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[18]=xr_r[q];
						yr[18]=464;
					}
					if( bomb[i]==19 ){
						TransparentBlt(dc2,xr_r[q],574,stoneR[z].width,stoneR[z].height,stoneR[z].dc,0,0,stoneR[z].width,stoneR[z].height,RGB(255,255,255));
						xr[19]=xr_r[q];
						yr[19]=574;
					}
					
					if( (double)(t2s.QuadPart - t1s.QuadPart) / f.QuadPart >= time_step){
						QueryPerformanceCounter(&t1s);
						q++;
						z++;
					}
					if(q>total){
						q=0;
						attack=0;
					}
					if(z>=3){
						z=0;
					}
					
				}
			}
		
		
		//人物控制
		if((GetAsyncKeyState(VK_UP)>=0)&&(GetAsyncKeyState(VK_DOWN)>=0)&&(GetAsyncKeyState(VK_LEFT)>=0)&&(GetAsyncKeyState(VK_RIGHT)>=0)){
			TransparentBlt(dc2,xg,yg,guy[LR].width,guy[LR].height,guy[LR].dc,0,0,guy[LR].width,guy[LR].height,RGB(255,255,255));
			if(LR==0)
				LR=1;
			else if (LR==1)
				LR=0;
			
		}
		else if((GetAsyncKeyState(VK_UP)<0)&&(lock[0]==0)){
			lock[0]=1;
			QueryPerformanceCounter(&t1g[0]);
			yg=yg-110;
			if(yg<=132){
				yg=132;
			}
			TransparentBlt(dc2,xg,yg,guy[LR].width,guy[LR].height,guy[LR].dc,0,0,guy[LR].width,guy[LR].height,RGB(255,255,255));
			
		} 
		else if((GetAsyncKeyState(VK_DOWN)<0)&&(lock[1]==0)){
			lock[1]=1;
			QueryPerformanceCounter(&t1g[1]);
			yg=yg+110;
			if(yg>=572){
				yg=572;
			}
			TransparentBlt(dc2,xg,yg,guy[LR].width,guy[LR].height,guy[LR].dc,0,0,guy[LR].width,guy[LR].height,RGB(255,255,255));
		} 
		else if((GetAsyncKeyState(VK_LEFT)<0)&&(lock[2]==0)){
			lock[2]=1;
			QueryPerformanceCounter(&t1g[2]);
			xg=xg-110;
			if(xg<=293){
				xg=293;
			}
			TransparentBlt(dc2,xg,yg,guy[LR].width,guy[LR].height,guy[LR].dc,0,0,guy[LR].width,guy[LR].height,RGB(255,255,255));
		} 
		else if((GetAsyncKeyState(VK_RIGHT)<0)&&(lock[3]==0)){
			lock[3]=1;
			QueryPerformanceCounter(&t1g[3]);
			xg=xg+110;
			if(xg>=733){
				xg=733;
			}
			TransparentBlt(dc2,xg,yg,guy[LR].width,guy[LR].height,guy[LR].dc,0,0,guy[LR].width,guy[LR].height,RGB(255,255,255));
		} 
		
		
		//按鍵解鎖
		for(i=0;i<=3;i++){
			if(lock[i]==1){
				QueryPerformanceCounter(&t2g[i]);
				if((double)(t2g[i].QuadPart-t1g[i].QuadPart)/fr.QuadPart>=lock_time){
					lock[i]=0;
				}
			}
		}
		
		
		
		//die?
		
		for (i=0;i<20;i++){
			if(dead==0){
			
				if( ( xg>=xr[i]-46 ) && (xg <= xr[i]+46) ){
					if( ( yg>=yr[i]-46 ) && (yg <= yr[i]+46) ){
					
						music_err=mciSendCommand(dID,MCI_STOP,MCI_WAIT,(DWORD_PTR)&music_play);
					
						StretchBlt(dc1,0,0,1024,768,death.dc,0,0,death.width,death.height,SRCCOPY);
						TransparentBlt(dc2,xg,yg,die[2].width,die[2].height,die[2].dc,0,0,die[2].width,die[2].height,RGB(255,255,255));
						SelectObject( dc1, font);
						sprintf(howmany,"恭喜你撐過了%d波",round-1);
		        	    TextOut(dc1,350,100,howmany,strlen(howmany));
		        	    int i,j;
							int counter;
							int a[8];
							int c=0;
							a[6]=0;
							a[7]=0;
							FILE *fin;
							if((fin=fopen("rank.txt","r"))==NULL){
								printf("error");							
							}
							a[5]=round-1;
							for(i=0;i<5;i++){		//讀取成績					
							fscanf(fin,"%d",&a[i]);
						    }
						    fclose(fin);
						    for(j=0;j<10;j++){		//排大小					
						      for(i=0;i<6;i++){
						    	if(a[i]<a[i+1]){
						    		c=a[i];
						    		a[i]=a[i+1];
						    		a[i+1]=c;
								}
							}
						}
						SelectObject( dc1, font2);  //排行榜 
						sprintf(howmany,"第一名:%d波",a[0]);
		        	    TextOut(dc1,0,0,howmany,strlen(howmany));
		        	    SelectObject( dc1, font2);
						sprintf(howmany,"第二名:%d波",a[1]);
		        	    TextOut(dc1,0,30,howmany,strlen(howmany));
						SelectObject( dc1, font2);
						sprintf(howmany,"第三名:%d波",a[2]);
		        	    TextOut(dc1,0,60,howmany,strlen(howmany));
						SelectObject( dc1, font2);
						sprintf(howmany,"第四名:%d波",a[3]);
		        	    TextOut(dc1,0,90,howmany,strlen(howmany));
						SelectObject( dc1, font2);
						sprintf(howmany,"第五名:%d波",a[4]);
		        	    TextOut(dc1,0,120,howmany,strlen(howmany));											        	    
						
							if((fin=fopen("rank.txt","w"))==NULL){
								printf("error");
							
							}
							for(i=0;i<5;i++){						
							fprintf(fin,"%d\n",a[i]);
						    } 
							fclose(fin);
							
						if(round<6){
							msgboxID=MessageBox(NULL,"哈哈你輸了哈哈","LOSE!",MB_RETRYCANCEL);
					        }							
						else if(round>=6 && round<19){
							MessageBox(NULL,"哈哈你輸了哈哈","LOSE!",MB_OK);
							StretchBlt(dc1,0,0,1024,768,bonus1.dc,0,0,bonus1.width,bonus1.height,SRCCOPY);
							msgboxID=MessageBox(NULL,"我漂亮嗎?","LOSE!",MB_RETRYCANCEL);
						}
						else if(round>=19 && round<22){
							MessageBox(NULL,"哈哈你輸了哈哈","LOSE!",MB_OK);
							StretchBlt(dc1,0,0,1024,768,bonus2.dc,0,0,bonus2.width,bonus2.height,SRCCOPY);
							msgboxID=MessageBox(NULL,"我漂亮嗎?","LOSE!",MB_RETRYCANCEL);
						}
						else if(round>=22 && round<25){
							MessageBox(NULL,"哈哈你輸了哈哈","LOSE!",MB_OK);
							StretchBlt(dc1,0,0,1024,768,bonus3.dc,0,0,bonus3.width,bonus3.height,SRCCOPY);
							msgboxID=MessageBox(NULL,"我漂亮嗎?","LOSE!",MB_RETRYCANCEL);
						}
						else if(round>=25 && round<28){
							MessageBox(NULL,"哈哈你輸了哈哈","LOSE!",MB_OK);
							StretchBlt(dc1,0,0,1024,768,bonus4.dc,0,0,bonus4.width,bonus4.height,SRCCOPY);
							msgboxID=MessageBox(NULL,"我漂亮嗎?","LOSE!",MB_RETRYCANCEL);
						}
						else if(round>=28){
							MessageBox(NULL,"哈哈你輸了哈哈","LOSE!",MB_OK);
							StretchBlt(dc1,0,0,1024,768,bonus5.dc,0,0,bonus5.width,bonus5.height,SRCCOPY);
							msgboxID=MessageBox(NULL,"我漂亮嗎?","LOSE!",MB_RETRYCANCEL);
						}
						 
						
						switch(msgboxID){
						
						case IDRETRY:
							screen=3;
							dead=1;
					    	start=0;
					    	break;
					    case IDCANCEL:
					    	screen=0;
					    	break;
					    
						}
				
		 				music_err=mciSendCommand(dID,MCI_STOP,0,(DWORD_PTR)&music_play);
						music_err=mciSendCommand(dID,MCI_SEEK,MCI_WAIT|MCI_SEEK_TO_START,NULL);	
		 				music_err=mciSendCommand(dID,MCI_PLAY,0,(DWORD_PTR)&music_play);
					}
				}
			}	

		}
		
		}
		
		
		// WARNING : dead 和 state 尚須處理 ,t1 t2等時間需要重算
		
		else if(screen==3){
			
			QueryPerformanceCounter(&t1es);
			QueryPerformanceCounter(&t1s);
			QueryPerformanceCounter(&t1);
			
			//第一次亂數 
			srand((unsigned)time(NULL));
			for(i=0;i<20;i++){
				out[i]=i;
			}
			for(i=0;i<20;i++){
				j=rand()%20;
				k=out[j];
				out[j]=out[i];
				out[i]=k;
			}
			for(i=0;i<number;i++){
				bomb[i]=out[i];
			}
			
			memset(xr,0,sizeof(xr));
			memset(yr,0,sizeof(yr));
			
			dead=0;
			screen=2;
			round=1;
			part=1;
			number=3;
			xg=513;
			yg=352;
			attack=0;
			q=0;
			z=0;
			
		}
		 	
		 if(screen_lock==1){
				QueryPerformanceCounter(&t2scr);
				if((double)(t2scr.QuadPart-t1scr.QuadPart)/fr.QuadPart>=lock_time){
					screen_lock=0;
				}
			}
		
		
		
		//-----------------------------------------------------
		BitBlt(dc1,0,0,sw,sh,dc2,0,0,SRCCOPY); 
		if(WM_QUIT == msg.message){
			break;			
		}
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}	
	} 
	ReleaseDC(hwnd,dc1); 
	DeleteObject(hbm); 
	DeleteDC(dc2); 
}
