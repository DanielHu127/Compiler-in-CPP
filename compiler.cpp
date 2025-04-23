//File: final_project.cpp
//Name: Daniel Hu
//Date: 04/23/2025
//Course: CompSci 1
//Description:  This is a custom parse tree generator.
//Usage:  This program will ask the user to input code-like syntax into the input file and output parse tree.

// g++ compiler.cpp -o compiler
// ./compiler.exe ./input.txt

//THINGS TO ADD:
/*
 - Keywords: int, float, return, if, else, for, while, switch, struct, class, public, private, virtual, namespace, constexpr, template
 + Operator Precedence: Unary, Multiplicative, Additive, Shift, Relational, Equality, Bitwise AND, Bitwise OR,
                        Logical AND, Logical OR, Conditional (prob not), Assignment, Comma
 - Unary Operations (Operations that come before the term): +, -, !, ~, *, & 
 + Exponent Operations: ^^
 + Multiplicative Operations: *, /, %
 + Additive Operations: +, - 
 + Shift Operations: <<, >>
 + Relational: <, <=, >, >=
 + Equality: ==, !=
 + Bitwise AND: &
 + Bitwise XOR: ^
 + Bitwise OR: |
 + Logical AND: &&
 + Logical OR: ||
 - Conditional (prob not): ?: 
 + Assignment: =, +=, -=, *=, /=, %=, <<=, >>=, &=, ^=, |=
 + Comma: ,
 - Variables and Identifiers
 - Control Flow statements: If, else, for, while
 - Delimiters: ( ) { } [ ] ; ,
 - Punctuation: :: . -> ... 
 + Functions: Declarations and Usage
 - Preprocesser: #include, #define, #ifdef, #pragma
 + Comments: # <#
 - Casts: (int), (double), (string), (char)
*/


#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#define internal static
#define global static
#define local_persist static
//Set Debug to 0 to not see logs
//Set Debug to 1 to see logs
#define DEBUG 1
#if DEBUG
#define LOG(x) std::cout<<x<<std::endl
#else
#define LOG(x) 
#endif

#define ERR(x) std::cerr<<x<<std::endl

//All predefined Token Type variants
enum TokenType{
    RETURN,
    IDENT,
    OPEN_BLOCK,
    CLOSE_BLOCK,
    OPEN_PAREN,
    CLOSE_PAREN,

    U_INT8_DECL,
    U_INT16_DECL,
    U_INT32_DECL,
    U_INT64_DECL,
    S_INT8_DECL,
    S_INT16_DECL,
    S_INT32_DECL,
    S_INT64_DECL,
    
    STRING_DECL,
    FLOAT_DECL,
    DOUBLE_DECL,
    BOOL_DECL,
    INT_LIT,
    BOOL_LIT,
    STRING_LIT,
    DOUBLE_LIT,
    FLOAT_LIT,
    IF,
    ELSE,
    FOR,
    WHILE,
    COMMA,

    ASSIGN,
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,
    EXP_ASSIGN,
    LSHF_ASSIGN,
    RSHF_ASSIGN,
    BAND_ASSIGN,
    BXOR_ASSIGN,
    BOR_ASSIGN,

    LOGICAL_OR,
    LOGICAL_AND,
    BIT_OR,
    BIT_XOR,
    BIT_AND,
    IS_EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    L_SHIFT,
    R_SHIFT,
    ADD,
    SUB,
    MULTIPLY,
    DIVIDE,
    MOD,
    EXPONENT,

    SEMICOLON,
    EXIT,
    TREE,
    GLOBAL,
    MAIN,
    //Parse
    BLOCK,
    PARENTHESES,
    STATEMENTLIST
};

//Token Class
//Stores the TokenType and the value of the token
class Token{
    public:
    TokenType type;
    std::string value = "";

    Token() : value(""){ }
    Token(TokenType t) : type(t), value(""){ }
    Token(TokenType t, std::string val) : type(t), value(val){ }
    
    static std::vector<Token> getTokens(std::ifstream &file);

    bool getAssociativity() const;
    int getPrecedence() const;

    bool isAssignment() const;
    bool isValue() const;
    bool isDeclaration() const;
};

