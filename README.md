# RunUnElevated
Run app as current logged-in user even from Administrators/NT-SYSTEM context



-------------------------------
IDEA
-------------------------------
Ever wanted to downgrade back to normal user rights when elevated? Probably not, usually it's the other way round - become admin but access denied XD. OK, the latter will be your problem and not topic here. So why my app exist and is important though in the first place?

Well, when you open any software as administrator (and accept UAC), it behaves not always like it seems. Microsoft programmers did something that can be considered as a consequence of dull database rules: Only one entry for each primary key (lol)! And each user profile can be treated as such primary key. You indeed get access to a higher tier of rights, but you haven't not only the rights of an administrator, your act the administrator profile account.


-------------------------------
LICENSE (FREEWARE)
-------------------------------
Permissions:
+ Private and Commcercial usage is allowed as long you don't demand money for it ;)
+ Forks are allowed, but they have to kept free of charge ;)
+ Free Distribution to friends or strangers is allowed, even wanted ;)

Limitations:
- Use the app at your own risk!
- Don't sell it as product or pretend to be its developer!
- Don't abuse it for malicious purposes!


-------------------------------
USAGE
-------------------------------
You have to use either a command-line interface like cmd, powershell, any other console or an automated call from script/program.

Syntax usage:  
- RunUnElevated.exe "[Path]File" ["Arg"] ["Directory"]

Examples:  
- RunUnElevated.exe "C:\Windows\system32\cmd.exe"
- RunUnElevated.exe explorer.exe
- RunUnElevated.exe "cmd.exe" "/c explorer.exe C:\Users\^"%username"%"
