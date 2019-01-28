#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct lexer
{
    string token;
    string lexeme;
};

vector<lexer> v;
int counter = 0;

void Rat18S();
void Opt_Function_Definitions();
void Opt_Declaration_List();
void Statement_List();
void Function_Definitions();
void Opt_Parameter_List();
void Body();
void Parameter_List();
void Parameter();
void Parameter_List_Prime();
void IDS();
void Qualifier();
void IDS_Prime();
void Declaration_List();
void Declaration();
void Declaration_List_Prime();
void Function();
void Function_Definitions_Prime();
void Statement();
void Statement_List();
void Statement_List_Prime();
void Compound();
void Assign();
void IF();
void Return();
void Print();
void Scan();
void While();
void Expression();
void Condition();
void IF_Prime();
void Term();
void Return_Prime();
void Expression_Prime();
void Factor();
void Term_Prime();
void Primary();
void Relop();
bool Empty(int);

int main()
{
    fstream file;
    file.open("output.txt", ios::in);
    string t, l, temp;
    file >> temp;
    file >> temp;
    file >> temp;
    while (file >> t >> l)
    {
        lexer lex;
        lex.token = t;
        lex.lexeme = l;
        v.push_back(lex);
    }
    while (counter+1 != v.size())
    {
        Rat18S();
    }
	return 0;
}

//<Rat18S> → <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>
void Rat18S()
{
    try
    {
        Opt_Function_Definitions();
        counter++;
    }
    catch (const char *msg)
    {
        cerr << msg << endl;
    }
    if (v[counter].token == "Separator" && v[counter].lexeme == "%%")
    {
        try
        {
            Opt_Declaration_List();
            Statement_List();
        }
        catch (const char *msg)
        {
            cerr << msg << endl;
        }
    }
}
//<Opt Function Definitions> → <Function Definitions>  |  ε
void Opt_Function_Definitions()
{
    if (v[counter].token == "Keyword" && v[counter].lexeme == "function") // epsilon
    {
        Function_Definitions();
    }
}

//<Function Definitions> → <Function> <Function Definitions_Prime>
void Function_Definitions()
{
    Function();
    Function_Definitions_Prime();
}

//<Function> →  function  <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body>
void Function()
{
    if (v[counter].token == "Keyword" && v[counter].lexeme == "function")
    {
        counter++;
        if (v[counter].token == "Identifier")
        {
            counter++;
            if (v[counter].token == "Separator" && v[counter].lexeme == "[")
            {
                Opt_Parameter_List();
                counter++;
                if (v[counter].token == "Separator" && v[counter].lexeme == "]")
                {
                    Opt_Declaration_List();
                    Body();
                }
            }
            else
            {
                throw "Error at Function()";
            }
        }
        else
        {
            throw "Error at Function()";
        }
    }
    else
    {
        throw "Error at Function()";
    }
}

//<Function Definitions_Prime> → ε | <Function Definitions>

void Function_Definitions_Prime()
{
    if (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "function")
    {
        Function_Definitions();
    }
}

//<Opt Parameter List> →   <Parameter List> | ε
void Opt_Parameter_List()
{
    if (v[counter + 1].token == "Identifier") //epsilon
    {
        Parameter_List();
    }
}

//<Parameter List> → <Parameter> <Parameter List_Prime>
void Parameter_List()
{
    Parameter();
    Parameter_List_Prime();
}

//<Parameter> →  <IDs > : <Qualifier>
void Parameter()
{
	counter++;
    IDS();
    counter++;
    if (v[counter].token == "Separator" && v[counter].lexeme == ":")
    {
        Qualifier();
    }
}

//<Parameter List_Prime> → ε | , <Parameter List>
void Parameter_List_Prime()
{
    if (v[counter + 1].token == "Separator" && v[counter + 1].lexeme == ",")
    {
        counter++;
        Parameter_List();
    }
}

//<IDs> → <Identifier> <IDs_Prime>
void IDS()
{
    if (v[counter].token == "Identifier")
    {
        IDS_Prime();
    }
    else
    {
        throw "Error at IDS()";
    }
}

//<IDs_Prime> → , <IDs>  |  ε
void IDS_Prime()
{

    if (v[counter + 1].token == "Separator" && v[counter + 1].lexeme == ",")
    {
        counter+=2;
        IDS();
    }
}

//<Qualifier> → int  |  boolean    |  real
void Qualifier()
{
    counter++;
    if (v[counter].token == "Keyword" && (v[counter].lexeme == "int" || v[counter].lexeme == "boolean" || v[counter].lexeme == "real"))
    {
        return;
    }
    throw "Error at Qualifier()";
}

//<Opt Declaration List> → <Declaration List>   |   ε
void Opt_Declaration_List() //Hard coded checking {
{
    if (v[counter + 1].token == "Keyword" && (v[counter + 1].lexeme == "int" || v[counter + 1].lexeme == "boolean" || v[counter + 1].lexeme == "real")) //Epsilon from qualifier
    {
        Declaration_List();
    }
}

