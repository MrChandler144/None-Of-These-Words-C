#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000

// v1 is the first version, pretty much straight out of ChatGPT
// v2 is a rework, with less chatGPT.

void makeLowerCase(char *str)
{
	// with thanks to ChatGPT
	
	if (str[0] == '\0')
	{
		printf("making an empty string lowercase :/\n");
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
		printf("running hasGoodStartCharacter on an empty string :/\n");
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
		printf("hmm you gave an empty string to removeFromFront :/\n");
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

int checkIsInLine(const char *line_ref, const char *word_ref)
{
	// this function is given a lowercase word and a sanitized lowercase line
	// and it checks the word is found in the line
	
	char pulled_word[MAX_WORD_LENGTH];	// the word pulled from the given line
	
	char word[MAX_WORD_LENGTH];			// the editing copy of word_ref
	strcpy(word, word_ref);
	makeLowerCase(word);
	
	char line[MAX_LINE_LENGTH];			// the editing copy of line_ref
	strcpy(line, line_ref);
	makeLowerCase(line);
	
	while ((line[0] != '\n') && (line[0] != '\0'))
	{		
		// transfer the next word in the line into line_word from line
		makeWord(pulled_word, line);

		// check if we found it		
		if (strcmp(pulled_word, word) == 0)
		{
			return 1;
		}
	}
	return 0; // No whole word match found
}

void runSearch(const char *word, int *wordsInBible)
{
	// this is the main function that actually looks for the word in the whole Bible
	// takes in a mixed case word and a counter variable
	
	// open the Bible
	FILE *fid = fopen("kjv.txt", "r");
    if (fid == NULL) {
        printf("Error opening file.\n");
        return;
    }
	
	// initialise variable to store a line
	char line_ref[MAX_LINE_LENGTH];				// the line that gets read from the file
    char line[MAX_LINE_LENGTH];					// the line we edit
	
	// pull a line
	int count = 0;
    while ((fgets(line, sizeof(line), fid)) && (count < 40000))
	{
		count++;
		// make a reference of the line for display purposes
		strcpy(line_ref, line);
		
		// make the line lowercase
		makeLowerCase(line);
		
		// the kjv.txt file I have has some references at the start like Ge1:1 to remove
		while ((line[0] != ' ') && (line[0] != '\0') && (line[0] != '\n'))
		{
			removeFromFront(line);
		}
		removeBadStartCharacters(line);
		
		// this is the bit that tries to find the word in the line
		if (checkIsInLine(line, word) == 1)
		{
			// found it
			*wordsInBible=*wordsInBible+1;
			printf("The word \"%s\" IS in the Bible!\n", word);
			printf("%s\n", line_ref);
			fclose(fid);
			return;
		}
		
    }

	fclose(fid);
	printf("The word \"%s\" IS NOT in the Bible!\n\n", word);
}

// strcpy(editingSentence, inputSentence);
// makeLowerCase(editingSentence);

int main()
{
	int wordsInBible=0;
	int wordsInSentence = 0;
	char nextWord[MAX_WORD_LENGTH];
	char editingSentence[MAX_LINE_LENGTH];
	
	// get the user input and put it into inputSentence
	char inputSentence[MAX_LINE_LENGTH];
    printf("Enter a sentence: ");
    fgets(inputSentence, sizeof(inputSentence), stdin);
	printf("\n~~~\n\n");
	
	// make the editing copy
	strcpy(editingSentence, inputSentence);
	
	// now run the search
	while ((editingSentence[0] != '\n') && (editingSentence[0] != '\0'))
	{		
		// transfer the word into nextWord from editing sentence
		makeWord(nextWord, editingSentence);
		
		// run the search of the word through the Bible
		runSearch(nextWord, &wordsInBible);	// uppercase
		
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
		printf("Thus it turns out %.1f%% of those words are in the Bible", 100*(double)wordsInBible/wordsInSentence);
	}

    return 0;
}