#ifndef _MOD_RFID125_H
#define _MOD_RFID125_h

void Scan(void);
void GetName(char *name, char *code);
void NewUser(void);
void RemoveUser(void);
int CheckCode(char* code);
int CheckName(char* name);
void ReadCode(char* code);
void WaitForChar(int *fd, char ch);
void FillUsers(void);
void ReadLines(FILE* file);

#endif
