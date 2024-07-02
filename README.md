# RunasMe
Run app as current logged-on user instance from Administrators/NT-SYSTEM/other-users context with specified UAC-behavior, optional waiting for termination and preferred method type when have to fallback.
<br/>

[<img src="https://user-images.githubusercontent.com/76787321/197257488-1b7aa8e9-9b6f-4600-949e-8ff477cb4bf4.png" width="23%"></img>](https://github.com/Dragodraki/RunUnElevated/releases/latest/download/RunUnElevated.exe)

<br><br>
-------------------------------
EXPLANATION
-------------------------------
Ever wanted to downgrade back to normal user rights when elevated? Probably not, usually it's the other way round - become admin but access denied (XD). OK, the latter will be your problem and isn't topic here. So why my app exist and is important anyway in the first place?

Well, when you open any software as administrator (and accept UAC), it behaves not like it seems. Microsoft programmers did something that can be considered as a consequence of dull database rules: Only one entry for each primary key (lol)! And each user profile can be treated as such primary key. You indeed get access to a higher tier of rights, but you not only have the rights of an administrator, you act as administrator profile account, with all of its environment variables and its own registry hive.

*--- Windows's UAC does not know "Run with elevated rights" command, but only "Run as administrator" account! ---*

When "Run as administrator" any installer wizard, its impact depends the following three scenarios:
1. Your account is member of the administrator group (you're an administrator)
2. Your account belongs to any other group (e.g. standard user with limited rights)
3. You are in environment of NT-SYSTEM/ROOT already (account type is unimportant here)

1 - In case of possibility 1 your started app/wizard use the same profile environments as your real account and you won't notice any difference except you run under high integrity level now. That's because the user account changed - user account A changed to user A again. For local single-user PCs RunasMe.exe does not offer anything you won't already have.
<br>
2 - In case of possibility 2 your started app runs with the environment of a stranger. E.g. a game started as administrator will save to a complete other profile than yours (often at the end after wizard installation when asked whether to run the app directly and you say yes). %AppData%, %LocalAppData% and so point the the stranger account and aren't saved in your account! The same goes for the registry hive: Instead of the logged-on HKCU hive another one from HCU\-SID- will be used (that's the HKCU on the administrator accounts desktop) If you later run without administrator rights (or the other way round), you won't see your progress like it never existed. Another example: Open cmd as administrator and type "whoami" - the answer is the account name you got your administrator token from.
<br>
3 - Possibility 3 is similar to 2 but the SYSTEM even hasn't an actual account profile. It's the case when your app is executed by service / agent or through apps like PSExec. Because of the environment variables differ a lot from real user accounts games can even crash or fail saving settings/progress.

But the worst is yet to come. If your session is elevated already (e.g. cmd.exe), Windows doesn't provide any proper way to downgrade back and start a game with your actual profile. You have never seen a button called "Downgrade back to desktop user", have you?
This is where my app comes in. No matter the user account of the current process, all apps parsed to RunasMe will use the (unelevated) interactive desktop user with it's environment folders and registry hive. Having an installer that has been required administrator rights to place your files in Program Files and asks to run the software right away, only needs my RunasMe.exe to start the game as intended, saving its progress/settings to the actual users location. Plus, its much saver to use than keeping elevated rights when not needed. You never can be sure the game/software not contain having security gaps which could lead to privilege escalations. Invoked by RunasMe.exe it simply does not matter as it will be executed with low/medium user integrity.

Announcement of security:
<br>
Though RunasMe is very mighty and can be useful in various situations, it's techniques does not rely on hacking at all. Consider it's features as 'secrets' that MS Windows's own services use all the time (just in this moment when you sitting in front yout browser and read this). How else for example system actions like defender toast notifications with root permissions should be triggered without leaving high risk of privilege escalations, hm?! How do you think Antivirus/GPU-Programs or any other software that have been invoked by services could show their interface to the users desktop without explicit storing his/her logon credentials otherwise? Unfortunately until now, Microsoft never implemented it as a regular GUI option. If you - my dear reader - should belong to one of those script kiddos hoping for abusement of RunasMe to grabbing UAC tokens without permission, I have to disappoint you thoroughly. RunasMe is, and never was, designed for or capable of bypassing the UAC in a manner you were granted any rights you didn't possess before already! Cheers!
<br><br>

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
<br>

-------------------------------
USAGE
-------------------------------
You have to use either a command-line interface like cmd, powershell, any other console or an automated call from script/program.

Syntax usage:
<br>
  &nbsp;&nbsp;RunasMe.exe [/Flags] [Path]File [Args]
<br>

Flags:
<br>
  &nbsp;&nbsp;/wait = Run until (associated) program is terminated
  <br>
  &nbsp;&nbsp;/fb=[mc/cu/off] = Fallback to macro / current user / exit
  <br>
  &nbsp;&nbsp;/uac=[off/on/force] = UAC behavior (default "uac=off")
  <br>
  &nbsp;&nbsp;/debug = Display result as MsgBox (reveals error cause)
<br>

Examples:
<br>
  &nbsp;&nbsp;RunasMe.exe cmd
  <br>
  &nbsp;&nbsp;RunasMe.exe "C:\Windows\system32\devmgmt.msc"
  <br>
  &nbsp;&nbsp;RunasMe.exe /uac=off /fb=mc sysdm.cpl
  <br>
  &nbsp;&nbsp;RunasMe.exe /wait /fb=off MyApp.exe
  <br>
  &nbsp;&nbsp;RunasMe.exe "cmd.exe" "/c explorer.exe %userprofile%
  <br>
  &nbsp;&nbsp;RunasMe.exe /debug "WhyTheFail" AndHelp"
<br>

Hint:
<br>
  &nbsp;&nbsp;You can also drag'n'drop files/shortcuts on the program.
<br><br>

-------------------------------
WINDOWS SUPPORT
-------------------------------
This software is - like all my - for Windows only. But this was the first app I have written for being compatible with Windows 2000. I find it important somewhat supporting old OS's too. Why? -Two reasons: Firstly is hardens you app and makes it more likely to being compatible with the next latest Windows version and seccondly: I simply hate Windows 10 and 11 - they are slow as a dinosaur, consumes RAM/space like a whale, have heavy security gaps with their lockscreen and Task-Manager ressources and advertisements you became literally crazy. The design sucks to, but thats my personal opinion.

But I'm getting off the subject, RunasMe is compatible and tested with the following Windows OS's:
- Windows 2000
- Windows XP
- Windows 7
- Windows 11
<br>

-------------------------------
HOW IT WORKS TECHNICALLY
-------------------------------
Do not worry, I'll stick to the superficial view and keep it as short as possible - for detailed information on the individual functions and their possible applications, please refer to Aunt Google or (boring) MS websites. RunasMe has several and different ways to create the target program under the desktops context with/without waiting and with/without showing the UAC:

If the current process's context is identical to the logged-on user (which means you simply run RunasMe.exe without runas.exe), the started process will be the same like when you double-click the EXE file. Functions like "*CreateProcess*" / "*ShellExecute*" and so on would come into mind here. Yes, it varies a bit when you chose to ignore the UAC, using it's manifest or force it and whether to wait for its termination or not but the tokenization is pretty much the same.<br>
To make the UAC use the target programs information and not that one of a 3rd referer, my included C++ program Run.exe is used.
If your process owner is not the same as the desktops account (e.g. you use the credentials from someone else or being executed as part of a service-based agent), RunasMe unfolds its full potential: Accessing someone's account without entering it's credentials is called "*Impersonation*" and possible with elevated rights only (high-integrity-level). This can be achieved by doubling the process token (a token represents the level of access a process/thread have) so the primary token is kept to that one of the non-desktop user and the impersonated token that represents here the logged-on user. "*CreateProcessAsUserW*" is a function that is one common often used for that. And we need a process to gain the access from, it's recommend to use explorer.exe here as it is well-known in IT, the Windows Explorer to being in charge of creating big part of the session environment and always present. Again, administrators and higher tiers are allowed to do Impersonation so the target program can be spanwed by using NirSofts "AdvancedRun.exe" with parameter /RunAs (5/6) and /RunAsProcessName "explorer.exe".  Limited users are medium-integrity-level and so they does not have this option, so they cannot obtain an impersonated token. According to your preferred fallback stategy (RunasMe flag "/fb=") either the current process owner is kept instead (not the aim but at least very stable), exit without throwing any error or using a macro with my C++ app "MacroRun.exe". The latter is the only one that so to speak 'impersonate' the logged-on user by using virtually mouse/keyboard strokes to open the run dialog command and running the desired app, e.g. with "*SendMessage*". Yes, its very dirty - like all virtually mouse/keyboard usage there is high chance that the user focus another program in the meantime and the input become inserted in the wrong app /which could be nasty in some situations). But it's the only (legal) way exist and the only that gets logged-on process from another user account that has limited rights only. There would be another way, at least theoretically (appoaching explorer.exe with parameters) but Microsoft made sure it opens on a completely other desktop (superhidden for our desktop) we cannot obtain any GUI or information from. As I said, my app does not rely on hacking and therfore I don't investigated this idea.
<br>
The process call were explained. But there are another topic - filesystem permissions. To make sure the logged-on user (which might differ from the process owner) can access the temporary files from NSIS I had to grant access to the first named explicitly. And also a lot of WMI information were needed to get the logged-on user's name, PID and profile path in the first way we have to give access to. As Microsoft made the WMIC.exe "deprecated" in the last years, I had to find a way making the needed queries with just WMI - NSIS is not much of a help here, so I had to program another C++ app called "GetSessionUser.exe". This one is able to gain the actual user we crave for - it might give false information on Windows 2000, but NSIS will not use it on 2000/XP then and better use one of the fallback strategies.
<br>
All together with RunasMe flags (/with, /uac=, /fb=, /debug), I had to wrap one launcher around the other (the one perfect launcher with all stuff included would be very utopian to program). That is the reason for why intense character escaping, C++ programming and testing on different OS's were indispensable for my app to being finished and cost my many extra days.
<br>

-------------------------------
SPECIAL THANKS
-------------------------------
Thank you very much for your arsenal of tools, NirSoft! Especially your product "AdvancedRun" comes in handy for me to ignore the UAC, which is somethat tricky. Another very, very useful feature is your GUI - as RunasMe not only use "AdvancedRun" of course, but also others that invoked or be invoked by it, I had to do a lot quotation mark escaping that gave me headache even with your app as 'debugger'. Maybe it would be helpful if you could add it to your website, because without quotation marks only one parameter without spaces would have been working.
Anyway: As RunasMe's project code is much more comprehensive than only calling AdvancedRun.exe, I decided against naming them within my app as developer.
<br>
