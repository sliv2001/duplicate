/*
 * searchHandler.cpp
 *
 *  Created on: 1 июл. 2023 г.
 *      Author: ivans
 */

#include <filesystem>
#include <iostream>
#include <fstream>
#include <array>
#include <openssl/md5.h>
#include <windows.h>
#include <locale>
#include <codecvt>
#include <string>
#include "searchHandler.hpp"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
using md5Result = std::array<unsigned char, MD5_DIGEST_LENGTH>;
using namespace std;

md5Result getMD5(std::filesystem::path filePath);
bool recycle_file_folder(std::filesystem::path path);

searchHandler::searchHandler(std::filesystem::path path) : path(path) {

}

searchHandler::~searchHandler() {}

void searchHandler::searchAndDelete() {
	for (const auto& dirEntry : recursive_directory_iterator(path)){
		if (!dirEntry.is_directory()){
			try{
				add(dirEntry.path().filename().string(), dirEntry.path());
			}
			catch (std::filesystem::filesystem_error &e){
				cout<<e.what()<<endl;
				cout<<e.path1()<<" "<<e.path2()<<endl;
			}
		}
	}
	for (auto &dups : files){
		if (dups.second.size()>1)
			reduce(dups.second);
	}
}

void searchHandler::add(std::string fileName, std::filesystem::path filePath) {
	files[fileName].push_back(filePath);
}

md5Result getMD5(std::filesystem::path filePath) {
	std::ifstream fin(filePath, std::ifstream::binary);
	MD5_CTX md5Context;
	MD5_Init(&md5Context);
	std::array<char, 1024> buf;
	while (fin.good()) {
		fin.read(buf.data(), buf.size());
		MD5_Update(&md5Context, buf.data(), fin.gcount());
	}
	md5Result result;
	MD5_Final(result.data(), &md5Context);
	return result;
}

void searchHandler::reduce(std::list<std::filesystem::path> &dups) {

	cout<<"Possible duplications "<<endl;
	for(auto &dup : dups){
		cout<<dup<<endl;
	}
	cout<<endl<<endl;

	list<md5Result> hashes;
	for (auto &f : dups){
		hashes.push_back(getMD5(f));
	}
	int index=0;
	for (auto h1=hashes.begin(); h1!=hashes.end(); h1++){
		auto h2 = h1;
		h2++;
		int i=index+1;
		while (h2!=hashes.end()){
			//if (!std::lexicographical_compare(h1->begin(), h1->end(), h2->begin(), h2->end())){
			if (*h1==*h2){
				if (deleteInPlace){
					if (!eraseAndDelete(dups, i)){
						h2++;
						i++;
					} else
						h2 = hashes.erase(h2);
				}
				else {
					throw "Feature isn't implemented yet";
				}


			}
			else{
				h2++;
				i++;
			}
		}
		index++;
	}
}

bool recycle_file_folder(std::filesystem::path path) {

	std::string widestr = path.string() + std::string(1, L'\0') + std::string(1, L'\0');

	SHFILEOPSTRUCT fileOp;
	fileOp.hwnd = NULL;
	fileOp.wFunc = FO_DELETE;
	fileOp.pFrom = widestr.c_str();
	fileOp.pTo = NULL;
	fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOERRORUI | FOF_NOCONFIRMATION | FOF_SILENT;
	int result = SHFileOperation(&fileOp);

	if (result != 0) {
		return false;
	}
	else {
		return true;
	}
}


bool searchHandler::eraseAndDelete(std::list<std::filesystem::path> &dups,
		int indexOfDeletion) {
	auto iter = dups.begin();
	for (int i=0; i<indexOfDeletion; i++)
		iter++;

	cout<<"Deleting "<<(*iter)<<endl;

	if (recycle_file_folder((*iter))){
		/*If it managed to delete file, then remove it from list of duplicates*/
		dups.erase(iter);
		return true;
	}

	return false;
}
