#ifndef __EXPPARSE_H_
#define __EXPPARSE_H_

class CLogViewerDlg;

int ExpParseMain(char * filenameString, char * dateString, char * dateString2);
void SetDlgPtr(CLogViewerDlg * ptr = 0L);

#define LINEMAX 128

// 화일을 열어 파싱하는 함수
int ParsePrint(const char * filename, char * date, char * date2);

int CheckDateline(char * line);					// 날짜 라인인지를 확인하는 함수
int CheckDate(char * line, char * date, char * date2);		// 해당 날짜인지를 확인하는 함수
void TrimTime(char * line);						// 해당 라인에서 time만 짤라 출력
void TrimExp(char * line);						// 해당 라인에서 Exp만 짤라 출력하는 함수

int CheckTacticline(char * line);				// 택틱 라인인지를 확인하는 함수
void SetTactic(char * line, FILE * fp);			// 택틱에 해당하는 라인들을 돌며 출력


void writeNextline();				// g_fp 파일에 다음줄로 이동시킴

// 날짜리스트를 확인후 없으면 InsertDatelist를 콜하는 함수
int CheckDatelist(char * date);
// 유효한 날짜가 입력되었는지 확인하는 함수
int CheckDateCorrect(char * date);
// 해당 날짜들을 적재하는 함수
void InsertDatelist(char * date);

// 출력을 통제하는 함수
void Output(char * string, FILE * fp = 0L);

// 기존의 화일명으로 새로운 화일명을 생성해 화일을 연다
FILE * SetNewLogfile(char * filename, char * date, char * date2);



#endif //__EXPPARSE_H_