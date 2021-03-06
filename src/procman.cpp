/*
 * Renzoku - Re-build, re-test, and re-run a program whenever the code changes
 * Copyright (C) 2015  Colton Wolkins
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */
#include <stdexcept>
#include "procman.hpp"

ProcessManager::ProcessManager(iLogger *logger) :
	mBuildStep(NULL),
	mProgram(NULL),
	logger(logger)
{
}

ProcessManager::~ProcessManager() {
	for(Process* proc : mProcesses)
		delete proc;
	delete mBuildStep;
	delete mProgram;
}

Process* ProcessManager::getBuildStep() {
	return mBuildStep;
}

Process* ProcessManager::getProgram() {
	return mProgram;
}

bool ProcessManager::runBuild() {
	if(mBuildStep->runAndWait())
	{
		LOG(logger, SUCCESS, "Successfully ran command");
		return true;
	}
	else
	{
		LOG(logger, ERROR, "A command failed to run :(");
		return false;
	}
}

bool ProcessManager::runProcesses() {
	for(Process* proc : mProcesses)
	{
		if(proc->runAndWait())
		{
			LOG(logger, SUCCESS, "Successfully ran command");
		}
		else
		{
			LOG(logger, ERROR, "A command failed to run :(");
		}
	}
	return true;
}

void ProcessManager::haltConstructionProcs() {
	for(Process* proc : mProcesses)
	{
		proc->kill();
	}
}

void ProcessManager::killAll() {
	mBuildStep->kill();
	haltConstructionProcs();
	mProgram->kill();
}

void ProcessManager::startProgram() {
	mProgram->run();
}

void ProcessManager::haltProgram() {
	mProgram->kill();
}

void ProcessManager::restartAll()
{
	haltConstructionProcs();
	if(runBuild()) {
		runProcesses();
		haltProgram();
		startProgram();
	}
}

void ProcessManager::setBuilder(Process* proc) {
	if(mBuildStep)
		throw std::runtime_error("Build Step already set");
	mBuildStep = proc;
}

void ProcessManager::addProcess(Process* proc) {
	mProcesses.push_back(proc);
}

void ProcessManager::setProgram(Process* proc) {
	if(mProgram)
		throw std::runtime_error("Pogram already set");
	mProgram = proc;
}

