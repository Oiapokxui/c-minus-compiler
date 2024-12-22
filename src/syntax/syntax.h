#pragma once

#include <stdbool.h> /* need this for boolean operations */
#include <stddef.h>  /* Defines NULL. */
#include <stdio.h>  /* Declares I/O functions and EOF const.  */
#include "../cminus.h"  /* Declare Symbol data structures and functions to operate them. */

void createVariable(char *id, struct State *state);
void createArrayVariable(char *id, int length, struct State *state);
void validateVariableType(char *type, char *id, struct State *state);
