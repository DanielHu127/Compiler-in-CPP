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
    OPERATOR,
    INT_LIT,
    IF,
    ELSE,
    FOR,
    WHILE,
    ADD,
    MULTIPLY,
    SEMICOLON,
    //TEMP
    TREE,
    BLOCK,
    STATEMENTLIST,
    EXPRESSION,
    TERM,
    FACTOR
};

/*
enum ParseType{
    TREE,
    BLOCK,
    EXPRESSION,
    TERM,
    FACTOR
};
*/

struct Token{
    TokenType type;
    std::string value = "";
};

struct ParseNode{
    Token token;
    std::vector<ParseNode*> children = {};
};

internal ParseNode createTree(std::queue<Token> &Tokens);
internal void createBlock(std::queue<Token> &Tokens, ParseNode &Node);
internal void createStatementList(std::queue<Token> &Tokens, ParseNode &Node);
internal void createStatement(std::queue<Token> &Tokens, ParseNode &Node);
internal void createReturnStatement(std::queue<Token> &Tokens, ParseNode &Node);
internal void createExpression(std::queue<Token> &Tokens, ParseNode &Node);
internal void createTerm(std::queue<Token> &Tokens, ParseNode &Node);
internal void createSemicolon(std::queue<Token> &Tokens, ParseNode &Node);
internal void createFactor(std::queue<Token> &Tokens, ParseNode &Node);
internal void createOperator(std::queue<Token> &Tokens, ParseNode &Node);


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
    ParseNode tree = {ParseNode{Token{TokenType::TREE, "Tree"}}};
    while(!Tokens.empty()){
        std::cout<<tree.token.value<<std::endl;
        if(Tokens.front().type == TokenType::OPEN_BLOCK){
            Tokens.pop();
            createBlock(Tokens, tree);
            Tokens.pop();
        }
        else if(Tokens.front().type == TokenType::RETURN){
            //NOTE: FIX THIS
            //ParseNode returnStatement = createReturnStatement(Tokens);
            //tree.children.push_back(&returnStatement);
        }
    }
    return tree;
}

internal void createBlock(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode block = {ParseNode{Token{TokenType::BLOCK, "Block"}}};
    std::cout<<block.token.value<<std::endl;
    createStatementList(Tokens, block);
    Node.children.push_back(&block);
}

internal void createStatementList(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode statementList = {ParseNode{Token{TokenType::STATEMENTLIST, "Statement List"}}};
    std::cout<<statementList.token.value<<std::endl;
    while(Tokens.front().type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = Tokens.front().type;
        ParseNode statement;
        if(currentToken == TokenType::RETURN){
            createReturnStatement(Tokens, statementList);
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            std::cerr<<"Not valid statement"<<std::endl;
            exit(EXIT_FAILURE);
        }
        Node.children.push_back(&statement);
    }
}

internal void createReturnStatement(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode returnStatement = {ParseNode{Tokens.front()}};
    std::cout<<returnStatement.token.value<<std::endl;
    Tokens.pop();
    createExpression(Tokens, returnStatement);
    createSemicolon(Tokens, returnStatement);
    Node.children.push_back(&returnStatement);
}

internal void createExpression(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode expression = {ParseNode{Token{TokenType::EXPRESSION, "Expression"}}};
    std::cout<<expression.token.value<<std::endl;
    if(Tokens.front().type == TokenType::INT_LIT){
        createTerm(Tokens, expression);
        Node.children.push_back(&expression);
    }

    if(Tokens.front().type == TokenType::ADD){
        createOperator(Tokens, expression);
        expression.children.push_back(&expression);
    }
}


internal void createTerm(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode term = {ParseNode{Token{TokenType::TERM, "Term"}}};
    std::cout<<term.token.value<<std::endl;
    if(Tokens.front().type == TokenType::INT_LIT){
        createFactor(Tokens, term);
        Node.children.push_back(&term);
    }
    if(Tokens.front().type == TokenType::MULTIPLY){
        createOperator(Tokens, term);
        createFactor(Tokens, term);
        Node.children.push_back(&term);
    }
}

internal void createFactor(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode factor = {ParseNode{Token{TokenType::FACTOR, "Factor"}}};
    std::cout<<factor.token.value<<std::endl;
    Tokens.pop();
    Node.children.push_back(&factor);
}

internal void createOperator(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode oper = {ParseNode{Tokens.front()}};

    if(Tokens.front().type == TokenType::MULTIPLY){
        oper = {ParseNode{Tokens.front()}};
        Node.children.push_back(&oper);
        Tokens.pop();
    }
    else if(Tokens.front().type == TokenType::ADD){
        oper = {ParseNode{Tokens.front()}};
        Node.children.push_back(&oper);
        Tokens.pop();
    }
}

internal void createSemicolon(std::queue<Token> &Tokens, ParseNode &Node){
    if(Tokens.front().type==TokenType::SEMICOLON){
        ParseNode semicolon = ParseNode{Tokens.front()};
        std::cout<<semicolon.token.value<<std::endl;
        Tokens.pop();
        Node.children.push_back(&semicolon);
    }else{
        std::cerr<<"No semicolon found"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

internal void printParseTree(ParseNode &Node){
    /*
    std::cout<<Node.token.value<<std::endl;
    for(int i = 0; i<Node.children.size();i++){
        printParseTree(*Node.children[i]);
    }
    */
    std::cout<<(*Node.children[0]).token.value<<std::endl;
    /*
    std::cout<<"Tree"<<std::endl;
    std::cout<<"----Block"<<std::endl;
    std::cout<<"--------Statement List"<<std::endl;
    std::cout<<"------------RETURN"<<std::endl;
    std::cout<<"----------------Expression"<<std::endl;
    std::cout<<"--------------------Term"<<std::endl;
    std::cout<<"------------------------Factor"<<std::endl;
    std::cout<<"----------------Semicolon"<<std::endl;
    */
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

    ParseNode parseTree = createTree(Tokens);

    printParseTree(parseTree);

    /*
    while(!Tokens.empty()){
        std::cout<<Tokens.front().value<<std::endl;
        Tokens.pop();
    }
    */

    //readFile(file);
    //std::ifstream file("input.txt");
    //std::ofstream out("test.asm");

    file.close();

    return EXIT_SUCCESS;
}