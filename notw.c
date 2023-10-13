#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000
#define FILENAME "niv.txt"

// you need to handle new lines the niv messes up when you give it an empty line only got \n
// if you want, the user doesn't know

// v1 is the first version, pretty much straight out of ChatGPT
// v2 is a rework, with less chatGPT.
// v3 tries to optimise it by copying less

void makeLowerCase(char *str)
{
	// with thanks to ChatGPT
	
	if (str[0] == '\0')
	{
		// apparently this is an edge case
		return;
	}

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (isupper((int)str[i]))
		{
			str[i] = (char)tolower((int)str[i]);
		}
	}
}

int hasGoodStartCharacter(char *str)
{
	// returns 0 if the first character in the string is not a character that goes in a word
	// returns 1 if it is
	
	// strchr("abc", editingSentence[0]) searches for editingSentence[0] within the string "abc".
	// If it finds a match, it returns a pointer to the matched character; otherwise, it returns NULL.
	
	// apparently this is an edge case
	if (str[0] == '\0')
	{
		return 0;
	}
	
	if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'0123456789", str[0]) == NULL)
	{
		return 0;
	} else {
		return 1;
	}
}

void removeFromFront(char *str) // fine
{
	// removes the first letter of a string by shifting the whole string down one
	if(str[0] == '\0')
	{
		return;
	}
	
	int i;
	for (i=0; str[i] != '\0'; i++)
	{
        str[i] = str[i+1];
		if (str[i] == '\0')
		{
			return;
		}
    }
}

void removeBadStartCharacters(char *str)
{
	while (hasGoodStartCharacter(str) == 0)
	{
		if ((str[0] == '\0') || (str[0] == '\n'))
		{
			return;
		}
		
		removeFromFront(str);
	}
}

void makeWord(char *nextWord, char *editingSentence)
{
	// transfers the first word in editingSentence to nextWord
	int letter_index = 0;
	
	// remove bad letters at the start
	removeBadStartCharacters(editingSentence);
	
	// then add good letters to nextWord
	while ((hasGoodStartCharacter(editingSentence) == 1) && (editingSentence[0] != '\0'))
	{
		nextWord[letter_index] = editingSentence[0];
		removeFromFront(editingSentence);
		letter_index++;		
	}
	// finish the word off
	nextWord[letter_index] = '\0';
}

int checkIsInLine(char *lowercase_line, const char *lowercase_word_ref)
{
	// this function is given a lowercase word and a lowercase line
	// and it checks the word is found in the line
	
	char pulled_word[MAX_WORD_LENGTH];		// the word pulled from the given line
	
	while ((lowercase_line[0] != '\n') && (lowercase_line[0] != '\0'))
	{		
		// transfer the next word in the line into pulled_word from lowercase_line
		makeWord(pulled_word, lowercase_line);

		// check if we found it		
		if (strcmp(pulled_word, lowercase_word_ref) == 0)
		{
			return 1;
		}
	}
	return 0; // No whole word match found
}

void runSearch(const char *word, const char *lowercase_word, int *wordsInBible)
{
	// this is the main function that actually looks for the word in the whole Bible
	// takes in a mixed case word and a counter pointer
	
	// open the Bible
	FILE *fid = fopen(FILENAME, "r");		// removed starting citations
    if (fid == NULL) {
        printf("Error opening file.\n");
        return;
    }
	
	// initialise variable to store a line
	char mixed_case_line[MAX_LINE_LENGTH];		// the line that gets read from the file
    char lowercase_line[MAX_LINE_LENGTH];		// the line we edit
	
	// pull a line
    while (fgets(mixed_case_line, sizeof(mixed_case_line), fid))
	{
		// make a reference of the line for display purposes
		strcpy(lowercase_line, mixed_case_line);
		
		// make the lowercase line lowercase
		makeLowerCase(lowercase_line);
		
		// this is the bit that tries to find the word in the line
		if (checkIsInLine(lowercase_line, lowercase_word) == 1)
		{
			// found it
			*wordsInBible=*wordsInBible+1;
			printf("The word \"%s\" IS in the Bible!\n", word);
			printf("%s\n", mixed_case_line);
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
	char nextWord[MAX_WORD_LENGTH];
	char lowercaseWord[MAX_WORD_LENGTH];
	char sentence[MAX_LINE_LENGTH];
	
	// get the user input and put it into inputSentence
    printf("Enter a sentence: ");
    fgets(sentence, sizeof(sentence), stdin);
	printf("\n~~~\n\n");
	
	// now run the search
	while ((sentence[0] != '\n') && (sentence[0] != '\0'))
	{		
		// transfer the word into nextWord from the sentence
		makeWord(nextWord, sentence);
		
		strcpy(lowercaseWord, nextWord);
		makeLowerCase(lowercaseWord);
		
		// run the search of the word through the Bible
		runSearch(nextWord, lowercaseWord, &wordsInBible);
		
		// update the variables
		wordsInSentence++;
	}
	
	
	// display results
	if (wordsInBible == 0)
	{
		printf("\n\nnone of those words are in the Bible\n\n");
	}
	else if (wordsInSentence != 0)
	{
		printf("Thus it turns out %.1f%% of those words are in the Bible\n", 100*(double)wordsInBible/wordsInSentence);
	}

    return 0;
}