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
 + Variables and Identifiers
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
#include <queue>
#include <set>
#include <iomanip>

#define internal static
#define global static
#define local_persist static
#define DEBUG 0
#if DEBUG
#define LOG(x) std::cout<<x<<std::endl
#else
#define LOG(x) 
#endif

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

struct Token{
    TokenType type;
    std::string value = "";
};

struct ParseNode{
    Token token;
    std::vector<ParseNode*> children = {};
};

std::vector<std::string> identifiers = {};

internal void createBlock(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createIfStatement(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);

internal void consumeTokens(std::vector<Token> &Tokens, int left, int right){
    for(int i = right; i>=left;i--){
        if (i >= 0 && i < Tokens.size()) {
            Tokens.erase(Tokens.begin() + i);
        } else {
            LOG("Index out of bounds");
        }
    }
}

std::vector<Token> getTokens(std::ifstream &file){
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
            }else{
                Tokens.push_back(Token{TokenType::IDENT, buffer.str()});
            }
            buffer.str("");
            buffer.clear();
        }else if(std::isdigit(ch)){
            while(std::isdigit(file.peek())){
                file.get(ch);
                buffer << ch;
            }
            Tokens.push_back(Token{TokenType::INT_LIT, buffer.str()});
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

internal void readFile(std::ifstream &file){
    std::string line;
    while(std::getline(file,line)){
        LOG(line);
    }
}

internal void createSemicolon(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    LOG(currentIndex);
    if(Tokens[currentIndex].type==TokenType::SEMICOLON){
        ParseNode * semicolon = new ParseNode{Tokens[currentIndex]};
        currentIndex++;
        LOG(semicolon->token.value);
        Node.children.push_back(semicolon);
    }else{
        LOG(Tokens[currentIndex].value);
        std::cerr<<"No semicolon found"<<std::endl;
        exit(EXIT_FAILURE);
    }
    LOG("Semicolon");
}

//true is left->right, false is right->left
internal bool getAssociativity(TokenType t){
    if(t == EXPONENT){
        return false;
    }else{
        return true;
    }
}

internal int getPrecedence(TokenType t){
    int precedence = -1000;
    if(t == EXPONENT){precedence = 11;}
    if(t == MULTIPLY || t == DIVIDE || t == MOD){precedence = 10;}
    if(t == ADD || t == SUB){precedence = 9;}
    if(t == L_SHIFT || t == R_SHIFT){precedence = 8;}
    if(t == LESS || t == LESS_EQUAL || t == GREATER || t == GREATER_EQUAL){precedence = 7;}
    if(t == IS_EQUAL || t == NOT_EQUAL){precedence = 6;}
    if(t == BIT_AND){precedence = 5;}
    if(t == BIT_XOR){precedence = 4;}
    if(t == BIT_OR){precedence = 3;}
    if(t == LOGICAL_AND){precedence = 2;}
    if(t == LOGICAL_OR){precedence = 1;}
    if(t == IDENT || t == OPEN_PAREN || t == INT_LIT || t == BOOL_LIT || t == STRING_LIT || t == DOUBLE_LIT){precedence = 0; LOG("BOO");}
    if(t == SEMICOLON || t == CLOSE_BLOCK || t == CLOSE_PAREN || t == COMMA){precedence = -1;}
    if (precedence==-1000){
        std::cerr<<"invalid operator precedence calculation: "<<t<<std::endl;
        exit(EXIT_FAILURE);
    }
    LOG(t);
    return precedence;
}

internal ParseNode* makeBinaryNode(ParseNode* left, Token oper, ParseNode* right){
    ParseNode * binaryNode = new ParseNode{oper, {}};
    binaryNode->children.push_back(left);
    binaryNode->children.push_back(right);
    //LOG(binaryNode.token.value);
    return binaryNode;
}

internal ParseNode* createExpression(int minPrec, std::vector<Token> &Tokens, int &index);

internal ParseNode* leftDenotation(ParseNode* left, ParseNode* next, std::vector<Token> &Tokens, int &index){
    ParseNode* binaryOp;
    if(getAssociativity(next->token.type)){
        LOG("Right Associativity "<<index);
        binaryOp = makeBinaryNode(left, next->token, createExpression(getPrecedence(next->token.type), Tokens, index));
    }else{
        LOG("Left Associativity "<<index);
        binaryOp = makeBinaryNode(left, next->token, createExpression(getPrecedence(next->token.type)-1, Tokens, index));
    }
    return binaryOp;
}

internal ParseNode* createExpression(int minPrec, std::vector<Token> &Tokens, int &index){
    ParseNode *first = new ParseNode{Tokens[index]};
    LOG("not while "<<index);
    index++;
    ParseNode *left = first;

    if(index==24){
        LOG("23 index value: "<<Tokens[index].value);
    }

    while(getPrecedence(Tokens[index].type)>minPrec){
        ParseNode *next = new ParseNode{Tokens[index]};
        LOG("in while "<<index);
        index++;
        left = leftDenotation(left, next, Tokens, index);
    }
    return left;
}

internal void createAssign(std::vector<Token> &Tokens, ParseNode &Node, int& currentIndex){
    ParseNode* assignment = new ParseNode{Tokens[currentIndex]};
    LOG(assignment->token.value);
    currentIndex++;
    assignment->children.push_back(createExpression(0, Tokens, currentIndex));
    Node.children.push_back(assignment);
    LOG(assignment->token.value);
}

internal bool isAssignment(TokenType t){
    return( t == ASSIGN || t == ADD_ASSIGN || t == SUB_ASSIGN || 
            t == MUL_ASSIGN || t == DIV_ASSIGN || t == MOD_ASSIGN || 
            t == EXP_ASSIGN || t == LSHF_ASSIGN || t == RSHF_ASSIGN || 
            t == BAND_ASSIGN || t == BXOR_ASSIGN || t == BOR_ASSIGN);
}

internal bool isValue(TokenType t) {
    return(t == IDENT || t == INT_LIT || t == BOOL_LIT || t == DOUBLE_LIT || t == FLOAT_LIT || t == STRING_LIT);
}

internal bool isDeclaration(TokenType t) {
    return(t == U_INT8_DECL || t == U_INT16_DECL || t == U_INT32_DECL || t == U_INT64_DECL ||
        t == S_INT8_DECL || t == S_INT16_DECL || t == S_INT32_DECL || t == S_INT64_DECL ||
        t == BOOL_DECL || t == STRING_DECL || t == FLOAT_DECL || t == DOUBLE_DECL);
}

internal void createFunctionUsage(std::vector<Token>& Tokens, ParseNode& Node, int& currentIndex) {
    while (Tokens[currentIndex].type != CLOSE_PAREN) {
        if (!isValue(Tokens[currentIndex].type)) {
            std::cerr << "Incorrect value within FunctionUsage parameter" << std::endl;
            exit(EXIT_FAILURE);
        }
        ParseNode* parameter = new ParseNode{ Tokens[currentIndex] };
        Node.children.push_back(parameter);
        currentIndex++;
        if (Tokens[currentIndex].type == COMMA) {
            currentIndex++;
        }
    }
    currentIndex++;
    LOG(currentIndex);
}

internal void createFunctionDeclaration(std::vector<Token>& Tokens, ParseNode& Node, int& currentIndex) {
    while (Tokens[currentIndex].type != CLOSE_PAREN) {
        if (!isDeclaration(Tokens[currentIndex].type)) {
            std::cerr << "Incorrect Typing within FunctionDeclaration parameter" << std::endl;
            exit(EXIT_FAILURE);
        }

        ParseNode* parameterTyping = new ParseNode{ Tokens[currentIndex] };
        currentIndex++;
        if (!isValue(Tokens[currentIndex].type)) {
            std::cerr << "Incorrect value within FunctionDeclaration parameter" << std::endl;
            exit(EXIT_FAILURE);
        }

        ParseNode* parameter = new ParseNode{ Tokens[currentIndex] };
        parameterTyping->children.push_back(parameter);
        Node.children.push_back(parameterTyping);
        currentIndex++;
        if (Tokens[currentIndex].type == COMMA) {
            currentIndex++;
        }
    }
    currentIndex++;
}

internal void createFunction(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex) {
    ParseNode* functionParameters = new ParseNode{ Tokens[currentIndex] };
    currentIndex++;

    if (isDeclaration(Tokens[currentIndex].type)) {
        LOG("BRUHHHHHHIUDHSFBSDBVSD");
        createFunctionDeclaration(Tokens, *functionParameters, currentIndex);
        Node.children.push_back(functionParameters);
        LOG(Tokens[currentIndex].value);
        if (Tokens[currentIndex].type == OPEN_BLOCK) {
            LOG("IUDAHHFOIUDA");
            ParseNode* functionBlock = new ParseNode{ Tokens[currentIndex] };
            createBlock(Tokens, *functionBlock, currentIndex);
            Node.children.push_back(functionBlock);
        }
    }
    else if (isValue(Tokens[currentIndex].type)) {
        createFunctionUsage(Tokens, *functionParameters, currentIndex);
        Node.children.push_back(functionParameters);
    }
}

internal void createDeclaration(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *declaration = new ParseNode{Tokens[currentIndex]};
    LOG(declaration->token.value);
    currentIndex++;
    ParseNode *ident = new ParseNode{Tokens[currentIndex]};
    currentIndex++;
    declaration->children.push_back(ident);
    LOG("HEYYYGSIUYGAD");
    if(isAssignment(Tokens[currentIndex].type)){
        LOG("ASSIGNMENT");
        LOG(isAssignment(Tokens[currentIndex].type));
        LOG(Tokens[currentIndex].type);
        LOG(currentIndex);
        createAssign(Tokens, *ident, currentIndex);
        LOG(Tokens[currentIndex].value);
        createSemicolon(Tokens, *ident, currentIndex);
    }
    else if (Tokens[currentIndex].type == OPEN_PAREN) {
        createFunction(Tokens, *ident, currentIndex);
        if(Tokens[currentIndex].type == CLOSE_PAREN){
            createSemicolon(Tokens, *ident, currentIndex);
            currentIndex++;
        }
    }
    Node.children.push_back(declaration);
    LOG(declaration->token.value);
}

internal void createReturnStatement(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *returnStatement = new ParseNode{Tokens[currentIndex]};
    LOG(returnStatement->token.value);
    currentIndex++;
    returnStatement->children.push_back(createExpression(0, Tokens, currentIndex));
    LOG("excuse me why");
    createSemicolon(Tokens, *returnStatement, currentIndex);
    Node.children.push_back(returnStatement);
    LOG(returnStatement->token.value);
}

internal void createWhileStatement(std::vector<Token>& Tokens, ParseNode& Node, int& currentIndex) {
    ParseNode* whileStatement = new ParseNode{ Tokens[currentIndex] };
    currentIndex++;
    if(Tokens[currentIndex].type != OPEN_PAREN){
        std::cerr<<"Missing Open Parentheses in while loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    currentIndex++;
    whileStatement->children.push_back(createExpression(0, Tokens, currentIndex));
    if(Tokens[currentIndex].type != CLOSE_PAREN){
        std::cerr<<"Missing Close Parentheses in while loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    currentIndex++;
    if (Tokens[currentIndex].type == OPEN_BLOCK) {
        createBlock(Tokens, *whileStatement, currentIndex);
    }else if(Tokens[currentIndex].type == SEMICOLON){
        createSemicolon(Tokens, *whileStatement, currentIndex);
    }else{
        std::cerr<<"Incorrect usage of while loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    Node.children.push_back(whileStatement);
}

internal void createForParameters(std::vector<Token>& Tokens, ParseNode& Node, int& currentIndex) {
    ParseNode* forParameters = new ParseNode{ Tokens[currentIndex] };
    currentIndex++;
    if (!isDeclaration(Tokens[currentIndex].type)) {
        std::cerr<<"Missing declaration in for loop"<<std::endl;
        exit(EXIT_FAILURE);
    }
    createDeclaration(Tokens, *forParameters, currentIndex);
    LOG("DECLARATION");
    LOG(Tokens[currentIndex].value);
    forParameters->children.push_back(createExpression(0, Tokens, currentIndex));
    LOG("EXPRESSION");
    if (Tokens[currentIndex].type != SEMICOLON) {
        LOG(Tokens[currentIndex].value);
        std::cerr << "Missing Semicolon within For loop" << std::endl;
        exit(EXIT_FAILURE);
    }
    createSemicolon(Tokens, *(forParameters->children[1]), currentIndex);
    LOG("SEMICOLON");

    if (Tokens[currentIndex].type != IDENT) {
        std::cerr << "Missing Identifier in For Loop assignment" << std::endl;
        exit(EXIT_FAILURE);
    }
    ParseNode* identifierAssignment = new ParseNode{ Tokens[currentIndex] };
    forParameters->children.push_back(identifierAssignment);
    LOG("IDENTIFIER");
    currentIndex++;
    createAssign(Tokens, *forParameters, currentIndex);
    currentIndex++;
    LOG("ASSIGNMENT");
    Node.children.push_back(forParameters);
}

internal void createForStatement(std::vector<Token>& Tokens, ParseNode& Node, int& currentIndex) {
    ParseNode* forStatement = new ParseNode{ Tokens[currentIndex] };
    currentIndex++;
    if (!Tokens[currentIndex].type == OPEN_PAREN) {
        std::cerr << "No Parentheses for For Loop Statement" << std::endl;
        exit(EXIT_FAILURE);
    }
    LOG("HEYY");
    createForParameters(Tokens, *forStatement, currentIndex);
    if (!Tokens[currentIndex].type == OPEN_BLOCK) {
        std::cerr << "No Block Code after For Loop Statement" << std::endl;
        exit(EXIT_FAILURE);
    }
    LOG("BLOCK TIME");
    LOG(Tokens[currentIndex].value);
    LOG(currentIndex);
    if(!Tokens[currentIndex].type == CLOSE_PAREN){
        std::cerr << "No Block Code after For Loop Statement" << std::endl;
        exit(EXIT_FAILURE);
    }
    createBlock(Tokens, *forStatement, currentIndex);
    LOG("BLOCK TIME");
    Node.children.push_back(forStatement);
}

internal void createElseStatement(std::vector<Token>& Tokens, ParseNode& Node, int& currentIndex) {
    ParseNode* elseStatement = new ParseNode{ Tokens[currentIndex] };
    currentIndex++;

    if (Tokens[currentIndex].type == IF) {
        createIfStatement(Tokens, *elseStatement, currentIndex);
    }
    else if (Tokens[currentIndex].type == OPEN_BLOCK) {
        createBlock(Tokens, *elseStatement, currentIndex);
    }
    Node.children.push_back(elseStatement);
}

internal void createIfStatement(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *ifStatement = new ParseNode{Tokens[currentIndex]};
    LOG(ifStatement->token.value);
    currentIndex++;
    if(Tokens[currentIndex].type == OPEN_PAREN){
        currentIndex++;
        ifStatement->children.push_back(createExpression(0, Tokens, currentIndex));
    }else{
        std::cerr<<"If statement doesn't have ()"<<std::endl;
        exit(EXIT_FAILURE);
    }
    if (Tokens[currentIndex].type == CLOSE_PAREN) {
        currentIndex++;
        createBlock(Tokens, *ifStatement, currentIndex);
    }else{
        std::cerr << "Mismatched () in ifStatement" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (Tokens[currentIndex].type == ELSE) {
        createElseStatement(Tokens, *ifStatement, currentIndex);
    }
    Node.children.push_back(ifStatement);
}

internal void createIdentifier(std::vector<Token> &Tokens, ParseNode &Node, int& currentIndex){
    ParseNode* identifier = new ParseNode{Tokens[currentIndex]};
    currentIndex++;
    if(isAssignment(Tokens[currentIndex].type)){
        createAssign(Tokens, *identifier, currentIndex);
    }else if(Tokens[currentIndex].type == OPEN_PAREN){
        currentIndex++;
        createFunctionUsage(Tokens, *identifier, currentIndex);
    }else{
        std::cerr<<"Incorrect usage of identifier"<<std::endl;
        exit(EXIT_FAILURE);
    }
    createSemicolon(Tokens, *identifier, currentIndex);
    Node.children.push_back(identifier);
}

internal void createStatementList(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *statementList = new ParseNode{Token{TokenType::STATEMENTLIST, "Statement List"}};
    LOG(statementList->token.value);
    while(Tokens[currentIndex].type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = Tokens[currentIndex].type;
        if(currentToken == TokenType::RETURN){
            createReturnStatement(Tokens, *statementList, currentIndex);
        }else if(currentToken == TokenType::IF){
            createIfStatement(Tokens, *statementList, currentIndex);
        }else if(currentToken == TokenType::OPEN_BLOCK){
            createBlock(Tokens, *statementList, currentIndex);
        }else if(isDeclaration(currentToken)){
            LOG("bruh");
            createDeclaration(Tokens, *statementList, currentIndex);
        }else if (currentToken == FOR) {
            createForStatement(Tokens, *statementList, currentIndex);
            LOG(Tokens[currentIndex].value);
            LOG(currentIndex);
        }else if (currentToken == WHILE) {
            createWhileStatement(Tokens, *statementList, currentIndex);
        }else if (currentToken == IDENT) {
            createIdentifier(Tokens, *statementList, currentIndex);
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            LOG(Tokens[currentIndex].value);
            LOG(currentIndex);
            std::cerr<<"Not valid statement"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    Node.children.push_back(statementList);
    LOG(statementList->token.value);
}

internal void createBlock(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode * block = new ParseNode{Token{TokenType::BLOCK, "Block"}};
    LOG(block->token.value);
    currentIndex++;
    createStatementList(Tokens, *block, currentIndex);
    currentIndex++;
    Node.children.push_back(block);
    LOG(block->token.value);
}

internal void createTree(std::vector<Token> &Tokens, ParseNode *tree, int &currentIndex){
    while(currentIndex<Tokens.size()){
        if(Tokens[currentIndex].type == TokenType::OPEN_BLOCK){
            createBlock(Tokens, *tree, currentIndex);
            LOG(tree->token.value);
            LOG(tree->children[0]->token.value);
            LOG(tree->children[0]->token.value);
        }
        else if(Tokens[currentIndex].type == TokenType::RETURN){
            //NOTE: FIX THIS
            //ParseNode returnStatement = createReturnStatement(Tokens);
            //tree.children.push_back(&returnStatement);
        }
    }
    LOG(tree->token.value);
}

internal void printParseTree(ParseNode *Node, int indent){
    for(int i = 0; i<indent;i++){
        std::cout<<"----";
    }
    std::cout<<Node->token.value<<std::endl;
    for(int i = 0; i<Node->children.size();i++){
        printParseTree(Node->children[i], indent+1);
    }
}

internal void deallocateParseTree(ParseNode *Node){
    for(ParseNode* child: Node->children){
        deallocateParseTree(child);
    }
    delete Node;
}

int main(int argc, char* argv[]){
    if(argc!= 2){
        std::cerr<<"INCORRECT USAGE"<<std::endl;
        std::cerr<<"Correct usage: Compiler <input.txt>"<<std::endl;
        return EXIT_FAILURE;
    }


    LOG(argv[1]);

    std::ifstream file(argv[1]);

    std::vector<Token> Tokens = getTokens(file);
    for(int i = 0; i<Tokens.size();i++){
        LOG(Tokens[i].value);
    }
    int currentIndex = 0;
    
    ParseNode* parseTree = new ParseNode{Token{TokenType::TREE, "Tree"}};
    createTree(Tokens, parseTree, currentIndex);

    LOG("\n\n\n");
    printParseTree(parseTree, 0);

    file.close();

    return EXIT_SUCCESS;
}