{
Global Conditional Definitions for Chief's UNZIP package
  By Dr Abimbola Olowofoyeku (The African Chief)
  Homepage: http://ourworld.compuserve.com/homepages/African_Chief
}

{$ifdef ver80}
   {$define Delphi}      {Delphi 1.x}
   {$define Delphi16}
   {$define Win16}
{$endif ver80}

{$ifdef ver90}
   {$define Delphi}      {Delphi 2.x}
   {$define Delphi32}
{$endif ver90}

{$ifdef ver100}
   {$define Delphi}      {Delphi 3.x}
   {$define Delphi32}
{$endif ver100}

{$ifdef Delphi}
   {$define Windows}
{$endif Delphi}

{$ifdef VirtualPascal}
   {.$define Delphi}   {Delphi 3.x}
   {$define Windows}
   {&Stdcall+}
{$endif VirtualPascal}

{$ifdef Win32}
  {$H-}                    { no long strings, please! }
{$else Win32}
  {$define assembler}      {use 16-bit assembler!}
{$endif Win32}


{$F+}

{.$define Debug}           {uncomment for debugging}

{$IFDEF Debug}
  {$F+,D+,Q-,L+,G+,R+,I-,S+,Y+,A+}
{$ELSE}
  {$F+,Q-,G+,R-,S-,I-,A+}
{$ENDIF}
