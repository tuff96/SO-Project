# SO-Project
c++ terminal
Scope: create a simple command line interpretor, able to interpret a set of internally-implemented commands.

Your task for this task assignment is to create a simple command line interpretor. The command line interpretor should offer the following facilities:

  * Accept user commands from command line. The interpretor will expose a specific prompt (e.g. '$>') showing that it is prepared to accept a user command.
  * Execute user commands, from the list of accepted commands. The following error situations MUST be considered:
  
      o Command execution returned an error. The exact error code must be reported.
  
      o Issued command is not an accepted command. Error information will contain 'offending' command name.
  
      o Arguments are in a wrong format: the only accepted format is "-o ARG", where ARG is the optional argument of your option. It is not accepted to use the '/' notation for options or the --option long notation for options. These errors should be identified during preprocessing of command line.

IMPLEMENTATION SUGGESTIONS

  * Maintain and manage some environmental information, including the root directory, current directory, prompt, or environmental variables (some of the implemented commands will address these issues).
  * Offer a command called "help", that will list all available commands with implemented options.
  * Offer a command called "version", that will offer author information, versioning information, and possibly other valuable information.
  * All commands will be implemented as external functions. A library of functions could be developed for this purpose.
          
      o Each command will accept the main calling convention: int yourCommandFunction (int argc, char *argv[]) ;. Implementation of the additional main parameter (char **envp) is not required.
      
      o Each command will return success or an error code (see error code manipulation, above).
      
      o Each command could report additional error information prior to command exit. E.g. a command that opens an existing file will report the fact that the file does not exist or that it has no permission to access the file.

REQUIREMENTS
------------
The following commands must be implemented, along with their parameters (see man help pages for information on each):

1. The "ls" command. Parameters that need to be implemented are: -l, -s, -a, -F

2. The "tac" command. Parameters that need to be implemented are: -b, -s

3. The "dir" command. No parameters need to be implemented.

4. Your program must also support pipes in the commands, e.g. > ls -l | grep -e 'tmp' | wc -l (it must work with any type of command!)

5. Your program must also support redirection in the commands, e.g. > ls -l > out.txt (it must work with any type of command!)

6. Your program must support both pipes and redirection in the same command line, e.g. > ls -l | grep -e 'tmp' > out.txt (it must work with any type of command!)
