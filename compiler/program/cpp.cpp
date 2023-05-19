#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;
//////////////////////////////////////////////////////////////////
//                   SEMANTIC ANALYZER                           /
//////////////////////////////////////////////////////////////////
int Scope;
struct test
{
	string ST;
	int scope = 0;

} SymbolTable[100];

//////////////////////////////////////////////////////////////////
//                    LEXICAL ANALYZER                           /
//////////////////////////////////////////////////////////////////
bool Operator_check(string str);		  // to check for all the assignment, logical and mathematical operators
										  // "+","-","*","/","%","^","++","--","+=","-=","=",">","<","<=",">=","!","!=","==","&","|","->","<-"
string Operator_tokenization(string str); // after being checked by the operator_check() go for Operator_check()
										  // for futther specification and to get a token  in return
bool Punctuator_Check(string str);		  // to check for linebreak
										  // "[","]","(",")","{","}",";",".",":",","

// tokenization of loops conditions and statements (identifiers, keywords, return statements ) are been done in main area
int Keyword_check(string str);			 // for checking if the element is from the defined keywords
										 // KEYWORD[18] = { "ForLP","WhileLP","TR","FL","choose","From","default","func","Structure","void","return"
										 // ,"Stop","main","begin","in_put","out_put" }
bool Real_no_validation(string str);	 // for validating the constant numbers
bool Decimal_no_validation(string str);	 // for validating the floating constant numbers
bool Validating_identifiers(string str); // for validating the identifier according to the rules defined
										 // $ denotes the start of the identifier
										 // alphabets and charachers are valid including _
bool character_validation(string str);	 // validate the character
bool IS_Current_index_num(char s);		 // valiate if current index is integer
//////////////////////////////////////////////////////////////////
//                    SYNTAX ANALYZER                            /
//////////////////////////////////////////////////////////////////
bool main_function(); // PRE_Main()   main int begin () { //body }  POST_Main()  @
bool PRE_Main();	  // only retuns true when main occurs, it checks for Structure, decleration, any function decleration and function before main
bool POST_Main();	  // returns true when @ occur, checks for any function present after main, all the function definition must be before @
bool Struct();		  // Structure identifier { // declerations from decl() }
bool G_decl();		  // <decl>
//----------------------------------------------CGF OF EXPRESSION GRAMMAR------------------------------------------
//-----------------BELOW ARE THE RECURSIVE DESCENDENT PARSER FUNCTION FOR THE VALIDATION OF FOLLOWING--------------
//***Validation of ADDSUB, DIVMUL, MOD, AND, OR, NOT, INCDEC, RELATIONAL OPERATORS (==, !=, >=, <=), CONST, IDs ***
//**********************************************ALSO INCUDE ;  )  ,  }  *******************************************
//-----------------------------------------------------------------------------------------------------------------
bool PARAMETER_2(); // <E> <PARAMETER_3>
					// )
					// part of parameter passing validation -------->{ checks for ) }----> ending of the parameter passing ---------> )
bool PARAMETER_3(); // This is just to remove the left recursion in prmtrs2
					// , <E> <PARAMETER_3>
					// )
					// find , incase of multiple parameter are been passes and ends at )
bool E();			// <T> <E_PRIME>
bool E_PRIME();		// AND_SUB_OP <T> <E_p>
					// And_Operator
					// Or_Operator
					// ;
					// )
					// ,
					// }
					// Relational_operator
bool T();			// <F> <T_PRIME>
bool T_PRIME();		// <DIVMULMOD> <F> <T_PRIME>
					// And_Operator
					// Or_Operator
					// ;
					// )
					// ,
					// }
					// Relational_operator
					// AND_SUB_OP
bool F();			// identifier <F_PRIME>
					// <const>
					// ( <OE> )
					// INCDEC Identifier
					// Not_Operator <F>
bool F_PRIME();		// ( <PARAMETER_2> )
					// INC_DEC
bool DIVMULMOD();	// DIVMUL
					// MOD
bool OE();			// <AE> <OE_PRIME>
bool OE_PRIME();	// Or_Operator <AE> <OE_PRIME>----->looping for another and ad or
					// ) or ;
bool AE();			// <R> <AE_PRIME>
bool AE_PRIME();	// And_Operator <R> <AE_PRIME>
					// Or_Operator or ) or ;
bool R();			// <E> <Relat_Op_p>
bool R_PRIME();		// Relational_operator <E> <Relat_Op_p>
					// And_Operator
					// Or_Operator
					// ;
					// )

//-------------------------------------------------------CGF OF DECLERATION GRAMMAR-------------------------------------------------
//-BELOW ARE THE RECURSIVE DESCENDENT PARSER FUNCTION FOR THE VALIDATION OF ALL TYPE OF DECLERATIONS INCLUDING MULTIDIMENSION ARRAY-
//----------------------------------------------------------------------------------------------------------------------------------

bool decl();		// datatype identifier <init> <list>
					// incase of array---->datatype identifier [ Arr_size() ] <array_init>---> refering to the breakline ;
bool Arr_size();	// check for , if more ids or ] if end of the ids
bool id_int();		// identifier Int_Const
bool init();		// (+=, -=, /=, *=, =) assignmen_operator identifier <init>
					// identifier <const> {all type of constants}
					// identifier <E> { maybe this is for validation of operators}
					// ,
					// ;
bool array_init();	// for more init
					// (+=, -=, /=, *=, =) assignmen_operator { <array_init2> }
					// ,
					// ;
bool array_init2(); // <value> }
					// <array_init3> }
bool array_init3(); // { <value> } <value2>
bool value();		// <E> <value1>
					// }
bool value1();		// , <E> <value1>
					// }
bool value2();		// concept: addition of more array_init3
					// , { <value> } <value2>------> for recursion
					// }
bool list();		// ;
					// , idenifier <list2>
bool list2();		// <init> <list>
					// [ <Arr_size()>----> checks of ids ] <a_init> <list>
bool Const();		// NUM
					// senBIT
					// ftNUM
					// Sentence
					// binary ...constants

//---------------------------------------------------------------CGF OF Loops and INput OUTput STATEMENTS--------------------------------------------
//-BELOW ARE THE RECURSIVE DESCENDENT PARSER FUNCTION FOR THE VALIDATION OF ALL STATEMENTS INCLLUDING SWITCH, FOR, WHILE, IFELSE, RETURN, ASSIGNMENT-
//---------------------------------------------------------------------------------------------------------------------------------------------------

bool body();				// <multilinestatement>
bool singlelinestatement(); // checks for loops, statements and declerations
							// returns true if any of them gets validated
bool multilinestatement();	// <singlelinestatement> <multilinestatement>
							// }
							// @
