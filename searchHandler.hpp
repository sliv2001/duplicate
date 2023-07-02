/*
 * searchHandler.hpp
 *
 *  Created on: 1 июл. 2023 г.
 *      Author: ivans
 */

#ifndef SEARCHHANDLER_HPP_
#define SEARCHHANDLER_HPP_

#include <map>
#include <list>
#include <string>
#include <filesystem>

class searchHandler {
public:
	searchHandler(std::filesystem::path path);
	virtual ~searchHandler();
	void searchAndDelete();
private:
	std::filesystem::path path;
	std::map<std::string, std::list<std::filesystem::path>> files;
	std::list<std::filesystem::path> forDeletion;
	bool deleteInPlace=true;

	void add(std::string fileName, std::filesystem::path filePath);
	void reduce(std::list<std::filesystem::path> &dups);
	bool eraseAndDelete(std::list<std::filesystem::path> &dups, int indexOfDeletion);
};

#endif /* SEARCHHANDLER_HPP_ */
