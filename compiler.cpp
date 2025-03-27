// g++ compiler.cpp -o compiler
// ./compiler.exe ./input.txt

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>

#define internal static
#define global static
#define local_persist static

enum TokenType{
    RETURN,
    EXIT,
    OPEN_BLOCK,
    CLOSE_BLOCK,
    IDENT,
    OPERATOR,
    INT_LIT,
    VARIABLES,
    INPUT,
    OUTPUT,
    PRINT,
    IF,
    ELSE,
    FOR,
    WHILE,
    IS_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    EQUAL,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    SEMICOLON,
    COMMA,
    DOT,
    ARROW,
    INT,
    AND,
    OR,
    NOT,
    STRING,
    CHAR,
    FLOAT
};

struct Token{
    TokenType type;
    std::string value = "";
};

struct ParseNode{
    Token token;
    std::vector<ParseNode*> children = {};
};

internal ParseNode createTree(std::queue<Token> &Tokens);
internal ParseNode createBlock(std::queue<Token> &Tokens);
internal ParseNode createStatementList(std::queue<Token> &Tokens);
internal ParseNode createStatement(std::queue<Token> &Tokens);
internal ParseNode createReturnStatement(std::queue<Token> &Tokens);
internal ParseNode createExpression(std::queue<Token> &Tokens);
internal ParseNode createTerm(std::queue<Token> &Tokens);
internal ParseNode createSemicolon(std::queue<Token> &Tokens);
internal ParseNode createFactor(std::queue<Token> &Tokens);
internal ParseNode createOperator(std::queue<Token> &Tokens);