bool Return();				// return Int_Const ;
bool WhileLP();				// WhileLP ( <OE> <AI> ) LoopBody
bool AI();					// ; <inc_dec> <assign>
							// )
bool inc_dec();				// identifier INCDEC
							// INCDEC identifier
							// )
bool assign();				// identifier AO_INCDEC <id_int>
bool LoopBody();			// ;
							// <singlelinestatement>
							// { <multiline statement> }
bool switch_st();			// choose ( identifier ) { <options> }
bool options();				// From identifier : <input_print_st> Stop ; <options>
							// From identifier : <input_print_st> Stop ; default : <input_print_st> Stop ;
bool if_else();				// ouput Input_Output_Operator String_Constant ;
bool or_else();				// FL <LoopBody>
							// }
							// datatype
							// TR
							// WhileLP
							// identifier
							// return
bool condition();			// identitfier Relational_operator identifier
bool input_print_st();		// in_put Input_Output_Operator identifier ;
							// out_put Input_Output_Operator String_Constant ;
bool A();					// ;
							// <decl>
bool B();					// ;
							// <condition>
bool assignment();			// identifier Assignment_Operator <E> ;                     // identifier += 2
							// identifier ( <prmtrs> ) ;
bool forLP();				// similar to for loop
							// ForLP ( <A> <B> ; <inc_dec> ) LoopBody

//---------------------------------------------------------------CGF OF FUNCTION---------------------------------------------------------------------
//-BELOW ARE THE RECURSIVE DESCENDENT PARSER FUNCTION FOR THE VALIDATION OF ALL STATEMENTS INCLLUDING SWITCH, FOR, WHILE, IFELSE, RETURN, ASSIGNMENT-
//---------------------------------------------------------------------------------------------------------------------------------------------------

bool function();		   // func <ReturnType> identifier ( <FN_Parameter> ){ <body> }
bool ReturnType();		   // void
						   // datatype
bool FN_Parameter();	   // datatype identifier <FN_Parameter_AL>
						   // )
bool FN_Parameter_AL();	   // , datatype identifier <FN_Parameter_AL>
						   // )
bool function_signature(); // identifier <ReturnType> ( <FN_Parameter> ) ;
bool function_call();	   // identifier <ReturnType> ( <PARAMETER2)  ;

//---------------------------------------------------------------------------------
//-----------------------------------LEXICAL ANALYSER------------------------------
//---------------------------------------------------------------------------------

int NUM = 0, EXTRA = 0; // num is for pointer of token in case of lexical
						// while in case of syntax extra is the pointer
string cp[300];			// cp is to store tokens

