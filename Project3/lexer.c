// A lexical analyzer for relational operators and identifiers
// NGH 2016 CMS 495

// The program recognizes the six basic Pascal-style relational operators
// =, <, >, <=, >=, <>, variable names, key words , comments and new lines 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for isalpha() and isdigit()


/*** Analyze an identifier ***/

// Inputs:
//  FILE *f: the file

// Outputs:
//  none: tokens are identified as the program runs
void analyzeIdentifier(FILE *f) {
    
    // Allocate a buffer to hold the identifier
    char *s = (char *) malloc(128 * sizeof(char));
    char copy[128]; 
    while (1) {
        // Read the next character
        char c = (char) fgetc(f);
        
       // c = tolower(c);
        // If the character is not alphabetic, we're done
        if (!isalpha(c) && !isdigit(c)) {
            int i;
            strncpy(copy, s, 32 ); 
            for (i = 0; copy[i]; i++){
            copy[i] = tolower(copy[i]);
                }
            // Check here if the token is a reserved keyword
            if(strcmp(copy, "var") == 0){
                printf("<VAR> ");
            }
            else if(strcmp(copy, "array") == 0){
                printf("<ARRAY> ");
            }
            else if(strcmp(copy, "begin") == 0){
                printf("<BEGIN> ");
            }
            else if(strcmp(copy, "const") == 0){
                printf("<CONST> ");
            }
            else if(strcmp(copy, "do") == 0){
                printf("<DO> ");
            }
            else if(strcmp(copy, "end") == 0){
                printf("<END> ");
            }
            else if(strcmp(copy, "else") == 0){
                printf("<ELSE> ");
            }
            else if(strcmp(copy, "if") == 0){
                printf("<IF> ");
            }
            else if(strcmp(copy, "of") == 0){
                printf("<OF> ");
            }
            else if(strcmp(copy, "procedure") == 0){
                printf("<PROCEDURE> ");
            }
            else if(strcmp(copy, "program") == 0){
                printf("<PROGRAM> ");
            }
            else if(strcmp(copy, "record") == 0){
                printf("<RECORD> ");
            }
            else if(strcmp(copy, "then") == 0){
                printf("<THEN> ");
            }
              else if(strcmp(copy, "type") == 0){
                printf("<TYPE> ");
            }
            else if(strcmp(copy, "while") == 0){
                printf("<WHILE> ");
            }
            // If it's not, report it as an ID
            else{
                printf("<NAME, %s> ", s);
            }
            // Push the character back on the stream
            ungetc(c, f);
            
            break;
        }
      
        
        // Else, add it to the identifier
        else {
            // Use strncat to make sure that only one character is appended
            strncat(s, &c, 1);
            // Maybe we should check to make sure the buffer isn't full?
        }
    }
    
    return;
}

void analyzeDigit(FILE *f) {
    
    // Allocate a buffer to hold the identifier
    char *i = (char *) malloc(128 * sizeof(char));
    
    while (1) {
        // Read the next character
        char x = (char) fgetc(f);
       
        // If the character is not alphabetic, we're done
        if (!isdigit(x)) {
            
                printf("<NUMERAL, %s> ", i);
        
                ungetc(x, f);
            break;
        }
        else {
            // Use strncat to make sure that only one character is appended
            strncat(i, &x, 1);
          
            // Maybe we should check to make sure the buffer isn't full?
        }
    }
    return; 
}
         

int analyzeComment(FILE *f) {
    
    // Allocate a buffer to hold the identifier
    char *i = (char *) malloc(128 * sizeof(char));
    
    while (1) {
        // Read the next character
        char x = (char) fgetc(f);
       
        // If the character is not alphabetic, we're done
        if (x == '}' ){
            
                printf("<COMMENT> ");
                break;
        }
        else if (x == EOF){
            printf("<ERROR, EOF prior to end of COMMENT> \n");
            
            return 1;
        
        }
        else {
            // Use strncat to make sure that only one character is appended
            strncat(i, &x, 1);
          
            // Maybe we should check to make sure the buffer isn't full?
        }
    }
    return 0; 
}         
            
/*** Analyze a file ***/

// Inputs:
//  char *filename : the name of the input file

// Outputs:
//  none: tokens are generated as the analysis executes