// Goes through the entire file and turns streams of characters into predefined tokens. 
// Returns a vector of all tokens found.
std::vector<Token> Token::getTokens(std::ifstream &file){
    std::vector<Token> Tokens;
    char ch;
    std::stringstream buffer;
    int count = 0;
    while(file.peek()!=EOF){
        ch = file.peek();
        if (std::isalpha(ch)){
            while(std::isalnum(file.peek()) || file.peek()=='_'){
                file.get(ch);
                buffer << ch;
            }
            //Check for a alnum token (ex. return, for, if)
            if(buffer.str()=="return"){
                Tokens.push_back(Token{TokenType::RETURN, "return"});
            }else if(buffer.str()=="exit"){
                Tokens.push_back(Token{TokenType::EXIT, "exit"});
            }else if(buffer.str()=="if"){
                Tokens.push_back(Token{TokenType::IF, "if"});
            }else if(buffer.str()=="else"){
                Tokens.push_back(Token{TokenType::ELSE, "else"});
            }else if(buffer.str()=="for"){
                Tokens.push_back(Token{TokenType::FOR, "for"});
            }else if(buffer.str()=="while"){
                Tokens.push_back(Token{TokenType::WHILE, "while"});
            }else if(buffer.str()=="uint8"){
                Tokens.push_back(Token{TokenType::U_INT8_DECL, "uint8"});
            }else if(buffer.str()=="uint16"){
                Tokens.push_back(Token{TokenType::U_INT16_DECL, "uint16"});
            }else if(buffer.str()=="uint32"){
                Tokens.push_back(Token{TokenType::U_INT32_DECL, "uint32"});
            }else if(buffer.str()=="uint64"){
                Tokens.push_back(Token{TokenType::U_INT64_DECL, "uint64"});
            }else if(buffer.str()=="int8"){
                Tokens.push_back(Token{TokenType::S_INT8_DECL, "int8"});
            }else if(buffer.str()=="int16"){
                Tokens.push_back(Token{TokenType::S_INT16_DECL, "int16"});
            }else if(buffer.str()=="int32"){
                Tokens.push_back(Token{TokenType::S_INT32_DECL, "int32"});
            }else if(buffer.str()=="int64"){
                Tokens.push_back(Token{TokenType::S_INT64_DECL, "int64"});
            }else if(buffer.str()=="bool"){
                Tokens.push_back(Token{TokenType::BOOL_DECL, "bool"});
            }else if(buffer.str()=="true"){
                Tokens.push_back(Token{TokenType::BOOL_LIT, "true"});
            }else if(buffer.str()=="false"){
                Tokens.push_back(Token{TokenType::BOOL_LIT, "false"});
            }else if(buffer.str()=="global"){
                Tokens.push_back(Token{TokenType::GLOBAL, "global"});
            }else if(buffer.str()=="main"){
                Tokens.push_back(Token{TokenType::MAIN, "main"});
            }else if(buffer.str()=="string"){
                Tokens.push_back(Token{TokenType::STRING_DECL, "string"});
            }else if(buffer.str()=="double"){
                Tokens.push_back(Token{TokenType::DOUBLE_DECL, "double"});
            }else if(buffer.str()=="float"){
                Tokens.push_back(Token{TokenType::FLOAT_DECL, "float"});
            }else{
                Tokens.push_back(Token{TokenType::IDENT, buffer.str()});
            }
            buffer.str("");
            buffer.clear();
        // Checks for numerical literals: int, float, double
        }else if(std::isdigit(ch) || ch == '.'){
            while(std::isdigit(file.peek())){
                file.get(ch);
                buffer << ch;
            }
            
            if(file.peek() == '.'){
                file.get(ch);
                buffer << ch;
                while(std::isdigit(file.peek())){
                    file.get(ch);
                    buffer << ch;
                }
                if(file.peek() == 'f'){
                    file.get(ch);
                    buffer<<ch;
                    Tokens.push_back(Token{TokenType::FLOAT_LIT, buffer.str()});
                }else{
                    Tokens.push_back(Token{TokenType::DOUBLE_LIT, buffer.str()});
                }
            }else{
                Tokens.push_back(Token{TokenType::INT_LIT, buffer.str()});
            }
            buffer.str("");
            buffer.clear();
        //Gets rid of spaces
        }else if(std::isspace(ch)){
            while(std::isspace(file.peek())){
                file.get(ch);
            }
            buffer.str("");
            buffer.clear();
        //Checks for special characters
        }else if(!std::isalnum(ch)){
            file.get(ch);
            if(ch==';'){
                Tokens.push_back(Token{TokenType::SEMICOLON, ";"});
            }else if(ch=='{'){
                Tokens.push_back(Token{TokenType::OPEN_BLOCK, "{"});
            }else if(ch=='}'){
                Tokens.push_back(Token{TokenType::CLOSE_BLOCK, "}"});
            }else if(ch=='('){
                Tokens.push_back(Token{TokenType::OPEN_PAREN, "("});
            }else if(ch==')'){
                Tokens.push_back(Token{TokenType::CLOSE_PAREN, ")"});
            }else if(ch=='^' && file.peek()== '^'){
                file.get(ch);
                if(file.peek()=='='){
                    Tokens.push_back(Token{TokenType::EXP_ASSIGN, "^^="});
                    file.get(ch);
                }else{
                    Tokens.push_back(Token{TokenType::EXPONENT, "^^"});
                }
            }else if(ch=='*' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::MULTIPLY, "*"});
            }else if(ch=='/' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::DIVIDE, "/"});
            }else if(ch=='%' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::MOD, "%"});
            }else if(ch=='+' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::ADD, "+"});
            }else if(ch=='-' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::SUB, "-"});
            }else if(ch=='<' && file.peek()=='<'){
                file.get(ch);
                if(file.peek()=='='){
                    Tokens.push_back(Token{TokenType::LSHF_ASSIGN, "<<="});
                    file.get(ch);
                }else{
                    Tokens.push_back(Token{TokenType::L_SHIFT, "<<"});
                }
            }else if(ch=='>' && file.peek()=='>'){
                file.get(ch);
                if(file.peek()=='='){
                    Tokens.push_back(Token{TokenType::RSHF_ASSIGN, ">>="});
                    file.get(ch);
                }else{
                    Tokens.push_back(Token{TokenType::R_SHIFT, ">>"});
                }
            }else if(ch=='>' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::GREATER, ">"});
            }else if(ch=='>' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::GREATER_EQUAL, ">="});
                file.get(ch);
            }else if(ch=='<' && file.peek()!='=' && file.peek()!='#'){
                Tokens.push_back(Token{TokenType::LESS, "<"});
            }else if(ch=='<' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::LESS_EQUAL, "<="});
                file.get(ch);
            }else if(ch=='=' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::IS_EQUAL, "=="});
                file.get(ch);
            }else if(ch=='!' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::NOT_EQUAL, "!="});
                file.get(ch);
            }else if(ch=='&' && file.peek()!='&' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::BIT_AND, "&"});
                file.get(ch);
            }else if(ch=='^' && file.peek()!='^' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::BIT_XOR, "^"});
                file.get(ch);
            }else if(ch=='|' && file.peek()!='|' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::BIT_OR, "|"});
                file.get(ch);
            }else if(ch=='&' && file.peek()=='&'){
                Tokens.push_back(Token{TokenType::LOGICAL_AND, "&&"});
                file.get(ch);
            }else if(ch=='|' && file.peek()=='|'){
                Tokens.push_back(Token{TokenType::LOGICAL_OR, "||"});
                file.get(ch);
            }else if(ch=='=' && file.peek()!= '='){
                Tokens.push_back(Token{TokenType::ASSIGN, "="});
            }else if(ch=='+' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::ADD_ASSIGN, "+="});
                file.get(ch);
            }else if(ch=='-' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::SUB_ASSIGN, "-="});
                file.get(ch);
            }else if(ch=='*' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::MUL_ASSIGN, "*="});
                file.get(ch);
            }else if(ch=='/' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::DIV_ASSIGN, "/="});
                file.get(ch);
            }else if(ch=='%' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::MOD_ASSIGN, "%="});
                file.get(ch);
            }else if(ch=='&' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::BAND_ASSIGN, "&="});
                file.get(ch);
            }else if(ch=='^' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::BXOR_ASSIGN, "^="});
                file.get(ch);
            }else if(ch=='|' && file.peek()== '='){
                Tokens.push_back(Token{TokenType::BOR_ASSIGN, "|="});
                file.get(ch);
            }else if(ch==','){
                Tokens.push_back(Token{TokenType::COMMA, ","});
            }else if(ch=='#'){
                while(file.peek()!='\n'){
                    file.get(ch);
                    if(file.peek()==EOF){
                        break;
                    }
                }
            }else if(ch=='<' && file.peek()== '#'){
                while(ch != '#' || file.peek()!='>'){
                    file.get(ch);
                    if(file.peek()==EOF){
                        break;
                    }
                }
                file.get(ch);
            }else if(ch=='\"'){
                buffer<<ch;
                while(file.peek()!='\"'){
                    file.get(ch);
                    buffer<<ch;
                    if(file.peek() == EOF){
                        break;
                    }
                }
                file.get(ch);
                buffer<<ch;
                Tokens.push_back(Token{TokenType::STRING_LIT, buffer.str()});
            }else{
                LOG(ch);
                ERR("Special Character Error");
                ERR("Unknown Character: "<<ch);
                exit(EXIT_FAILURE);
            }
            buffer.str("");
            buffer.clear();
        }
    }
    return Tokens;
}

