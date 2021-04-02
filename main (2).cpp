//By: Ben Lanoue, Kristin Tragarz, Nick Dea
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;

class Expr;
class Stmt;

// Runtime Global Variables
int pc;  				// program counter
vector<string> lexemes; 
vector<string> tokens; 
vector<string>::iterator lexitr;
vector<string>::iterator tokitr;
map<string, int> vartable;              // map of variables and their values
vector<Stmt *> insttable; 		// table of instructions
map<string, string> symboltable;        // map of variables to datatype (i.e. sum t_integer)

class Expr{ // expressions are evaluated!
public:
	virtual int eval() = 0;
	virtual string toString() = 0;
	virtual ~Expr(){}
};


//Kristin Tragarz
class ConstExpr : public Expr{
private:
	int value;
public:
        //pre: 1st parameter is a valid number or boolean from file
        //post: ConstExpr is initialized with the number or boolean
	ConstExpr(int val){
            value = val;
        };
        
        //pre: none
        //post: number or boolean is returned
	int eval(){
            return value;
        };
        
        //pre: none
        //post: Expr contents are converted to a string and returned
	string toString(){
            string temp;
            temp = "" + value;
            return(temp);
        };   
};

//Ben Lanoue
class IdExpr : public Expr{
private:
	string id;
public:
        //pre: valid variable name as s
        //post: id is initiated to s
    
	IdExpr(string s){
            id = s;
        }
        //pre:id needs to be in the vartable map
        //post: returns the value of id from vartable map
        
	int eval(){
            map<string,int>::iterator a = vartable.find(id);
            return a->second;
        }
        
        //pre:none
        //post:returns a string that shows id value and its value
        //on the vartable map
        
	string toString(){
            string asnString = "id: " + id + " " + "value: " + to_string(eval()) + "\n";
            return asnString;
        }
};


//Nick Dea
class InFixExpr : public Expr {
private:
    vector<Expr *> exprs;
    vector<string> ops; // tokens of operators
    vector<Expr *>::iterator exprsitr;
    vector<string>::iterator opsitr;
public:
    // pre:Valid expression pointer vector and vector of strings containing operators
    // post:InFixExpr is initialized
    InFixExpr(vector<Expr *> exp, vector<string> o) {
        exprs = exp;
        ops = o;
        exprsitr = exprs.begin();
        opsitr = ops.begin();
    }
    // pre:none
    // post:Deletes the expression pointers

    ~InFixExpr() {
        for (int i = 0; i < exprs.size(); i++) {
            delete exprs.at(i);
        }
    }
    // pre:none
    // post:Evaluates the infix expression(s); returning an integer.

    int eval() {
        int result = (**exprsitr).eval();
        exprsitr++;
        while (exprsitr != exprs.end()) {
            if (*opsitr == "+") {
                result = result + (**exprsitr).eval();
                exprsitr++;
                opsitr++;
            } else if (*opsitr == "-") {
                result = result - (**exprsitr).eval();
                exprsitr++;
                opsitr++;
            } else if (*opsitr == "*") {
                result = result * (**exprsitr).eval();
                exprsitr++;
                opsitr++;
            } else if (*opsitr == "/") {
                result = result / (**exprsitr).eval();
                exprsitr++;
                opsitr++;
            } else if (*opsitr == "%") {
                result = result % (**exprsitr).eval();
                exprsitr++;
                opsitr++;
            }
        }
        return result;
    }

    // pre:none
    // post:Prints the expressions, operators, and what the result is.

    string toString() {
        string expressions = "";
        for (int i = 0; i < exprs.size(); i++) {
            expressions = expressions + " " + to_string((*exprs.at(i)).eval());
        }
        string operators = "";
        for (int i = 0; i < ops.size(); i++) {
            expressions = expressions + " " + ops.at(i);
        }
        return "Expressions: " + expressions +
                "\n" + "Operators: " + operators
                + "\n" + "After eval(): " + to_string(eval());

    }
};
// Runtime Global Methods

// prints vartable, instable, symboltable				

class Stmt{ // statements are executed!
private:
	string name;
public:
        Stmt(){};
	Stmt(string n){
            name = n;
        }
	virtual ~Stmt(){};
	virtual string toString() = 0;
	virtual void execute() = 0;
        void setName(string n){
            name = n;
        }
        string getName(){
            return name;
        }
};


//Kristin Tragarz
class AssignStmt : public Stmt{
private:
	string var;
	Expr* p_expr;
public:
	//pre: none
        //post: AssignStmt object is initialized 
        AssignStmt(){};
        
	//pre: 1st parameter is valid token name
        //post: AssignStmt object is initialized with a name
        AssignStmt(string n):Stmt(n)
        {}
        
