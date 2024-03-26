#include <iostream>
#include <string.h>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
#include <cmath>

using namespace std;

string data_folder = "data";

void initializeApp(){
	struct stat data_dir;
	char* data_folder_char = new char[data_folder.length() + 1];
	strcpy(data_folder_char, data_folder.c_str());
	if(stat(data_folder_char, &data_dir) != 0){
		mkdir(data_folder_char, 0777);
	}
}
void checkLanguageDir(char *lang){
	struct stat lang_file;
	string filename = data_folder + "/";
	filename = filename + lang + ".txt";
	char* filename_char = new char[filename.length() + 1];
	strcpy(filename_char, filename.c_str());
	if(stat(filename_char, &lang_file) != 0){
		ofstream outfile (filename_char);
		outfile.close();
	}
}
void addDefinition(char *lang, char *new_word, char *translation){
	string filename = data_folder + "/";
	filename = filename + lang + ".txt";
	ofstream outfile;
	outfile.open(filename, ios_base::app);
	outfile << new_word << "\n" << translation << endl;
	outfile.close();
}
void listDictionary(char *lang){
	string filename = data_folder + "/";
	filename = filename + lang + ".txt";
	ifstream inputfilecount (filename);
	// Get number of lines
	int lines = 0;
	string lineText;
	while(getline(inputfilecount, lineText)){
		lines++;
	}
	inputfilecount.close();
	ifstream inputfile (filename);
	string words[(lines/2) + 1]; 
	string translations[(lines/2) + 1];
	words[0] = "Word";
	translations[0] = "Translation";
	int word_max_length = 4;
	int translation_max_length = 11;
	for(int i=0; i<lines/2; i++){
		getline(inputfile, words[i+1]);
		getline(inputfile, translations[i+1]);
		if(words[i+1].length() > word_max_length){
			word_max_length = words[i+1].length();
		}
		if(translations[i+1].length() > translation_max_length){
			translation_max_length = translations[i+1].length();
		}
	}
	for(int i=0; i<(lines/2)+1; i++){
		float word_missing_spaces = word_max_length - words[i].length();
		for(int j=0; j<floor(word_missing_spaces/2); j++){
			words[i] = " " + words[i];
		}
		for(int j=0; j<ceil(word_missing_spaces/2); j++){
			words[i] = words[i] + " ";
		}
		float translation_missing_spaces = translation_max_length - translations[i].length();
		for(int j=0; j<floor(translation_missing_spaces/2); j++){
			translations[i] = " " + translations[i];
		}
		for(int j=0; j<ceil(translation_missing_spaces/2); j++){
			translations[i] = translations[i] + " ";
		}
	}
	string separator = "";
	for(int i=0; i<(word_max_length+translation_max_length+7); i++){
		separator = separator + "_";
	}
	cout << separator << endl;
	cout << "| " << words[0] << " | " << translations[0] << " |" << endl;
	cout << separator << endl;
	for(int i=1; i<(lines/2)+1; i++){
		cout << "| " << words[i] << " | " << translations[i] << " |" << endl;
	}
	cout << separator << endl;
}
void setEntry(char *lang, char *word, char *translation){
	string filename = data_folder + "/";
	filename = filename + lang + ".txt";
	ifstream inputfile (filename);
	string fileStr;
	string currentLine;
	bool updated = false;
	while(getline(inputfile, currentLine)){
		char* currentLine_char = new char[currentLine.length() + 1];
		strcpy(currentLine_char, currentLine.c_str());
		if(strcmp(word, currentLine_char) == 0){
			fileStr = fileStr + currentLine + "\n";
			fileStr = fileStr + translation + "\n";
			getline(inputfile, currentLine);
			updated = true;
		}else{
			fileStr = fileStr + currentLine + "\n";
			getline(inputfile, currentLine);
			fileStr = fileStr + currentLine + "\n";
		}
	}
	inputfile.close();
	ofstream outfile (filename);
	outfile << fileStr << endl;
	if(updated == false){
		addDefinition(lang, word, translation);
	}
}
void viewEntry(char *lang, char *word){
	string filename = data_folder + "/";
	filename = filename + lang + ".txt";
	ifstream inputfile (filename);
	bool found = false;
	string previousLine;
	string currentLine;
	int line = 0;
	while(getline(inputfile, currentLine)){
		line++;
		char* currentLine_char = new char[currentLine.length() + 1];
		strcpy(currentLine_char, currentLine.c_str());
		if(strcmp(word, currentLine_char) == 0){
			found = true;
			if(line%2 == 0){
				cout << currentLine << " -> " << previousLine << endl;
			}else{
				previousLine = currentLine;
				getline(inputfile, currentLine);
				cout << previousLine << " -> " << currentLine << endl;
			}
		}
		previousLine = currentLine;
	}
	if(found == false){
		cout << "Couldn't find " << word << " in the database" << endl;
	}
}
void printHelp(){
	cout << "Usage: ./lang_dict [OPTIONS] \n";
	cout << "Create your own dictionaries.\n \n";
	cout << "Arguments:\n";
	cout << "  -l, --language LANGUAGE	     select the language of the dictionary\n";
	cout << "  add ORIGINAL TRANSLATED	     add a new entry to your dictionary\n";
	cout << "  set ORIGINAL TRANSLATED	     set the value of an existing or new entry\n";
	cout << "  view WORD		  	     view the translation of a word\n";
	cout << "  list			  	     list all the entries of a dictionary\n\n";
	cout << "Examples:\n";
	cout << "./lang_dict -l danish add jeg me    add 'jeg' and it's translation 'me' to the danish dictionary\n";
	cout << "./lang_dict -l spanish list	     list all the entries in the spanish dictionary" << endl;
}

int main(int argc, char **argv){
	initializeApp();
	if(argc > 1){
		if(strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--language") == 0){
			checkLanguageDir(argv[2]);
			if(strcmp(argv[3], "add") == 0){
				addDefinition(argv[2], argv[4], argv[5]);
			}else if(strcmp(argv[3], "list") == 0){
				listDictionary(argv[2]);
			}else if(strcmp(argv[3], "set") == 0){
				setEntry(argv[2], argv[4], argv[5]);
			}else if(strcmp(argv[3], "view") == 0){
				viewEntry(argv[2], argv[4]);
			}else{
				printHelp();
			}
		}else{
			printHelp();
		}
	}else{
		printHelp();
	}
	return 0;
}
