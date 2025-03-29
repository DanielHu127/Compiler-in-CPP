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
    LOGIC_OP,
    SEMICOLON,
    EXIT,
    TREE,
    //Parse
    BLOCK,
    STATEMENTLIST,
    EXPRESSION,
    TERM,
    FACTOR,
    CONDITION
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

internal void createTree(std::queue<Token> &Tokens);
internal void createBlock(std::queue<Token> &Tokens, ParseNode &Node);
internal void createStatementList(std::queue<Token> &Tokens, ParseNode &Node);
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
        ch = file.peek();
        if (std::isalpha(ch)){
            while(std::isalnum(file.peek())){
                file.get(ch);
                buffer << ch;
            }
            //Check for a alnum token (ex. return, for, if)
            if(buffer.str()=="return"){
                Tokens.push(Token{TokenType::RETURN, "return"});
            }else if(buffer.str()=="exit"){
                Tokens.push(Token{TokenType::EXIT, "exit"});
            }else if(buffer.str()=="if"){
                Tokens.push(Token{TokenType::IF, "if"});
            }else if(buffer.str()=="else"){
                Tokens.push(Token{TokenType::ELSE, "else"});
            }else if(buffer.str()=="for"){
                Tokens.push(Token{TokenType::FOR, "for"});
            }else if(buffer.str()=="int"){
                Tokens.push(Token{TokenType::INT_DECL, "int"});
            }else if(buffer.str()=="true"){
                Tokens.push(Token{TokenType::BOOL_LIT, "true"});
            }else if(buffer.str()=="false"){
                Tokens.push(Token{TokenType::BOOL_LIT, "false"});
            }else{
                Tokens.push(Token{TokenType::IDENT, buffer.str()});
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
            }else if(ch=='{'){
                Tokens.push(Token{TokenType::OPEN_BLOCK, "{"});
            }else if(ch=='}'){
                Tokens.push(Token{TokenType::CLOSE_BLOCK, "}"});
            }else if(ch=='+'){
                Tokens.push(Token{TokenType::ADD, "+"});
            }else if(ch=='-'){
                Tokens.push(Token{TokenType::SUB, "-"});
            }else if(ch=='*'){
                Tokens.push(Token{TokenType::MULTIPLY, "*"});
            }else if(ch=='/'){
                Tokens.push(Token{TokenType::DIVIDE, "/"});
            }else if(ch=='%'){
                Tokens.push(Token{TokenType::MOD, "%"});
            }else if(ch=='('){
                Tokens.push(Token{TokenType::OPEN_PAREN, "("});
            }else if(ch==')'){
                Tokens.push(Token{TokenType::CLOSE_PAREN, ")"});
            }else if(ch=='='){
                Tokens.push(Token{TokenType::ASSIGN, "="});
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

internal void createTree(std::queue<Token> &Tokens, ParseNode *tree){
    while(!Tokens.empty()){
        std::cout<<tree->token.value<<std::endl;
        if(Tokens.front().type == TokenType::OPEN_BLOCK){
            Tokens.pop();
            createBlock(Tokens, *tree);
            std::cout<<tree->children[0]->token.value<<std::endl;
            Tokens.pop();
        }
        else if(Tokens.front().type == TokenType::RETURN){
            //NOTE: FIX THIS
            //ParseNode returnStatement = createReturnStatement(Tokens);
            //tree.children.push_back(&returnStatement);
        }
    }
    std::cout<<tree->token.value<<std::endl;
}

internal void createBlock(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode * block = new ParseNode{Token{TokenType::BLOCK, "Block"}};
    std::cout<<block->token.value<<std::endl;
    createStatementList(Tokens, *block);
    Node.children.push_back(block);
    std::cout<<block->token.value<<std::endl;
}

internal void createStatementList(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *statementList = new ParseNode{Token{TokenType::STATEMENTLIST, "Statement List"}};
    std::cout<<statementList->token.value<<std::endl;
    while(Tokens.front().type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = Tokens.front().type;
        if(currentToken == TokenType::RETURN){
            createReturnStatement(Tokens, *statementList);
        }else if(currentToken == TokenType::IF){
            createIfStatement(Tokens, *statementList);
        }else if(currentToken == TokenType::CLOSE_BLOCK){
            break;
        }else{
            std::cerr<<"Not valid statement"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
    Node.children.push_back(statementList);
    std::cout<<statementList->token.value<<std::endl;
}

internal void createIfStatement(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *ifStatement = new ParseNode{Tokens.front()};
    std::cout<<ifStatement->token.value<<std::endl;
    Tokens.pop();
    if(Tokens.front().type == OPEN_PAREN){
        Tokens.pop();
        createCondition(Tokens, *ifStatement);
        createBlock(Tokens, *ifStatement);
        Node.children.push_back(ifStatement);
        if(Tokens.front().type == CLOSE_PAREN){
            Tokens.pop();
        }else{
            std::cerr<<"Mismatched () in ifStatement"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }else{
        std::cerr<<"If statement doesn't have ()"<<std::endl;
        exit(EXIT_FAILURE);
    }
}

internal void createReturnStatement(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *returnStatement = new ParseNode{Tokens.front()};
    std::cout<<returnStatement->token.value<<std::endl;
    Tokens.pop();
    createExpression(Tokens, *returnStatement);
    createSemicolon(Tokens, *returnStatement);
    std::cout<<returnStatement->token.value<<std::endl;
    Node.children.push_back(returnStatement);
    std::cout<<returnStatement->token.value<<std::endl;
}

internal void createCondition(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *condition = new ParseNode{Token{TokenType::CONDITION, "Condition"}};
    std::cout<<condition->token.value<<std::endl;
    while(true){ //NOTE:FIX
        createExpression(Tokens, *condition);
    }
    Node.children.push_back(condition);
}

internal void createExpression(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *expression = new ParseNode{Token{TokenType::EXPRESSION, "Expression"}};
    std::cout<<expression->token.value<<std::endl;
    TokenType currentToken;
    while(Tokens.front().type == TokenType::INT_LIT){
        if(Tokens.front().type == TokenType::INT_LIT){
            createTerm(Tokens, *expression);
        }
        currentToken = Tokens.front().type;
        if(currentToken == ADD || currentToken == SUB){
            createOperator(Tokens, *expression);
        }

    }
    Node.children.push_back(expression);
    std::cout<<expression->token.value<<std::endl;
}


internal void createTerm(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *term = new ParseNode{Token{TokenType::TERM, "Term"}};
    std::cout<<term->token.value<<std::endl;
    TokenType currentToken;
    while(Tokens.front().type == TokenType::INT_LIT){
        if(Tokens.front().type == TokenType::INT_LIT){
            createFactor(Tokens, *term);
        }
        currentToken = Tokens.front().type;
        if( currentToken == MULTIPLY || 
            currentToken == DIVIDE || 
            currentToken == MOD){
            createOperator(Tokens, *term);
        }
    }
    Node.children.push_back(term);
    std::cout<<term->token.value<<std::endl;
}

internal void createFactor(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode *factor = new ParseNode{Tokens.front()};
    std::cout<<factor->token.value<<std::endl;
    Tokens.pop();
    Node.children.push_back(factor);
    std::cout<<factor->token.value<<std::endl;
}

internal void createOperator(std::queue<Token> &Tokens, ParseNode &Node){
    ParseNode * oper = new ParseNode{Tokens.front()};
    TokenType currentToken = Tokens.front().type;

    if( currentToken == ADD || 
        currentToken == SUB || 
        currentToken == MULTIPLY || 
        currentToken == DIVIDE || 
        currentToken == MOD){
        Node.children.push_back(oper);
        Tokens.pop();
    }else{
        std::cerr<<"Not an operator: "<<Tokens.front().value<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<oper->token.value<<std::endl;
}

internal void createSemicolon(std::queue<Token> &Tokens, ParseNode &Node){
    if(Tokens.front().type==TokenType::SEMICOLON){
        ParseNode * semicolon = new ParseNode{Tokens.front()};
        std::cout<<semicolon->token.value<<std::endl;
        Tokens.pop();
        Node.children.push_back(semicolon);
    }else{
        std::cerr<<"No semicolon found"<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<"Semicolon"<<std::endl;
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

    std::queue<Token> Tokens = getTokens(file);
    ParseNode* parseTree = new ParseNode{Token{TokenType::TREE, "Tree"}};
    createTree(Tokens, parseTree);

    std::cout<<"\n\n\n"<<std::endl;
    printParseTree(parseTree, 0);

    file.close();

    return EXIT_SUCCESS;
}