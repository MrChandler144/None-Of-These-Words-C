#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000

// v1 is the first version, pretty much straight out of ChatGPT

void makeLowerCase(char *str)
{
    int i;
	for (i=0; str[i] != '\0'; i++)
	{
        str[i] = (char)tolower(str[i]);
    }
}

int isWholeWord(const char *longerString, const char *referenceWord)
{
    // Get the length of the reference word
    int wordLength = strlen(referenceWord);

    // Search for the reference word in the longer string
    const char *match = strstr(longerString, referenceWord);

    while (match != NULL)
	{
        // Check if the matched word is a whole word
        if ((match == longerString || !isalpha(match[-1])) && !isalpha(match[wordLength]))
		{
            return 1; // Whole word match found
        }

        // Continue searching for the next occurrence
        match = strstr(match + 1, referenceWord);
    }

    return 0; // No whole word match found
}

void searchInFile(const char *word, int *wordsInBible)
{
    // open the file
	FILE *fid = fopen("niv.txt", "r");
    if (fid == NULL) {
        printf("Error opening file.\n");
        return;
    }
	
	// initialise variable to store a line
    char line[MAX_LINE_LENGTH];
    char lineLowercase[MAX_LINE_LENGTH];
    char wordLowercase[MAX_LINE_LENGTH];
	
	
	
	// pull a line
    while (fgets(line, sizeof(line), fid))
	{
		// this is the bit that tries to find the word in the line
		strcpy(lineLowercase, line);
		strcpy(wordLowercase, word);
		
		makeLowerCase(lineLowercase);
		makeLowerCase(wordLowercase);
		
		if (isWholeWord(lineLowercase, wordLowercase))
		{
			// found it
			*wordsInBible=*wordsInBible+1;
			printf("The word \"%s\" IS in the Bible!\n", word);
			printf("%s\n", line);
			fclose(fid);
			return;
		}
    }

	fclose(fid);
	printf("The word \"%s\" IS NOT in the Bible!\n\n", word);
}

int main()
{
    int wordsInBible=0;
	int wordsInSentence = 0;
	
	// get the user input
	char inputSentence[MAX_LINE_LENGTH];
    printf("Enter a sentence: ");
    fgets(inputSentence, sizeof(inputSentence), stdin);
	printf("\n~~~\n\n");
	
	// break it up
    char *word = strtok(inputSentence, " ,\n");
    // look
	while (word != NULL)
	{
        searchInFile(word, &wordsInBible);
		wordsInSentence++;
        word = strtok(NULL, " ,\n");
    }
	
	// display results
	if (wordsInBible == 0)
	{
		printf("\n\nnone of those words are in the Bible\n\n");
	} else if (wordsInSentence != 0)
	{
		printf("Thus it turns out %.1f%% of those words are in the Bible", 100*(double)wordsInBible/wordsInSentence);
	}

    return 0;
}