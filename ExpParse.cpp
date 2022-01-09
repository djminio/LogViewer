#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <string.h>
#include "ExpParse.h"

#include "resource.h"
#include "LogViewerDlg.h"

using namespace std;

typedef struct s_tagDATE {
	int month;
	int date;
} s_DATE, *lpDATE;

// 날짜들을 관리할 전역변수
s_DATE datelist[100];
int listcnt = 1;
FILE * g_fp = 0L;

int rowcnt = 0;			// 열번호


CLogViewerDlg * pDlg = 0L;
int listline = 0;

int ExpParseMain(char * filenameString, char * dateString, char * dateString2)
{
	
/*
	// 이부분이 debug에서는 정상적으로 동작하나 release 에서 이상하게 동작하므로 막음
	if(!CheckDateCorrect(dateString))
	{
		MessageBox(NULL,(LPCTSTR)dateString, "", MB_OK);
		return 0;
	}
*/


	char temp[LINEMAX];

	if(!(g_fp = SetNewLogfile(filenameString, dateString, dateString2)))
		return 0;

	pDlg->ClearAll();
	rowcnt = 1;

	// 가이드 라인
	Output("----------------------------------------------------------------------------------------------------------------------", g_fp);
	writeNextline();

	// 검색한 파일
	memset(temp, 0L, sizeof(temp));
	sprintf(temp, "Searched for %s: ", filenameString);
	Output(temp, g_fp);
	writeNextline();

	// 검색한 날짜
	memset(temp, 0L, sizeof(temp));
	sprintf(temp, "%s - Log Parse of %s : ", dateString, dateString2);
	Output(temp, g_fp);
	writeNextline();

	// 가이드 라인
	Output("----------------------------------------------------------------------------------------------------------------------", g_fp);
	writeNextline();

	Output("Date (time)\tExp\tFame\tCrapple\tSwordmanship\tArchery\tFencing\tMace Fighting\tPierce\tWhirl\tHurl\tParrying\tDouble Swordmanship\tDouble Mace Fighting\tMagery\tOrison\t", g_fp);
	writeNextline();


	// 파싱시작 실패시 if문 진입
	if(!(ParsePrint(filenameString, dateString, dateString2)))
	{
		Output("---------------------------------------------------------", g_fp);
		writeNextline();
		memset(temp, 0L, sizeof(temp));
		sprintf(temp, "Could not open the following file >>> %s", filenameString);
		Output(temp, g_fp);
		writeNextline();
		Output("---------------------------------------------------------", g_fp);
		writeNextline();
		
		//실패 하였으므로
		fclose(g_fp);
		g_fp = 0L;
		return 0;
	}

	fclose(g_fp);
	g_fp = 0L;

	return 1;
}

// 화일을 열어 파싱하는 함수
int ParsePrint(const char * filename, char * date, char * date2)
{
	FILE * fp = 0L;
	fp = fopen(filename, "rb");
	
	// 읽기 실패시
	if(!fp)
	{
		return 0;
	}

	char linebuf[LINEMAX];
	char * lineStartPtr = 0L;

	int pagecnt = 15;

	while(!feof(fp))
	{
		fgets(linebuf, LINEMAX, fp);
		// 날짜를 나타내는 라인이라면 
		if(CheckDateline(linebuf))
		{
			if(CheckDate(linebuf, date, date2))
			{
				TrimTime(linebuf);	
				// 다음라인을 취해 보여준다
				fgets(linebuf, LINEMAX, fp);

				TrimExp(linebuf);

				// 택틱 라인까지 이동
				while(!CheckTacticline(linebuf))
					fgets(linebuf, LINEMAX, fp);

				SetTactic(linebuf, fp);

				//다음 열로 이동
				rowcnt++;

				// 15개씩 끊어서 보여주자
				if(!(--pagecnt))
				{
					getch();
					Output("", g_fp);
					writeNextline();
					pagecnt = 15;
				}
			}
		}


	}	
	
	getch();
	fclose(fp);
	return 1;
}

int CheckDateline(char * line)
{
	// 3글자중 하나라도 '-' 이 아니라면 0을 리턴
	if(*line != '-' || *(line + 1) != '-' || *(line + 2) != '-')
		return 0 ;

	return 1;
}