//<Declaration List> → <Declaration> ; <Declaration List_Prime>
void Declaration_List()
{
    Declaration();
    counter++;
    if (v[counter].token == "Separator" && v[counter].lexeme == ";")
    {
        Declaration_List_Prime();
    }
    else
    {
        throw "Error at Declaration_List()";
    }
}

//<Declaration List_Prime> → ε | <Declaration List>
void Declaration_List_Prime()
{
    if (v[counter + 1].token == "Keyword" && (v[counter + 1].lexeme == "int" || v[counter + 1].lexeme == "boolean" || v[counter + 1].lexeme == "real")) //Not a declaration list, Epsilon, HARD CODED THE "{"
    {
        Declaration_List();
    }
}

//<Declaration> →  <Qualifier> <IDs>
void Declaration()
{
    Qualifier();
	counter++;
    IDS();
}

//<Body> → {  < Statement List>  }
void Body()
{
    counter++;
    if (v[counter].token == "Separator" && v[counter].lexeme == "{")
    {
        Statement_List();
    }
    else
    {
        throw "Error at Body()";
        return;
    }
    counter++;
    if (!(v[counter].token == "Separator" && v[counter].lexeme == "}"))
    {
        throw "Error at Body()";
    }
}

//<Statement List> → <Statement> <Statement List_Prime>
void Statement_List()
{
    Statement();
	if (Empty(counter))
	{
		return;
	}
    Statement_List_Prime();
}

//<Statement> ->   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
void Statement()
{
    if (v[counter + 1].token == "Separator" && v[counter + 1].lexeme == "{")
    {
        Compound();
    }
    else if (v[counter + 1].token == "Identifier")
    {
        Assign();
    }
    else if (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "if")
    {
        IF();
    }
    else if (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "return")
    {
        Return();
    }
    else if (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "put")
    {
        Print();
    }
    else if (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "get")
    {
        Scan();
    }
    else if (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "while")
    {
        While();
    }
    else
    {
        throw "Error at Statement";
    }
}

//<Compound> ->   {  <Statement List>  }
void Compound()
{
    counter++;
    if (v[counter].token == "Separator" && v[counter].lexeme == "{")
    {
        Statement_List();
    }
    else
    {
        throw "Error at Compound()";
        return;
    }
    counter++;
    if (!(v[counter].token == "Separator" && v[counter].lexeme == "}"))
    {
        throw "Error at Compound()";
    }
}

//<Assign> -> 	<Identifier> = <Expression> ;
void Assign()
{
    counter++;
    if (v[counter].token == "Identifier")
    {
        counter++;
        if (v[counter].lexeme == "=")
        {
			counter++;
            Expression();
            counter++;
            if (!(v[counter].token == "Separator" && v[counter].lexeme == ";"))
            {
                throw "Error at Assign()";
            }
        }
        else
        {
            throw "Error at Assign()";
        }
    }
    else
    {
        throw "Error at Assign()";
    }
}

//<If> → if (<Condition>) <Statement> <If_Prime>
void IF()
{
    counter++;
    if (v[counter].token == "Keyword" && v[counter].lexeme == "if")
    {
        counter++;
        if (v[counter].token == "Separator" && v[counter].lexeme == "(")
        {
            Condition();
            counter++;
            if (v[counter].token == "Separator" && v[counter].lexeme == ")")
            {
                Statement();
                IF_Prime();
            }
            else
            {
                throw "Error at IF()";
            }
        }
        else
        {
            throw "Error at IF()";
        }
    }
    else
    {
        throw "Error at IF()";
    }
}

//<If_Prime> → endif | else <Statement> endif
void IF_Prime()
{
    counter++;
    if (v[counter].token == "Keyword" && v[counter].lexeme == "endif")
    {
        return;
    }
    else if (v[counter].token == "Keyword" && v[counter].lexeme == "else")
    {
        Statement();
        counter++;
        if (v[counter].token != "Keyword" && v[counter].lexeme != "endif")
        {
            throw "Error at IF_Prime()";
        }
    }
    else
    {
        throw "Error at IF_Prime()";
    }
}

//<Return> → return <Return_Prime>
void Return()
{
    counter++;
    if (v[counter].token == "Keyword" && v[counter].lexeme == "return")
    {
        Return_Prime();
    }
    else
    {
        throw "Error at Return()";
    }
}

//<Return_Prime> → ; | <Expression> ;
void Return_Prime()
{
    counter++;
    if (v[counter].token == "Separator" && v[counter].lexeme == ";")
    {
        return;
    }
    else if (v[counter].lexeme == "-" || v[counter].token == "Identifier" || v[counter].token == "Integer" || v[counter].lexeme == "(" || v[counter].token == "Real" ||
             (v[counter].token == "Keyword" && (v[counter].lexeme == "true" || v[counter].lexeme == "false")))
    {
        Expression();
        counter++;
        if (v[counter].token != "Separator" && v[counter].lexeme != ";")
        {
            throw "Error at Return_Prime()";
        }
    }
    else
    {
        throw "Error at Return_Prime()";
    }
}

