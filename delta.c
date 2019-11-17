#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"linux_kbhit.h"
#include"getch.h"
#define COLOR_DEF 12
#define COLOR_GRN 2
//c에는 bool이 없다. 따라서 이렇게 열거형으로 만들어줘서 사용해야 함.
typedef enum Boolean{
	false=0,
	true = 1
}Bool;
//Data Store들을 정의하기
typedef struct Mode{
	Bool alarm_buzzing;
	int category_alpha; //대분류
	int category_beta; //소분류
	Bool stopwatch_indicator; //indicator 켜졌는지,
	Bool alarm_indicator; //indicator 꺼졌는지,
}mode;
typedef struct Time{
	int YY; //년도
	int MT; //월
	int DD; //일
	int HH; //시
	int MM; //분
	int SS; //초
	int MS; //ms
}Time;
typedef struct Alm{ //시작 시간
	Time alarmTime;
}alm;
typedef struct StopWatch{
	//LapTime은 StartTime을 기반으로 업데이트 된다.
	Time stopwatchTime;
	Time startTime;
	Time lapTime;

}stopwatch;
//Data Store들 선언하기
alm AL; //알람
stopwatch ST; //스톱워치
mode MD; //모드
Time CT; //현재 시간
int Backlight; //글자색
//0 : Alarm Buzzing, 1 : 대분류, 2: 소분류 , 3 : Stopwatch_Indicator, 4 : Alarm indicator
Bool buttonA_interface(char input){
	if(input=='a' || input =='A'){
		//printf("Button A Selected \n");
		return true;
	}
	return false;
}
Bool buttonB_interface(char input){
	if(input=='b' || input=='B'){
		//printf("Button B Selected \n");
		return true;
	}
	return false;
}
Bool buttonC_interface(char input){
	if(input=='c' || input=='C'){
		//printf("Button C Selected \n");
		return true;
	}
	return false;
}
Bool buttonD_interface(char input){
	if(input=='d' || input=='D'){
		//printf("Button D Selected \n");
		return true;
	}
	return false;
}
void Alarm(){ //소리 울리기
	printf("\a");
}
void init(){ //초기화. 프로그램 첫 실행시에 호출됨.
	//시간 초기화
	CT.YY = 2019, CT.MT=1, CT.DD=1, CT.HH=0, CT.MM=0, CT.SS = 0, CT.MS=0;
	//알람 초기화
	AL.alarmTime.YY = 2019, AL.alarmTime.MT = 1, AL.alarmTime.DD = 1, AL.alarmTime.HH = 0, AL.alarmTime.MM = 0, AL.alarmTime.SS = 0, AL.alarmTime.MS = 0;
	//스톱워치
	ST.stopwatchTime.YY = 2019, ST.stopwatchTime.MT = 1, ST.stopwatchTime.DD = 1, ST.stopwatchTime.HH = 0, ST.stopwatchTime.MM, ST.stopwatchTime.SS = 0, ST.stopwatchTime.MS = 0;
	ST.startTime.YY = 2019, ST.startTime.MT = 1, ST.startTime.DD = 1, ST.startTime.HH = 0, ST.startTime.MM = 0, ST.startTime.SS = 0, ST.startTime.MS = 0;
	ST.lapTime.YY = 2019, ST.lapTime.MT = 1, ST.lapTime.DD = 1, ST.lapTime.HH = 0, ST.lapTime.MM = 0, ST.lapTime.SS = 0, ST.lapTime.MS = 0;
	//모드 초기화
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 1, MD.category_beta =1;
	Backlight = Backlight_Controller(COLOR_GRN);
}
int Button_Selector() {
	//sleep(1);
	int Selected_Button = 0;
	Bool isA = false, isB = false, isC = false, isD = false;
	int value=-1;
	while (value = linux_kbhit()) { //no button인 경우 이곳을 거치지 않는다.
		isA = false; isB = false; isC = false; isD = false;
		char temp = getch();
		isD = buttonD_interface(temp);
		if(isD){
			Selected_Button = 4;
			break;
		}
		isC = buttonC_interface(temp);
		if(isC){
			Selected_Button = 3;
			break;
		}
		isB = buttonB_interface(temp);
		if(isB){
			Selected_Button = 2;
			break;
		}
		isA = buttonA_interface(temp);
		if(isA){
			Selected_Button  =1;
			break;
		}
		else{
			break;
		}
		if (isD) Selected_Button = 4;
		else if (isC) Selected_Button = 3;
		else if (isB) Selected_Button = 2;
		else if (isA) Selected_Button = 1;
	}
	//Selected Button : 0 = No button, 1 = A, 2 = B, 3 = C, 4 = D
	return Selected_Button;
}
void Mode_Changer(mode Mode_to_Change){ //MD를 수정할 수 있는 함수
	MD = Mode_to_Change; //값 복사
}
void Button_Operator(int Selected_Button) {
	Bool alarm_buzzing = MD.alarm_buzzing;
	int category_alpha = MD.category_alpha;
	int category_beta = MD.category_beta;
	Bool stopwatch_indicator = MD.stopwatch_indicator;
	Bool alarm_indicator = MD.alarm_indicator;

	// 모드의 대분류(category_alpha)-소분류(category_beta)-Selected_Button 순서로 작성
	if (alarm_buzzing) { //알람 울리기가 최우선
		if (Selected_Button == 0) {
			// 버튼이 눌리지 않으면 5초 동안 가만히 있다가 스스로 종료
		}
		else { // 알람을 끔
			MD.alarm_buzzing = false; // 알람 안 울림 상태로 바꿔주고
			AL.alarmTime.YY = 2019;
			AL.alarmTime.MT = 1;
			AL.alarmTime.DD = 1;
			AL.alarmTime.HH = 0;
			AL.alarmTime.MM = 0;
			AL.alarmTime.SS = 0;
			AL.alarmTime.MS = 0; // 알람 설정 시각을 초기화
		}
	}
	else {
		if (MD.category_alpha == 1) {
			switch (MD.category_beta) {
			case 1: // 1.1 timekeeping
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 2;
					break;
				case 2: // B
					break;
				case 3: // C
					MD.category_alpha = 2;
					MD.category_beta = 1;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 2: // 1.2 timekeeping_change_sec
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.SS == 59) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.SS = 0;
					}
					else {
						CT.SS++; // 현재 시각 초 1 증가
					}
					// stopwatch 시작 초도 같이 늘려주기
					if(ST.startTime.SS == 59) {
						ST.startTime.SS = 0;
						ST.startTime.MM++;
					}
					else {
						ST.startTime.SS++;
					}
					break;
				case 3: // C
					MD.category_beta = 3;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 3: // 1.3 timekeeping_change_hr
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.HH == 23) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.HH = 0;
					}
					else CT.HH++; // 현재 시각 시간 1 증가
					// stopwatch 시작 시간도 같이 늘려주기
					if(ST.startTime.HH == 23) {
						ST.startTime.HH = 0;
						ST.startTime.DD++;
					}
					else {
						ST.startTime.HH++;
					}
					break;
				case 3: // C
					MD.category_beta = 4;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 4: // 1.4 timekeeping_change_min
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.MM == 59) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.MM = 0;
					}
					else CT.MM++; // 현재 시각 분 1 증가
					// stopwatch 시작 분도 같이 늘려주기
					if(ST.startTime.MM == 59) {
						ST.startTime.MM = 0;
						ST.startTime.HH++;
					}
					else {
						ST.startTime.MM++;
					}
					break;
				case 3: // C
					MD.category_beta = 5;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 5: // 1.5 timekeeping_change_yr
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.YY == 99) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.YY = 19;
					}
					else CT.YY++; // 현재 시각 년 1 증가
					// stopwatch 시작 년도 같이 늘려주기
					if(ST.startTime.YY == 99) {
						ST.startTime.YY = 19;
					}
					else {
						ST.startTime.YY++;
					}
					break;
				case 3: // C
					MD.category_beta = 6;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 6: // 1.6 timekeeping_change_mnth
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					if (CT.MT == 12) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						CT.MT = 1;
					}
					else CT.MT++; // 현재 시각 달 1 증가
					// stopwatch 시작 월도 같이 늘려주기
					if(ST.startTime.MT == 12) {
						ST.startTime.MT = 0;
						ST.startTime.YY++;
					}
					else {
						ST.startTime.MT++;
					}
					break;
				case 3: // C
					MD.category_beta = 7;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 7: // 1.7 timekeeping_change_day
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 1;
					break;
				case 2: // B
					// 최대치가 된 상태에서 다시 입력하면 최저값으로
					switch (CT.MT) {
						// 한 달에 31일이 있는 경우
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if (CT.DD == 31) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
							CT.DD = 1;
						}
						else CT.DD++;
						// stopwatch 시작 일도 같이 늘려주기
						if(ST.startTime.DD == 31) {
							ST.startTime.DD = 1;
							ST.startTime.MT++;
						}
						else ST.startTime.DD++;
						break;
						// 한 달에 28일이 있는 경우(윤년은 제외)
					case 2:
						if (CT.DD == 28) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
							CT.DD = 1;
						}
						else CT.DD++;
						// stopwatch 시작 일도 같이 늘려주기
						if(ST.startTime.DD == 28) {
							ST.startTime.DD = 1;
							ST.startTime.MT++;
						}
						else ST.startTime.DD++;
						break;
						// 한 달에 30일이 있는 경우
					case 4:
					case 6:
					case 9:
					case 11:
						if (CT.DD == 30) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
							CT.DD = 1;
						}
						else CT.DD++;
						// stopwatch 시작 일도 같이 늘려주기
						if(ST.startTime.DD == 30) {
							ST.startTime.DD = 1;
							ST.startTime.MT++;
						}
						else ST.startTime.DD++;
						break;
					}
					break;
				case 3: // C
					MD.category_beta = 2;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			default: break;
			}
		}
		else if (MD.category_alpha == 2) {
			switch (MD.category_beta) {
			case 1: // 2.1 stopwatch
				switch (Selected_Button) {
				case 1: // A
					if (stopwatch_indicator == 0) {
						// ST.stopwatchTime = 0을 해줌
						ST.stopwatchTime.YY = 0;
						ST.stopwatchTime.MT = 0;
						ST.stopwatchTime.DD = 0;
						ST.stopwatchTime.HH = 0;
						ST.stopwatchTime.MM = 0;
						ST.stopwatchTime.SS = 0;
						ST.stopwatchTime.MS = 0;
						// ST.startTime = 0을 해줌
						ST.startTime.YY = 0;
						ST.startTime.MT = 0;
						ST.startTime.DD = 0;
						ST.startTime.HH = 0;
						ST.startTime.MM = 0;
						ST.startTime.SS = 0;
						ST.startTime.MS = 0;
					}
					else if (stopwatch_indicator == 1) {
						ST.lapTime = ST.stopwatchTime;
						MD.category_alpha = 2;
						MD.category_beta = 2;
					}
					break;
				case 2: // B
					if (stopwatch_indicator == 0) {
						MD.stopwatch_indicator = 1;
						// ST.stopwatchTime += ST.startTime을 해줌
						ST.stopwatchTime.YY += ST.startTime.YY;
						ST.stopwatchTime.MT += ST.startTime.MT;
						ST.stopwatchTime.DD += ST.startTime.DD;
						ST.stopwatchTime.HH += ST.startTime.HH;
						ST.stopwatchTime.MM += ST.startTime.MM;
						ST.stopwatchTime.SS += ST.startTime.SS;
						ST.stopwatchTime.MS += ST.startTime.MS;
						ST.stopwatchTime = CT;
					}
					else if (stopwatch_indicator == 1) {
						MD.stopwatch_indicator = 0;
						// ST.lapTime += CT - ST.startTime;
						ST.lapTime.YY += (CT.YY - ST.startTime.YY);
						ST.lapTime.MT += (CT.MT - ST.startTime.MT);
						ST.lapTime.DD += (CT.DD - ST.startTime.DD);
						ST.lapTime.HH += (CT.HH - ST.startTime.HH);
						ST.lapTime.MM += (CT.MM - ST.startTime.MM);
						ST.lapTime.SS += (CT.SS - ST.startTime.SS);
						ST.lapTime.MS += (CT.MS - ST.startTime.MS);
					}
					break;
				case 3: // C
					MD.category_alpha = 3;
					MD.category_beta = 1;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 2: // 2.2 stopwatch_LAP
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					MD.category_beta = 1;
					break;
				case 3: // C
					MD.category_alpha = 3;
					MD.category_beta = 1;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			default: break;
			}
		}
		else { // MD.category_alpha == 3
			switch (MD.category_beta) {
			case 1: // 3.1 alarm
				switch (Selected_Button) {
				case 1: // A
					MD.category_beta = 2;
					break;
				case 2: // B
					break;
				case 3: // C
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 2: // 3.2 alarm_change_hr
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					if (AL.alarmTime.HH == 23) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						AL.alarmTime.HH = 0;
					}
					else AL.alarmTime.HH++; // 알람 시작 시간 1 증가
					break;
				case 3: // C
					MD.category_beta = 3;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			case 3: // alarm_change_min
				switch (Selected_Button) {
				case 1: // A
					break;
				case 2: // B
					if (AL.alarmTime.MM = 59) { // 최대치가 된 상태에서 다시 입력하면 최저값으로
						AL.alarmTime.MM = 0;
					}
					AL.alarmTime.MM++; // 알람 시작 분 1 증가
					break;
				case 3: // C
					MD.category_beta = 2;
					break;
				case 4: // D
					Backlight = Backlight_Controller(Backlight);
					break;
				default: break;
				}
				break;
			}
		}
	}
}
void Realtime_Manager() {
	// 1. CT를 동기화 (int 연산들을 시간 범위 내로 맞춰줌)
	// ST.stopwatchTime 시간 범위 내로 맞추기
	if(ST.stopwatchTime.MS >= 1000) {
		ST.stopwatchTime.MS -= 1000;
		ST.stopwatchTime.SS++;
	}
	if(ST.stopwatchTime.SS >= 60) {
		ST.stopwatchTime.SS -= 60;
		ST.stopwatchTime.MM++;
	}
	if(ST.stopwatchTime.MM >= 60) {
		ST.stopwatchTime.MM-= 60;
		ST.stopwatchTime.HH++;
	}
	if(ST.stopwatchTime.HH >= 24) {
		ST.stopwatchTime.HH -= 24;
		ST.stopwatchTime.DD++;
	}
	switch(ST.stopwatchTime.MT) { // 달 별로 일 범위 안에 들어오는지를 확인
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if(ST.stopwatchTime.DD > 31) {
			ST.stopwatchTime.DD -= 31;
			ST.stopwatchTime.MT++;
		}
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if(ST.stopwatchTime.DD > 30) {
			ST.stopwatchTime.DD -= 30;
			ST.stopwatchTime.MT++;
		}
		break;
	case 2: // 윤년은 고려하지 않음
		if(ST.stopwatchTime.DD > 28) {
			ST.stopwatchTime.DD -= 28;
			ST.stopwatchTime.MT++;
		}
		break;
	default: break;
	}
	if(ST.stopwatchTime.MT > 12) {
		ST.stopwatchTime.MT -=  12;
		ST.stopwatchTime.YY++;
	}

	// ST.lapTime을 시간 범위 내로 맞추기
	if(ST.lapTime.MS >= 1000) {
			ST.lapTime.MS -= 1000;
		ST.lapTime.SS++;
	}
	if(ST.lapTime.SS >= 60) {
		ST.lapTime.SS -= 60;
		ST.lapTime.MM++;
	}
	if(ST.lapTime.MM >= 60) {
		ST.lapTime.MM-= 60;
		ST.lapTime.HH++;
	}
	if(ST.lapTime.HH >= 24) {
		ST.lapTime.HH -= 24;
		ST.lapTime.DD++;
	}
	switch(ST.lapTime.MT) { // 달 별로 일 범위 안에 들어오는지를 확인
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		if(ST.lapTime.DD > 31) {
			ST.lapTime.DD -= 31;
			ST.lapTime.MT++;
		}
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		if(ST.lapTime.DD > 30) {
			ST.lapTime.DD -= 30;
			ST.lapTime.MT++;
		}
		break;
	case 2: // 윤년은 고려하지 않음
		if(ST.lapTime.DD > 28) {
			ST.lapTime.DD -= 28;
			ST.lapTime.MT++;
		}
		break;
	default: break;
	}
	if(ST.lapTime.MT > 12) {
		ST.lapTime.MT -= 12;
		ST.lapTime.YY++;
	}
	if(ST.lapTime.MS < 0) {
		ST.stopwatchTime.MS += 1000;
		ST.stopwatchTime.YY++;
	}
	if(ST.lapTime.SS < 0) {
		ST.lapTime.SS += 60;
		ST.lapTime.MM--;
	}
	if(ST.lapTime.MM < 0) {
		ST.lapTime.MM += 60;
		ST.lapTime.HH--;
	}
	if(ST.lapTime.HH < 0) {
		ST.lapTime.HH += 24;
		ST.lapTime.DD--;
	}
	if(ST.lapTime.DD < 0) {
		switch(ST.lapTime.MT) { // 달 별로 일 범위 안에 들어오는지를 확인
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			ST.lapTime.DD += 31;
			ST.lapTime.MT--;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			ST.lapTime.DD += 30;
			ST.lapTime.MT--;
			break;
		case 2: // 윤년은 고려하지 않음
			ST.lapTime.DD += 28;
			ST.lapTime.MT--;
			break;
		default: break;
		}
	}
	if(ST.lapTime.MT < 0) {
		ST.lapTime.MT += 12;
		ST.lapTime.YY--;
	}

	// 2. 자동으로 알람 끄기
	if (MD.alarm_buzzing && CT.SS >= 5) // 알람이 울리는 중일 때 자동으로 끄는 코드
		MD.alarm_buzzing = false; // 알람의 소리는 1. alarm_indicator == true일때, 2. alarm_Time == Current_Time일 때 켜진다.
	// 알람이 켜지는 건 매 정각 분(xx분 00초)이고, 알람은 5초만 켜지므로, 현재 시간의 초가 5보다 클 때 알람을 자동으로 끄면 된다.

	//알람 buzzing모드 켜기
	if (MD.alarm_indicator) {
		if (CT.YY == AL.alarmTime.YY && CT.MT == AL.alarmTime.MT && CT.DD == AL.alarmTime.DD && CT.HH == AL.alarmTime.HH && CT.MM == AL.alarmTime.MM) { // 현재 시간과 알람 시간이 같은지 비교하는 과정
			if (CT.SS < 5) MD.alarm_buzzing = true; // 현재 시간의 초가 5초 미만이면 알람 끄는것과 같은 논리로 알람이 울리고 있을 수밖에 없다.
		}
	}



}
void show(int alpha_cat, char list[7][3], int blink_location) {
	//alpha_cat는 대분류를 뜻합니다. 대분류에 따라 표시될 화면의 구성이 조금씩 다릅니다.
	//list는 7개의 위치에 해당하는 문자열입니다. 모두 두 글자로 이루어져 있습니다.
	//blink_location은 깜빡일 화면의 위치를 뜻합니다.(0에서 7의 값을 가집니다)
	//blink_location이 0일 경우는 모든 화면을 표시합니다. Panel_and_Speaker_Controller에서
	//시간의 경과를 판별한 뒤, %연산(을 이용할 계획)을 이용하여 깜빡임을 구현합니다.(0과 위치를 적절히 전달)
	//결론은, blink_location이 0이면 모든 위치의 화면을 표시,
	//그렇지 않으면 지정된 위치(blink_location)의 화면을 표시하지 않습니다.
	if(blink_location) {
		list[blink_location - 1][0] = '0';
		list[blink_location - 1][1] = '0';
	}

	if(alpha_cat == 1) {//Tikekeeping
		printf("        ####################\n");
		printf("     ###                    ###\n");
		printf("  ###      %3s       %s/%s      ###\n", list[0], list[1], list[2]);
		printf("##                                ##\n");
		printf("##    %s                          ##\n", list[3]);
		printf("##          %s : %s . %s          ##\n", list[4], list[5], list[6]);
		printf("  ###                          ###\n");
		printf("     ###                    ###\n");
		printf("        ####################\n");
	}
	else if(alpha_cat == 2) {//Stopwatch
		printf("        ####################\n");
		printf("     ###                    ###\n");
		printf("  ###      %s       %s:%s      ###\n", list[0], list[1], list[2]);
		printf("##                                ##\n");
		printf("##    %s                          ##\n", list[3]);
		printf("##          %s\'  %s\"  %s          ##\n", list[4], list[5], list[6]);
		printf("  ###                          ###\n");
		printf("     ###                    ###\n");
		printf("        ####################\n");
	}
	else if(alpha_cat == 3) {//Alarm
		printf("        ####################\n");
		printf("     ###                    ###\n");
		printf("  ###      %s       %s/%s      ###\n", list[0], list[1], list[2]);
		printf("##                                ##\n");
		printf("##    %s                          ##\n", list[3]);
		printf("##          %s : %s               ##\n", list[4], list[5]);
		printf("  ###                          ###\n");
		printf("     ###                    ###\n");
		printf("        ####################\n");
	}
}
void configure_set(char list[7][3], int location, char goal[3]) {
	//list엔 configure 될 값들이
	//location엔 0에서 6사이의 변경할 위치
	//goal에는 바꿀 값이 들어있다
	list[location][0] = goal[0];
	list[location][1] = goal[1];
	list[location][2] = '\0';
	return;
}