int CheckDate(char * line, char * date, char * date2)
{
	char temp[6];
	memset(temp, 0, sizeof(temp));

	// 날짜가 적힌 라인을 찾아낸다
	int start = 0;
	while(*(line + start) != ')')
		start++;

	start += 2;

	for(int i = 0;  i < 5; i++)
	{
		temp[i] = *(line + start + i);
	}
	temp[5] = 0L;

	// 현재 시간과 비교시간 2개를 비교한다
	s_DATE now, time, time2;
	char tmp[3];

	// 각 시간에 날짜를 대입
	tmp[0] = temp[0];		tmp[1] = temp[1];	tmp[2] = 0L;
	now.month = atoi(tmp);
	tmp[0] = temp[3];		tmp[1] = temp[4];
	now.date = atoi(tmp);

	tmp[0] = date[0];		tmp[1] = date[1];
	time.month = atoi(tmp);
	tmp[0] = date[3];		tmp[1] = date[4];
	time.date = atoi(tmp);
	
	tmp[0] = date2[0];		tmp[1] = date2[1];
	time2.month = atoi(tmp);
	tmp[0] = date2[3];		tmp[1] = date2[4];
	time2.date = atoi(tmp);

	// 1이면 범위 내이고 0이면 틀린것이다
	if( now.month < time.month || now.month > time2.month )	return 0;


	if( now.month == time.month)
	{
		if( now.date < time.date)		return 0;
	}

	if( now.month == time2.month)
	{
		if( now.date > time2.date)		return 0;
	}

	return 1;

}

int CheckDatelist(char * date)
{
	// 월을 셋팅
	char month[3];
	month[0] = date[0];
	month[1] = date[1];
	month[2] = 0L;
	// 일을 셋팅
	char day[3];
	day[0] = date[3];
	day[1] = date[4];
	day[2] = 0L;
	
	s_DATE newdate;
	newdate.month = atoi(month);
	newdate.date = atoi(day);

	// 날짜리스트 전체를 돌아본후 없으면 삽입하자
	for(int i = 0; i < listcnt; i++)
	{
		if(datelist[i].month == newdate.month && datelist[i].date == newdate.date)
			return 0;
	}

	InsertDatelist(date);

	return 1;

}

void InsertDatelist(char * date)
{
	// 월을 셋팅
	char month[3];
	month[0] = date[0];
	month[1] = date[1];
	month[2] = 0L;
	// 일을 셋팅
	char day[3];
	day[0] = date[3];
	day[1] = date[4];
	day[2] = 0L;
	
	s_DATE newdate;
	newdate.month = atoi(month);
	newdate.date = atoi(day);

	memcpy(&(datelist[listcnt]), &newdate, sizeof(DATE));
	listcnt++;

}

void TrimExp(char * line)
{
	char temp[LINEMAX];
	int tempcnt = 0;
	memset(temp, 0L, sizeof(temp));

	int i = 0;
	while( *(line + i++) != '/')	;

	// Exp: 를 제외
	i += 4;
	temp[tempcnt++] = '\t';

	while( *(line + i) != '/')
	{
		temp[tempcnt++] = *(line + i++);
	}
	temp[tempcnt++] = '\t';
	temp[tempcnt] = 0L;

	Output(temp, g_fp);
	pDlg->AddItem(rowcnt, 1, temp);

	// fame을 찾는다
	tempcnt = 0;
	i++;
	while( *(line + i++) != '/')	;

	i += 5;
	while( *(line + i) != ')')
	{
		temp[tempcnt++] = *(line + i++);
	}
	temp[tempcnt++] = '\t';
	temp[tempcnt] = 0L;

	Output(temp, g_fp);
	pDlg->AddItem(rowcnt, 2, temp);

}

int CheckDateCorrect(char * date)
{

	// 월을 셋팅
	char month[3];
	month[0] = date[0];
	month[1] = date[1];
	month[2] = 0L;
	// 일을 셋팅
	char day[3];
	day[0] = date[3];
	day[1] = date[4];
	day[2] = 0L;

	s_DATE newdate;
	newdate.month = atoi(month);
	newdate.date = atoi(day);

	char temp[128];
	wsprintf(temp, "%d	%d", newdate.month, newdate.date);

	// 유효한 범위인지 확인
	if(newdate.month < 1 || newdate.month > 12)		return 0;
	if(newdate.date < 1 || newdate.date > 31)	return 0;

	return 1;
}