int index = 0;
int main()
{
	int line = 0; // for tracking line no
	string read, read1, str1, str2, str3, str4, tokn;
	// read for elements from input file into input variable
	// read1 for elements to be appended
	// str1, str2, str3 , str4 are to play with the the indexes of the tokens
	// taking an input form a file name input.txt
	// tokn is to keep track of the current token from read
	ifstream input;
	string file = "code.txt";
	input.open("code.txt");
	if (input.fail())
	{
		cout << " Failed to open the file : " << file << endl;
		cout << " ***********check he path*********" << endl;
		cout << " ***********check file name ******" << endl;
		exit(1); // Exiting the program
	}

	// saving the tokens in the file token.txt
	fstream token;
	string filee = "Tokenized.txt";
	token.open("Tokenized.txt");

	if (token.fail())
	{
		cout << " Failed to open Tokenizer file: " << filee << endl;
		cout << " ***********check he path*********" << endl;
		cout << " ***********check file name ******" << endl;
		exit(1); // Exiting the program
	}
	cout << "***************************************" << endl;
	cout << "*************LEXICAL ANALISER**********" << endl;
	cout << "***************************************" << endl;
	cout << "\n";
	while (getline(input, read1)) // appending all the lines into read1 variable
	{
		read += read1;
		line++; // counting the number of lines in provided code
		//////////////////////////Comments ///////////////////////////////
		for (int i = 0; i <= read.length(); i++)
		{
			if (read[i] == '-' && read[i + 1] == '>') // single line comment ->
			{
				for (int j = i; j < read.length(); j++) // incrementing the pointer till the end of the line to ignore the comments
					i++;
			}
			else if (read[i] == '|' && read[i + 1] == '|' && read[i + 2] == '<') // Multi line comment ||<
			{
				bool flag1 = false;
				for (int j = i; j < read.length(); j++) // for ignoring the multiline comment
				{
					if (read[i] == '>' && read[i + 1] == '|' && read[i + 2] == '|') // if found >|| structure then Break the multilie comment
					{																// this is the ending of multiline comment in between the line
						flag1 = true;
						line++;
						i += 3; // incrementing the space of pointers consumed by the commments
						break;
					}
					i++;
				}
				if (flag1 == false) // when the flag is false means the line is a comment
				{					// if the multiline comment is not ending in between the line then it must be in the further lines
				  // therefore deleted this line
					read.clear(); // clearing the line that is comment
					while (getline(input, read1))
					{
						bool flag2 = false;
						read += read1;
						for (int k = 0; k <= read.length(); k++)
						{
							if (read[i] == '>' && read[i + 1] == '|' && read[i + 2] == '|')
							{
								flag2 = true; // this is for if the
								line++;		  // the multiline comment
								i = k + 3;	  // does not end in the single line
								break;		  // then find it in the preceding
							}
						}
						if (flag2 == true)
							break;
						else
						{
							read.clear();
							line++;
						}
					}
				}
			}

			str1.clear();
			str2.clear();
			str3.clear();
			// to keep track of the token parts arrving after then word
			str1 += read[i];
			str1 += read[i + 1]; // to see if the next element is punctuator or operator
			str2 += read[i];
			// cecking if the passed element is an of the following, if yes the continue the else to check further either append it to check if keyword
			// punctuator_Check = line break elements
			// Operators_Check = RO, AO
			// if the next element is not an operator or punctuator then increment it to check it for the whole element that may be an identifier of keyword
			// go to else if end line, end string, empty space, single colon or double colon
			if (!Punctuator_Check(str1) && !Operator_check(str1) && !Punctuator_Check(str2) && !Operator_check(str2) && read[i] != '\n' && read[i] != '\0' && read[i] != ' ' && read[i] != '\'' && read[i] != '"')
				tokn += read[i];
			else
			{
				if (read[i] == '.' && IS_Current_index_num(read[i + 1])) // this is for checking the element is float or not (checking num compatibility after .)
				{
					string str4;
					for (int j = i; j < read.length(); j++) // if yes then finding the length of the number after decimal point
					{
						tokn += read[i];
						i++;
						str4 = read[i];
						// iif any other  . or space, line breaker, operator occurs then break the loop otherwisse keep appending
						if (read[i] == '.' || read[i] == ' ' || read[i] == '\0' || Punctuator_Check(str4) || Operator_check(str4))
						{
							i--; // giving chance to some next funciton
							break;
						}
					}
				}
				if (tokn.length() >= 1) // checking the previously apprnded elements if they are identifier, keywords or undefined elements
				{
					//************************************Keyword check recap*********************************************************
					// string KEYWORD[18] = { "ForLP","WhileLP","TR","FL","choose","From","default","func","Structure","void","return"
					//,"Stop","main","begin","in_put","out_put" };   										--> 1  (Builtin Keywords)
					// string key_dt[6] = { "NUM",    "ftNUM",    "senBIT",    "Sentence"   ,"binary" };  	--> 2  (DataTypes)
					// string key_dtt[2] = { "false",    "true" }; 											--> 3  (Boolean conatants)

					if (Keyword_check(tokn) == 2 || Keyword_check(tokn) == 1 || Keyword_check(tokn) == 3)
					{
						if (Keyword_check(tokn) == 2) // tokenizing for Data types
						{
							token << "( DATATYPE, " << tokn << " , " << line << " )" << endl;
							cout << "( DATATYPE, " << tokn << " , " << line << " )" << endl;
							cp[NUM] = "DATATYPE";
						}
						else if (Keyword_check(tokn) == 1) // tokenizing the predifined keywords
						{
							token << "( " << tokn << " , " << tokn << " , " << line << " )" << endl;
							cout << "( " << tokn << " , " << tokn << " , " << line << " )" << endl;
							cp[NUM] = tokn;
							if (tokn != "out_put" && tokn != "in_put" && tokn != "begin" && tokn != "Stop" && tokn != "FL" && tokn != "From" && tokn != "default" && tokn != "Structure" && tokn != "void" && tokn != "return")
								Scope++;
						}
						else if (Keyword_check(tokn) == 3) // tokenizing the boolean Constants
						{
							token << "( Boolean_Const, " << tokn << " , " << line << " )" << endl;
							cout << "( Boolean_Const, " << tokn << " , " << line << " )" << endl;
							cp[NUM] = "Boolean_Const";
						}
						NUM++;
					}
					else if (Real_no_validation(tokn)) // tokenizing the integer constants
					{
						token << "( Int_Const, " << tokn << " , " << line << " )" << endl;
						cout << "( Int_Const, " << tokn << " , " << line << " )" << endl;
						cp[NUM] = "Int_Const";
						NUM++;
					}
					else if (Decimal_no_validation(tokn)) // tokenizing the float constants
					{
						token << "( Float_Const, " << tokn << " , " << line << " )" << endl;
						cout << "( Float_Const, " << tokn << " , " << line << " )" << endl;
						cp[NUM] = "Float_Const";
						NUM++;
					}
					else if (Validating_identifiers(tokn)) // tokenizing the identifiers
					{
						///////////////////////id DEF
						for (int i = 0; i < 100; i++)
						{
							if (SymbolTable[i].ST == tokn && SymbolTable[i].scope == Scope)
							{

								cout << " ______________________________________" << endl;
								cout << "|                                     |" << endl;
								cout << "|             SEMANTIC ERROR          |" << endl;
								cout << "|_____________________________________|" << endl;
								cout << "\nRedecleration Error " << tokn << " at line no: " << line << endl
									 << endl;
								exit(1);
							}
						}
						/////////////////////////////////////////////////////////////////////////////////////
						token << "( IDENTIFIER, " << tokn << " , " << line << " )" << endl;
						cout << "( IDENTIFIER, " << tokn << " , " << line << " )" << endl;
						cp[NUM] = "IDENTIFIER";
						NUM++;
						/////////////////////////////////
						SymbolTable[index].ST = tokn;
						SymbolTable[index].scope = Scope;
						index++;
						/////////////////////////////////////////////////////////////////////////////////////
					}
					else
						cout << "\nLEXICAL ERROR:\nline [" << line << "]  \nvalue:[  " << tokn << "  ]\n" // if the appended elements are not from the above throw error
							 << endl;

					tokn.clear();
				}
				if (read[i] == '"') // checking the double parenthesis during the code reading
				{					// if found the double parenthesis then consider it as start of the string
					bool flag = false;
					for (int j = i; j < read.length(); j++)
					{
						tokn += read[i];
						i++;

						if (read[i] == '"' && read[i - 1] != '\\') // if again found the double parenthesis then consider it as the end of the string
						{										   // provided that the previous element is not \\ is character escape
							tokn += read[i];
							flag = true;
							break;
						}
						else if (read[i] == '"' && read[i - 1] == '\\' && read[i - 2] == '\\' && read[i - 3] != '\\')
						{
							tokn += read[i];
							flag = true;
							break;
						}
						else if (read[i] == '"' && read[i - 1] == '\\' && read[i - 2] == '\\' && read[i - 3] == '\\' && read[i - 4] == '\\' && read[i - 5] != '\\')
						{
							tokn += read[i];
							flag = true;
							break;
						}
					}
					if (flag == true)
					{
						token << "( String_Constant, " << tokn << " , " << line << " )" << endl;
						cout << "( String_Constant, " << tokn << " , " << line << " )" << endl;
						cp[NUM] = "String_Constant";
						NUM++;
						flag = false;
						tokn.clear();
					}
					else
						cout << "\nLEXICAL ERROR:\nline [" << line << "]  \nvalue:[  " << tokn << "  ]\n" // if not from the above functions then
							 << endl;																	  // throw the error of undefined use of double parenthesis ""
				}
				if (read[i] == '\'')
				{
					int tc = i + 2;
					if (read[i + 1] == '\\')
						tc++;
					for (int j = i; j < read.length(); j++)
					{
						tokn += read[i];
						if (j == tc)
							break;
						i++;
					}

					if (character_validation(tokn))
					{
						token << "( Charater_constant, " << tokn << " , " << line << " )" << endl;
						cout << "( Charater_constant, " << tokn << " , " << line << " )" << endl;
						cp[NUM] = "Charater_constant";
						NUM++;
					}
					else
					{
						cout << "\nLEXICAL ERROR:\nline [" << line << "]  \nvalue:[  " << tokn << "  ]\n" // if not from the above functions then
							 << endl;																	  // throw the error of undefined use of single cotes ' '
					}
					tokn.clear();
				}
			}
			// if found from any opreator or input output arrows
			if (str1 == "|" || str1 == "&" || str1 == "<=" || str1 == ">=" || str1 == "!=" || str1 == "==" || str1 == "++" || str1 == "--" || str1 == "+=" || str1 == "-=" || str1 == "=+" || str1 == "=-" || str1 == "->" || str1 == "<-")
			{
				str3 = str1;
				i++;
			}
			// finding the mathematical expression with + and - after getting and operator from the next index
			// number ADDSUB number (extended by . for float)
			else if (!IS_Current_index_num(read[i - 1]) && (read[i] == '+' || read[i] == '-') && (IS_Current_index_num(read[i + 1]) || read[i + 1] == '.'))
				tokn += "+";
			else
				str3 = str2; // str2 = read[i]
			if (str3 == ".") // if found the . at read[i] then clear it else check for then extended operators or punctuators to be tokenized
				str3 = "";	 // . is been used as the line breaker in the tokens, so we will ignore it because it can a part of float been used as in expression
			if (Operator_check(str3) || Punctuator_Check(str3))
			{
				if (Operator_check(str3))
				{
					string opt = Operator_tokenization(str3); // "==", ">", "<", "<=", ">=", "!="
					token << "( " << opt << " , " << str3 << " , " << line << " )" << endl;
					cout << "( " << opt << " , " << str3 << " , " << line << " )" << endl;
					cp[NUM] = opt;
				}
				if (Punctuator_Check(str3) == true) // "[", "]", "(", ")", "{", "}", ";", ".", ":", ","
				{
					token << "( " << str3 << " , " << str3 << " , " << line << " )" << endl;
					cout << "( " << str3 << " , " << str3 << " , " << line << " )" << endl;
					cp[NUM] = str3;
				}
				NUM++;
			}
		}
		read.clear();
		tokn.clear();
	}
	cp[NUM] = "@"; // end of the function code appended by @
				   // readin of code done
				   // tokenizing and saving done
	input.close(); // closing the code file
	token.close(); // closing the token file
	cout << " ______________________________________" << endl;
	cout << "|                                      |" << endl; // now printing the lexems
	cout << "|              LEXEMES                 |" << endl;
	cout << "|______________________________________|" << endl;
	cout << "\n";
	for (int i = 0; i <= NUM; i++)
	{										  // printing formate
		cout << i << "=>  " << cp[i] << endl; // token number   =>  TOKEN
	}
	if (main_function() == true)
	{
		cout << " ______________________________________" << endl;
		cout << "|                                      |" << endl; // main funciton to check and validate the cfgs
		cout << "|              VALID CODE              |" << endl; // if all parsing done correctly and returned true
		cout << "|______________________________________|" << endl;
	}
	else
	{
		cout << " ______________________________________" << endl;
		cout << "|                                     |" << endl;
		cout << "|               INVALID CODE          |" << endl; // if any error occured then retuned false
		cout << "|               SYNTAX ERROR          |" << endl;
		cout << "|_____________________________________|" << endl;
	}

	fstream SYMtable;
	SYMtable.open("SYMtable.txt");
	if (SYMtable.fail())
	{
		cout << " Failed to open Symbol Table file:" << filee << endl;
		cout << " ***********check he path*********" << endl;
		cout << " ***********check file name ******" << endl;
		exit(1); // Exiting the program
	}
	for (int i=0; i < 100; i++)
	{
		if (SymbolTable[i].ST != "")
			SYMtable << SymbolTable[i].ST << "     " << SymbolTable[i].scope << endl;
	}
	SYMtable.close();
	return 0;
}

