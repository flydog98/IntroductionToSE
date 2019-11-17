#include<stdio.h>
#include<windows.h>
#include<time.h>
#include"linux_kbhit.h"
#include"getch.h"
#define COLOR_DEF 4
#define COLOR_GRN 2
//c���� bool�� ����. ���� �̷��� ���������� ������༭ ����ؾ� ��.
typedef enum Boolean {
	false = 0,
	true = 1
}Bool;
//Data Store���� �����ϱ�
typedef struct Mode {
	Bool alarm_buzzing;
	int category_alpha; //��з�
	int category_beta; //�Һз�
	Bool stopwatch_indicator; //indicator ��������,
	Bool alarm_indicator; //indicator ��������,
}mode;
typedef struct Time {
	int YY; //�⵵ (2019-2099)
	int MT; //�� (1-12)
	int DD; //�� (MT: 1,3,5,7,8,10,12 -> 1-31, MT: 4,6,9,11 -> 1-30, MT: 2 -> 1-28)
	int HH; //�� (0-24)
	int MM; //�� (0-59)
	int SS; //�� (0-59)
	int MS; //ms (0-999)
}Time;
typedef struct Alarm { //���� �ð�
	Time alarmTime;
}alm;
typedef struct StopWatch {
	//LapTime�� StartTime�� ������� ������Ʈ �ȴ�.
	Time stopwatchTime;
	Time startTime;
	Time lapTime;

}stopwatch;
//Data Store�� �����ϱ�
alm AL; //�˶�
stopwatch ST; //�����ġ
mode MD; //���
Time CT; //���� �ð�
int Backlight; //���ڻ�
//0 : Alarm Buzzing, 1 : ��з�, 2: �Һз� , 3 : Stopwatch_Indicator, 4 : Alarm indicator
Bool buttonA_interface(char input) {
	if (input == 'a' || input == 'A') {
		//printf("Button A Selected \n");
		return true;
	}
	return false;
}
Bool buttonB_interface(char input) {
	if (input == 'b' || input == 'B') {
		//printf("Button B Selected \n");
		return true;
	}
	return false;
}
Bool buttonC_interface(char input) {
	if (input == 'c' || input == 'C') {
		//printf("Button C Selected \n");
		return true;
	}
	return false;
}
Bool buttonD_interface(char input) {
	if (input == 'd' || input == 'D') {
		//printf("Button D Selected \n");
		return true;
	}
	return false;
}
Bool buttonNone_interface(char input) {
	if (input == NULL) {
		//printf("No Button Selected \n");
		return true;
	}
	return false;
}
void init() { //�ʱ�ȭ. ���α׷� ù ����ÿ� ȣ���.
	//�ð� �ʱ�ȭ
	CT.YY = 2019, CT.MT = 1, CT.DD = 1, CT.HH = 0, CT.MM = 0, CT.SS = 0, CT.MS = 0;
	//�˶� �ʱ�ȭ
	AL.alarmTime.YY = 2019, AL.alarmTime.MT = 1, AL.alarmTime.DD = 1, AL.alarmTime.HH = 0, AL.alarmTime.MM = 0, AL.alarmTime.SS = 0, AL.alarmTime.MS = 0;
	//�����ġ
	ST.stopwatchTime.YY = 2019, ST.stopwatchTime.MT = 1, ST.stopwatchTime.DD = 1, ST.stopwatchTime.HH = 0, ST.stopwatchTime.MM, ST.stopwatchTime.SS = 0, ST.stopwatchTime.MS = 0;
	ST.startTime.YY = 2019, ST.startTime.MT = 1, ST.startTime.DD = 1, ST.startTime.HH = 0, ST.startTime.MM = 0, ST.startTime.SS = 0, ST.startTime.MS = 0;
	ST.lapTime.YY = 2019, ST.lapTime.MT = 1, ST.lapTime.DD = 1, ST.lapTime.HH = 0, ST.lapTime.MM = 0, ST.lapTime.SS = 0, ST.lapTime.MS = 0;
	//��� �ʱ�ȭ
	MD.alarm_buzzing = false, MD.alarm_indicator = false, MD.stopwatch_indicator = false;
	MD.category_alpha = 0, MD.category_beta = 0;
	Backlight = Backlight_Controller(COLOR_GRN);
}
int Button_Selector() {
	Sleep(1000);
	int Selected_Button = 0;
	Bool isA = false, isB = false, isC = false, isD = false;
	int value = -1;
	while (value = linux_kbhit()) { //no button�� ��� �̰��� ��ġ�� �ʴ´�.
		isA = false; isB = false; isC = false; isD = false;
		char temp = getch();
		isD = buttonD_interface(temp);
		if (isD) {
			Selected_Button = 4;
			break;
		}
		isC = buttonC_interface(temp);
		if (isC) {
			Selected_Button = 3;
			break;
		}
		isB = buttonB_interface(temp);
		if (isB) {
			Selected_Button = 2;
			break;
		}
		isA = buttonA_interface(temp);
		if (isA) {
			Selected_Button = 1;
			break;
		}
		else {
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
void Button_Operator(int Selected_Button) {
	Bool alarm_buzzing = MD.alarm_buzzing;
	int category_alpha = MD.category_alpha;
	int category_beta = MD.category_beta;
	Bool stopwatch_indicator = MD.stopwatch_indicator;
	Bool alarm_indicator = MD.alarm_indicator;

	// ����� ��з�(category_alpha)-�Һз�(category_beta)-Selected_Button ������ �ۼ�
	if (alarm_buzzing) { //�˶� �︮�Ⱑ �ֿ켱
		if (Selected_Button == 0) {
			// ��ư�� ������ ������ 5�� ���� ������ �ִٰ� ������ ����
		}
		else { // �˶��� ��
			MD.alarm_buzzing = false; // �˶� �� �︲ ���·� �ٲ��ְ�
			AL.alarmTime.YY = 2019;
			AL.alarmTime.MT = 1;
			AL.alarmTime.DD = 1;
			AL.alarmTime.HH = 0;
			AL.alarmTime.MM = 0;
			AL.alarmTime.SS = 0;
			AL.alarmTime.MS = 0; // �˶� ���� �ð��� �ʱ�ȭ
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
					if (CT.SS == 59) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						CT.SS == 0;
					}
					else CT.SS++; // ���� �ð� �� 1 ����
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
					if (CT.HH == 23) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						CT.HH == 0;
					}
					else CT.HH++; // ���� �ð� �ð� 1 ����
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
					if (CT.MM == 59) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						CT.MM = 0;
					}
					else CT.MM++; // ���� �ð� �� 1 ����
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
					if (CT.YY == 2099) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						CT.YY = 2019;
					}
					else CT.YY++; // ���� �ð� �� 1 ����
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
					if (CT.MT == 12) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						CT.MT = 1;
					}
					else CT.MT++; // ���� �ð� �� 1 ����
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
					// �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
					switch (CT.MT) {
						// �� �޿� 31���� �ִ� ���
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if (CT.DD == 31) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
							CT.DD = 1;
						}
						else CT.DD++;
						break;
						// �� �޿� 28���� �ִ� ���(������ ����)
					case 2:
						if (CT.DD == 28) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
							CT.DD = 1;
						}
						else CT.DD++;
						break;
						// �� �޿� 30���� �ִ� ���
					case 4:
					case 6:
					case 9:
					case 11:
						if (CT.DD == 30) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
							CT.DD = 1;
						}
						else CT.DD++;
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
						// ST.stopwatchTime = 0�� ����
						ST.stopwatchTime.YY = 0;
						ST.stopwatchTime.MT = 0;
						ST.stopwatchTime.DD = 0;
						ST.stopwatchTime.HH = 0;
						ST.stopwatchTime.MM = 0;
						ST.stopwatchTime.SS = 0;
						ST.stopwatchTime.MS = 0;
						// ST.startTime = 0�� ����
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
						// ST.stopwatchTime += ST.startTime�� ����
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
					if (AL.alarmTime.HH == 23) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						AL.alarmTime.HH = 0;
					}
					else AL.alarmTime.HH++; // �˶� ���� �ð� 1 ����
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
					if (AL.alarmTime.MM = 59) { // �ִ�ġ�� �� ���¿��� �ٽ� �Է��ϸ� ����������
						AL.alarmTime.MM = 0;
					}
					AL.alarmTime.MM++; // �˶� ���� �� 1 ����
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
	// 1. CT�� ����ȭ (int ������� �ð� ���� ���� ������)
	// ST.stopwatchTime �ð� ���� ���� ���߱�
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
	switch(ST.stopwatchTime.MT) { // �� ���� �� ���� �ȿ� ���������� Ȯ��
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
	case 2: // ������ ������� ����
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

	// ST.lapTime�� �ð� ���� ���� ���߱�
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
	switch(ST.lapTime.MT) { // �� ���� �� ���� �ȿ� ���������� Ȯ��
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
	case 2: // ������ ������� ����
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
		switch(ST.lapTime.MT) { // �� ���� �� ���� �ȿ� ���������� Ȯ��
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
		case 2: // ������ ������� ����
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

}
void Mode_Changer(mode Mode_to_Change) { //MD�� ������ �� �ִ� �Լ�
	MD = Mode_to_Change; //�� ����
}
void Panel_and_Speaker_Controller() {

}
int Backlight_Controller(int backlight) { //�� ����
	if (backlight == COLOR_DEF) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_GRN);
		return COLOR_GRN;
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEF);
		return COLOR_DEF;
	}
}
int main() {
	init();
	//�׽�Ʈ ����. ���� �ǹ� X
	int Selected_Button = 0;
	while (true) {
		Selected_Button = Button_Selector();
		Button_Operator(Selected_Button);
	}
	return 0;
}
