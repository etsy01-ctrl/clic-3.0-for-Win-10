unit DLLSample;

interface

uses WinTypes, WinProcs, SysUtils;

type
ParamRec = record
   wStructLen: Word;   { The length of the structure }
   hMainWnd: HWnd;     { Handle to main window }
   wMaxReplaces: Word; { Maximum number of replaces }
   wRepNameWidth: Word;{ The width of a replace name }
   wRepStrWidth: Word; { The width of each replace string }
   wCurrReps: Word;    { Current number of replace strings }
   szRepName: PChar;   { The actual replace names }
   szRepStr: PChar;    { The actual replace values }
   wRunMode: Word;     { The installation mode }
   fLogFile: Integer;  { A file handle to the log file }
   szParam: PChar;     { String parameter from Wise Installation System }
end;

function SampleFunction(var DLLParams: ParamRec): Boolean; export;
procedure GetVariable(var DLLParams: ParamRec; const VarName: string; var VarValue: string); export;
procedure SetVariable(var DLLParams: ParamRec; const VarName: string; const NewValue: string); export;

implementation

function SampleFunction(var DLLParams: ParamRec): Boolean;
var
   SysDir: string;
   szSysDir: array[0..255] of char;
begin
   { Get the value of the %SYS% variable from Wise Installation }
   GetVariable(DLLParams,'SYS',SysDir);
   StrPCopy(szSysDir,SysDir);
   MessageBox(0,szSysDir,'System Directory',MB_OK);
   { Display the parameter passed in the Call DLL Dialog }
   MessageBox(0,DLLParams.szParam,'Parameter Value',MB_OK);
   { Create a new Variable called TESTIT }
   SetVariable(DLLParams,'TESTIT','This is the new value of TESTIT');
   { Change the value of the %INST% variable to A: }
   SetVariable(DLLParams,'INST','A:');
   SampleFunction := False;
end;

procedure GetVariable(var DLLParams: ParamRec; const VarName: string; var VarValue: string);
var
   i: Integer;
   szVarName: array[0..255] of char;
begin
   VarValue := '';
   szVarName[0] := '%';
   StrPCopy(@szVarName[1],VarName);
   StrCat(szVarName,'%');
   for i := 0 to DLLParams.wCurrReps do
   begin
      if (StrComp(szVarName,@DLLParams.szRepName[i * DLLParams.wRepNameWidth]) = 0) then
      begin
         VarValue := StrPas(@DLLParams.szRepStr[i * DLLParams.wRepStrWidth]);
         Exit;
      end;
   end;
end;

procedure SetVariable(var DLLParams: ParamRec; const VarName: string; const NewValue: string);
var
   i: Integer;
   szVarName: array[0..255] of char;
begin
   szVarName[0] := '%';
   StrPCopy(@szVarName[1],VarName);
   StrCat(szVarName,'%');
   for i := 0 to DLLParams.wCurrReps do
   begin
      if (StrComp(szVarName,@DLLParams.szRepName[i * DLLParams.wRepNameWidth]) = 0) then
      begin
         StrPCopy(@DLLParams.szRepStr[i * DLLParams.wRepStrWidth],NewValue);
         Exit;
      end;
   end;
   StrCopy(@DLLParams.szRepName[DLLParams.wCurrReps * DLLParams.wRepNameWidth],szVarName);
   StrPCopy(@DLLParams.szRepStr[DLLParams.wCurrReps * DLLParams.wRepStrWidth],NewValue);
   DLLParams.wCurrReps := DLLParams.wCurrReps + 1;
end;

end.

