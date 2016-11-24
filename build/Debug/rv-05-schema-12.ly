\header{ 
  title = "rhythm notations of value [ 1/3 1/6 1/2 ] (schema-12.txt)"
}

\score {
<<
\new RhythmicStaff {
\time 1/4
\tuplet 3/2 { e8  { e16 e16~ }  e8 } 
\tuplet 3/2 { e8 e16 e8. }  \bar "||" 
}
>>
}

\version "2.18.2"
