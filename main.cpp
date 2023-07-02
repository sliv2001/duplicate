/*
 * main.cpp
 *
 *  Created on: 1 июл. 2023 г.
 *      Author: ivans
 */

#include <iostream>
#include "cxxopts.hpp"
#include "searchHandler.hpp"

using namespace std;

std::string getInitialPath(int argc, char **argv) {
	/*Parse input*/
	cxxopts::Options options("duplicate",
			"This is a program to check image duplicates");
	options.add_options()("input", "Input directory for recursive check",
			cxxopts::value<std::string>());
	options.parse_positional( { "input" });
	auto result = options.parse(argc, argv);
	return result["input"].as<std::string>();
}

int main(int argc, char** argv){

	/*Parse input*/
	string path;
	try{
		path = getInitialPath(argc, argv);
	}
	catch (cxxopts::exceptions::exception &e){
		cout<<e.what()<<endl;
		return 0;
	}

	/*Create handler*/
	searchHandler handler(path);

	/*search*/
	handler.searchAndDelete();

	return 0;
}

