\header{ 
  title = "rhythm notations of value [ 3/4 1/4 ] (schema-10.txt)"
}

\score {
<<
\new RhythmicStaff {
\time 1/4
 { e8~  { e16 e16 }  } 
 { e8. e16 }  \bar "||" 
\tuplet 3/2 { e8~ e8~  {  { e32 e32~ }  e16 }  } 
\tuplet 3/2 { e8~ e8~  { e32~ e16. }  }  \bar "||" 
}
>>
}

\version "2.18.2"
