#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType *next;
};

// Fill out this structure
struct HashType
{
	struct RecordType* bucket[23];
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		struct RecordType* curr = pHashArray->bucket[i];
		if(curr != NULL){
			printf("index %d -> ", i);
			while(curr != NULL){
				printf("%d, %c, %d -> ", curr->id, curr->name, curr->order);
				curr = curr->next;
			}
			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	struct HashType hashArray;
	for(int i = 0; i < 23; ++i){
		hashArray.bucket[i] = NULL;
	}

	for(int i = 0; i < recordSz; ++i){
		int index = hash(pRecords[i].id);
		struct RecordType* newRecord = (struct RecordType*)malloc(sizeof(struct RecordType));
		if(newRecord == NULL){
			return -1;
		}
		*newRecord = pRecords[i];
		newRecord->next = hashArray.bucket[index];
		hashArray.bucket[index] = newRecord;
	}
	displayRecordsInHash(&hashArray, 23);
}