// Returns the precedence of a token. If not a valid token, returns an error.
int Token::getPrecedence() const{
    int precedence = -1000;
    if(type == EXPONENT){precedence = 11;}
    if(type == MULTIPLY || type == DIVIDE || type == MOD){precedence = 10;}
    if(type == ADD || type == SUB){precedence = 9;}
    if(type == L_SHIFT || type == R_SHIFT){precedence = 8;}
    if(type == LESS || type == LESS_EQUAL || type == GREATER || type == GREATER_EQUAL){precedence = 7;}
    if(type == IS_EQUAL || type == NOT_EQUAL){precedence = 6;}
    if(type == BIT_AND){precedence = 5;}
    if(type == BIT_XOR){precedence = 4;}
    if(type == BIT_OR){precedence = 3;}
    if(type == LOGICAL_AND){precedence = 2;}
    if(type == LOGICAL_OR){precedence = 1;}
    if(type == IDENT || type == OPEN_PAREN || type == INT_LIT || type == BOOL_LIT || type == STRING_LIT || type == DOUBLE_LIT){precedence = 0; LOG("BOO");}
    if(type == SEMICOLON || type == CLOSE_BLOCK || type == CLOSE_PAREN || type == COMMA){precedence = -1;}
    if (precedence==-1000){
        ERR("Invalid operator precedence calculation: "<<value);
        exit(EXIT_FAILURE);
    }
    LOG(type);
    return precedence;
}