std::queue<Token> getTokens(std::ifstream &file){
    std::queue<Token> Tokens;
    char ch;
    std::stringstream buffer;
    int count = 0;
    while(file.peek()!=EOF){
        //std::cout<<count<<std::endl;
        //count++;
        ch = file.peek();
        //std::cout<<ch<<std::endl;
        if (std::isalpha(ch)){
            while(std::isalnum(file.peek())){
                file.get(ch);
                buffer << ch;
            }
            //Check for a alnum token (ex. return, for, if)
            if(buffer.str()=="return"){
                Tokens.push(Token{TokenType::RETURN, "RETURN"});
            }else if(buffer.str()=="exit"){
                Tokens.push(Token{TokenType::EXIT, "EXIT"});
            }else if(buffer.str()=="if"){
                Tokens.push(Token{TokenType::IF, "IF"});
            }else if(buffer.str()=="else"){
                Tokens.push(Token{TokenType::ELSE, "ELSE"});
            }else if(buffer.str()=="for"){
                Tokens.push(Token{TokenType::FOR, "FOR"});
            }else if(buffer.str()=="int"){
                Tokens.push(Token{TokenType::INT, "INT"});
            }else{
                std::cerr<<"This Is Wrong"<<std::endl;
                exit(EXIT_FAILURE);
            }
            buffer.str("");
            buffer.clear();
        }else if(std::isdigit(ch)){
            while(std::isdigit(file.peek())){
                file.get(ch);
                buffer << ch;
            }
            Tokens.push(Token{TokenType::INT_LIT, buffer.str()});
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
                Tokens.push(Token{TokenType::SEMICOLON, ";"});
                //std::cout<<Tokens.back().value<<std::endl;
            }else if(ch=='{'){
                Tokens.push(Token{TokenType::OPEN_BLOCK, "OPEN_BLOCK"});
                //std::cout<<Tokens.back().value<<std::endl;
            }else if(ch=='}'){
                Tokens.push(Token{TokenType::CLOSE_BLOCK, "CLOSE_BLOCK"});
                //std::cout<<Tokens.back().value<<std::endl;
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

internal ParseNode createTree(std::queue<Token> &Tokens){
    ParseNode tree = {ParseNode{Tokens.front()}};
    while(!Tokens.empty()){
        if(tree.token.type == TokenType::OPEN_BLOCK){
            Tokens.pop();
            ParseNode open_block = createBlock(Tokens);
            tree.children.push_back(&open_block);
        }
        else if(tree.token.type == TokenType::RETURN){
            ParseNode returnStatement = createReturnStatement(Tokens);
            tree.children.push_back(&returnStatement);
        }
    }
    return tree;
}

internal ParseNode createBlock(std::queue<Token> &Tokens){
    ParseNode block = {ParseNode{Tokens.front()}};
    return createStatementList(Tokens);
}

internal ParseNode createStatementList(std::queue<Token> &Tokens){
    ParseNode statementList = {ParseNode{Tokens.front()}};
    while(Tokens.front().type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = Tokens.front().type;
        ParseNode statement;
        if(currentToken == TokenType::RETURN){
            statement = createReturnStatement(Tokens);
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            std::cerr<<"Not valid statement"<<std::endl;
            exit(EXIT_FAILURE);
        }
        statementList.children.push_back(&statement);
    }
    return statementList;
}

internal ParseNode createReturnStatement(std::queue<Token> &Tokens){
    ParseNode returnStatement = {ParseNode{Tokens.front()}};
    ParseNode expression = createExpression(Tokens);
    returnStatement.children.push_back(&expression);
    ParseNode semicolon = createSemicolon(Tokens);
    returnStatement.children.push_back(&semicolon);
    return returnStatement;
}

internal ParseNode createExpression(std::queue<Token> &Tokens){
    ParseNode expression = {ParseNode{Tokens.front()}};

    if(Tokens.front().type == TokenType::INT_LIT){
        ParseNode term = createTerm(Tokens);
        expression.children.push_back(&term);
    }

    if(Tokens.front().type == TokenType::ADD){
        ParseNode add = createOperator(Tokens);
        expression.children.push_back(&add);
    }

    return expression;
}


internal ParseNode createTerm(std::queue<Token> &Tokens){
    ParseNode term = {ParseNode{Tokens.front()}};
    if(Tokens.front().type == TokenType::INT_LIT){
        ParseNode factor = createFactor(Tokens);
        term.children.push_back(&factor);
    }
    if(Tokens.front().type == TokenType::MULTIPLY){
        ParseNode mult = createOperator(Tokens);
        term.children.push_back(&mult);
        ParseNode factor = createFactor(Tokens);
        term.children.push_back(&factor);
    }
    return term;
}

internal ParseNode createFactor(std::queue<Token> &Tokens){
    ParseNode factor = {ParseNode{Tokens.front()}};
    return factor;
}

internal ParseNode createOperator(std::queue<Token> &Tokens){
    ParseNode oper = {ParseNode{Tokens.front()}};

    if(Tokens.front().type == TokenType::MULTIPLY){
        oper = {ParseNode{Tokens.front()}};
    }
    else if(Tokens.front().type == TokenType::ADD){
        oper = {ParseNode{Tokens.front()}};
    }

    return oper;
}

internal ParseNode createSemicolon(std::queue<Token> &Tokens){
    if(Tokens.front().type==TokenType::SEMICOLON){
        return ParseNode{Tokens.front()};
    }else{
        std::cerr<<"No semicolon found"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

internal void printParseTree(ParseNode &Tree){
}

int main(int argc, char* argv[]){
    if(argc!= 2){
        std::cerr<<"INCORRECT USAGE"<<std::endl;
        std::cerr<<"Correct usage: Compiler <input.txt>"<<std::endl;
        return EXIT_FAILURE;
    }


    std::cout<<argv[1]<<std::endl;

    std::ifstream file(argv[1]);

    std::queue<Token> Tokens = getTokens(file);

    while(!Tokens.empty()){
        std::cout<<Tokens.front().value<<std::endl;
        Tokens.pop();
    }

    //readFile(file);
    //std::ifstream file("input.txt");
    //std::ofstream out("test.asm");

    file.close();

    return EXIT_SUCCESS;
}