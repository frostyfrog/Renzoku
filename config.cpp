/*
 * =====================================================================================
 *
 *       Filename:  config.cpp
 *
 *    Description:  Configuration file retrieval
 *
 *        Version:  1.0
 *        Created:  05/23/2015 10:51:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Colton Wolkins (frostyfrog), frostyfrog2@gmail.com
 *
 * =====================================================================================
 */
#include "config.hpp"

std::vector<std::string> Config::getProjects()
{
	std::vector<std::string> v;
	v.push_back("building");
	return v;
}
std::string Config::getBuildCommand(std::string project)
{
	return "make";
}
std::string Config::getTestCommand(std::string project)
{
	return "make test";
}
std::string Config::getRunCommand(std::string project)
{
	return "";
}
