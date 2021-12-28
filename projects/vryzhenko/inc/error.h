// error.h
#ifndef USH_ERROR_H
#define USH_ERROR_H


/// A function that stops application and throws error message to the standard error output.
/// \param str string argument
void error_throw(int err, char *str);

void error_get_usage();


#endif // !USH_ERROR_H
