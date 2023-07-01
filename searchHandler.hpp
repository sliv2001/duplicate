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

class searchHandler {
public:
	searchHandler();
	virtual ~searchHandler();
private:
	std::map<std::string, std::list<std::string>> files;
};

#endif /* SEARCHHANDLER_HPP_ */
