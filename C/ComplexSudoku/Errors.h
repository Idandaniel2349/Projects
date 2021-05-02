/*
 * Errors.h
 * This module is responsible to hold errors commands call throughout the game.
 * This header file only takes care of functions definitions.
 */

#ifndef ERRORS_H_
#define ERRORS_H_
#include "MainAuxDefinitions.h"

void mallocError(GameStatus* status);
void invalidSave(char *s);
void invalidCellValue(int blockSize);
void valNum(int blockSize);
void notNatural(char*s);
void fileTooShort(char * x);
void fileReadFailure(char * x);

void fileOpenFailure(char * x);
void InvalidCommand();
void InvalidFilePath();
void InvalidFileFormat();
void undoLimit();
void redoLimit();
GameStatus InvalidCommandName();
GameStatus InvalidCommandMode(int commType);
GameStatus InvalidInputLength();
GameStatus TooMuchParameters(int commType);
GameStatus InvalidNumberOfParameters(int commType,int NumOfParameters);
void setInFixedCellError();
void erroneousBoardInValidate();
void erroneousBoardNumSolutions();

#endif /* ERRORS_H_ */


