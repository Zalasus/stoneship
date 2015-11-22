
%macro s_uid 2

    k_short %1
    k_int   %2

%endmacro


%macro r_world 2

    k_short 0xC5                                              ; record type
    k_int (rec %+ %1 %+ _e - rec %+ %1 %+ _s)                 ; record data size
    k_short 0                                                 ; record flags
    k_int %1                                                  ; record ID
    rec %+ %1 %+ _s:
    
        k_short 0x100 ; world data
        k_int (rec %+ %1 %+ _sub100_e - rec %+ %1 %+ _sub100_s)
        rec %+ %1 %+ _sub100_s:
            k_bstring %2 ; world name                 
        rec %+ %1 %+ _sub100_e:
        
    rec %+ %1 %+ _e:
    
%endmacro

%macro r_entity 4

    k_short 0xD0
    k_int (rec %+ %1 %+ _e - rec %+ %1 %+ _s)                 ; record data size
    k_short 0                                                 ; record flags
    k_int %1                                                  ; record ID
    rec %+ %1 %+ _s:
    
        k_short 0x1 ; entity base data
        k_int (rec %+ %1 %+ _sub1_e - rec %+ %1 %+ _sub1_s)
        rec %+ %1 %+ _sub1_s:
            s_uid %2, %3
            k_short %4                 
        rec %+ %1 %+ _sub1_e:
        
    rec %+ %1 %+ _e:

%endmacro



%assign lastUID 1
%assign recNum 0


%macro r_begin 1
    k_short %1
    k_int (rec %+ recNum %+ _e - rec %+ recNum %+ _s)       ; record data size
    k_short 0                                                 ; record flags
    k_int lastUID                                             ; record ID
    rec %+ recNum %+ _s:
%endmacro

%macro r_begin_idless 1
    k_short %1
    k_int (rec %+ recNum %+ _e - rec %+ recNum %+ _s)       ; record data size
    k_short 0                                                 ; record flags
    k_int 0xFFFFFFFF                                             ; record ID
    rec %+ recNum %+ _s:
%endmacro

%macro r_end 0
    rec %+ recNum %+ _e:
    %assign lastUID lastUID+1
    %assign recNum recNum+1
%endmacro

%macro r_end_idless 0
    rec %+ recNum %+ _e:
    %assign recNum recNum+1
%endmacro


%define REC_ENTITY 0xD0


%define SUB_MODEL 0x02
%define SUB_DISPLAY 0x06
%define SUB_DESCRIPTION 0x07
%define SUB_TRADING 0x08
%define SUB_INVENTORY 0x09
%define SUB_ICON 0x0A
%define SUB_IDENTIFICATION 0x0B
%define SUB_DATA 0x100
%define SUB_EDITOR 0xFFF0
%define SUB_MODIFY_META 0xFFF9

%define IS_ESSENTIAL 0x01
%define IS_CURRENCY 0x02
%define IS_UNIDENTIFIED 0x04
%define IS_UNIQUE 0x10


%macro r_sub_begin 1
    %assign currentSubtype %1
    k_short %1
    k_int (rec %+ recNum %+ _sub %+ currentSubtype %+ _e - rec %+ recNum %+ _sub %+ currentSubtype %+ _s)
    rec %+ recNum %+ _sub %+ currentSubtype %+ _s:
%endmacro        


%macro r_sub_end 0
    rec %+ recNum %+ _sub %+ currentSubtype %+ _e:
%endmacro


