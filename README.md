連続   作業 (Renzoku sagyō -- Continuous Work)
===========================================
This is a project that I am working on that has I needed to use while I made
programs in C. I have found need of a utility that does what this program is
supposed to do. The goal of this program is as follows:

- Wait for any project files to change
- Build the project if it is a project that needs to be built
- If we succeed to build the program and there are unit tests
  - Run the unit tests
- If there were unit tests and they all passed
  - Kill any current running instance of our program (covers daemons)
  - Relauch the built program

Yes, there are some programs out there that do similar things (like Grunt), but
they aren't able to fill my need completely or easily. I just want something
that I can install and use with a few simple tweaks to a config file (like
Travis' yaml configs).

# vim: set tw=79 ts=2 sts=2 sw=2 ai et :