// returns true if token is an assignment operator
bool Token::isAssignment() const{
    return( type == ASSIGN || type == ADD_ASSIGN || type == SUB_ASSIGN || 
            type == MUL_ASSIGN || type == DIV_ASSIGN || type == MOD_ASSIGN || 
            type == EXP_ASSIGN || type == LSHF_ASSIGN || type == RSHF_ASSIGN || 
            type == BAND_ASSIGN || type == BXOR_ASSIGN || type == BOR_ASSIGN);
}

// returns true if token is a value
bool Token::isValue() const{
    return(type == IDENT || type == INT_LIT || type == BOOL_LIT || type == DOUBLE_LIT || type == FLOAT_LIT || type == STRING_LIT);
}

// returns true if token is a declaration
bool Token::isDeclaration() const{
    return(type == U_INT8_DECL || type == U_INT16_DECL || type == U_INT32_DECL || type == U_INT64_DECL ||
        type == S_INT8_DECL || type == S_INT16_DECL || type == S_INT32_DECL || type == S_INT64_DECL ||
        type == BOOL_DECL || type == STRING_DECL || type == FLOAT_DECL || type == DOUBLE_DECL);
}

//returns the associativity of the token. 
//true is left->right, false is right->left
bool Token::getAssociativity() const{
    return type == EXPONENT;
}

// The ParseNode class
// Each object stores a token and its corresponding children.
// Also has static class members current_index and tokens that are referenced everywhere
// Creates a tree structure
class ParseNode{
    public:
    Token token;
    std::vector<ParseNode*> children = {};

    static int current_index;
    static std::vector<Token> tokens;

    ParseNode(Token t) : token(t), children({}) { }
    ParseNode(Token t, std::vector<ParseNode*> c) : token(t), children(c){ }

    ~ParseNode(){
        for(ParseNode* child : children){
            delete child;
        }
    }

    void createTree();
    void createBlock();
    void createStatementList();

