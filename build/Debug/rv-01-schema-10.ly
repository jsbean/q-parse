\header{ 
  title = "rhythm notations of value [ 1/2 1/2 ] (schema-10.txt)"
}

\score {
<<
\new RhythmicStaff {
\time 1/4
 { e8 e8 }  \bar "||" 
\tuplet 3/2 { e8~  { e16 e16~ }  e8 } 
\tuplet 3/2 { e8.  e8. }  \bar "||" 
}
>>
}

\version "2.18.2"