void analyze(char *filename) {
    int linenumber = 1;
    int endOfFileError = 0; 
    char next;
    // Open the file and check for errors
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("open");
        exit(1);
    }
    
    // Loop until reaching EOF
    while (1) {
        
        // Read the next character
        char c = (char) fgetc(f);
        
        // Check for end-of-file
        if (c == EOF) {
            if(endOfFileError == 0){
            printf("<ENDTEXT> \n");
            break;
            }
            else{
                break;
            }
            
        }
        
        // Check for errors
        else if (c < 0) {
            perror("read");
            exit(1);
        }
        
        // If the character is alphabetic, look for an identifier
        else if (isalpha(c) ){ 
            
            // Push the current character back on the stream
            ungetc(c, f);
            analyzeIdentifier(f);
        }
        else if(isdigit(c)){
            ungetc(c,f);
            analyzeDigit(f);
        }
        // comment 
        else if (c == '{'){
            ungetc(c, f);
            endOfFileError= analyzeComment(f);
        }
        
        // If the token is =, it can only be an equals operator
        else if (c == '=') {
            printf("<EQUAL> ");
        }
        
        // If the token is <, there are three possible cases
        else if (c == '<') {
            
            // Read the next character to determine which case applies
            c = (char) fgetc(f);
            
            // <= case
            if (c == '=') {
                printf("<NOTGREATER> ");
            }
            
            // <> case
            else if (c == '>') {
                printf("<NOTEQUAL> ");
            }
            
            // Default case: <
            else {
                printf("<LESS> ");
                
                // The current value of c is not part of this token.  Put it 
                // back on the stream so it can be read as the start of a new
                // token
                ungetc(c, f);
            }
        }
        
        // Similar cases for >
        else if (c == '>') {
            c = (char) fgetc(f);

            // >= case
            if (c == '=') {
                printf("<NOTLESS> ");
            }
            
            // Default case is >
            else {
                printf("<GREATER> ");
                
                // Push the current character back on the stream
                ungetc(c, f);
            }
        }
        
        // period && double dot
        else if(c == '.'){
            c = (char) fgetc(f);
            // .. double dot
            if(c == '.'){
                printf("<DOUBLEDOT> ");
            }
            else {
                printf("<PERIOD> ");
                ungetc(c, f);
            }
        }
        
        // colon and becomes 
        else if(c == ':'){
            c = (char) fgetc(f);
            // := becomes
            if(c == '='){
                printf("<BECOMES> ");
            }
            // : colon
            else {
                printf("<COLON> ");
                ungetc(c, f);
            }
        }
        
        // newline \n
        else if (c == '\n'){
            // 
               linenumber ++; 
            printf("\n");
           
        }
        
        // or 
        else if(c == '|'){
            next = (char) fgetc(f);
            // 
            if(next == '|'){
               printf("<OR> ");
            }
            else {
               printf("<ERROR , %c, %d> \n", c, linenumber);
               ungetc(next, f);
               return;
            }
        
        }
        
        // && and 
        else if(c == '&'){
             next = (char) fgetc(f);
            // 
            if(next == '&'){
               printf("<AND> ");
            }
            else {
               printf("<ERROR , %c, %d> \n ", c, linenumber);
               ungetc(next, f);
               return;
            }
        }
        
        // Asterick * 
        else if(c == '*'){
            printf("<ASTERICK> ");
        }
        // comma , 
        else if(c == ','){
            printf("<COMMA> ");
        }
        // divide / 
        else if(c == '/'){
            printf("<DIV> ");
        }
        // left bracket [
        else if(c == '['){
            printf("<LEFTBRACKET> ");
        }
        // left parens (
        else if(c == '('){
            printf("<LEFTPARENS> ");
        }
        // minus - 
        else if(c == '-'){
            printf("<MINUS> ");
        }
        
        // mod %
        else if(c == '%'){
            printf("<MOD> ");
        }
        
        // not
        else if(c == '!'){
            printf("<NOT> ");
        }
        // plus + 
        else if(c == '+'){
            printf("<PLUS> ");
        }
        // right bracket ]
        else if(c == ']'){
            printf("<RIGHTBRACKET> ");
        }
        // right parens
        else if(c == ')'){
            printf("<RIGHTPARENS> ");
        }
        // semi colon ; 
        else if(c == ';'){
            printf("<SEMICOLON> ");
        }
        else{
            if(c != ' '){
            printf("\n <ERROR, UNKNOWN TOKEN %c, %d> \n", c, linenumber);
            endOfFileError = 1;
            }
        }
    }
    return;
}

int main(int argc, char *argv[]) {
    
    
     if (argc != 3) {
       fprintf(stderr, "Usage: ./lexer -i inputfile \n");
        exit(1);
    }
    
    char *inFileName = argv[2];
    
    analyze(inFileName);
    return 0;
}