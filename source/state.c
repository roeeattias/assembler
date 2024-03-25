#include "../headers/state.h"

State global_state;

void setCurrentFile(const char *file) {
  if (global_state.current_file != NULL) {
    free(global_state.current_file);
    global_state.current_file = NULL;
  }
  /* allocating a new memory for file path and coping it to the global state */
  global_state.current_file = calloc(strlen(file), sizeof(char));
  strcpy(global_state.current_file, file);
}

void setTransitionFailed(const int transition, const bool value)
{
  switch (transition)
  {
  case FIRST_TRANSITION:
    /* setting the status of the first transition (failed ot succedded)*/
    global_state.first_transition_failed = value;
    break;
  case SECOND_TRANSITION:
    /* setting the status of the second transition (failed ot succedded)*/
    global_state.second_transition_failed = value;
    break;
  default:
    break;
  }
}

void resetCurrentLine()
{
  global_state.currect_line = 0;
}

void incrementCurrentLine()
{
  global_state.currect_line++;
}

char *getCurrentFile() {
  return global_state.current_file;
}

unsigned int getCurrentLine()
{
  return global_state.currect_line;
}
