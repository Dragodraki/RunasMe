!ifndef __WMI_Included__
!define __WMI_Included__
!include LogicLib.nsh
!include "TextFunc.nsh"
!define Explode_WMI "!insertmacro Explode_WMI"
 
!define WMIGetInfo "!Insertmacro WMIGetInfo"
!define WMIKillProcess "!Insertmacro WMIKillProcess"
!define WMISetPriority "!Insertmacro WMISetPriority"
 
!macro  Explode_WMI Length  Separator   String
    Push    `${Separator}`
    Push    `${String}`
    Call    Explode_WMI
    Pop     `${Length}`
!macroend
 
Function Explode_WMI
  ; Initialize variables
  Var /GLOBAL explString
  Var /GLOBAL explSeparator
  Var /GLOBAL explStrLen
  Var /GLOBAL explSepLen
  Var /GLOBAL explOffset
  Var /GLOBAL explTmp
  Var /GLOBAL explTmp2
  Var /GLOBAL explTmp3
  Var /GLOBAL explArrCount
 
  ; Get input from user
  Pop $explString
  Pop $explSeparator
 
  ; Calculates initial values
  StrLen $explStrLen $explString
  StrLen $explSepLen $explSeparator
  StrCpy $explArrCount 1
 
  ${If}   $explStrLen <= 1          ;   If we got a single character
  ${OrIf} $explSepLen > $explStrLen ;   or separator is larger than the string,
    Push    $explString             ;   then we return initial string with no change
    Push    1                       ;   and set array's length to 1
    Return
  ${EndIf}
 
  ; Set offset to the last symbol of the string
  StrCpy $explOffset $explStrLen
  IntOp  $explOffset $explOffset - 1
 
  ; Clear temp string to exclude the possibility of appearance of occasional data
  StrCpy $explTmp   ""
  StrCpy $explTmp2  ""
  StrCpy $explTmp3  ""
 
  ; Loop until the offset becomes negative
  ${Do}
    ;   If offset becomes negative, it is time to leave the function
    ${IfThen} $explOffset == -1 ${|} ${ExitDo} ${|}
 
    ;   Remove everything before and after the searched part ("TempStr")
    StrCpy $explTmp $explString $explSepLen $explOffset
 
    ${If} $explTmp == $explSeparator
        ;   Calculating offset to start copy from
        IntOp   $explTmp2 $explOffset + $explSepLen ;   Offset equals to the current offset plus length of separator
        StrCpy  $explTmp3 $explString "" $explTmp2
 
        Push    $explTmp3                           ;   Throwing array item to the stack
        IntOp   $explArrCount $explArrCount + 1     ;   Increasing array's counter
 
        StrCpy  $explString $explString $explOffset 0   ;   Cutting all characters beginning with the separator entry
        StrLen  $explStrLen $explString
    ${EndIf}
 
    ${If} $explOffset = 0                       ;   If the beginning of the line met and there is no separator,
                                                ;   copying the rest of the string
        ${If} $explSeparator == ""              ;   Fix for the empty separator
            IntOp   $explArrCount   $explArrCount - 1
        ${Else}
            Push    $explString
        ${EndIf}
    ${EndIf}
 
    IntOp   $explOffset $explOffset - 1
  ${Loop}
 
  Push $explArrCount
FunctionEnd
 
!macro WMIGetInfo _NameSpace _Class _Property _CallBackFunction
#Push registers to stack
Push $0
Push $1
Push $2
Push $R0
Push $R1
Push $R2
 
#Execute WMI command to stack
nsexec::exectostack "wmic /NAMESPACE:\\${_NameSpace} path ${_Class} get ${_Property}"
pop $0
pop $1
 
#Trim blank lines
${TrimNewLines} "$1" $1
 
#Explode each result to a stack
${Explode_WMI}  $R1  "$\n" "$1"
 
#The first line is the same as ${_Property} so we remove it from the stack and subtract it from the results
Pop $2
IntOp $R1 $R1 - 1
 
#Loop through results and do stuff here
${For} $1 1 $R1
	Pop $2
	StrCpy $R0 $1
	StrCpy $R2 $2
	Call ${_CallBackFunction}
${Next}
 
Pop $R2
Pop $R1
Pop $R0
Pop $2
Pop $1
Pop $0
!macroend
 
!macro WMIKillProcess _ProcName
Push $0
Push $1
nsexec::exectostack "wmic process where name='${_ProcName}' delete"
pop $0
pop $0
StrCpy $0 $0 2
${If} $0 == "No"
pop $0
pop $0
push 0
${Else}
pop $0
pop $0
push 1
${EndIf}
!macroend
 
!macro WMISetPriority _ProcName _Priority
push $0
 
StrCpy $0 ''
 
${If} ${_Priority} == Low
StrCpy $0 64
${ElseIf} ${_Priority} == BelowNormal
StrCpy $0 16384
${ElseIf} ${_Priority} == Normal
StrCpy $0 32
${ElseIf} ${_Priority} == AboveNormal
StrCpy $0 32768
${ElseIf} ${_Priority} == High
StrCpy $0 128
${ElseIf} ${_Priority} == RealTime
StrCpy $0 16384
${EndIf}
 
${If} $0 != ''
nsexec::exectostack "wmic process where name='${_ProcName}' call setpriority $0"
pop $0
pop $0
StrCpy $0 $0 2
${If} $0 == "No"
pop $0
pop $0
push 0
${Else}
pop $0
pop $0
push 1
${EndIf}
${EndIf}
!macroend
!endif