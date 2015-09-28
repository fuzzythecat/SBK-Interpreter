#include<cstdio>
#include<cmath>
#include<cstring>
#include<cstdlib>
#include<cassert>
#include<iostream>
#include<string>
#include<vector>
#include<stack>

#define MAX_SIZE 100000

using namespace std;

/* 쌀 = '0', 보리 = '1', 콩 = ';' */

enum OPERATION_TYPE
{
	INC_DATA_PTR = 0,
	DEC_DATA_PTR = 1,
	INC_BYTE_AT_PTR = 2,
	DEC_BYTE_AT_PTR = 3,
	OUTPUT_DATA_AT_PTR = 4,
	SCAN_DATA_AT_PTR = 5,
	BEGIN_WHILE = 6,
	END_WHILE = 7
};

char c_array[MAX_SIZE], *c_ptr = c_array;
stack<int> operation_queue;

void IncDataPtr(void);
void DecDataPtr(void);
void IncByteAtPtr(void);
void DecByteAtPtr(void);
void OutputDataAtPtr(void);
void ScanDataAtPtr(void);
void BeginWhile(vector<int>&, int);
void EndWhile(vector<int>&, int);

char* LoadFile(char const *);
char* TruncateFile(char const *, bool *);
int BinaryToDecimal(char const *);
void StoreOperation(vector<int>&, char *);
void FetchOperation(vector<int>&, int);
void CheckForError(bool const);
void RunOperation(int const, vector<int>&, int);
void (*OperationTable[6]) (void) = {IncDataPtr, DecDataPtr, IncByteAtPtr, DecByteAtPtr, OutputDataAtPtr, ScanDataAtPtr};

int main(void)
{	
	char *ori_file, *code;
	printf("Input the file name: ");
	char path[100];scanf("%s", path);
	bool fin_flag = false;
	vector<int> operation_container; 
	ori_file = LoadFile(path);
	code = TruncateFile(ori_file, &fin_flag); 

	CheckForError(fin_flag);

	StoreOperation(operation_container, code);

	FetchOperation(operation_container, 0);
	
	free(ori_file);
	free(code);

	return 0;
}


/*{{{*/char* LoadFile(char const *path)
{
	FILE *fp;
	char *file;
	int size;

	fp = fopen (path ,"rb");
	if(!fp) perror(path),exit(1);

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	file = (char*)calloc(1, size+1);
	if(!file)
	{
		fclose(fp); fputs("Failed to allocate memory", stderr); exit(1);
	}

	if(fread(file, size, 1, fp) != 1)
	{
		fclose(fp); free(file); fputs("Failed to read the entire file", stderr); exit(1);
	}
	fclose(fp);

	return file;
}
/*}}}*/
/*{{{*/char* TruncateFile(char const *ori_file, bool *fin_flag)
{
	int idx = 0, size = strlen(ori_file);
	char *file = (char*)malloc(sizeof(char) * size);
	bool comment_flag = false;
	char comment_char = '@';

	for(int i = 0; i < size; ++i)
	{
		if(comment_flag)
		{
			for(;i < size; ++i)
				if(ori_file[i] == comment_char) 
				{ 
					comment_flag = false;
					break; 
				}	
		}
		else if(ori_file[i] == comment_char)
			comment_flag = true;	
		else if(strncmp(ori_file+i, "쌀", 3) == 0)
		{
			//	strncpy(file+idx, ori_file+i, 4);
			strncpy(file+idx, "0", 1);
			i += 2; idx++;
		}
		else if(strncmp(ori_file+i, "콩", 3) == 0)
		{
			strncpy(file+idx, "|", 1);
			i += 2; idx++;
		}
		else if(strncmp(ori_file+i, "보리", 6) == 0)
		{
			//	strncpy(file+idx, ori_file+i, 8);	
			strncpy(file+idx, "1", 1);
			i += 5; idx ++;
		}
		else if(strncmp(ori_file+i, "취사", 6) == 0)
		{
			*fin_flag = true;
			break;
		}
	}	

	file[idx] = '\0';

	return file;
}
/*}}}*/
/*{{{*/void StoreOperation(vector<int>& operation_container, char *code)
{
	char *binary = strtok(code, "|");
	while(binary != NULL)
	{
		//		printf("operation %d pushed\n", BinaryToDecimal(binary));
		operation_container.push_back(BinaryToDecimal(binary));
		binary = strtok(NULL, "|");
	}
}
/*}}}*/
/*{{{*/int BinaryToDecimal(char const *binary)
{
	int dec = 0, digit = strlen(binary);
	for(int i = 0; i < digit; ++i)
		dec += (int)pow(2.0, digit - i-1) * (binary[i] - '0');
	return dec;
}
/*}}}*/
/*{{{*/void FetchOperation(vector<int>& operation_container, int operation_idx)
{
//	for(int i = operation_idx; i < operation_container.size(); ++i)
//		printf("operation = %d ", operation_container[i]);


	for(int i = operation_idx; i < operation_container.size(); ++i)
	{
		int operation = operation_container[i];
		RunOperation(operation, operation_container, i+1);		
	}
}
/*}}}*/

/*{{{*/void CheckForError(bool fin_flag)
{
	if(!fin_flag)
		fputs("Unable to cook the file", stderr), exit(1);
}
/*}}}*/

//8 basic ptr operations
/*{{{*/
void IncDataPtr(void)
{
	c_ptr++;
}
void DecDataPtr(void)
{
	c_ptr--;
}
void IncByteAtPtr(void)
{
	(*c_ptr)++;
}
void DecByteAtPtr(void)
{
	(*c_ptr)--;
}
void OutputDataAtPtr(void)
{
	printf("%c", *c_ptr);
}
void ScanDataAtPtr(void)
{
	scanf("%c", c_ptr);
	while(getchar()!='\n');
}
void BeginWhile(vector<int>& operation_container, int operation_idx)
{
	operation_queue.push(operation_idx);
//	printf("go back to %d\n", operation_idx);
	FetchOperation(operation_container, operation_idx);
}
void EndWhile(vector<int>& operation_container, int operation_idx)
{	
	//printf("!go back to %d\n", operation_queue.top());


	if(operation_queue.empty())
	{
//		fputs("No matching { for }\n", stderr);
		exit(1);
	}	
	if(*(c_ptr) != false)
		FetchOperation(operation_container, operation_queue.top());
	else
	{
		operation_queue.pop();	
		FetchOperation(operation_container, operation_idx);
	}
}
/*}}}*/

/*{{{*/void RunOperation(int operation, vector<int>& operation_container, int operation_idx)
{
	if(operation <= 5 && operation >= 0)
		OperationTable[operation]();
	else if(operation == BEGIN_WHILE)
		BeginWhile(operation_container, operation_idx);
	else if(operation == END_WHILE)
		EndWhile(operation_container, operation_idx);
}
/*}}}*/

