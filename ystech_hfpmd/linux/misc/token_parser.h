/*
 * token_parser.h - Token parser and helper functions.
 *
 * Authors: Jia Wang
 *
 */

#ifndef TOKEN_PARSER_H
#define TOKEN_PARSER_H



namespace misc
{

// delimit chars are spaces
char *skip_space(char *buf, cstr_t delimit);
char *goto_space(char *buf, cstr_t delimit);
const char *skip_space(const char *buf, cstr_t delimit);
const char *goto_space(const char *buf, cstr_t delimit);

// in-place split by delimit chars
void split_tokens(char *buf, std::vector<cstr_t> *ptokens, cstr_t delimit);

// split to string vector
void split_tokens(const char *buf, std::vector<std::string> *ptokens, cstr_t delimit);

// faster op through bmp
struct delimit_bmp {unsigned int bmp[8];};
delimit_bmp create_delimit_bmp(cstr_t delimit);
char *skip_space(char *buf, const delimit_bmp &bmp);
char *goto_space(char *buf, const delimit_bmp &bmp);
void split_tokens(char *buf, std::vector<cstr_t> *ptokens, const delimit_bmp &bmp);

} // namespace misc

#endif // TOKEN_PARSER_H
