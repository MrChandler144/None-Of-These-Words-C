#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000

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

void removeFromFront(char *str)
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

int main()
{
	// open the Bible
	FILE *fid1 = fopen("kjv.txt", "r");
	if (fid1 == NULL) {
		printf("Error opening file.\n");
		return;
	}
	FILE *fid2 = fopen("kjv2.txt", "w");
	if (fid2 == NULL) {
		printf("Error opening file.\n");
		return;
	}

	char line[MAX_LINE_LENGTH];

	// pull a line
	int count = 0;
	while ((fgets(line, sizeof(line), fid1)) && (count < 40000))
	{
		count++;
		
		// the kjv.txt file I have has some references at the start like Ge1:1 to remove
		while ((line[0] != ' ') && (line[0] != '\0') && (line[0] != '\n'))
		{
			removeFromFront(line);
		}
		removeBadStartCharacters(line);
		
		// now write the line
		fprintf(fid2, "%s", line);
	}
	fclose(fid1);
	fclose(fid2);

    return 0;
}