#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;


class analyser{

	public:

		void set_symbols(string st){
			string sub;
			istringstream iss(st);

			do{
				iss >> sub;
				this->symbols.push_back(sub);
			}while(iss);
		}

		void set_stateInLine(string st){
			string state, sub;
			istringstream iss(st);
			unsigned cont = 0;

			iss >> state;

			do{
				iss >> sub;
				table[state][this->symbols[cont++]] = sub;
			}while(iss);
		}

		void set_out(string line){
			string aux, state = ">S";

			for( unsigned i = 0; i < line.size(); i++ ){
				aux = line[i];
				if( is_separator(line[i]) ){
					if ( state[1] == '#' ) {         //is final
						this->out.push_back(state);
					}
					state = ">S";
					if(line[i] == ' ') continue;
				}
				if(table[state].find(aux) != table[state].end()){
					if(table[state][aux] != "erro")
						state = table[state][aux];
					else if(state[1] == '#' && (table[">S"].find(aux)!=table[state].end()))
						state = table[">S"][aux];
					else
						this->out.push_back("erro");
				}else
						this->out.push_back("erro");

			}
			if ( state[1] == '#' ) {         //is final
				this->out.push_back(state);
			}

		}

		void print_out(const char * fName)	{
			ofstream arqSaida;
			arqSaida.open(fName, ofstream::out);
			for ( unsigned i = 0; i < out.size(); i++ )
				arqSaida << this->out[i] << endl;

			arqSaida.close();
		}

		void show_out(){
			for ( unsigned i = 0; i < out.size(); i++ )
				cout << this->out[i] << endl;
		}

		bool is_separator(char symbol){
			string strOperators = "+- )(/*:=<>";
			if(strOperators.find(symbol) != string::npos)
				return true;
			return false;
		}

	private:
		map < string, map < string, string> > table ;
		vector<string> symbols;
		vector<string> out;

};


int main ( int argc, char ** argv ) {
	analyser ant;
	string line;
	string arqSaida (argv[1]);
	ifstream source_mat ("../doc/table.csv");
	ifstream code (argv[1]);

	if(source_mat.is_open()){
		getline(source_mat, line);
		ant.set_symbols(line);

		while(getline(source_mat, line)){
			ant.set_stateInLine(line);
		}
	}else{
		cout << "No table file" << endl;
		return 1;
	}

	if(code.is_open()){
		while(getline(code, line)){
			ant.set_out(line);
		}
	}

	ant.show_out();

	arqSaida += ".lextbl";
	ant.print_out(arqSaida.c_str());

	return 0;
}