void Output(char * string, FILE * fp)
{
	cout << string << "\n";
//	CString str = string;
//	str += "\n";

//	pDlg->m_ctrListText.InsertItem(listline++, string);
	
	unsigned short nl = 0x0a0d;		// 다음줄임을 표시

	// 파일쓰기라면
	if(fp)
	{
		fwrite(string, 1, strlen(string), fp);
//		fwrite(&nl, sizeof(nl), 1, fp);
	}
	
}

FILE * SetNewLogfile(char * filename, char * date, char * date2)
{
	int s = strlen(filename);
		
	char newname[LINEMAX];
	int namecnt = s - 4;
	memcpy(newname, filename, namecnt);

	sprintf(newname + namecnt, "_%s_%s.txt", date, date2);

	FILE * fp = 0L;
	fp = fopen(newname, "wb");


	return fp;

}

// Output 에 사용할 다이얼로그 포인터 셋팅
void SetDlgPtr(CLogViewerDlg * ptr)
{
	pDlg = ptr;
}

void TrimTime(char * line)
{
	char temp[16];
	memset(temp, 0L, sizeof(temp));

	int start = 0;
	while(*(line + start) != ')')
		start++;

	start += 2;

	for(int i = 0;  i < 14; i++)
	{
		temp[i] = *(line + start + i);
	}

	// 시간에 괄효를 쳐주자
	temp[5] ='(';
	temp[14] = ')';

//	Output(temp, g_fp);
	pDlg->AddItem(rowcnt, 0, temp);

	cout << temp;
	fwrite(temp, 1, sizeof(temp), g_fp);

}

int CheckTacticline(char * line)				// 택틱 라인인지를 확인하는 함수
{
	char temp[] = " [Tactic Info]";

	for(int i = 0; i < strlen(temp); i++)
	{
		if( *(line + i) != temp[i])		return 0;		// 글자가 하나라도 틀리면 거짓
	}

	return 1;
}

void SetTactic(char * line, FILE * fp)
{
	char TacticName[LINEMAX];
	char temp[LINEMAX];

	// 총 택틱의 숫자만큼 루프
	for(int colnum = 0; colnum < 13; colnum++)
	{
		memset(TacticName, 0L, sizeof(TacticName));
		memset(temp, 0L, sizeof(temp));
		fgets(line, LINEMAX, fp);

/*		// 각 택틱에 맞는 이름을 셋팅
		switch(colnum)
		{
		case 0:
			strcpy(TacticName,"	  Crapple-");
			break;
		case 1:
			strcpy(TacticName,"	  Swordmanship-");
			break;
		case 2:
			strcpy(TacticName,"	  Archery-");
			break;
		case 3:
			strcpy(TacticName,"	  Fencing-");
			break;
		case 4:
			strcpy(TacticName,"	  Mace Fighting-");
			break;
		case 5:
			strcpy(TacticName,"	  Pierce-");
			break;
		case 6:
			strcpy(TacticName,"	  Whirl-");
			break;
		case 7:
			strcpy(TacticName,"	  Hurl-");
			break;
		case 8:
			strcpy(TacticName,"	  Parrying-");
			break;
		case 9:
			strcpy(TacticName,"	  Double Swordmanship-");
			break;
		case 10:
			strcpy(TacticName,"	  Double Mace Fighting-");
			break;
		case 11:
			strcpy(TacticName,"	  Magery-");
			break;
		case 12:
			strcpy(TacticName,"	  Orison-");
			break;
		default:
			break;
		}
*/
//		strcpy(TacticName, "\t");
//		Output(TacticName, g_fp);

		// temp에 실제 택틱 수치를 셋팅하자
		int start = 0;
		while( *(line + start++) != '(')
			;


		int i = 0;
		do
		{
			temp[i] = *(line + start + i);
			
		}while( *(line + start + ++i) != ')' );

		pDlg->AddItem(rowcnt, colnum + 3, temp);

		Output(temp, g_fp);

		// Tab
		strcpy(TacticName, "\t");
		Output(TacticName, g_fp);


	}

	writeNextline();
}

void writeNextline()
{
	unsigned short nl = 0x0a0d;		// 다음줄임을 표시
	fwrite(&nl, sizeof(nl), 1, g_fp);
}