        //pre: 1st parameter is valid token name, 2nd parameter is valid id name, 3rd parameter is
        //      a created expr pointer 
        //post: AssignStmt object is initialized
        AssignStmt(string n, string v, Expr* p):Stmt(n){
            var = v;
            p_expr = p;
        }
        
        //pre: none
        //post: allocated memory for p_expr is released
        ~AssignStmt(){
            delete p_expr;
        }
        
        //pre: none
        //post: contents of assignment statement instruction are converted to a string and returned
	string toString(){
            string asnString;
            asnString = "name: " + getName() + " var: " + var + " expr: " + p_expr->toString() + "\n";
            return(asnString);
        }
        
        //pre: none
        //post: assignment statement is executed
	void execute(){
            int temp = p_expr->eval();
            vartable.erase(var);
            vartable.insert({var, temp});
        }
};




//Ben Lanoue
class InputStmt : public Stmt{
private:
	string var;
public:
        //pre: valid variable name, valid token
        //post: initializes name and var
    
	InputStmt(string n, string v):Stmt(n)
        {
            var = v;
        }
        //pre: none
        //post: none
        
	~InputStmt(){}
        
        //pre:none
        //post:returns the values of name and var into a string
        
	string toString(){
            string asnString = "name: " + getName() + " var: " + var;
            return asnString;

        }
        
        //pre: name and var initialized, var is on the vartable map
        //post: the key var will have a value of of whatever int
        //the user inputs in vartable
        
	void execute(){
            int temp;
            cin >> temp;
            vartable.erase(var);
            vartable.insert({var,temp});
        }
};

//Nick Dea
class StrOutStmt : public Stmt {
private:
    string value;
public:

    // pre:Valid token name and value
    // post:StrOutStmt is initialized

    StrOutStmt(string n, string v):Stmt(n)
    {
        value = v;
    }
    // pre:none
    // post:none

    ~StrOutStmt() {

    }
    // pre:none
    // post:returns a string with the name and value of the statement   

    string toString() {
        return "Name: " + getName() + " Value: " + value + "\n";
    }
    // pre:none
    // post:prints out the value to the console

    void execute() {
        cout << value << endl;
    }
};

//Ben Lanoue
class ExprOutStmt : public Stmt{
private:
	Expr* p_expr;
public:
        //pre:valid name token, valid expression   
        //post: initializes name and p_expr
    
	ExprOutStmt(string n, Expr* p):Stmt(n)
        {
            p_expr = p;
        }
        //pre: p_expr points to an Expr object
        //post: p_expr deallocates the memory of p_expr
        
	~ExprOutStmt(){
            delete p_expr;
        }
        //pre: none
        //post: returns a string with the value of name and
        //p_expr values
        
	string toString(){
            string asnString;
            asnString = "name: " + getName() + " " + p_expr->toString();
        }
        
        //pre: none
        //post: sets p_expr to p
        
        void setPointer(Expr* p){
            p_expr = p;
        }
        
        //pre: p_expr is valid
        //post: p_expr prints out 0 or 1 (false or true respectivaly)
        
	void execute(){
            cout << (p_expr->eval()) << endl;
        }
};

//Kristin Tragarz
class IfStmt : public Stmt{
private:
	Expr* p_expr;
	int elsetarget;
public:
        //pre: 1st parameter is valid token name and 2nd parameter is a Expr pointer already created
        //post: IfStmt is initialized
	IfStmt(string n, Expr* p):Stmt(n)
        {
            p_expr = p;
        }
	
        //pre: none
        //post: pointer memory is released
        ~IfStmt(){
            delete p_expr;
        }
	
        //pre: none
        //post: elsetarget is returned
        int getTarget(){
           return elsetarget; 
        }
        
        //pre: 1st parameter is a positive integer
        //post: elsetarget is set
        void setTarget(int t){
            elsetarget = t;
        }
        
        //pre: none
        //post: content of IfStmt is converted to string and returned
        string toString(){
            string ifStmt;
            ifStmt = "name: " + getName() + " p_expr: " + p_expr->toString() + " target: " + to_string(elsetarget) + "\n";
        }
        
        //pre: none
        //post: expression is evaluated and returned to call if true to continue else set the pc to the elsetarget
	void execute(){
            if(p_expr->eval() != 1)
                pc = elsetarget;
        }
};

//Nick Dea
class WhileStmt : public Stmt {
private:
    Expr* p_expr;
    int elsetarget;
public:
    // pre:Valid token name and expression pointer
    // post:StrOutStmt is initialized
    WhileStmt(string n, Expr* p):Stmt(n)
    {
        p_expr = p;
    }
    // pre:none
    // post:frees up the expression pointer

    ~WhileStmt() {
        delete p_expr;
    }
    // pre:none
    // post:prints the name, expression, and elsetarget

