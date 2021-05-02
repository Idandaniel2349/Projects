/*
 * Parser.h
 * This module handle and parse the user's input and act accordingly.
 * This file contains declarations of functions being used in this module.
 */

#ifndef PARSER_H_
#define PARSER_H_

GameStatus getUserCommands(Mode* mode, GameBoard* boardPtr, int* MarkE,DoublyLinkedList *dolist, int* inputLen);
GameStatus getInput(char* inputArr);
int validateName(char* inputArr);
GameStatus validateMode(Mode mode, int commType);
int extractParameters(char* inputArr, char* parametersArr[]);
GameStatus validateNumOfParameters(int commType, int numOfParameters);
ParameterType checkParameterType(char* parameter);
GameStatus validateParametersRange(int commType, int numOfParameters, char* parametersArr[], int boardSize);
GameStatus validateParametersType(int commType, int numOfParameters, char* parametersArr[]);

#endif /* PARSER_H_ */