//<Print> ->	put ( <Expression>);
void Print()
{
    counter++;
    if (v[counter].token == "Keyword" && v[counter].lexeme == "put")
    {
        counter++;
        if (v[counter].token == "Separator" && v[counter].lexeme == "(")
        {
			counter++;
            Expression();
			counter++;
            if (v[counter].token == "Separator" && v[counter].lexeme == ")")
            {
                counter++;
                if (v[counter].token != "Separator" && v[counter].lexeme != ";")
                {
                    throw "Error at Print()";
                }
            }
            else
            {
                throw "Error at Print()";
            }
        }
        else
        {
            throw "Error at Print()";
        }
    }
    else
    {
        throw "Error at Print()";
    }
}

//<Scan> ::=	get ( <IDs> );
void Scan()
{
    counter++;
    if (v[counter].token == "Keyword" && v[counter].lexeme == "get")
    {
        counter++;
        if (v[counter].token == "Separator" && v[counter].lexeme == "(")
        {
			counter++;
            IDS();
            counter++;
            if (v[counter].token == "Separator" && v[counter].lexeme == ")")
            {
                counter++;
                if (v[counter].token != "Separator" && v[counter].lexeme != ";")
                {
                    throw "Error at Print()";
                }
            }
            else
            {
                throw "Error at Print()";
            }
        }
        else
        {
            throw "Error at Print()";
        }
    }
    else
    {
        throw "Error at Print()";
    }
}

//<While> ->  while ( <Condition>  )  <Statement>
void While()
{
    counter++;
    if (v[counter].token == "Keyword" && v[counter].lexeme == "while")
    {
        counter++;
        if (v[counter].token == "Separator" && v[counter].lexeme == "(")
        {
			counter++;
            Condition();
            counter++;
            if (v[counter].token == "Separator" && v[counter].lexeme == ")")
            {
                Statement();
            }
            else
            {
                throw "Error at Print()";
            }
        }
        else
        {
            throw "Error at Print()";
        }
    }
    else
    {
        throw "Error at Print()";
    }
}

// <Expression> → <Term> <Expression_Prime>
void Expression()
{
    Term();
    Expression_Prime();
}

//<Term> → <Factor><Term_Prime>
void Term()
{
    Factor();
    Term_Prime();
}

//<Factor> →   	-  <Primary> | <Primary>
void Factor()
{
    if (v[counter + 1].lexeme == "-")
    {
        counter+=2;
        Primary();
    }
    else
    {
        Primary();
    }
}

//<Primary> →     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  <Real>  |   true   |  false
void Primary()
{
	if (v[counter].token == "Identifier" && v[counter + 1].lexeme == "(")
	{
		counter+=2;
		IDS();
		counter++;
		if (v[counter].lexeme != ")")
		{
			throw "Error at Primary() ah";
			return;
		}
	}
	else if (v[counter].token == "Identifier")
	{
		return;
	}
    else if (v[counter].token == "Integer")
    {
        return;
    }
    else if (v[counter].lexeme == "(")
    {
		counter++;
        Expression();
        counter++;
        if (v[counter].lexeme != ")")
        {
            throw "Error at Primary() lo";
            return;
        }
    }
    else if (v[counter].token == "Real")
    {
        return;
    }
    else if (v[counter].token == "Keyword" && (v[counter].lexeme == "true" || v[counter].lexeme == "false"))
    {
        return;
    }
    else
    {
        throw "Error at Primary() ahhh";
    }
}

//<Term_Prime> → * <Factor> <Term_Prime> | / <Factor> <Term_Prime> | ε
void Term_Prime()
{

    if (v[counter+1].lexeme == "*" || v[counter+1].lexeme == "/")
    {
		counter+=2;
        Factor();
		Term_Prime();
    }
}

//<Expression_Prime>→ + <Term><Expression_Prime> | - <Term><Expression_Prime>|ε
void Expression_Prime()
{

    if (v[counter + 1].lexeme == "+" || v[counter + 1].lexeme == "-")
    {
        counter+=2;
        Term();
        Expression_Prime();
    }
}

//<Statement List_Prime> → ε | <Statement List>
void Statement_List_Prime()
{

    if ((v[counter + 1].token == "Separator" && v[counter + 1].lexeme == "{") ||
        (v[counter + 1].token == "Identifier") ||
        (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "if") ||
        (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "return") ||
        (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "put") ||
        (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "get") ||
        (v[counter + 1].token == "Keyword" && v[counter + 1].lexeme == "while"))
    {
        Statement_List();
    }
}

//<Condition> → <Expression>  <Relop>   <Expression>
void Condition()
{
    Expression();
    Relop();
    Expression();
}

//<Relop> →    	==   |   ^=	|   >     |   <	|   =>	|   =<
void Relop()
{
    counter++;
    if (v[counter].lexeme == "==" || v[counter].lexeme == "^=" || v[counter].lexeme == ">" || v[counter].lexeme == "<" || v[counter].lexeme == "=>" || v[counter].lexeme == "=<")
    {
		counter++;
        return;
    }
    else
    {
        throw "Error at Relop()";
    }
}

bool Empty(int counter)
{
	int _counter = v.size();
	if (counter + 1 == _counter)
	{
		return true;
	}
	return false;
}