    string toString() {
        return "Name: " + getName() + " Expression: " + p_expr->toString() + " Target: " + to_string(elsetarget);
    }
    // pre:none
    // post:returns a boolean if the while statement can continue
    // and moves the program counter back to elsetarget 

    void execute() {
        if (p_expr->eval() != 1)
            pc = elsetarget;
    }
    // pre:valid expression pointer
    // post:sets p_expr to the input pointer

    void setPointer(Expr* p) {
        p_expr = p;
    }
    // pre:valid elsetarget
    // post:sets the elsetarget to the input elsetarget

    void setTarget(int t) {
        elsetarget = t;
    }
    // pre:none
    // post:returns the elsetarget

    int getTarget() {
        return elsetarget;
    }
};


class Compiler{
private:
	//Kristin Tragarz
        //pre: none
        //post: an IfStmt object is created and added to the instruction table
        void buildIf(){
            tokitr++; lexitr++;
            if(*tokitr != "s_lparen")
                return;
            
            string name = "t_if";
            string express = "";

            tokitr++; lexitr++;
            while(*tokitr != "s_rparen"){
                express = express + *lexitr;
                tokitr++;
                lexitr++; 
            }
           
            Expr* exP = buildExpr(express);
            IfStmt* ifs = new IfStmt(name,exP);
            insttable.push_back(ifs);
            
            return;
        }
   
	//Nick Dea
        //pre:None
        //post:Creates a WhileStmt object and adds it to the instruction table

        void buildWhile() {
            tokitr++;
            lexitr++;
            if (*tokitr != "s_lparen") {
                return;
            }
            string name = "t_while";
            string express = "";

            tokitr++;
            lexitr++;
            while (*tokitr != "s_rparen") {
                express = express + *lexitr;
                tokitr++;
                lexitr++;
            }
            Expr* exprP = buildExpr(express);
            WhileStmt* whs = new WhileStmt(name, exprP);
            insttable.push_back(whs);
        }
        /*
        //Ben Lanoue
        //pre: a valid token name
        //post: returns a Stmt object
	void buildStmt(){ // parent class, declares name
            string b = *tokitr;
            Stmt p = Stmt(b);
            return p;
        }*/
	
        //Kristin Tragarz
        //pre: none
        //post: an AssignStmt object is created and added to the instruction table
        void buildAssign(){
            string var = *tokitr--;
            string name = "s_assign";
            
            tokitr++;
            lexitr++;
            
            string express = "";
            while(*tokitr != "s_semi"){
                express = express + *lexitr;
                tokitr++;
                lexitr++;
            }
            
            Expr* exP = buildExpr(express);
            AssignStmt* as = new AssignStmt(name,var,exP);
            insttable.push_back(as);
        }
        
        //Ben Lanoue
	//pre: none
	//post: makes a InputStmt object and puts it in the instruction vector
	void buildInput(){
            lexitr++;lexitr++;
            tokitr++;tokitr++;
            string b = *lexitr;
            
            InputStmt* p = new InputStmt("t_input", b);
            insttable.push_back(p);
            
        }
        
        //Kristin Tragarz
        //pre: none
        //post: either an ExprOutout or StmtOutput object is created and added to the instruction table
	void buildOutput(){
            //skip past left paren
            tokitr++; lexitr++;
            tokitr++; lexitr++;
 
            
            string express = "";
            //now at inside the ()
            while(*tokitr != "s_rparen"){
                express = express + *lexitr;
                tokitr;
                lexitr;
            }
            
            //determine which type of statement is contained within the ()
            bool whatExprs = isOperator(express);
            if(whatExprs == true){
                StrOutStmt* stmtOut = new StrOutStmt("t_output", express);
                insttable.push_back(stmtOut);
            }
            else{
                Expr* press = buildExpr(express);
                ExprOutStmt* expOut = new ExprOutStmt("t_output",press);
                insttable.push_back(expOut);
            }
        }
        
        //Kristin Tragarz
        //pre: string is comprised of letters/numbers within the () of an output statement
        //post: returns false if an operator is found, true otherwise
        bool isOperator(string s){
            bool check;
            if (s[0] == '>' || '<' || '+' || '-' || '*' || '/' ){
                check = false;
                return check;
            }
            else{
                string substr = "";
                int size = s.size();
                for(int i = 1; i < size; i++){
                    substr = substr + s[i];
                }
                check = true;
                return(isOperator(substr));
            }
            return check;
        }
	
