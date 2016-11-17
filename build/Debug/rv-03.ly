\header{ 
  title = "rhythm notation of value [ 1/2 1/6 1/3 ] (schema schema-05.txt )"
}

\score {
<<
\new RhythmicStaff {
\time 1/4
\tuplet 3/2 { e8~  { e16 e16 }  e8 } 
 { e8 \tuplet 3/2 { e16 e16~ e16 }  } 
 { e8  { \tuplet 3/2 { e32~ e32 e32~ }  e16 }  } 
\tuplet 3/2 { e8~ \tuplet 3/2 { e16~  { e32 e32~ }  e16 }  e8 } 
\tuplet 3/2 { e8~ \tuplet 3/2 { e16~ \tuplet 3/2 { e32~  { e64 e64~ }  e32~ }  e16 }  e8 } 
\tuplet 5/2 { e8~ e8~  { e16 e16~ }   { \tuplet 3/2 { e32~ e32 e32~ }  e16~ }  e8 } 
\tuplet 7/2 { e8~ e8~ e8~  { e16 e16~ }   { e16~ \tuplet 3/2 { e32 e32~ e32~ }  }  e8~ e8 } 
}
>>
}

\version "2.18.2"
