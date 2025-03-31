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
    LOGIC_OP,
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

internal void createTree(std::vector<Token> &Tokens, ParseNode *tree, int &currentIndex);
internal void createBlock(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createStatementList(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createReturnStatement(std::vector<Token> &Tokens, ParseNode &Node,int &currentIndex);
internal void createExpression(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createTerm(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createSemicolon(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createFactor(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createOperator(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createCondition(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);
internal void createIfStatement(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex);


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
            while(std::isalnum(file.peek())){
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
            }else if(ch=='%'){
                Tokens.push_back(Token{TokenType::MOD, "%"});
            }else if(ch=='('){
                Tokens.push_back(Token{TokenType::OPEN_PAREN, "("});
            }else if(ch==')'){
                Tokens.push_back(Token{TokenType::CLOSE_PAREN, ")"});
            }else if(ch=='='){
                Tokens.push_back(Token{TokenType::ASSIGN, "="});
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

internal void createTree(std::vector<Token> &Tokens, ParseNode *tree, int &currentIndex){
    while(!Tokens.empty()){
        std::cout<<tree->token.value<<std::endl;
        if(Tokens[currentIndex].type == TokenType::OPEN_BLOCK){
            currentIndex++;
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

internal void createBlock(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode * block = new ParseNode{Token{TokenType::BLOCK, "Block"}};
    std::cout<<block->token.value<<std::endl;
    currentIndex++;
    createStatementList(Tokens, *block, currentIndex);
    Node.children.push_back(block);
    std::cout<<block->token.value<<std::endl;
}

internal void createStatementList(std::vector<Token> &Tokens, ParseNode &Node, int &currentIndex){
    ParseNode *statementList = new ParseNode{Token{TokenType::STATEMENTLIST, "Statement List"}};
    std::cout<<statementList->token.value<<std::endl;
    while(Tokens[currentIndex].type!=TokenType::CLOSE_BLOCK){
        TokenType currentToken = Tokens[currentIndex].type;
        if(currentToken == TokenType::RETURN){
            createReturnStatement(Tokens, *statementList, index);
        }else if(currentToken == TokenType::IF){
            createIfStatement(Tokens, *statementList, index);
        }else if(currentToken == TokenType::OPEN_BLOCK){
            createBlock(Tokens, *statementList, index);
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

internal void createIfStatement(std::vector<Token> &Tokens, ParseNode &Node, int index){
    int currentIndex = index;
    ParseNode *ifStatement = new ParseNode{Tokens[index]};
    std::cout<<ifStatement->token.value<<std::endl;
    currentIndex++;
    if(Tokens[currentIndex].type == OPEN_PAREN){
        currentIndex++;
        createCondition(Tokens, *ifStatement, index);
        if(Tokens[currentIndex].type == CLOSE_PAREN){
            createBlock(Tokens, *ifStatement, index);
            consumeTokens(Tokens, index, currentIndex);
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

internal void createReturnStatement(std::vector<Token> &Tokens, ParseNode &Node, int index){
    ParseNode *returnStatement = new ParseNode{Tokens[index]};
    std::cout<<returnStatement->token.value<<std::endl;
    Tokens.erase(Tokens.begin()+index);
    createExpression(Tokens, *returnStatement, index);
    createSemicolon(Tokens, *returnStatement, index);
    Node.children.push_back(returnStatement);
    std::cout<<returnStatement->token.value<<std::endl;
}

internal void createCondition(std::vector<Token> &Tokens, ParseNode &Node, int index){
    ParseNode *condition = new ParseNode{Token{TokenType::CONDITION, "Condition"}};
    std::cout<<condition->token.value<<std::endl;
    while(currentIndex < Tokens.size() && (currentToken == MULTIPLY || currentToken == DIVIDE || currentToken == MOD)){
        Node.children.push_back();
    }
        createExpression(Tokens, *condition, index);
    Node.children.push_back(condition);
}

internal void createExpression(std::vector<Token> &Tokens, ParseNode &Node, int index){
    ParseNode *expression = new ParseNode{Token{TokenType::EXPRESSION, "Expression"}};
    std::cout<<expression->token.value<<std::endl;
    TokenType currentToken;
    while(currentIndex < Tokens.size() && (currentToken == MULTIPLY || currentToken == DIVIDE || currentToken == MOD)){
        Node.children.push_back();
    }
    while(Tokens[index].type == TokenType::INT_LIT){
        if(Tokens[index].type == TokenType::INT_LIT){
            createTerm(Tokens, *expression, index);
        }
        currentToken = Tokens[index].type;
        if(currentToken == ADD || currentToken == SUB){
            createOperator(Tokens, *expression, index);
        }

    }
    Node.children.push_back(expression);
    std::cout<<expression->token.value<<std::endl;
}

internal void createRelationalExpression(std::vector<Token> &Tokens, ParseNode &Node, int index){

}

internal void createLogical(std::vector<Token> &Tokens, ParseNode &Node, int index){
    while(currentIndex < Tokens.size() && (currentToken == MULTIPLY || currentToken == DIVIDE || currentToken == MOD)){
        Node.children.push_back();
    }
}



internal void createTerm(std::vector<Token> &Tokens, ParseNode &Node, int index){
    ParseNode *term = new ParseNode{Token{TokenType::TERM, "Term"}};
    createFactor(Tokens, *term, index);
    std::cout<<term->token.value<<std::endl;
    int currentIndex = index;
    TokenType currentToken = Tokens[currentIndex].type;
    while(currentIndex < Tokens.size() && (currentToken == MULTIPLY || currentToken == DIVIDE || currentToken == MOD)){
        Node.children.push_back();
    }
    Node.children.push_back(term);
    std::cout<<term->token.value<<std::endl;
}

internal void createFactor(std::vector<Token> &Tokens, ParseNode &Node, int index){
    int currentIndex = index;
    if(Tokens[currentIndex].type == OPEN_PAREN){
        ParseNode *parenBlock = new ParseNode{Token{TokenType::PARENTHESES, "Paren Block()"}};
        currentIndex++;
        createExpression(Tokens, *parenBlock, currentIndex);
        if(Tokens[currentIndex].type != CLOSE_PAREN){
            std::cerr<<"Mismatched parentheses"<<std::endl;
            exit(EXIT_FAILURE);
        }
        consumeTokens(Tokens, index, currentIndex);
        Node.children.push_back(parenBlock);
    }else{
        ParseNode *factor = new ParseNode{Tokens[index]};
        std::cout<<factor->token.value<<std::endl;
        Tokens.erase(Tokens.begin()+index);
        Node.children.push_back(factor);
        std::cout<<factor->token.value<<std::endl;
    }
}

internal void createOperator(std::vector<Token> &Tokens, ParseNode &Node, int index){
    ParseNode * oper = new ParseNode{Tokens[index]};
    TokenType currentToken = Tokens[index].type;

    if( currentToken == ADD || 
        currentToken == SUB || 
        currentToken == MULTIPLY || 
        currentToken == DIVIDE || 
        currentToken == MOD){
        Node.children.push_back(oper);
        Tokens.erase(Tokens.begin()+index);
    }else{
        std::cerr<<"Not an operator: "<<Tokens[index].value<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout<<oper->token.value<<std::endl;
}

internal void createSemicolon(std::vector<Token> &Tokens, ParseNode &Node, int index){
    if(Tokens[index].type==TokenType::SEMICOLON){
        ParseNode * semicolon = new ParseNode{Tokens[index]};
        std::cout<<semicolon->token.value<<std::endl;
        Tokens.erase(Tokens.begin()+index);
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

    std::vector<Token> Tokens = getTokens(file);
    ParseNode* parseTree = new ParseNode{Token{TokenType::TREE, "Tree"}};
    createTree(Tokens, parseTree, 0);

    std::cout<<"\n\n\n"<<std::endl;
    printParseTree(parseTree, 0);

    file.close();

    return EXIT_SUCCESS;
}