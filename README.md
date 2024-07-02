# *Info: New greatly improved version to be released soon!*
<br/>

# RunasMe
Run app as current logged-on user instance from Administrators/NT-SYSTEM/other-users context with specified UAC-behavior, optional waiting for termination and preferred method type when have to fallback.
<br/>

[<img src="https://user-images.githubusercontent.com/76787321/197257488-1b7aa8e9-9b6f-4600-949e-8ff477cb4bf4.png" width="23%"></img>](https://github.com/Dragodraki/RunUnElevated/releases/latest/download/RunUnElevated.exe)
<br/>

-------------------------------
EXPLANATION
-------------------------------
Ever wanted to downgrade back to normal user rights when elevated? Probably not, usually it's the other way round - become admin but access denied (XD). OK, the latter will be your problem and isn't topic here. So why my app exist and is important anyway in the first place?

Well, when you open any software as administrator (and accept UAC), it behaves not like it seems. Microsoft programmers did something that can be considered as a consequence of dull database rules: Only one entry for each primary key (lol)! And each user profile can be treated as such primary key. You indeed get access to a higher tier of rights, but you not only have the rights of an administrator, you act as administrator profile account, with all of its environment variables and its own registry hive.

* *--- Windows's UAC does not know "Run with elevated rights" command, but only "Run as administrator" account! ---* *

When "Run as administrator" any installer wizard, its impact depends the following three scenarios:
1. Your account is member of the administrator group (you're an administrator)
2. Your account belongs to any other group (e.g. standard user with limited rights)
3. You are in environment of NT-SYSTEM/ROOT already (account type is unimportant here)

1 - In case of possibility 1 your started app/wizard use the same profile environments as your real account and you won't notice any difference except you run under high integrity level now. That's because the user account changed - user account A changed to user A again. For local single-user PCs RunasMe.exe does not offer anything you won't already have.
2 - In case of possibility 2 your started app runs with the environment of a stranger. E.g. a game started as administrator will save to a complete other profile than yours (often at the end after wizard installation when asked whether to run the app directly and you say yes). %AppData%, %LocalAppData% and so point the the stranger account and aren't saved in your account! The same goes for the registry hive: Instead of the logged-on HKCU hive another one from HCU\-SID- will be used (that's the HKCU on the administrator accounts desktop) If you later run without administrator rights (or the other way round), you won't see your progress like it never existed. Another example: Open cmd as administrator and type "whoami" - the answer is the account name you got your administrator token from.
3 - Possibility 3 is similar to 2 but the SYSTEM even hasn't an actual account profile. It's the case when your app is executed by service / agent or through apps like PSExec. Because of the environment variables differ a lot from real user accounts games can even crash or fail saving settings/progress.

But the worst is yet to come. If your session is elevated already (e.g. cmd.exe), Windows doesn't provide any proper way to downgrade back and start a game with your actual profile. You have never seen a button called "Downgrade back to desktop user", have you?
This is where my app comes in. No matter the user account of the current process, all apps parsed to RunasMe will use the (unelevated) interactive desktop user with it's environment folders and registry hive. Having an installer that has been required administrator rights to place your files in Program Files and asks to run the software right away, only needs my RunasMe.exe to start the game as intended, saving its progress/settings to the actual users location. Plus, its much saver to use than keeping elevated rights when not needed. You never can be sure the game/software not contain having security gaps which could lead to privilege escalations. Invoked by RunasMe.exe it simply does not matter as it will be executed with low/medium user integrity.

Hint: When downgrade with "RunUnElevated.exe" your app will display UAC prompt when needing elevated rights. "RunUnElevated.exe" isn't suited for apps that have to be run elevated, but only to makes it possible to act out of administrator/system as user again!


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
