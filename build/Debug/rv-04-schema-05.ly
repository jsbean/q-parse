\header{ 
  title = "rhythm notations of value [ 3/4 1/4 ] (schema-05.txt)"
}

\score {
<<
\new RhythmicStaff {
\time 1/4
 { e8~  { e16 e16 }  } 
 { e8. e16 }  \bar "||" 
\tuplet 3/2 { e8~ e8~  {  { e32 e32~ }  e16 }  } 
\tuplet 3/2 { e8~ e8~  { e32~ e16. }  }  \bar "||" 
 { e8~ \tuplet 3/2 { e16~  { e32 e32~ }  e16 }  } 
 { e8~ \tuplet 3/2 { e16.  e16. }  }  \bar "||" 
\tuplet 5/2 { e8~ e8~ e8~  { e16~  { e32 e32~ }  }  e8 } 
\tuplet 5/2 { e8~ e8~ e8.~  { e32 e32~ }  e8 }  \bar "||" 
\tuplet 3/2 { e8~ e8~ \tuplet 3/2 {  { e32~  { e64 e64~ }  }  e16~ e16 }  } 
\tuplet 3/2 { e8~ e8~ \tuplet 3/2 {  { e32. e64~ }  e16~ e16 }  }  \bar "||" 
\tuplet 3/2 { e8~ e8~  { \tuplet 3/2 { e32~  { e64 e64~ }  e32~ }  e16 }  } 
\tuplet 3/2 { e8~ e8~  { \tuplet 3/2 { e32.  e32.~ }  e16 }  }  \bar "||" 
 { e8~ \tuplet 3/2 { e16~ \tuplet 3/2 { e32~  { e64 e64~ }  e32~ }  e16 }  } 
 { e8~ \tuplet 3/2 { e16~ \tuplet 3/2 { e32.  e32.~ }  e16 }  }  \bar "||" 
\tuplet 7/2 { e8~ e8~ e8~ e8~ e8~  {  { e32 e32~ }  e16~ }  e8 } 
\tuplet 7/2 { e8~ e8~ e8~ e8~ e8~  { e32 e32~ }  e8. }  \bar "||" 
\tuplet 5/2 { e8~ e8~ e8~  { e16~ \tuplet 3/2 { e32~  { e64 e64~ }  e32~ }  }  e8 } 
\tuplet 5/2 { e8~ e8~ e8.~ \tuplet 3/2 { e32.  e32.~ }  e8 }  \bar "||" 
\tuplet 3/2 { e8~ e8~ \tuplet 3/2 { \tuplet 3/2 { e32~ e32~  {  { e128 e128~ }  e64~ }  }  e16~ e16 }  } 
\tuplet 3/2 { e8~ e8~ \tuplet 3/2 { \tuplet 3/2 { e32~ e32~  { e128~ e64.~ }  }  e16~ e16 }  }  \bar "||" 
\tuplet 7/2 { e8~ e8~ e8~ e8~ e8~  { \tuplet 3/2 { e32~  { e64 e64~ }  e32~ }  e16~ }  e8 } 
\tuplet 7/2 { e8~ e8~ e8~ e8~ e8~ \tuplet 3/2 { e32.  e32.~ }  e8. }  \bar "||" 
\tuplet 11/2 { e8~ e8~ e8~ e8~ e8~ e8~ e8~ e8~  {  { e32 e32~ }  e16~ }  e8~ e8 } 
\tuplet 11/2 { e8~ e8~ e8~ e8~ e8~ e8~ e8~ e8~  { e32 e32~ }  e8.~ e8 }  \bar "||" 
\tuplet 13/2 { e8~ e8~ e8~ e8~ e8~ e8~ e8~ e8~ e8~  { e16~  { e32 e32~ }  }  e8~ e8~ e8 } 
\tuplet 13/2 { e8~ e8~ e8~ e8~ e8~ e8~ e8~ e8~ e8.~  { e32 e32~ }  e8~ e8~ e8 }  \bar "||" 
}
>>
}

\version "2.18.2"
