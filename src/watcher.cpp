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
Watcher::Watcher(std::string dir, bool recursive) :
	mRecursive(recursive),
	mDirectory(dir)
{
	mINotify = inotify_init();
	watchDirectory(dir);
}
void Watcher::watchDirectory(std::string dir)
{
}
void Watcher::removeWatch(std::string dir)
{
}
void Watcher::watchFileType(std::string ft)
{
}
void Watcher::listen()
{
}

#ifdef DEBUG
void watcher() {
	int length, i = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];
	time_t timer = time(0)-15;
	struct tm timediff = {0};

	pid_t progpid;
	progpid = runprog();

	char cwd[1024];
	if( getcwd(cwd, sizeof(cwd)) != NULL )
		fprintf(stdout, "\033[0;32m=>Current working dir: %s\033[0m\n", cwd);
	else
		perror("getcwd");

	fd = inotify_init();
	if( fd < 0 )
		perror("inotify_init");
	wd = inotify_add_watch(fd, cwd, IN_MODIFY);
	while(gRunning) {
		length = read( fd, buffer, EVENT_BUF_LEN);
		i = 0;
		if( length <= 0 )
			perror("read");

		//printf("File %s!%d!\n", buffer,length);
		while( i < length ) {
			struct inotify_event *event = ( struct inotify_event * ) &buffer[i];
			//printf("File %s -> 0x%x!\n", event->name, event->mask);
			if( event->len ) {
				if(event->mask & IN_MODIFY) {
					if(! (event->mask & IN_ISDIR)) {
						if(ends_with(event->name, ".c") || ends_with(event->name, ".cpp")) {
							printf("\033[0;32m=> File %s was modified!\033[0m\n", event->name);
							//printf("%ju\n", timer);
							//printf("%ju\n", timer+5);
							//printf("%ju\n", time(0));
							timediff = *localtime(&timer);
							timediff.tm_sec += 5;
							if(mktime(&timediff) <= time(0)) {
								if(gRunning && !rebuild()) {
									//printf("%ju\n", mktime(&timediff));
									timer = time(0);
									if(killprog(progpid))
										progpid = runprog();
								}
							}
						}
					}
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}
	killprog(progpid);
	inotify_rm_watch(fd, wd);
	close(fd);
}

volatile sig_atomic_t gRunning = 1;
void catch_int(int sig) {
	gRunning = 0;
	fprintf(stderr, "\033[0;31m=> Caught deadly SIGINT, termination immenent!\033[0m\n");
	fflush(stderr);
	signal(sig, catch_int);
}
int ends_with(const char* str, const char* suffix) {
	if(!str || !suffix)
		return 0;
	size_t lenstr = strlen(str);
	size_t lensuffix = strlen(suffix);
	if(lensuffix > lenstr)
		return 0;
	return strncmp(str+lenstr - lensuffix, suffix, lensuffix) == 0;
}

int main(int argc, char** argv) {
	struct sigaction int_catcher;
	int_catcher.sa_handler = catch_int;
	//signal(SIGINT, catch_int);
	sigaction(SIGINT,&int_catcher,0);
	watcher();
	return EXIT_SUCCESS;
}
#endif