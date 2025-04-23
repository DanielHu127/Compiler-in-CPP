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
#define DEBUG 1
#if DEBUG
#define LOG(x) std::cout<<x<<std::endl
#else
#define LOG(x) 
#endif

#define ERR(x) std::cerr<<x<<std::endl

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
    STATEMENTLIST,
};

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
        }else if(std::isspace(ch)){
            while(std::isspace(file.peek())){
                file.get(ch);
            }
            buffer.str("");
            buffer.clear();
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
                }
            }else if(ch=='<' && file.peek()== '#'){
                while(ch != '#' || file.peek()!='>'){
                    file.get(ch);
                }
                file.get(ch);
            }else if(ch=='\"'){
                buffer<<ch;
                while(file.peek()!='\"'){
                    file.get(ch);
                    buffer<<ch;
                }
                file.get(ch);
                buffer<<ch;
                Tokens.push_back(Token{TokenType::STRING_LIT, buffer.str()});
            }else{
                LOG(ch);
                std::cerr<<"Special Character Error"<<std::endl;
                exit(EXIT_FAILURE);
            }
            buffer.str("");
            buffer.clear();
        }
    }
    return Tokens;
}

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
        std::cerr<<"invalid operator precedence calculation: "<<type<<std::endl;
        exit(EXIT_FAILURE);
    }
    LOG(type);
    return precedence;
}

bool Token::isAssignment() const{
    return( type == ASSIGN || type == ADD_ASSIGN || type == SUB_ASSIGN || 
            type == MUL_ASSIGN || type == DIV_ASSIGN || type == MOD_ASSIGN || 
            type == EXP_ASSIGN || type == LSHF_ASSIGN || type == RSHF_ASSIGN || 
            type == BAND_ASSIGN || type == BXOR_ASSIGN || type == BOR_ASSIGN);
}

bool Token::isValue() const{
    return(type == IDENT || type == INT_LIT || type == BOOL_LIT || type == DOUBLE_LIT || type == FLOAT_LIT || type == STRING_LIT);
}

bool Token::isDeclaration() const{
    return(type == U_INT8_DECL || type == U_INT16_DECL || type == U_INT32_DECL || type == U_INT64_DECL ||
        type == S_INT8_DECL || type == S_INT16_DECL || type == S_INT32_DECL || type == S_INT64_DECL ||
        type == BOOL_DECL || type == STRING_DECL || type == FLOAT_DECL || type == DOUBLE_DECL);
}

//true is left->right, false is right->left
bool Token::getAssociativity() const{
    return type == EXPONENT;
}

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

    static void printParseTree(ParseNode* node, int indent);

    static ParseNode* makeBinaryNode(ParseNode* left, Token oper, ParseNode* right);
    static ParseNode* createExpression(int minPrec);
    static ParseNode* leftDenotation(ParseNode* left, ParseNode* next);
};

int ParseNode::current_index;
std::vector<Token> ParseNode::tokens;

void ParseNode::createTree(){
    while(current_index<tokens.size()){
        if(tokens[current_index].type == TokenType::OPEN_BLOCK){
            createBlock();
            LOG(token.value);
            LOG(children[0]->token.value);
            LOG(children[0]->token.value);
        }else{
            ERR("No Open Block Found at start of program");
            exit(EXIT_FAILURE);
        }
    }
    LOG(token.value);
}

void ParseNode::createBlock(){
    ParseNode * block = new ParseNode{Token{TokenType::BLOCK, "Block"}};
    LOG(block->token.value);
    current_index++;
    (*block).createStatementList();
    current_index++;
    children.push_back(block);
    LOG(block->token.value);
}

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
            LOG("bruh");
            (*statementList).createDeclaration();
        }else if (currentToken == FOR) {
            (*statementList).createForStatement();
            LOG(tokens[current_index].value);
            LOG(current_index);
        }else if (currentToken == WHILE) {
            (*statementList).createWhileStatement();
        }else if (currentToken == IDENT) {
            (*statementList).createIdentifier();
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            LOG(tokens[current_index].value);
            LOG(current_index);
            std::cerr<<"Not valid statement"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    children.push_back(statementList);
    LOG(statementList->token.value);
}

void ParseNode::createReturnStatement(){
    ParseNode *returnStatement = new ParseNode{tokens[current_index]};
    LOG(returnStatement->token.value);
    current_index++;
    returnStatement->children.push_back(createExpression(0));
    (*returnStatement).createSemicolon();
    children.push_back(returnStatement);
    LOG(returnStatement->token.value);
}

