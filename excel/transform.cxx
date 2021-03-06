#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <algorithm> // for max_element

void seperate_string(std::string line, std::vector<std::string> &strs, const char sep );
bool ISEMPTY(std::string content);
double string2double(std::string str);

int main(int argc, char** argv){
	// ***********************************************************
	// => Preset
	std::vector<std::string> country_name_vec;
	std::vector<int> country_code_vec;
	std::vector<std::string> date_name_vec;
	std::string file;
	std::string s_card;
	std::vector<std::string> inputDatas;
	inputDatas.push_back("A.txt");
	inputDatas.push_back("B.txt");
	inputDatas.push_back("C.txt");
	inputDatas.push_back("D.txt");
	inputDatas.push_back("E.txt");
	inputDatas.push_back("F.txt");
	inputDatas.push_back("G.txt");
	inputDatas.push_back("H.txt");
	inputDatas.push_back("I.txt");
	inputDatas.push_back("J.txt");
	inputDatas.push_back("K.txt");
	inputDatas.push_back("L.txt");
	std::vector<std::vector<std::vector<double> > > DataSet; // country, date, value
	std::ofstream logstream;
	file = "log.txt";
	logstream.open(file.c_str());
	if(!logstream){
		std::cout<<"Cannot open "<<file<<" in ofstream format"<<std::endl;
		return -1;
	}

	// ***********************************************************
	// => Read inputCountries
	file = "inputCountries.txt";
	std::ifstream fin_card_country(file.c_str());
	if(!fin_card_country){
		std::cout<<"Cannot find "<<file<<std::endl;
		return -1;
	}
	// read file
	while(std::getline(fin_card_country,s_card)){
		std::vector<std::string> segments;
		seperate_string(s_card,segments,'\t');
		country_name_vec.push_back(segments[0]);
		country_code_vec.push_back((int)string2double(segments[1]));
	}
	// output countries
	for (int i = 0; i < country_name_vec.size(); i++ ){
		logstream<<"Country["<<i<<"]: \""<<country_name_vec[i]<<"\", ("<<country_code_vec[i]<<")"<<std::endl;
	}

	// ***********************************************************
	// => Read inputDates
	file = "inputDates.txt";
	std::ifstream fin_card_date(file.c_str());
	if(!fin_card_date){
		std::cout<<"Cannot find "<<file<<std::endl;
		return -1;
	}
	// read file
	while(std::getline(fin_card_date,s_card)){
		std::vector<std::string> segments;
		seperate_string(s_card,segments,'\t');
		date_name_vec.push_back(segments[0]);
	}
	// output countries
	for (int i = 0; i < date_name_vec.size(); i++ ){
		logstream<<"Date["<<i<<"]: \""<<date_name_vec[i]<<"\""<<std::endl;
	}

	// ***********************************************************
	// => Reset DataSet
	for ( int iCountry = 0; iCountry<country_name_vec.size(); iCountry++ ){
		std::vector<std::vector<double> > dates;
		for ( int iDate = 0; iDate<date_name_vec.size(); iDate++ ){
			std::vector<double> values;
			for ( int iValue = 0; iValue<inputDatas.size(); iValue++ ){
				values.push_back(-1);
			}
			dates.push_back(values);
		}
		DataSet.push_back(dates);
	}

	// ***********************************************************
	// => Read DataSet
	for ( int iValue = 0; iValue<inputDatas.size(); iValue++ ){
		file = inputDatas[iValue];
		std::ifstream fin_card_date(file.c_str());
		if(!fin_card_date){
			std::cout<<"Cannot find "<<file<<std::endl;
			return -1;
		}
		// read file
		std::vector<std::string> tempcountry_name_vec;
		std::vector<int> tempcountry_to_country;
		std::vector<int> country_to_tempcountry;
		int iline = 1;
		while(std::getline(fin_card_date,s_card)){
			if (iline==1){
				seperate_string(s_card,tempcountry_name_vec,'\t');
				// format
				for (int itemp = 0; itemp < tempcountry_name_vec.size(); itemp++ ){
					std::transform(tempcountry_name_vec[itemp].begin(),tempcountry_name_vec[itemp].end(),tempcountry_name_vec[itemp].begin(),toupper);
					if (tempcountry_name_vec[itemp]=="SOUTH KOREA") tempcountry_name_vec[itemp]="KOREA (SOUTH)";
					if (tempcountry_name_vec[itemp]=="RUSSIA") tempcountry_name_vec[itemp]="RUSSIAN FEDERATION";
					if (tempcountry_name_vec[itemp]=="UAE") tempcountry_name_vec[itemp]="UNITED ARAB EMIRATES";
				}
				// build map
				for (int itemp = 0; itemp < tempcountry_name_vec.size(); itemp++ ){
					for (int iCountry = 0; iCountry<country_name_vec.size(); iCountry++ ){
						tempcountry_to_country.push_back(-1);
						if (country_name_vec[iCountry] == tempcountry_name_vec[itemp]){
							tempcountry_to_country[itemp] = iCountry;
							break;
						}
					}
				}
				for (int iCountry = 0; iCountry<country_name_vec.size(); iCountry++ ){
					for (int itemp = 0; itemp < tempcountry_name_vec.size(); itemp++ ){
						country_to_tempcountry.push_back(-1);
						if (country_name_vec[iCountry] == tempcountry_name_vec[itemp]){
							country_to_tempcountry[iCountry] = itemp;
							break;
						}
					}
				}
				// output map
				logstream<<std::endl;
				logstream<<"Dictionary for "<<inputDatas[iValue]<<":"<<std::endl;
				for (int itemp = 0; itemp < tempcountry_name_vec.size(); itemp++ ){
					logstream<<"tempcountry["<<itemp<<"]: \""<<tempcountry_name_vec[itemp]<<"\" to ("<<tempcountry_to_country[itemp]<<")"<<std::endl;
				}
				for (int iCountry = 0; iCountry < country_name_vec.size(); iCountry++ ){
					logstream<<"country["<<iCountry<<"]: \""<<country_name_vec[iCountry]<<"\" to ("<<country_to_tempcountry[iCountry]<<")"<<std::endl;
				}
			}
			else if (iline<=3){
			}
			else{
				std::vector<std::string> segments;
				seperate_string(s_card,segments,'\t');
				std::vector<double> values;
				for (int i = 0; i < segments.size(); i++ ){
					if (segments[i]=="") values.push_back(-1);
					else values.push_back(string2double(segments[i]));
					int iDate = iline - 4;
					//logstream<<inputDatas[iValue]<<": "<<tempcountry_name_vec[i]<<" @ "<<date_name_vec[iDate]<<": from \""<<segments[i]<<"\" to ("<<values[i]<<")"<<std::endl;
				}
				if (values.size()!=tempcountry_name_vec.size()){
					logstream<<"WARNING! in line["<<iline<<"] of file["<<file<<"], values size "<<values.size()<<" != country size "<<tempcountry_name_vec.size()<<" !!!"<<std::endl;
				}
				else{
					for (int itemp = 0; itemp < tempcountry_name_vec.size(); itemp++ ){
						if (tempcountry_to_country[itemp]!=-1){
							int iCountry = tempcountry_to_country[itemp];
							int iDate = iline - 4;
							DataSet[iCountry][iDate][iValue] = values[itemp];
						}
					}
				}
			}
			iline++;
		}
	}

	// ***********************************************************
	// => Dump DataSet
	std::ofstream fout;
	file = "output.txt";
	fout.open(file.c_str());
	if(!fout){
		std::cout<<"Cannot open "<<file<<" in ofstream format"<<std::endl;
		return -1;
	}

	for ( int iCountry = 0; iCountry<country_name_vec.size(); iCountry++ ){
		for ( int iDate = 0; iDate<date_name_vec.size(); iDate++ ){
			fout<<country_name_vec[iCountry]<<"\t"<<date_name_vec[iDate]<<"\t"<<country_code_vec[iCountry]<<"\t";
			for ( int iValue = 0; iValue<inputDatas.size(); iValue++ ){
				if (DataSet[iCountry][iDate][iValue]==-1)
					fout<<""<<"\t";
				else
					fout<<DataSet[iCountry][iDate][iValue]<<"\t";
			}
			fout<<std::endl;
		}
	}
}

bool ISEMPTY(std::string content){
	bool flag = false;
	const char* c_card = content.c_str();
	int length = strlen(c_card);
	int offset = 0;
	for ( ; offset < length; offset++ ){
		if ( c_card[offset] != ' ' ) break;
	}
	if ( c_card[offset] == '#' || (c_card[offset] == '/' && c_card[offset+1] == '/') || length - offset == 0 ){
		flag = true;
	}
	return flag;
}

void seperate_string(std::string line, std::vector<std::string> &strs, const char sep ){
	std::string token;
	std::stringstream ss(line);
	while(std::getline(ss, token, sep)){
		token.erase(token.find_last_not_of('\t')+1);
		token.erase(0,token.find_first_not_of('\t'));
		token.erase(token.find_last_not_of(' ')+1);
		token.erase(0,token.find_first_not_of(' '));
		token.erase(token.find_last_not_of('\r')+1);
		token.erase(0,token.find_first_not_of('\r'));
		strs.push_back(token);
	}
}

double string2double(std::string str){
	double val;
	std::stringstream ss(str);
	ss>>val;
	return val;
}