    void createReturnStatement();
    void createWhileStatement();
    void createForStatement();
    void createForParameters();
    void createIfStatement();
    void createElseStatement();
    void createDeclaration();
    void createIdentifier();
    void createAssign();

    void createFunction();
    void createFunctionUsage();
    void createFunctionDeclaration();

    void createSemicolon();

    static void printParseTree(ParseNode* node, std::ofstream &output, int indent);

    static ParseNode* makeBinaryNode(ParseNode* left, Token oper, ParseNode* right);
    static ParseNode* createExpression(int minPrec);
    static ParseNode* leftDenotation(ParseNode* left, ParseNode* next);
};

// Needed for linker to link these static class members correctly.
int ParseNode::current_index;
std::vector<Token> ParseNode::tokens;

// Creates the root node of the parse tree
void ParseNode::createTree(){
    while(current_index<tokens.size()){
        if(tokens[current_index].type == TokenType::OPEN_BLOCK){
            createBlock();
            LOG(token.value);
            LOG(children[0]->token.value);
            LOG(children[0]->token.value);
        }else{
            ERR("No Open Block Found at start of program");
            ERR("Found this instead: "<<tokens[current_index].value);
            exit(EXIT_FAILURE);
        }
    }
    LOG(token.value);
}

// creates a block node for the parse tree.
void ParseNode::createBlock(){
    ParseNode * block = new ParseNode{Token{TokenType::BLOCK, "Block"}};
    LOG(block->token.value);
    current_index++;
    (*block).createStatementList();
    current_index++;
    children.push_back(block);
    LOG(block->token.value);
}

// creates a list of statements for the parse tree.
void ParseNode::createStatementList(){
    ParseNode *statementList = new ParseNode{Token{TokenType::STATEMENTLIST, "Statement List"}};
    LOG(statementList->token.value);
    while(tokens[current_index].type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = tokens[current_index].type;
        if(currentToken == TokenType::RETURN){
            (*statementList).createReturnStatement();
        }else if(currentToken == TokenType::IF){
            (*statementList).createIfStatement();
        }else if(currentToken == TokenType::OPEN_BLOCK){
            (*statementList).createBlock();
        }else if(tokens[current_index].isDeclaration()){
            (*statementList).createDeclaration();
        }else if (currentToken == FOR) {
            (*statementList).createForStatement();
        }else if (currentToken == WHILE) {
            (*statementList).createWhileStatement();
        }else if (currentToken == IDENT) {
            (*statementList).createIdentifier();
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            ERR("Not valid statement");
            ERR("Found this instead: "<<tokens[current_index].value);
            exit(EXIT_FAILURE);
        }
        if(current_index>=tokens.size()){
            ERR("Missing Closing Bracket");
            ERR("Found end of file instead");
            exit(EXIT_FAILURE);
        }
    }
    children.push_back(statementList);
    LOG(statementList->token.value);
}