void ParseNode::createWhileStatement() {
    ParseNode* whileStatement = new ParseNode{ tokens[current_index] };
    current_index++;
    if(tokens[current_index].type != OPEN_PAREN){
        std::cerr<<"Missing Open Parentheses in while loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    current_index++;
    whileStatement->children.push_back(createExpression(0));
    if(tokens[current_index].type != CLOSE_PAREN){
        std::cerr<<"Missing Close Parentheses in while loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    current_index++;
    if (tokens[current_index].type == OPEN_BLOCK) {
        (*whileStatement).createBlock();
    }else if(tokens[current_index].type == SEMICOLON){
        (*whileStatement).createSemicolon();
    }else{
        std::cerr<<"Incorrect usage of while loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    children.push_back(whileStatement);
}

void ParseNode::createForStatement() {
    ParseNode* forStatement = new ParseNode{ tokens[current_index] };
    current_index++;
    if (!tokens[current_index].type == OPEN_PAREN) {
        std::cerr << "No Parentheses for For Loop Statement" << std::endl;
        exit(EXIT_FAILURE);
    }
    LOG("HEYY");
    (*forStatement).createForParameters();
    if (!tokens[current_index].type == OPEN_BLOCK) {
        std::cerr << "No Block Code after For Loop Statement" << std::endl;
        exit(EXIT_FAILURE);
    }
    LOG("BLOCK TIME");
    LOG(tokens[current_index].value);
    LOG(current_index);
    if(!tokens[current_index].type == CLOSE_PAREN){
        std::cerr << "No Block Code after For Loop Statement" << std::endl;
        exit(EXIT_FAILURE);
    }
    (*forStatement).createBlock();
    LOG("BLOCK TIME");
    children.push_back(forStatement);
}

void ParseNode::createForParameters() {
    ParseNode* forParameters = new ParseNode{ tokens[current_index] };
    current_index++;
    if (!tokens[current_index].isDeclaration()) {
        std::cerr<<"Missing declaration in for loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    (*forParameters).createDeclaration();
    LOG("DECLARATION");
    LOG(tokens[current_index].value);
    forParameters->children.push_back(createExpression(0));
    LOG("EXPRESSION");
    if (tokens[current_index].type != SEMICOLON) {
        LOG(tokens[current_index].value);
        std::cerr << "Missing Semicolon within For loop" << std::endl;
        exit(EXIT_FAILURE);
    }
    (*forParameters->children[1]).createSemicolon();
    LOG("SEMICOLON");

    if (tokens[current_index].type != IDENT) {
        std::cerr << "Missing Identifier in For Loop assignment" << std::endl;
        exit(EXIT_FAILURE);
    }
    ParseNode* identifierAssignment = new ParseNode{ tokens[current_index] };
    forParameters->children.push_back(identifierAssignment);
    LOG("IDENTIFIER");
    current_index++;
    (*forParameters).createAssign();
    current_index++;
    LOG("ASSIGNMENT");
    children.push_back(forParameters);
}

void ParseNode::createIfStatement(){
    ParseNode *ifStatement = new ParseNode{tokens[current_index]};
    LOG(ifStatement->token.value);
    current_index++;
    if(tokens[current_index].type == OPEN_PAREN){
        current_index++;
        ifStatement->children.push_back(createExpression(0));
    }else{
        std::cerr<<"If statement doesn't have ()"<<std::endl;
        exit(EXIT_FAILURE);
    }
    if (tokens[current_index].type == CLOSE_PAREN) {
        current_index++;
        (*ifStatement).createBlock();
    }else{
        std::cerr << "Mismatched () in ifStatement" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (tokens[current_index].type == ELSE) {
        (*ifStatement).createElseStatement();
    }
    children.push_back(ifStatement);
}

void ParseNode::createElseStatement() {
    ParseNode* elseStatement = new ParseNode{ tokens[current_index] };
    current_index++;

    if (tokens[current_index].type == IF) {
        (*elseStatement).createIfStatement();
    }
    else if (tokens[current_index].type == OPEN_BLOCK) {
        (*elseStatement).createBlock();
    }
    children.push_back(elseStatement);
}

void ParseNode::createDeclaration(){
    ParseNode *declaration = new ParseNode{tokens[current_index]};
    LOG(declaration->token.value);
    current_index++;
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
    }
    children.push_back(declaration);
    LOG(declaration->token.value);
}

void ParseNode::createIdentifier(){
    ParseNode* identifier = new ParseNode{tokens[current_index]};
    current_index++;
    if(tokens[current_index].isAssignment()){
        (*identifier).createAssign();
    }else if(tokens[current_index].type == OPEN_PAREN){
        current_index++;
        (*identifier).createFunctionUsage();
    }else{
        std::cerr<<"Incorrect usage of identifier"<<std::endl;
        exit(EXIT_FAILURE);
    }
    (*identifier).createSemicolon();
    children.push_back(identifier);
}

void ParseNode::createAssign(){
    ParseNode* assignment = new ParseNode{tokens[current_index]};
    LOG(assignment->token.value);
    current_index++;
    assignment->children.push_back(createExpression(0));
    children.push_back(assignment);
    LOG(assignment->token.value);
}

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
        }
    }
    else if (tokens[current_index].isValue()) {
        (*functionParameters).createFunctionUsage();
        children.push_back(functionParameters);
    }
}

void ParseNode::createFunctionUsage() {
    while (tokens[current_index].type != CLOSE_PAREN) {
        if (!tokens[current_index].isValue()) {
            std::cerr << "Incorrect value within FunctionUsage parameter" << std::endl;
            exit(EXIT_FAILURE);
        }
        ParseNode* parameter = new ParseNode{ tokens[current_index] };
        children.push_back(parameter);
        current_index++;
        if (tokens[current_index].type == COMMA) {
            current_index++;
        }
    }
    current_index++;
    LOG(current_index);
}

void ParseNode::createFunctionDeclaration() {
    while (tokens[current_index].type != CLOSE_PAREN) {
        if (!tokens[current_index].isDeclaration()) {
            std::cerr << "Incorrect Typing within FunctionDeclaration parameter" << std::endl;
            exit(EXIT_FAILURE);
        }

        ParseNode* parameterTyping = new ParseNode{ tokens[current_index] };
        current_index++;
        if (!tokens[current_index].isValue()) {
            std::cerr << "Incorrect value within FunctionDeclaration parameter" << std::endl;
            exit(EXIT_FAILURE);
        }

        ParseNode* parameter = new ParseNode{ tokens[current_index] };
        parameterTyping->children.push_back(parameter);
        children.push_back(parameterTyping);
        current_index++;
        if (tokens[current_index].type == COMMA) {
            current_index++;
        }
    }
    current_index++;
}

void ParseNode::createSemicolon(){
    LOG(current_index);
    if(tokens[current_index].type==TokenType::SEMICOLON){
        ParseNode * semicolon = new ParseNode{tokens[current_index]};
        current_index++;
        LOG(semicolon->token.value);
        children.push_back(semicolon);
    }else{
        LOG(tokens[current_index].value);
        std::cerr<<"No semicolon found"<<std::endl;
        exit(EXIT_FAILURE);
    }
    LOG("Semicolon");
}



ParseNode* ParseNode::makeBinaryNode(ParseNode* left, Token oper, ParseNode* right){
    ParseNode * binaryNode = new ParseNode{oper, {}};
    binaryNode->children.push_back(left);
    binaryNode->children.push_back(right);
    //LOG(binaryNode.token.value);
    return binaryNode;
}

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

ParseNode* ParseNode::createExpression(int minPrec){
    ParseNode *first = new ParseNode{tokens[current_index]};
    LOG("not while "<<current_index);
    current_index++;
    ParseNode *left = first;

    if(current_index==24){
        LOG("23 index value: "<<tokens[current_index].value);
    }

    while(tokens[current_index].getPrecedence()>minPrec){
        ParseNode *next = new ParseNode{tokens[current_index]};
        LOG("in while "<<current_index);
        current_index++;
        left = leftDenotation(left, next);
    }
    return left;
}

void ParseNode::printParseTree(ParseNode *node, int indent){
    for(int i = 0; i<indent;i++){
        std::cout<<"----";
    }
    std::cout<<node->token.value<<std::endl;
    for(int i = 0; i<node->children.size();i++){
        printParseTree(node->children[i], indent+1);
    }
}

internal void readFile(std::ifstream &file){
    std::string line;
    while(std::getline(file,line)){
        LOG(line);
    }
}

int main(int argc, char* argv[]){
    //Checks for correct usage of command arguments. Makes sure there is an input file.
    if(argc!= 2){
        ERR("INCORRECT USAGE");
        ERR("Please put your input file into the command line arguments.");
        return EXIT_FAILURE;
    }

    LOG(argv[1]);

    std::ifstream file(argv[1]);

    //Turns the file string into a vector of tokens.
    ParseNode::tokens = Token::getTokens(file);

    for(int i = 0; i<ParseNode::tokens.size();i++){
        LOG(ParseNode::tokens[i].value);
    }

    //Creates the Parse Tree
    ParseNode* parse_tree = new ParseNode{Token{TokenType::TREE, "Tree"}};
    ParseNode::current_index = 0;
    (*parse_tree).createTree();

    LOG("\n\n\n");

    //prints the parse tree
    ParseNode::printParseTree(parse_tree, 0);

    //Closes file after use
    file.close();

    return EXIT_SUCCESS;
}