void int_to_str(int to, char temp[3]) {
	// to는 두 자리 이하의 자연수라는 것을 가정하고 사용
	if (to < 10) {
		temp[0] = '0';
		temp[1] = (char)(to + '0');
		temp[2] = '\0';
	}
	else {
		temp[0] = (char)(to / 10 + '0');
		temp[1] = (char)(to % 10 + '0');
		temp[2] = '\0';
	}
	return;
}
void Panel_and_Speaker_Controller() {
	int flag1 = MD.category_alpha;
	int flag2 = MD.category_beta;
	char list[7][3]; // configure된 값들을 저장
	int blink_location = 0;
	char temp[3] = "  "; // 임시로 쓰일 저장소
		// 깜빡일 위치를 저장
		//만약 깜빡임을 구현하기 위해 깜빡일 위치를
		//표시하여야 할 경우는 0을 저장한다


	//if문들로 각각에 맞게 configure
	//configure 된 값들은 모두 2글자의 문자열이다 + 깜빡일 위치

	//alarm_indicator의 경우 공통되었으므로 미리 만들어 두었다
	if (MD.stopwatch_indicator) temp[1] = 'A';
	configure_set(list, 3, temp);

	switch (flag1) {
	case 1: // Timekeeping 모드
		//configure_set(list, 0, ""); // CT에 요일 넣어서 완성
		int_to_str(CT.MM, temp);
		if (CT.MM < 10) temp[0] = ' ';
		configure_set(list, 1, temp);
		int_to_str(CT.DD, temp);
		configure_set(list, 2, temp);
		int_to_str(CT.HH, temp);
		configure_set(list, 4, temp);
		int_to_str(CT.MM, temp);
		configure_set(list, 5, temp);
		int_to_str(CT.SS, temp);
		configure_set(list, 6, "");
		switch (flag2) {
		case 1: defalut: break;
		case 2: blink_location = 7; break;// 초
		case 3: blink_location = 5; break;// 시간
		case 4: blink_location = 6; break;// 분
		//case 5: blink_location// 년???????????????????
		case 6: blink_location = 2; break; // 월
		case 7: blink_location = 3; break;// 일
		}
		break;
	case 2: // Stopwatch 모드
		configure_set(list, 0, "ST");
		int_to_str(CT.HH, temp);
		configure_set(list, 1, temp);
		int_to_str(CT.MM, temp);
		configure_set(list, 2, temp);
		if (flag2 == 1) {//Stopwatch
			int_to_str(ST.stopwatchTime.MM, temp);
			configure_set(list, 4, temp);
			int_to_str(ST.stopwatchTime.SS, temp);
			configure_set(list, 5, temp);
			int_to_str(ST.stopwatchTime.MS, temp);
			configure_set(list, 6, temp);
		}
		if (flag2 == 2) {//LAP
			int_to_str(ST.lapTime.MM, temp);
			configure_set(list, 4, temp);
			int_to_str(ST.lapTime.SS, temp);
			configure_set(list, 5, temp);
			int_to_str(ST.lapTime.MS, temp);
			configure_set(list, 6, temp);
		}
		break;
	case 3: // Alarm 모드
		configure_set(list, 0, "AL");
		int_to_str(CT.MM, temp);
		if (CT.MM < 10) temp[0] = ' ';
		configure_set(list, 1, temp);
		int_to_str(CT.DD, temp);
		configure_set(list, 2, temp);
		int_to_str(AL.alarmTime.HH, temp);
		configure_set(list, 4, temp);
		int_to_str(AL.alarmTime.MM, temp);
		configure_set(list, 5, temp);
		configure_set(list, 6, "  ");
		if (flag2 == 2) blink_location = 5; // 시간
		else if (flag2 == 3) blink_location = 6; // 분
		break;
	default: // 엄밀한 명세에 의하면 없어도 되는 코드
			break;
	}


	//show()를 구현하여 configure된 값들을 표시
	//1초에 한번씩 깜빡일 예정
	if (CT.SS % 2) blink_location = 0;
	show(flag1, list, blink_location);

	return;
}
int Backlight_Controller(int backlight){ //색 변경
	//backlight가 COLOR_DEF라면 -> COLOR_GRN으로 변경
	//backlight가 COLOR_GRN라면 -> COLOR_DEF로 변경
	//2가지밖에 없고 이를 번갈아 가며 사용하는 형태
	if(backlight==COLOR_DEF){
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_GRN);
		//printf("Color Change to COLOR_GREEN \n");
		return COLOR_GRN;
	}
	else{
		SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), COLOR_DEF);
		//printf("Color_Change to COLOR_RED \n");
		return COLOR_DEF;
	}
}
int main(){
	init();
	//테스트 구간. 아직 의미 X
	int Selected_Button= 0;
	while(true){
		Selected_Button = Button_Selector();
		Button_Operator(Selected_Button);
		Realtime_Manager();
		Panel_and_Speaker_Controller();
		system("clear");
	}
	return 0;
}