bool IS_Current_index_num(char s)
{
	if (s >= '0' && s <= '9')
		return true;
	return false;
}
bool Operator_check(string str)
{
	bool flag = false;
	string opt[22] = {"+", "-", "*", "/", "%", "^", "++", "--", "+=", "-=", "=", ">", "<", "<=", ">=", "!", "!=", "==", "&", "|", "->", "<-"};
	for (int i = 0; i < 22; i++)
	{
		if (opt[i] == str)
		{
			flag = true;
			break;
		}
	}
	return flag;
}
string Operator_tokenization(string str)
{
	string relational_opt[6] = {"==", ">", "<", "<=", ">=", "!="};
	for (int i = 0; i < 6; i++)
	{
		if (relational_opt[i] == str)
			return "Relational_operator";
	}
	if (str == "=")
		return "Assignment_Operator";
	else if (str == "!")
		return "Not_Operator";
	else if (str == "|")
		return "Or_Operator";
	else if (str == "&")
		return "And_Operator";
	else if (str == "+" || str == "-")
		return "ADDSUB";
	else if (str == "->" || str == "<-")
		return "Input_Output_Operator";
	else if (str == "*" || str == "/")
		return "DIVMUL";
	else if (str == "%")
		return "MOD";
	else if (str == "++" || str == "--")
		return "INCDEC";
	else if (str == "+=" || str == "-=" || str == "+=" || str == "-=")
		return "AO_INCDEC";
	else
		return "NONE";
}
bool Punctuator_Check(string str)
{
	bool flag = false;
	string punc[10] = {"[", "]", "(", ")", "{", "}", ";", ".", ":", ","};
	for (int i = 0; i < 10; i++)
	{
		if (punc[i] == str)
		{
			flag = true;
			break;
		}
	}
	return flag;
}
int Keyword_check(string str)
{
	string KEYWORD[16] = {"ForLP", "WhileLP", "TR", "FL", "choose", "From", "default", "func", "Structure", "void", "return", "Stop", "main", "begin", "in_put", "out_put"};
	// for checking keywords
	string key_dt[5] = {"NUM", "ftNUM", "senBIT", "Sentence", "binary"}; // for checking the data types
	string key_dtt[2] = {"false", "true"};								 // for checking the bool values

	for (int i = 0; i < 16; i++)
	{
		if (KEYWORD[i] == str)
			return 1;
	}
	for (int i = 0; i < 5; i++)
	{
		if (key_dt[i] == str)
			return 2;
	}
	for (int i = 0; i < 2; i++)
	{
		if (key_dtt[i] == str)
			return 3;
	}
	return 0;
}
bool Real_no_validation(string str) // for validating constants expression
{
	int cs = 0, fs = 1; // fs is for representing the final state, cs is for current state(column index)
	int arr[4][3] = {	// explanation done in copy validate expression ( 3 + 5 -7 )
					 {2, 2, 1},
					 {3, 3, 1},
					 {3, 3, 1},
					 {3, 3, 3}};

	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '+') // state answer: 2 or 3 (not acccepted)
			cs = arr[cs][0];
		else if (str[i] == '-') // state answer: 2 or 3 (not acccepted)
			cs = arr[cs][1];
		else if (str[i] >= '0' && str[i] <= '9') // state answer: 1 (acccepted)  // state answer will be 3 if none of the +, - or constant occur
			cs = arr[cs][2];
		else
		{
			cs = 3;
			break;
		}
	}
	if (cs == fs) // if validated then return true
		return true;
	else
		return false;
}
bool Decimal_no_validation(string str) // for validating float expression
{
	int cs = 0, fs = 3; // fs is for representing the final state, cs is for current state(column index)
	int arr[5][4] = {	// explanation done in copy validate expression ( 3.2 + 5.7 -7.3 )
					 {1, 1, 1, 2},
					 {4, 4, 1, 2},
					 {4, 4, 3, 4},
					 {4, 4, 3, 4},
					 {4, 4, 4, 4}};
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '+') // state answer: 1 or 4 (not acccepted)
			cs = arr[cs][0];
		else if (str[i] == '-') // state answer: 1 or 4 (not acccepted)
			cs = arr[cs][1];
		else if (str[i] >= '0' && str[i] <= '9') // state answer: 3 (acccepted) accept if the previous state is 3 or 1
			cs = arr[cs][2];
		else if (str[i] == '.')
			cs = arr[cs][3]; // state answer: 2 or 4 (not acccepted), next state should always be 2 or 3 to remain in valid loop
		else				 // means . after . is possible by (3,3)
		{
			cs = 4; // if none of them then break  cs=4 for not been equal to cs==fs
			break;
		}
	}
	if (cs == fs) // if validated then return true
		return true;
	else
		return false;
}
bool character_validation(string str) // string formate == ' any string '
{
	int cs = 0, fs = 4; // fs is for representing the final state, cs is for current state(column index)
	int arr[6][3] = {
		{1, 5, 5},
		{5, 2, 3},
		{3, 3, 3},
		{4, 5, 5},
		{5, 5, 5},
		{5, 5, 5}};

	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '\'') //----------------> representing the start of character '
			cs = arr[cs][0];
		else if (str[i] == '\\')
			//------------------------------------> representing the escape sequence of character '
			cs = arr[cs][1];
		else if (str[i] != '\'' && str[i] != '\\' || str[i] == ' ') //--> representing the end of character '
			cs = arr[cs][2];
		else
		{
			cs = 5;
			break;
		}
	}
	if (cs == fs)
		return true;
	else
		return false;
}
bool Validating_identifiers(string str)
{
	int cs = 0, fs = 2; // fs is for representing the final state, cs is for current state(column index)
	int arr[5][4] = {
		{1, 4, 4, 4},
		{4, 3, 2, 4},
		{4, 2, 2, 2},
		{4, 4, 2, 2},
		{4, 4, 4, 4}};
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '~') // ~ be a part of identifier if it is present in the first index
			cs = arr[cs][0];
		else if (str[i] == '_') // identifier can end at _ if the previous is alphabet, number, _
			cs = arr[cs][1];
		else if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) // alphabet always accepted if the previous not an alphabet, identifier starting with
			cs = arr[cs][2];
		else if (str[i] >= '0' && str[i] <= '9') // accepted if not in the start, can also be accepted on ending (after ~, after alphabet, after _)
			cs = arr[cs][3];
		else
		{
			cs = 4;
			break;
		}
	}
	if (cs == fs)
		return true;
	else
		return false;
}
// ---------------------------------------------------------------syntax analysis-----------------------------------------------------------
// ------------------------------------------------ PRE_Main()   main int begin () { //body }  POST_Main()  @-------------------------------

