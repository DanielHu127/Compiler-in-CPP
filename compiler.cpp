// g++ compiler.cpp -o compiler
// ./compiler.exe ./input.txt

//THINGS TO ADD:
/*
 - Keywords: int, float, return, if, else, for, while, switch, class, public, private, virtual, namespace, constexpr, template
 - Operator Precedence: Unary, Multiplicative, Additive, Shift, Relational, Equality, Bitwise AND, Bitwise OR,
                        Logical AND, Logical OR, Conditional (prob not), Assignment, Comma
 - Unary Operations (Operations that come before the term): +, -, !, ~, *, & 
 - Multiplicative Operations: *, /, %
 - Additive Operations: +, - 
 - Shift Operations: <<, >>
 - Relational: <, <=, >, >=
 - Equality: ==, !=
 - Bitwise AND: &
 - Bitwise XOR: ^
 - Bitwise OR: |
 - Logical AND: &&
 - Logical OR: ||
 - Conditional (prob not): ?: 
 - Assignment: =, +=, -=, *=, /=, %=, <<=, >>=, &=, ^=, |=
 - Comma: ,
 - Variables and Identifiers
 - Control Flow statements: If, else, for, while
 - Delimiters: ( ) { } [ ] ; ,
 - Punctuation: :: . -> ... 
 - Preprocesser: #include, #define, #ifdef, #pragma
 - Comments: // /*
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

enum TokenType{
    RETURN,
    IDENT,
    OPEN_BLOCK,
    CLOSE_BLOCK,
    OPEN_PAREN,
    CLOSE_PAREN,
    INT_DECL,
    INT_LIT,
    BOOL_LIT,
    ASSIGN,
    IF,
    ELSE,
    FOR,
    WHILE,
    ADD,
    SUB,
    MULTIPLY,
    DIVIDE,
    MOD,
    RELATION_OP,
    LOGICAL_AND,
    LOGICAL_OR,
    SEMICOLON,
    EXIT,
    TREE,
    GLOBAL,
    MAIN,
    //Parse
    BLOCK,
    PARENTHESES,
    STATEMENTLIST,
    EXPRESSION,
    TERM,
    FACTOR,
    CONDITION,
    RELATIONAL,
    LOGICAL,
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
internal void createLogical(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);

internal void consumeTokens(std::vector<Token> &Tokens, int left, int right){
    for(int i = right; i>=left;i--){
        if (i >= 0 && i < Tokens.size()) {
            Tokens.erase(Tokens.begin() + i);
        } else {
            std::cout << "Index out of bounds" << std::endl;
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
            }else if(buffer.str()=="int"){
                Tokens.push_back(Token{TokenType::INT_DECL, "int"});
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
            }else if(ch=='+'){
                Tokens.push_back(Token{TokenType::ADD, "+"});
            }else if(ch=='-'){
                Tokens.push_back(Token{TokenType::SUB, "-"});
            }else if(ch=='*'){
                Tokens.push_back(Token{TokenType::MULTIPLY, "*"});
            }else if(ch=='/'){
                Tokens.push_back(Token{TokenType::DIVIDE, "/"});
            }else if(ch=='#'){
                while(file.peek()!='\n'){
                    file.get(ch);
                }
            }else if(ch=='%'){
                Tokens.push_back(Token{TokenType::MOD, "%"});
            }else if(ch=='('){
                Tokens.push_back(Token{TokenType::OPEN_PAREN, "("});
            }else if(ch==')'){
                Tokens.push_back(Token{TokenType::CLOSE_PAREN, ")"});
            }else if(ch=='=' && file.peek()!= '='){
                Tokens.push_back(Token{TokenType::ASSIGN, "="});
            }else if(ch=='&' && file.peek()=='&'){
                Tokens.push_back(Token{TokenType::LOGICAL_AND, "&&"});
                file.get(ch);
            }else if(ch=='|' && file.peek()=='|'){
                Tokens.push_back(Token{TokenType::LOGICAL_OR, "||"});
                file.get(ch);
            }else if(ch=='=' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::RELATION_OP, "=="});
                file.get(ch);
            }else if(ch=='!' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::RELATION_OP, "!="});
                file.get(ch);
            }else if(ch=='>' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::RELATION_OP, ">"});
            }else if(ch=='>' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::RELATION_OP, ">="});
                file.get(ch);
            }else if(ch=='<' && file.peek()!='='){
                Tokens.push_back(Token{TokenType::RELATION_OP, "<"});
            }else if(ch=='<' && file.peek()=='='){
                Tokens.push_back(Token{TokenType::RELATION_OP, "<="});
                file.get(ch);
            }else{
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
        std::cout<<line<<std::endl;
    }
}

internal void createSemicolon(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    std::cout<<currentIndex<<std::endl;
    if(Tokens[currentIndex].type==TokenType::SEMICOLON){
        ParseNode * semicolon = new ParseNode{Tokens[currentIndex++]};
        std::cout<<semicolon->token.value<<std::endl;
        Node.children.push_back(semicolon);
    }else{
        std::cerr<<"No semicolon found"<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<"Semicolon"<<std::endl;
}

internal void createFactor(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    if(Tokens[currentIndex].type == OPEN_PAREN){
        ParseNode *parenBlock = new ParseNode{Token{TokenType::PARENTHESES, "Paren Block()"}};
        currentIndex++;
        createLogical(Tokens, *parenBlock, currentIndex);
        if(Tokens[currentIndex].type != CLOSE_PAREN){
            std::cerr<<"Mismatched parentheses"<<std::endl;
            exit(EXIT_FAILURE);
        }
        Node.children.push_back(parenBlock);
        currentIndex++;
    }else{
        ParseNode *factor = new ParseNode{Tokens[currentIndex]};
        std::cout<<factor->token.value<<std::endl;
        std::cout<<currentIndex<<std::endl;
        currentIndex++;
        
        Node.children.push_back(factor);
        std::cout<<factor->token.value<<std::endl;
    }
}

internal void createTerm(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *termOperator = new ParseNode{Token{TokenType::TERM, "Term"}};
    createFactor(Tokens, *termOperator, currentIndex);
    
    std::cout<<termOperator->token.value<<std::endl;
    TokenType currentToken = Tokens[currentIndex].type;
    if(currentIndex < Tokens.size() && (currentToken == MULTIPLY || currentToken == DIVIDE || currentToken == MOD)){
        Token operatorToken = Tokens[currentIndex++];
        termOperator->token = operatorToken;
        if(currentIndex < Tokens.size()-1 && (Tokens[currentIndex+1].type == MULTIPLY || Tokens[currentIndex+1].type == DIVIDE || Tokens[currentIndex+1].type == MOD)){
            createTerm(Tokens, *termOperator, currentIndex);
        }else{
            createFactor(Tokens, *termOperator, currentIndex);
        }
    }
    Node.children.push_back(termOperator);
    std::cout<<termOperator->token.value<<std::endl;
}

internal void createExpression(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *expressionOperator = new ParseNode{Token{TokenType::EXPRESSION, "Expression"}};
    createTerm(Tokens, *expressionOperator, currentIndex);

    std::cout<<expressionOperator->token.value<<std::endl;
    TokenType currentToken = Tokens[currentIndex].type;
    if(currentIndex < Tokens.size() && (currentToken == ADD || currentToken == SUB)){
        Token operatorToken = Tokens[currentIndex++];
        expressionOperator->token = operatorToken;
        if(currentIndex < Tokens.size()-1 && (Tokens[currentIndex+1].type == ADD || Tokens[currentIndex+1].type == SUB)){
            createExpression(Tokens, *expressionOperator, currentIndex);
        }else{
            createTerm(Tokens, *expressionOperator, currentIndex);
        }
    }
    Node.children.push_back(expressionOperator);
    std::cout<<expressionOperator->token.value<<std::endl;
}

internal void createRelational(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *relationalOperator = new ParseNode{Token{TokenType::RELATION_OP, "Relational"}};
    createExpression(Tokens, *relationalOperator, currentIndex);

    std::cout<<relationalOperator->token.value<<std::endl;
    TokenType currentToken = Tokens[currentIndex].type;
    if(currentIndex < Tokens.size() && (currentToken == RELATION_OP)){
        //std::cout<<relationalOperator->token.value<<std::endl;
        //std::cout<<relationalOperator->token.value<<std::endl;
        //std::cout<<relationalOperator->token.value<<std::endl;
        //std::cout<<relationalOperator->token.value<<std::endl;
        //std::cout<<relationalOperator->token.value<<std::endl;
        Token operatorToken = Tokens[currentIndex++];
        relationalOperator->token = operatorToken;
        if(currentIndex < Tokens.size()-1 && (Tokens[currentIndex+1].type == RELATION_OP)){
            createRelational(Tokens, *relationalOperator, currentIndex);
        }else{
            createExpression(Tokens, *relationalOperator, currentIndex);
        }
    }
    Node.children.push_back(relationalOperator);
    std::cout<<relationalOperator->token.value<<std::endl;
}

internal void createLogical(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    TokenType currentToken = Tokens[currentIndex].type;
    ParseNode *logicalOperator = new ParseNode{Token{TokenType::LOGICAL, "Logical"}};
    createRelational(Tokens, *logicalOperator, currentIndex);
    while(currentIndex < Tokens.size() && (currentToken == LOGICAL_AND || currentToken == LOGICAL_OR)){
        logicalOperator = new ParseNode{Token{TokenType::LOGICAL, "Logical"}};
        createRelational(Tokens, *logicalOperator, currentIndex);
        Token operatorToken = Tokens[currentIndex++];
        logicalOperator->token = operatorToken;
        if(currentIndex < Tokens.size()-1 && (Tokens[currentIndex+1].type == LOGICAL_AND || Tokens[currentIndex+1].type == LOGICAL_OR)){
            std::cout<<"WHY is LOGICAL HERE"<<std::endl;
            createLogical(Tokens, *logicalOperator, currentIndex);
        }else{
            std::cout<<"WHY is RELATIONAL HERE"<<std::endl;
            createRelational(Tokens, *logicalOperator, currentIndex);
        }
        currentToken = Tokens[currentIndex].type;
    }
    /*
    ParseNode *logicalOperator = new ParseNode{Token{TokenType::LOGICAL, "Logical"}};
    createRelational(Tokens, *logicalOperator, currentIndex);
    TokenType currentToken = Tokens[currentIndex].type;
    if(currentIndex < Tokens.size() && (currentToken == LOGICAL_AND || currentToken == LOGICAL_OR)){
        Token operatorToken = Tokens[currentIndex++];
        logicalOperator->token = operatorToken;
        currentIndex--;
        std::cout<<"This is the current index: "<<currentIndex<<std::endl;
        if(currentIndex < Tokens.size()-1 && (Tokens[currentIndex+1].type == LOGICAL_AND || Tokens[currentIndex+1].type == LOGICAL_OR)){
            std::cout<<"WHY is LOGICAL HERE"<<std::endl;
            createLogical(Tokens, *logicalOperator, currentIndex);
            currentIndex++;
        }
        if(currentIndex<Tokens.size()-1 && (Tokens[currentIndex+1].type != LOGICAL_AND && Tokens[currentIndex+1].type != LOGICAL_OR)){
            std::cout<<"WHY is RELATIONAL HERE"<<std::endl;
            createRelational(Tokens, *logicalOperator, currentIndex);
        }
    }
        */
    /*
    while(currentIndex < Tokens.size() && (currentToken == LOGICAL_AND || currentToken == LOGICAL_OR)){
        std::cout<<"WHY AM I HERE"<<std::endl;
        std::cout<<logicalOperator->token.value<<std::endl;
        std::cout<<currentIndex<<std::endl;
        Token operatorToken = Tokens[currentIndex];
        logicalOperator->token = operatorToken;
        if(currentIndex < Tokens.size() && (Tokens[currentIndex].type == LOGICAL_AND || Tokens[currentIndex].type == LOGICAL_OR)){
            std::cout<<"I AM LOGICALLLLLL"<<std::endl;
            currentIndex++;
            createLogical(Tokens, *logicalOperator, currentIndex);
        }else{
            std::cout<<"I AM RELATIONALLL"<<std::endl;
            std::cout<<logicalOperator->token.value<<std::endl;
            createRelational(Tokens, *logicalOperator, currentIndex);
        }
        //std::cout<<currentIndex<<std::endl;
        //std::cout<<currentIndex<<std::endl;
        //std::cout<<currentIndex<<std::endl;
        //std::cout<<currentIndex<<std::endl;
        currentToken = Tokens[currentIndex].type;
    }
        */
    Node.children.push_back(logicalOperator);
    //std::cout<<logicalOperator->token.value<<std::endl;
}

