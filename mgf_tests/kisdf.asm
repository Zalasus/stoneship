
%macro k_byte 1
    db %1
%endmacro

%macro k_short 1
    dw %1
%endmacro

%macro k_int 1
    dd %1
%endmacro

%macro k_long 1
    dq %1
%endmacro



%macro k_float 1
    dd %1
%endmacro



%macro k_bstring 1
    %strlen _len %1
    k_byte _len
    db %1
    %undef _len
%endmacro

%macro k_sstring 1
    %strlen _len %1
    k_short _len
    db %1
    %undef _len
%endmacro

%macro k_istring 1
    %strlen _len %1
    k_int _len
    db %1
    %undef _len
%endmacro

