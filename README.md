# RunUnElevated
Run app as current logged-in user even from Administrators/NT-SYSTEM context

<br/>

[<img src="https://user-images.githubusercontent.com/76787321/197257488-1b7aa8e9-9b6f-4600-949e-8ff477cb4bf4.png" width="23%"></img>](https://github.com/Dragodraki/BringExeToFront/releases/latest/download/BringExeToFront.exe)
<br/>

-------------------------------
EXPLANATION
-------------------------------
Ever wanted to downgrade back to normal user rights when elevated? Probably not, usually it's the other way round - become admin but access denied (XD). OK, the latter will be your problem and isn't topic here. So why my app exist and is important anyway in the first place?

Well, when you open any software as administrator (and accept UAC), it behaves not like it seems. Microsoft programmers did something that can be considered as a consequence of dull database rules: Only one entry for each primary key (lol)! And each user profile can be treated as such primary key. You indeed get access to a higher tier of rights, but you haven't not only the rights of an administrator, you act as administrator profile account. Which account it is, depends on the circumstances. Here are three different possible:
1. Your account is member of the administrator group (you're an administrator)
2. Your account belongs to any other group (e.g. standard user with limited rights)
3. You are in environment of NT-SYSTEM/ROOT already (account type is unimportant here)

In case of possibility 1 your started app/wizard use the same profile environments as your real account and you won't notice any difference except you run under high integrity level now.
In case of possibility 2 your started app runs with the environment variables of a stranger. E.g. a game started as administrator will save to a complete other profile than yours (often at the end after wizard installation when asked whether to run the app directly and you say yes). %AppData%, %LocalAppData% and so point the the stranger account and aren't saved in your account! If you later run without administrator rights (or the other way round), you won't see your progress like it never existed. Another example: Open cmd as administrator and type "whoami" - the answer is the account name you got your administrator token from.
Possibility 3 is similar to 2 but the SYSTEM even hasn't an actual account profile. It's the case when your app is executed by service module or through apps like PSExec. Because of the environment variables differ a lot from real user accounts games can even crash or fail saving settings/progress.

But the worst is yet to come. Is your session already elevated (e.g. cmd.exe), Windows doesn't provide any proper way to downgrade back and start a game with your actual profile.
This is where my app comes in. You can have a session either as administrator or even as system, this tool let you downgrade to your real profile environment. Having an installer that has been required administrator rights to place your files in Program Files and asks to run the software right away, needs my RunUnElevated.exe to start the game from the correct logged-in profile.

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