bool main_function()
{
	bool flag = false;

	if (PRE_Main()) // only retuns true when main occurs, it checks for Structure, decleration, any function decleration and function before main
	{
		EXTRA++;
		if (cp[EXTRA] == "main")
		{
			EXTRA++;
			if (cp[EXTRA] == "DATATYPE" || cp[EXTRA] == "void")
			{
				EXTRA++;
				if (cp[EXTRA] == "begin")
				{
					EXTRA++;
					if (cp[EXTRA] == "(")
					{
						EXTRA++;
						if (cp[EXTRA] == ")")
						{
							EXTRA++;
							if (cp[EXTRA] == "{")
							{
								EXTRA++;
								if (body())
								{
									EXTRA++;
									if (cp[EXTRA] == "}")
									{
										EXTRA++;
										if (POST_Main()) // returns true when @ occur, checks for any function present after main, all the function definition must be before @
										{
											EXTRA++;
											if (cp[EXTRA] == "@")
											{
												flag = true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return flag;
}
bool PRE_Main() // only retuns true when main occurs, it checks for Structure, decleration, any function decleration and function before main
{				// validate the previously held CFGs
	bool flag = false;

	if (Struct())
	{
		EXTRA++;
		if (PRE_Main())
		{
			flag = true;
		}
	}
	else if (G_decl())
	{
		EXTRA++;
		if (PRE_Main())
		{
			flag = true;
		}
	}
	else if (function())
	{
		EXTRA++;
		if (PRE_Main())
		{
			flag = true;
		}
	}
	else if (function_signature())
	{
		EXTRA++;
		if (PRE_Main())
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == "main")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool POST_Main() // returns true when @ occur, checks for any function present after main, all the function definition must be before @
{
	bool flag = false;

	if (function())
	{
		EXTRA++;
		if (POST_Main())
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == "@")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool body() // for checking the statements inside of the function of loops
{
	bool flag = false;

	if (multilinestatement())
	{
		flag = true;
	}

	return flag;
}
bool multilinestatement() // checks for more than one single line statement
{
	bool flag = false;

	if (singlelinestatement())
	{
		EXTRA++;
		if (multilinestatement()) // multiline statements can have more than one singleline statement that ends on the } or @
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == "}" || cp[EXTRA] == "@")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool singlelinestatement() // single line statements can have
{
	bool flag = false;
	if (assignment()) // return true if the read sequence is function of assignment of the decleration
	{
		flag = true;
	}
	else if (Return()) // for returning a constant from a function return return elements can only be real const
	{
		flag = true;
	}
	else if (decl()) // for decleration a function
	{
		flag = true;
	}
	else if (WhileLP()) // while loop
	{
		flag = true;
	}
	else if (if_else()) // if else statement
	{
		flag = true;
	}
	else if (switch_st()) // switch statement
	{
		flag = true;
	}
	else if (forLP()) // for loop
	{
		flag = true;
	}
	else if (input_print_st()) // input and output statement
	{
		flag = true;
	}
	return flag;
}
bool Struct() // Structure identifier { // declerations from decl() }
{
	bool flag = false;

	if (cp[EXTRA] == "Structure")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (cp[EXTRA] == "{")
			{
				EXTRA++;
				if (decl())
				{
					EXTRA++;
					if (cp[EXTRA] == "}")
					{
						EXTRA++;
						if (cp[EXTRA] == ";")
						{
							flag = true;
						}
					}
				}
			}
		}
	}

	return flag;
}
bool Return() // it will check for return keyword and will return only a real constant---------return Int_Const--
{
	bool flag = false;

	if (cp[EXTRA] == "return")
	{
		EXTRA++;
		if (cp[EXTRA] == "Int_Const")
		{
			EXTRA++;
			if (cp[EXTRA] == ";")
			{
				flag = true;
			}
		}
	}

	return flag;
}

//*******************************************************************
//*************Further is the expression validation CFG**************
//***************Explanation done in Notes and Report****************
//*****************See parse tree for better view********************
//*******************************************************************
bool decl()
{
	bool flag = false;

	if (cp[EXTRA] == "DATATYPE")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (init())
			{
				EXTRA++;
				if (list())
				{
					flag = true;
				}
			}
			else if (cp[EXTRA] == "[")
			{
				EXTRA++;
				if (Arr_size())
				{
					EXTRA++;
					if (cp[EXTRA] == "]")
					{
						EXTRA++;
						if (array_init())
						{
							EXTRA++;
							if (list())
							{
								flag = true;
							}
						}
					}
				}
			}
		}
	}

	return flag;
}
bool Arr_size()
{
	bool flag = false;

	if (id_int())
	{
		EXTRA++;

		if (cp[EXTRA] == "]")
		{
			EXTRA--;
			flag = true;
		}
		else if (cp[EXTRA] == ",")
		{
			EXTRA++;
			if (id_int())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "]")

		EXTRA--;
	flag = true;

	return flag;
}
bool id_int()
{
	bool flag = false;

	if (cp[EXTRA] == "IDENTIFIER")
	{
		flag = true;
	}
	else if (cp[EXTRA] == "Int_Const")
	{
		flag = true;
	}

	return flag;
}
bool init()
{
	bool flag = false;

	if (cp[EXTRA] == "Assignment_Operator")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (init())
			{
				flag = true;
			}
		}
		else if (Const())
		{
			flag = true;
		}
		else if (E())
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == ";" || cp[EXTRA] == ",")
	{
		EXTRA--;
		flag = true;
	}
	return flag;
}
bool array_init()
{
	bool flag = false;

	if (cp[EXTRA] == "Assignment_Operator")
	{
		EXTRA++;
		if (cp[EXTRA] == "{")
		{
			EXTRA++;
			if (array_init2())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == ";" || cp[EXTRA] == ",")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool array_init2()
{
	bool flag = false;

	if (value())
	{
		EXTRA++;
		if (cp[EXTRA] == "}")
		{
			flag = true;
		}
	}
	else if (array_init3())
	{
		EXTRA++;
		if (cp[EXTRA] == "}")
		{
			flag = true;
		}
	}

	return flag;
}
bool value()
{
	bool flag = false;

	if (E())
	{
		EXTRA++;
		if (value1())
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == "}")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool value1()
{
	bool flag = false;

	if (cp[EXTRA] == ",")
	{
		EXTRA++;
		if (E())
		{
			EXTRA++;
			if (value1())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "}")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool array_init3()
{
	bool flag = false;

	if (cp[EXTRA] == "{")
	{
		EXTRA++;
		if (value())
		{
			EXTRA++;
			if (cp[EXTRA] == "}")
			{
				EXTRA++;
				if (value2())
				{
					flag = true;
				}
			}
		}
	}
	else if (cp[EXTRA] == "}")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool value2()
{
	bool flag = false;

	if (cp[EXTRA] == ",")
	{
		EXTRA++;
		if (cp[EXTRA] == "{")
		{
			EXTRA++;
			if (value())
			{
				EXTRA++;
				if (cp[EXTRA] == "}")
				{
					EXTRA++;
					if (value2())
					{
						flag = true;
					}
				}
			}
		}
	}

	else if (cp[EXTRA] == "}")
	{
		EXTRA--;

		flag = true;
	}

	return flag;
}
bool list()
{
	bool flag = false;

	if (cp[EXTRA] == ";")
		flag = true;

	else if (cp[EXTRA] == ",")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (list2())
			{
				flag = true;
			}
		}
	}

	return flag;
}
bool list2()
{
	bool flag = false;

	if (init())
	{
		EXTRA++;
		if (list())
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == "[")
	{
		EXTRA++;
		if (Arr_size())
		{
			EXTRA++;
			if (cp[EXTRA] == "]")
			{
				EXTRA++;
				if (array_init())
				{
					EXTRA++;
					if (list())
					{
						flag = true;
					}
				}
			}
		}
	}

	return flag;
}
bool Const()
{
	bool flag = false;

	if (cp[EXTRA] == "Int_Const")

		flag = true;

	else if (cp[EXTRA] == "Charater_constant")
		flag = true;

	else if (cp[EXTRA] == "Float_Const")
		flag = true;

	else if (cp[EXTRA] == "String_Constant")
		flag = true;

	else if (cp[EXTRA] == "Boolean_Const")
		flag = true;

	return flag;
}
bool OE()
{
	bool flag = false;

	if (AE())
	{
		EXTRA++;
		if (OE_PRIME())
		{
			flag = true;
		}
	}

	return flag;
}
bool OE_PRIME()
{
	bool flag = false;

	if (cp[EXTRA] == "Or_Operator")
	{
		EXTRA++;
		if (AE())
		{
			EXTRA++;
			if (OE_PRIME())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == ")" || cp[EXTRA] == ";")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool AE()
{
	bool flag = false;

	if (R())
	{
		EXTRA++;
		if (AE_PRIME())
		{
			flag = true;
		}
	}

	return flag;
}
bool AE_PRIME()
{
	bool flag = false;

	if (cp[EXTRA] == "And_Operator")
	{
		EXTRA++;
		if (R())
		{
			EXTRA++;
			if (AE_PRIME())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "Or_Operator" || cp[EXTRA] == ")" || cp[EXTRA] == ";")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool R()
{
	bool flag = false;

	if (E())
	{
		EXTRA++;
		if (R_PRIME())
		{
			flag = true;
		}
	}

	return flag;
}
bool R_PRIME()
{
	bool flag = false;

	if (cp[EXTRA] == "Relational_operator")
	{
		EXTRA++;
		if (E())
		{
			EXTRA++;
			if (R_PRIME())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "And_Operator" || cp[EXTRA] == "Or_Operator" || cp[EXTRA] == ";" || cp[EXTRA] == ")")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool E()
{
	bool flag = false;

	if (T())
	{
		EXTRA++;
		if (E_PRIME())
		{
			flag = true;
		}
	}

	return flag;
}
bool E_PRIME()
{
	bool flag = false;

	if (cp[EXTRA] == "ADDSUB")
	{
		EXTRA++;
		if (T())
		{
			EXTRA++;
			if (E_PRIME())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "And_Operator" || cp[EXTRA] == "Or_Operator" || cp[EXTRA] == ";" || cp[EXTRA] == ")" || cp[EXTRA] == "," || cp[EXTRA] == "}" || cp[EXTRA] == "Relational_operator")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool T()
{
	bool flag = false;

	if (F())
	{
		EXTRA++;
		if (T_PRIME())
		{
			flag = true;
		}
	}
	return flag;
}
bool T_PRIME()
{
	bool flag = false;

	if (DIVMULMOD())
	{
		EXTRA++;
		if (F())
		{
			EXTRA++;
			if (T_PRIME())
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "And_Operator" || cp[EXTRA] == "Or_Operator" || cp[EXTRA] == ";" || cp[EXTRA] == ")" || cp[EXTRA] == "," || cp[EXTRA] == "}" || cp[EXTRA] == "Relational_operator" || cp[EXTRA] == "ADDSUB")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool F()
{
	bool flag = false;

	if (cp[EXTRA] == "IDENTIFIER")
	{
		EXTRA++;
		if (F_PRIME())
		{
			flag = true;
		}
	}
	else if (Const())
	{
		flag = true;
	}

	else if (cp[EXTRA] == "(")
	{
		EXTRA++;
		if (OE())
		{
			EXTRA++;
			if (cp[EXTRA] == ")")
			{
				flag = true;
			}
		}
	}
	else if (cp[EXTRA] == "INCDEC")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == "Not_Operator")
	{
		EXTRA++;
		if (F())
		{
			flag = true;
		}
	}

	return flag;
}
bool F_PRIME()
{
	bool flag = false;

	if (cp[EXTRA] == "(")
	{
		EXTRA++;
		if (PARAMETER_2())
		{
			EXTRA++;
			if (cp[EXTRA] == ")")
			{
				flag = true;
			}
		}
	}

	else if (cp[EXTRA] == "INCDEC")
	{
		flag = true;
	}

	else if (cp[EXTRA] == "DIVMUL" || cp[EXTRA] == "ADDSUB" || cp[EXTRA] == "MOD" || cp[EXTRA] == "And_Operator" || cp[EXTRA] == "Or_Operator" || cp[EXTRA] == ";" || cp[EXTRA] == ")" || cp[EXTRA] == "," || cp[EXTRA] == "}" || cp[EXTRA] == "Relational_operator")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool DIVMULMOD()
{
	bool flag = false;

	if (cp[EXTRA] == "DIVMUL")
	{
		flag = true;
	}

	else if (cp[EXTRA] == "MOD")
	{
		flag = true;
	}

	return flag;
}
//<AI><inc> |<Assign>|epsilon
//<ID_int> ID | int const
//<inc> ID incdec | incdec ID

//*******************************************************************
//***********************Expression CFG valiation done***************
//**********Further is the validation of loops and statements********
//*******************************************************************
bool WhileLP() // ---------WhileLP( condition ){ body }-------------
{
	bool flag = false;

	if (cp[EXTRA] == "WhileLP")
	{
		EXTRA++;
		if (cp[EXTRA] == "(")
		{
			EXTRA++;
			if (OE())
			{
				EXTRA++;
				if (AI())
				{
					EXTRA++;
					if (cp[EXTRA] == ")")
					{
						EXTRA++;
						if (LoopBody())
						{
							flag = true;
						}
					}
				}
			}
		}
	}

	return flag;
}
bool forLP() // --------ForLP ( A B ; INCDEC) { body }
{
	bool flag = false;

	if (cp[EXTRA] == "ForLP")
	{
		EXTRA++;
		if (cp[EXTRA] == "(")
		{
			EXTRA++;
			if (A())
			{
				EXTRA++;
				if (B())
				{
					EXTRA++;
					if (cp[EXTRA] == ";")
					{
						EXTRA++;
						if (inc_dec())
						{
							EXTRA++;
							if (cp[EXTRA] == ")")
							{
								EXTRA++;
								if (LoopBody())
								{

									flag = true;
								}
							}
						}
					}
				}
			}
		}
	}

	return flag;
}
bool A()
{
	if (cp[EXTRA] == ";")
	{
		EXTRA++;
		return true;
	}
	else if (decl())
	{
		return true;
	}
	return false;
}
bool B()
{
	if (cp[EXTRA] == ";")
	{
		EXTRA++;
		return true;
	}
	else if (condition())
	{
		return true;
	}
	return false;
}
bool condition()
{
	if (cp[EXTRA] == "IDENTIFIER")
	{
		EXTRA++;
		if (cp[EXTRA] == "Relational_operator")
		{
			EXTRA++;
			if (cp[EXTRA] == "IDENTIFIER")
			{
				EXTRA++;
				return true;
			}
		}
	}
	return false;
}
bool AI()
{
	bool flag = false;

	if (cp[EXTRA] == ";")
	{
		EXTRA++;
		if (inc_dec())
			flag = true;

		else if (assign())
			flag = true;
	}

	else if (cp[EXTRA] == ")")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool inc_dec()
{
	bool flag = false;

	if (cp[EXTRA] == "IDENTIFIER")
	{
		EXTRA++;
		if (cp[EXTRA] == "INCDEC")

			flag = true;
	}

	else if (cp[EXTRA] == "INCDEC")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
			flag = true;
	}
	else if (cp[EXTRA] == ")")
	{
		EXTRA++;
		return true;
	}

	return flag;
}
bool assign()
{
	bool flag = false;

	if (cp[EXTRA] == "IDENTIFIER")
	{
		EXTRA++;
		if (cp[EXTRA] == "AO_INCDEC")
		{
			EXTRA++;
			if (id_int())
			{
				flag = true;
			}
		}
	}

	return flag;
}
bool LoopBody()
{
	bool flag = false;

	if (cp[EXTRA] == ";")
		flag = true;

	else if (singlelinestatement())
		flag = true;

	else if (cp[EXTRA] == "{")
	{
		EXTRA++;
		if (multilinestatement())
		{
			EXTRA++;
			if (cp[EXTRA] == "}")
				flag = true;
		}
	}

	return flag;
}
bool if_else()
{
	bool flag = false;

	if (cp[EXTRA] == "TR")
	{
		EXTRA++;
		if (cp[EXTRA] == "(")
		{
			EXTRA++;
			if (OE())
			{
				EXTRA++;
				if (cp[EXTRA] == ")")
				{
					EXTRA++;
					if (LoopBody())
					{
						EXTRA++;
						if (or_else())
							flag = true;
					}
				}
			}
		}
	}

	return flag;
}
bool or_else()
{
	bool flag = false;

	if (cp[EXTRA] == "FL")
	{
		EXTRA++;
		if (LoopBody())
			flag = true;
	}
	else if (cp[EXTRA] == "}" || cp[EXTRA] == "DATATYPE" || cp[EXTRA] == "TR" || cp[EXTRA] == "WhileLP" || cp[EXTRA] == "IDENTIFIER" || cp[EXTRA] == "return")
	{ // this is because we have not decremented extra in the loopBody so we cant recompare }
		EXTRA--;
		flag = true;
	}

	return flag;
}

bool function() //------------func Identifier ( parameter ) { body }----------------
{
	bool flag = false;

	if (cp[EXTRA] == "func")
	{
		EXTRA++;
		if (ReturnType())
		{
			EXTRA++;
			if (cp[EXTRA] == "IDENTIFIER")
			{
				EXTRA++;
				if (cp[EXTRA] == "(")
				{
					EXTRA++;
					if (FN_Parameter())
					{
						EXTRA++;
						if (cp[EXTRA] == ")")
						{
							EXTRA++;
							if (cp[EXTRA] == "{")
							{
								EXTRA++;
								if (body())
								{
									EXTRA++;
									if (cp[EXTRA] == "}")
									{
										flag = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return flag;
}
bool ReturnType()
{
	bool flag = false;

	if (cp[EXTRA] == "void")
	{
		flag = true;
	}
	else if (cp[EXTRA] == "DATATYPE")
	{
		flag = true;
	}

	return flag;
}
bool FN_Parameter()
{
	bool flag = false;

	if (cp[EXTRA] == "DATATYPE")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (FN_Parameter_AL())
			{
				flag = true;
			}
		}
	}
	if (cp[EXTRA] == ")")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool FN_Parameter_AL()
{
	bool flag = false;
	if (cp[EXTRA] == ",")
	{
		EXTRA++;
		if (cp[EXTRA] == "DATATYPE")
		{
			EXTRA++;
			if (cp[EXTRA] == "IDENTIFIER")
			{
				EXTRA++;
				if (FN_Parameter_AL())
				{
					flag = true;
				}
			}
		}
	}
	if (cp[EXTRA] == ")")
	{
		EXTRA--;
		flag = true;
	}
	return flag;
}
bool function_signature()
{
	bool flag = false;

	if (ReturnType())
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (cp[EXTRA] == "(")
			{
				EXTRA++;
				if (FN_Parameter())
				{
					EXTRA++;
					if (cp[EXTRA] == ")")
					{
						EXTRA++;
						if (cp[EXTRA] == ";")
						{
							flag = true;
						}
					}
				}
			}
		}
	}

	return flag;
}
bool function_call()
{
	bool flag = false;

	if (cp[EXTRA] == "IDENTIFIER")
	{
		EXTRA++;
		if (cp[EXTRA] == "(")
		{
			EXTRA++;
			if (PARAMETER_2())
			{
				EXTRA++;
				if (cp[EXTRA] == ")")
				{
					EXTRA++;
					if (cp[EXTRA] == ";")
					{
						flag = true;
					}
				}
			}
		}
	}

	return flag;
}
bool PARAMETER_2()
{
	bool flag = false;

	if (E())
	{
		EXTRA++;
		if (PARAMETER_3())
		{
			flag = true;
		}
	}
	else if (cp[EXTRA] == ")")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool PARAMETER_3()
{
	bool flag = false;
	if (cp[EXTRA] == ",")
	{
		EXTRA++;
		if (E())
		{
			EXTRA++;
			if (PARAMETER_3())
			{
				flag = true;
			}
		}
	}

	else if (cp[EXTRA] == ")")
	{
		EXTRA--;
		flag = true;
	}

	return flag;
}
bool switch_st()
{
	bool flag = false;
	if (cp[EXTRA] == "choose")
	{
		EXTRA++;
		if (cp[EXTRA] == "(")
		{
			EXTRA++;
			if (cp[EXTRA] == "IDENTIFIER")
			{
				EXTRA++;
				if (cp[EXTRA] == ")")
				{
					EXTRA++;
					if (cp[EXTRA] == "{")
					{
						EXTRA++;
						if (options())
						{
							EXTRA++;
							if (cp[EXTRA] == "}")
							{
								flag = true;
							}
						}
					}
				}
			}
		}
	}
	return flag;
}

bool options()
{
	bool flag = false;

	if (cp[EXTRA] == "From")
	{
		EXTRA++;
		if (cp[EXTRA] == "IDENTIFIER")
		{
			EXTRA++;
			if (cp[EXTRA] == ":")
			{
				EXTRA++;
				if (input_print_st())
				{
					EXTRA++;
					if (cp[EXTRA] == "Stop")
					{
						EXTRA++;
						if (cp[EXTRA] == ";")
						{
							EXTRA++;
							if (options())
							{
								flag = true;
							}
							else if (cp[EXTRA] == "default")
							{
								EXTRA++;
								if (cp[EXTRA] == ":")
								{
									EXTRA++;
									if (input_print_st())
									{
										EXTRA++;
										if (cp[EXTRA] == "Stop")
										{
											EXTRA++;
											if (cp[EXTRA] == ";")
											{
												flag = true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return flag;
}
bool input_print_st()
{
	bool flag = false;
	if (cp[EXTRA] == "in_put")
	{
		EXTRA++;
		if (cp[EXTRA] == "Input_Output_Operator")
		{
			EXTRA++;
			if (cp[EXTRA] == "IDENTIFIER")
			{
				EXTRA++;
				if (cp[EXTRA] == ";")
				{
					flag = true;
				}
			}
		}
	}
	else if (cp[EXTRA] == "out_put")
	{
		EXTRA++;
		if (cp[EXTRA] == "Input_Output_Operator")
		{
			EXTRA++;
			if (cp[EXTRA] == "String_Constant")
			{
				EXTRA++;
				if (cp[EXTRA] == ";")
				{
					flag = true;
				}
			}
		}
	}
	return flag;
}
bool G_decl()
{
	bool flag = false;

	if (decl())
	{
		flag = true;
	}

	return flag;
}
bool assignment()
{
	bool flag = false;

	if (cp[EXTRA] == "IDENTIFIER")
	{
		EXTRA++;
		if (cp[EXTRA] == "Assignment_Operator")
		{
			EXTRA++;
			if (E())
			{
				EXTRA++;
				if (cp[EXTRA] == ";")
				{
					flag = true;
				}
			}
		}
		else if (cp[EXTRA] == "(")
		{
			EXTRA++;
			if (PARAMETER_2())
			{
				EXTRA++;
				if (cp[EXTRA] == ")")
				{
					EXTRA++;
					if (cp[EXTRA] == ";")
					{
						flag = true;
					}
				}
			}
		}
	}
	return flag;
}