// creates a return statement for the parse tree.
void ParseNode::createReturnStatement(){
    ParseNode *returnStatement = new ParseNode{tokens[current_index]};
    LOG(returnStatement->token.value);
    current_index++;
    if(tokens[current_index].isValue()){
        returnStatement->children.push_back(createExpression(0));
    }
    if(tokens[current_index].type != SEMICOLON){
        ERR("Missing semicolon for return statement");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    (*returnStatement).createSemicolon();
    children.push_back(returnStatement);
    LOG(returnStatement->token.value);
}


// creates a while statement for the parse tree
void ParseNode::createWhileStatement() {
    ParseNode* whileStatement = new ParseNode{ tokens[current_index] };
    current_index++;
    if(tokens[current_index].type != OPEN_PAREN){
        ERR("Missing Open Parentheses in while loop");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    current_index++;
    if(!tokens[current_index].isValue()){
        ERR("Missing Value within while loop");
        exit(EXIT_FAILURE);
    }
    whileStatement->children.push_back(createExpression(0));
    if(tokens[current_index].type != CLOSE_PAREN){
        ERR("Missing Close Parentheses in while loop");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    current_index++;
    if (tokens[current_index].type == OPEN_BLOCK) {
        (*whileStatement).createBlock();
    }else if(tokens[current_index].type == SEMICOLON){
        (*whileStatement).createSemicolon();
    }else{
        ERR("Incorrect usage of while loop");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    children.push_back(whileStatement);
}

// creates a for loop statement for the parse tree.
void ParseNode::createForStatement() {
    ParseNode* forStatement = new ParseNode{ tokens[current_index] };
    current_index++;
    if (!tokens[current_index].type == OPEN_PAREN) {
        ERR("No Parentheses for For Loop Statement");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }

    (*forStatement).createForParameters();
    if (!tokens[current_index].type == OPEN_BLOCK) {
        ERR("No Open Bracket after For Loop Statement");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }

    if(!tokens[current_index].type == CLOSE_BLOCK){
        ERR("No Closing Bracket after For Loop Statement");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    (*forStatement).createBlock();

    children.push_back(forStatement);
}

// creates the for loop parameters for the parse tree.
void ParseNode::createForParameters() {
    ParseNode* forParameters = new ParseNode{ tokens[current_index] };
    current_index++;
    if (!tokens[current_index].isDeclaration()) {
        ERR("Missing declaration in for loop");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    (*forParameters).createDeclaration();
    LOG("DECLARATION");
    LOG(tokens[current_index].value);
    if(!tokens[current_index].isValue()){
        ERR("Missing Value within for loop declaration parameter");
        exit(EXIT_FAILURE);
    }
    forParameters->children.push_back(createExpression(0));
    LOG("EXPRESSION");
    if (tokens[current_index].type != SEMICOLON) {
        LOG(tokens[current_index].value);
        ERR("Missing Semicolon within For loop");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    (*forParameters->children[1]).createSemicolon();
    LOG("SEMICOLON");

    if (tokens[current_index].type != IDENT) {
        ERR("Missing Identifier in For Loop assignment");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    ParseNode* identifierAssignment = new ParseNode{ tokens[current_index] };
    forParameters->children.push_back(identifierAssignment);
    LOG("IDENTIFIER");
    current_index++;
    if(!tokens[current_index].isAssignment()){
        ERR("Missing Assignment operator in For Loop Parameters");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    (*forParameters).createAssign();
    current_index++;
    LOG("ASSIGNMENT");
    children.push_back(forParameters);
}

// creates an if statement for the parse tree.
void ParseNode::createIfStatement(){
    ParseNode *ifStatement = new ParseNode{tokens[current_index]};
    LOG(ifStatement->token.value);
    current_index++;
    if(tokens[current_index].type == OPEN_PAREN){
        current_index++;
        if(!tokens[current_index].isValue()){
            ERR("Missing Value within if statement");
            exit(EXIT_FAILURE);
        }
        ifStatement->children.push_back(createExpression(0));
    }else{
        ERR("If statement doesn't have Openning Parentheses");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    if (tokens[current_index].type == CLOSE_PAREN) {
        current_index++;
        (*ifStatement).createBlock();
    }else{
        ERR("Mismatched () in if Statement");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    if (tokens[current_index].type == ELSE) {
        (*ifStatement).createElseStatement();
    }
    children.push_back(ifStatement);
}

//create an else statement for the parse tree.
void ParseNode::createElseStatement() {
    ParseNode* elseStatement = new ParseNode{ tokens[current_index] };
    current_index++;

    if (tokens[current_index].type == IF) {
        (*elseStatement).createIfStatement();
    }
    else if (tokens[current_index].type == OPEN_BLOCK) {
        (*elseStatement).createBlock();
    }else{
        ERR("Improper usage of Else statement");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    children.push_back(elseStatement);
}

// creates the start of a declaration for the parse tree
void ParseNode::createDeclaration(){
    ParseNode *declaration = new ParseNode{tokens[current_index]};
    LOG(declaration->token.value);
    current_index++;
    if(tokens[current_index].type != IDENT){
        ERR("Missing Identifier after declaration");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    ParseNode *ident = new ParseNode{tokens[current_index]};
    current_index++;
    declaration->children.push_back(ident);
    if(tokens[current_index].isAssignment()){
        LOG("ASSIGNMENT");
        LOG(tokens[current_index].isAssignment());
        LOG(tokens[current_index].type);
        LOG(current_index);
        (*ident).createAssign();
        LOG(tokens[current_index].value);
        (*ident).createSemicolon();
    }
    else if (tokens[current_index].type == OPEN_PAREN) {
        (*ident).createFunction();
        if(tokens[current_index].type == CLOSE_PAREN){
            (*ident).createSemicolon();
            current_index++;
        }
    }else{
        ERR("Improper Declaration. Was looking for function or variable declaration");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    children.push_back(declaration);
    LOG(declaration->token.value);
}

//creates an identifier for a parse tree
void ParseNode::createIdentifier(){
    ParseNode* identifier = new ParseNode{tokens[current_index]};
    current_index++;
    if(tokens[current_index].isAssignment()){
        (*identifier).createAssign();
    }else if(tokens[current_index].type == OPEN_PAREN){
        current_index++;
        (*identifier).createFunctionUsage();
    }else{
        ERR("Incorrect usage of identifier");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    (*identifier).createSemicolon();
    children.push_back(identifier);
}

// creates an assignment for the parse tree
void ParseNode::createAssign(){
    ParseNode* assignment = new ParseNode{tokens[current_index]};
    LOG(assignment->token.value);
    current_index++;
    if(tokens[current_index].isValue()){
        assignment->children.push_back(createExpression(0));
    }
    children.push_back(assignment);
    LOG(assignment->token.value);
}

// creates the start of a function for the parse tree
void ParseNode::createFunction() {
    ParseNode* functionParameters = new ParseNode{ tokens[current_index] };
    current_index++;

    if (tokens[current_index].isDeclaration()) {
        (*functionParameters).createFunctionDeclaration();
        children.push_back(functionParameters);
        LOG(tokens[current_index].value);
        if (tokens[current_index].type == OPEN_BLOCK) {
            ParseNode* functionBlock = new ParseNode{ tokens[current_index] };
            (*functionBlock).createBlock();
            children.push_back(functionBlock);
        }else if(tokens[current_index].type == SEMICOLON){
            createSemicolon();
        }else{
            ERR("Improper function usage");
            ERR("Missing either semicolon or open bracket");
            exit(EXIT_FAILURE);
        }
    }
    else if (tokens[current_index].isValue()) {
        (*functionParameters).createFunctionUsage();
        children.push_back(functionParameters);
    }else{
        ERR("Improper usage of function");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
}

// creates a function usage for a parse tree.
void ParseNode::createFunctionUsage() {
    while (tokens[current_index].type != CLOSE_PAREN) {
        if (!tokens[current_index].isValue()) {
            ERR("Incorrect value within Function Usage parameter");
            ERR("Found this instead: "<<tokens[current_index].value);
            exit(EXIT_FAILURE);
        }
        ParseNode* parameter = new ParseNode{ tokens[current_index] };
        children.push_back(parameter);
        current_index++;
        if (tokens[current_index].type == COMMA) {
            current_index++;
        }
        if(current_index>=tokens.size()){
            ERR("Found end of file instead of closing parentheses");
            exit(EXIT_FAILURE);
        }
    }

    current_index++;
    LOG(current_index);
}

//creats a function declaration for a parse tree.
void ParseNode::createFunctionDeclaration() {
    while (tokens[current_index].type != CLOSE_PAREN) {
        if (!tokens[current_index].isDeclaration()) {
            ERR("Incorrect Typing within Function Declaration parameter");
            ERR("Found this instead: "<<tokens[current_index].value);
            exit(EXIT_FAILURE);
        }

        ParseNode* parameterTyping = new ParseNode{ tokens[current_index] };
        current_index++;
        if (!tokens[current_index].isValue()) {
            ERR("Incorrect value within FunctionDeclaration parameter");
            ERR("Found this instead: "<<tokens[current_index].value);
            exit(EXIT_FAILURE);
        }

        ParseNode* parameter = new ParseNode{ tokens[current_index] };
        parameterTyping->children.push_back(parameter);
        children.push_back(parameterTyping);
        current_index++;
        if (tokens[current_index].type == COMMA) {
            current_index++;
        }

        if (current_index>=tokens.size()) {
            ERR("Found end of file instead of closing parentheses");
            exit(EXIT_FAILURE);
        }
    }
    current_index++;
}

// creates a semicolon for the parse tree.
void ParseNode::createSemicolon(){
    LOG(current_index);
    if(tokens[current_index].type==TokenType::SEMICOLON){
        ParseNode * semicolon = new ParseNode{tokens[current_index]};
        current_index++;
        LOG(semicolon->token.value);
        children.push_back(semicolon);
    }else{
        ERR("No semicolon found");
        ERR("Found this instead: "<<tokens[current_index].value);
        exit(EXIT_FAILURE);
    }
    LOG("Semicolon");
}

// creates a binary node (value) for an expression in a parse tree.
ParseNode* ParseNode::makeBinaryNode(ParseNode* left, Token oper, ParseNode* right){
    ParseNode * binaryNode = new ParseNode{oper, {}};
    binaryNode->children.push_back(left);
    binaryNode->children.push_back(right);
    return binaryNode;
}

// called when expression continues left
ParseNode* ParseNode::leftDenotation(ParseNode* left, ParseNode* next){
    ParseNode* binaryOp;
    if(next->token.getAssociativity()){
        LOG("Right Associativity "<<current_index);
        binaryOp = makeBinaryNode(left, next->token, createExpression(next->token.getPrecedence()));
    }else{
        LOG("Left Associativity "<<current_index);
        binaryOp = makeBinaryNode(left, next->token, createExpression(next->token.getPrecedence()-1));
    }
    return binaryOp;
}

// called to create an expression
ParseNode* ParseNode::createExpression(int minPrec){
    ParseNode *first = new ParseNode{tokens[current_index]};
    LOG("not while "<<current_index);
    current_index++;
    ParseNode *left = first;

    while(tokens[current_index].getPrecedence()>minPrec){
        ParseNode *next = new ParseNode{tokens[current_index]};
        LOG("in while "<<current_index);
        current_index++;
        left = leftDenotation(left, next);
    }
    return left;
}

// prints the entire parse tree
void ParseNode::printParseTree(ParseNode *node, std::ofstream &output, int indent){
    for(int i = 0; i<indent;i++){
        output<<"----";
    }
    output<<node->token.value<<std::endl;
    for(int i = 0; i<node->children.size();i++){
        printParseTree(node->children[i], output, indent+1);
    }
}

//reads the entire file. Used for debugging.
internal void readFile(std::ifstream &file){
    std::string line;
    while(std::getline(file,line)){
        LOG(line);
    }
}

int main(int argc, char* argv[]){
    //Checks for correct usage of command arguments. Makes sure there is an input file.
    if(argc!= 3){
        ERR("INCORRECT USAGE");
        ERR("Please put your input file and output file into the command line arguments.");
        ERR("Correct usage: input.txt output.txt");
        return EXIT_FAILURE;
    }

    LOG(argv[1]);

    //Set up input and output files based on arguments.
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);

    //Ask if user wants help with syntax
    while(1){
        std::cout<<"Does the user need help with syntax? (Y/N)"<<std::endl;
        std::string decision;
        std::cin >> decision;
        if(decision == "Y"){
            std::cout<<"Okay, showing syntax guide"<<std::endl;
            std::cout<<"Every Program needs to be encased within brackets {}.\n";
            std::cout<<"You can put statements within the brackets.\n";
            std::cout<<"These statements include: return, if, for, while, function declarations, function usage, variable declaration and variable usage.\n";
            std::cout<<"The syntax is relatively similar to C++ with typing going first, then identifier when declaring something.\n";
            std::cout<<"However, there are some new int types as I didn't like how c++ dealt with integers.\n";
            std::cout<<"Instead of using int, you have to use one of the following: uint8, uint16, uint32, uint64, int8, int16, int32, int64\n";
            std::cout<<"Other than that, there are: double, float, bool, and string\n";
            std::cout<<"For operations there are the classical c++ operators and an extra exponent operator: ^^\n";
            std::cout<<"That's pretty much it, happy parsing!\n"<<std::endl;
        }else if(decision == "N"){
            std::cout<<"Okay, processing parse tree"<<std::endl;
            break;
        }else{
            std::cout<<"Invalid decision, please try again"<<std::endl;
        }
    }


    //Turns the file string into a vector of tokens.
    ParseNode::tokens = Token::getTokens(input);

    for(int i = 0; i<ParseNode::tokens.size();i++){
        LOG(ParseNode::tokens[i].value);
    }

    //Creates the Parse Tree
    ParseNode* parse_tree = new ParseNode{Token{TokenType::TREE, "Tree"}};
    ParseNode::current_index = 0;
    (*parse_tree).createTree();

    LOG("\n\n\n");

    //prints the parse tree
    ParseNode::printParseTree(parse_tree, output, 0);

    //Closes files after use
    input.close();
    output.close();

    return EXIT_SUCCESS;
}