// g++ compiler.cpp -o compiler
// ./compiler.exe

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>

#define internal static
#define global static
#define local_persist static

enum TokenType{
    RETURN,
    EXIT,
    IDENT,
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

std::vector<Token> getTokens(std::ifstream &file){
    std::vector<Token> Tokens = {};
    char ch;
    std::stringstream buffer;
    while(file.peek()!=EOF){
        ch = file.peek();
        if (std::isalpha(ch)){
            while(std::isalnum(file.peek())){
                file.get(ch);
                buffer << ch;
            }
            //Check for a alnum token (ex. return, for, if)
            if(buffer.str()=="return"){
                Tokens.push_back(Token{TokenType::RETURN, "RETURN"});
            }else if(buffer.str()=="exit"){
                Tokens.push_back(Token{TokenType::EXIT, "EXIT"});
            }else if(buffer.str()=="if"){
                Tokens.push_back(Token{TokenType::IF, "IF"});
            }else if(buffer.str()=="else"){
                Tokens.push_back(Token{TokenType::ELSE, "ELSE"});
            }else if(buffer.str()=="for"){
                Tokens.push_back(Token{TokenType::FOR, "FOR"});
            }else if(buffer.str()=="int"){
                Tokens.push_back(Token{TokenType::INT, "INT"});
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
/*
std::string peek(const std::ifstream &file){
    
}

std::string read(const std::ifstream &file){

}
*/
internal void readFile(std::ifstream &file){
    std::string line;
    while(std::getline(file,line)){
        std::cout<<line<<std::endl;
    }
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

    //readFile(file);
    //std::ifstream file("input.txt");
    //std::ofstream out("test.asm");

    file.close();

    return EXIT_SUCCESS;
}