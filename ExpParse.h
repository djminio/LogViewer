#ifndef __EXPPARSE_H_
#define __EXPPARSE_H_

class CLogViewerDlg;

int ExpParseMain(char * filenameString, char * dateString, char * dateString2);
void SetDlgPtr(CLogViewerDlg * ptr = 0L);

#define LINEMAX 128

// ȭ���� ���� �Ľ��ϴ� �Լ�
int ParsePrint(const char * filename, char * date, char * date2);

int CheckDateline(char * line);					// ��¥ ���������� Ȯ���ϴ� �Լ�
int CheckDate(char * line, char * date, char * date2);		// �ش� ��¥������ Ȯ���ϴ� �Լ�
void TrimTime(char * line);						// �ش� ���ο��� time�� ©�� ���
void TrimExp(char * line);						// �ش� ���ο��� Exp�� ©�� ����ϴ� �Լ�

int CheckTacticline(char * line);				// ��ƽ ���������� Ȯ���ϴ� �Լ�
void SetTactic(char * line, FILE * fp);			// ��ƽ�� �ش��ϴ� ���ε��� ���� ���


void writeNextline();				// g_fp ���Ͽ� �����ٷ� �̵���Ŵ

// ��¥����Ʈ�� Ȯ���� ������ InsertDatelist�� ���ϴ� �Լ�
int CheckDatelist(char * date);
// ��ȿ�� ��¥�� �ԷµǾ����� Ȯ���ϴ� �Լ�
int CheckDateCorrect(char * date);
// �ش� ��¥���� �����ϴ� �Լ�
void InsertDatelist(char * date);

// ����� �����ϴ� �Լ�
void Output(char * string, FILE * fp = 0L);

// ������ ȭ�ϸ����� ���ο� ȭ�ϸ��� ������ ȭ���� ����
FILE * SetNewLogfile(char * filename, char * date, char * date2);



#endif //__EXPPARSE_H_