	//Ben Lanoue
        //pre: string b, which is the expression
        //post: returns a pointer to a Expr object this method made
	Expr* buildExpr(string b){
            int i = 0;
            map<string,int>::iterator v;
            v = vartable.find(b);
            bool isConst = true;
            bool isVar = (v != vartable.end());
            bool isInFix = false;
            while(i != b.size()) {
               
                if(b.at(i) < 48 && b.at(i) > 57){
                     isConst = false;
                }   
                i++;
            }
            
            string::size_type sz;   // alias of size_t

            
            if(isConst){
                int b2 = stoi (b,&sz);
                //int b2 = (int) b;
                Expr* p = new ConstExpr(b2);
                return p;
            }
            else if(isVar){
                Expr* p = new IdExpr(b);
                return p;
            }
            else{
                //This does not work, but returns 0/false
                //so the code continues
                Expr* p = new IdExpr("0");
                return p;
                
            }
        }
	
        //Kristin, Ben, Nick
        //pre: 1st parameter to be a valid, open file
        //post: token/lexeme vectors are created
        void populateTokenLexemes(istream& infile){
            string line, tok, lex;
            int pos;
            getline(infile, line);

            while (!infile.eof()) {
                pos = line.find(":");
                tok = line.substr(0, pos - 1);
                lex = line.substr(pos + 2, line.length() - pos - 2);
                tokens.push_back(tok);
                lexemes.push_back(lex);
                getline(infile, line);
            }
            tokitr = tokens.begin();
            lexitr = lexemes.begin();
            
        }
        
        //Kristin, Ben, Nick
        //pre: 1st parameter needs to be a valid, open file
        //post: symboltable is created
	void populateSymbolTable(istream& infile){
            string line, key, value;
            int pos;
            getline(infile, line);

            while (!infile.eof()) {
                pos = line.find(":");
                value = line.substr(0, pos - 1);
                key = line.substr(pos + 2, line.length() - pos - 2);
                symboltable.insert({key,value});

                getline(infile, line);
            }
        }

public:
	Compiler(istream& source, istream& symbols){
            populateTokenLexemes(source);
            populateSymbolTable(symbols);
	}	
        
        //Kristin Tragarz
        //pre: tok/lex vectors and symboltable has been constructed
        //post: instruction table is built
	bool compile(){
            //Iterates until we hit begin on the vectors. 
            while((*tokitr) != "t_begin"){
                tokitr++;
                lexitr++;
            }
            tokitr++;
            lexitr++;
            
            //starts with the first line after begin, checks cases, builds 
            while(tokitr != tokens.end()){
                if((*tokitr) == "s_assign"){
                    buildAssign();
                }
                
                if((*tokitr) == "t_if"){
                    buildIf();
                }
                
                if((*tokitr) == "t_while"){
                    buildWhile();
                }
                if((*tokitr) == "t_output"){
                    buildOutput();
                }
                if((*tokitr) == "t_input"){
                    buildInput();
                }
                tokitr++;
                lexitr++;
            }
            return true;
        }  

	//Ben Lanoue
	//pre: none
	//post: runs the insttable
	void run(){             // executes the instruction table
            pc = 0;
            for (pc; pc < insttable.size(); pc++){
                
                
                if((insttable.at(pc)->getName()).compare("t_if") == 0){
                    (*insttable.at(pc)).execute();
                    pc--;
                }
                else if (insttable.at(pc)->getName().compare("t_while") == 0){
                    (*insttable.at(pc)).execute();
                     pc--;       
                }
                else{
                    (*insttable.at(pc)).execute();
                }
            }
        }	
        
};

// Nick Dea
// pre:none
// post:prints vartable, instable, symboltable
void dump() {
    map<string, int>::iterator vtabitr = vartable.begin();
    vector<Stmt *>::iterator itabitr = insttable.begin();
    map<string, string>::iterator stabitr = symboltable.begin();

    cout << endl;
    cout << "Variable table:" << endl;
    while (vtabitr != vartable.end()) {
        cout << vtabitr->first << " " << vtabitr->second << endl;
        vtabitr++;
    }
    cout << endl;
    cout << "Instruction table:" << endl;
    while (itabitr != insttable.end()) {
        cout << (*itabitr)->toString() << endl;
        itabitr++;
    }
    cout << endl;
    cout << "Symbol table:" << endl;
    while (stabitr != symboltable.end()) {
        cout << stabitr->first << " " << stabitr->second << endl;
        stabitr++;
    }
    return;
}

//Kristin Tragarz
int main(int argc, char** argv) {
    string sourceF;
    string symbolF;
    
    cout << "Enter the name of the source code file: ";
    cin >> sourceF;
    
    ifstream sourcefile(sourceF);
    if(!sourcefile){
        cout << "File could not be found. Ending program" << endl;
        exit(-1);
    }
    
    cout << "\nEnter the name of the symbol table file: ";
    cin >> symbolF;
    
    ifstream symbolfile(symbolF);
    if(!symbolfile){
        cout << "File could not be found. Ending program" << endl;
        exit(-1);
    }
    
    Compiler c(sourcefile, symbolfile);
    c.compile();
    c.run();
    
    dump();
    return 0;
    
}
