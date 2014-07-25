if exists("b:current_syntax")
  finish
endif

" Keywords
syn match state_key '\w\+' contained display
syn keyword fsm_tag fsm contained
syn match fsm_head_1 '^\s*fsm' transparent contains=fsm_tag
syn match fsm_head '^\s*fsm\s\+\w\+\s*$' transparent contains=fsm_tag,state_key


syn keyword state_tag state contained
syn match state_head_1 '^\s*state' transparent contains=state_tag
syn match state_head '^\s*state\s\+\w\+\s*$' transparent contains=state_tag,state_key


syn match in_val '\w\+' contained display
syn match mapsto ':' contained display nextgroup=out_val
syn match out_val '\w\+' contained display
syn match next_state '@' contained display nextgroup=state_key

syn match map_string_1 '^\t\+\w' transparent contains=in_val
syn match map_string_2 '^\t\+\w\+\s*:' transparent contains=in_val,mapsto
syn match map_string_3 '^\t\+\w\+\s*:\s*\w\+' transparent contains=in_val,mapsto,out_val
syn match map_string_4 '^\t\+\w\+\s*:\s*\w\+\s*@' transparent contains=in_val,mapsto,out_val,next_state
syn match map_string   '^\t\+\w\+\s*:\s*\w\+\s*@\s*\w\+\s*$' transparent contains=in_val,mapsto,out_val,next_state,state_key

let b:current_syntax = "fsm"

hi def link fsm_tag		Type
hi def link state_tag	Type

hi def link mapsto		Statement
hi def link next_state	Statement

hi def link in_val		Constant
hi def link out_val		Constant
hi def link state_key	Constant

hi def link map_string	Statement