internal void createReturnStatement(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *returnStatement = new ParseNode{Tokens[currentIndex]};
    std::cout<<returnStatement->token.value<<std::endl;
    currentIndex++;
    createLogical(Tokens, *returnStatement, currentIndex);
    std::cout<<"excuse me why"<<std::endl;
    createSemicolon(Tokens, *returnStatement, currentIndex);
    Node.children.push_back(returnStatement);
    std::cout<<returnStatement->token.value<<std::endl;
}

internal void createIfStatement(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *ifStatement = new ParseNode{Tokens[currentIndex]};
    std::cout<<ifStatement->token.value<<std::endl;
    currentIndex++;
    if(Tokens[currentIndex].type == OPEN_PAREN){
        currentIndex++;
        createRelational(Tokens, *ifStatement, currentIndex);
        if(Tokens[currentIndex].type == CLOSE_PAREN){
            createBlock(Tokens, *ifStatement, currentIndex);
            Node.children.push_back(ifStatement);
        }else{
            std::cerr<<"Mismatched () in ifStatement"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }else{
        std::cerr<<"If statement doesn't have ()"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

internal void createStatementList(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *statementList = new ParseNode{Token{TokenType::STATEMENTLIST, "Statement List"}};
    std::cout<<statementList->token.value<<std::endl;
    while(Tokens[currentIndex].type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = Tokens[currentIndex].type;
        if(currentToken == TokenType::RETURN){
            createReturnStatement(Tokens, *statementList, currentIndex);
        }else if(currentToken == TokenType::IF){
            createIfStatement(Tokens, *statementList, currentIndex);
        }else if(currentToken == TokenType::OPEN_BLOCK){
            createBlock(Tokens, *statementList, currentIndex);
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            std::cout<<Tokens[currentIndex].value<<std::endl;
            std::cerr<<"Not valid statement"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    Node.children.push_back(statementList);
    std::cout<<statementList->token.value<<std::endl;
}

internal void createBlock(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode * block = new ParseNode{Token{TokenType::BLOCK, "Block"}};
    std::cout<<block->token.value<<std::endl;
    currentIndex++;
    createStatementList(Tokens, *block, currentIndex);
    currentIndex++;
    Node.children.push_back(block);
    std::cout<<block->token.value<<std::endl;
}

internal void createTree(std::vector<Token> &Tokens, ParseNode *tree, int &currentIndex){
    while(currentIndex<Tokens.size()){
        if(Tokens[currentIndex].type == TokenType::OPEN_BLOCK){
            createBlock(Tokens, *tree, currentIndex);
            std::cout<<tree->children[0]->token.value<<std::endl;
        }
        else if(Tokens[currentIndex].type == TokenType::RETURN){
            //NOTE: FIX THIS
            //ParseNode returnStatement = createReturnStatement(Tokens);
            //tree.children.push_back(&returnStatement);
        }
    }
    std::cout<<tree->token.value<<std::endl;
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


    std::cout<<argv[1]<<std::endl;

    std::ifstream file(argv[1]);

    std::vector<Token> Tokens = getTokens(file);
    for(int i = 0; i<Tokens.size();i++){
        std::cout<<Tokens[i].value<<std::endl;
    }
    ParseNode* parseTree = new ParseNode{Token{TokenType::TREE, "Tree"}};
    int currentIndex = 0;
    createTree(Tokens, parseTree, currentIndex);

    std::cout<<"\n\n\n"<<std::endl;
    printParseTree(parseTree, 0);

    file.close();

    return EXIT_SUCCESS;
}