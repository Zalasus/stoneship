
%macro s_uid 2

    k_short %1
    k_int   %2

%endmacro

%macro r_book 3

    k_short 0x820                                             ; record type
    k_int (rec %+ %1 %+ _e - rec %+ %1 %+ _s)                 ; record data size
    k_short 0                                                 ; record flags
    k_int %1                                                  ; record ID
    rec %+ %1 %+ _s:
    
        k_short 0x02 ; world information
        k_int (rec %+ %1 %+ _sub02_e - rec %+ %1 %+ _sub02_s)
        rec %+ %1 %+ _sub02_s:
            k_bstring 'folio_12.model'                 
        rec %+ %1 %+ _sub02_e:
		
	    k_short 0x06 ; display name
        k_int (rec %+ %1 %+ _sub06_e - rec %+ %1 %+ _sub06_s)
        rec %+ %1 %+ _sub06_s:
            k_bstring %2 ; display name            
        rec %+ %1 %+ _sub06_e:
		
	    k_short 0x07 ; description
        k_int (rec %+ %1 %+ _sub07_e - rec %+ %1 %+ _sub07_s)
        rec %+ %1 %+ _sub07_s:
            k_sstring '' ; description            
        rec %+ %1 %+ _sub07_e:
        
	    k_short 0x08 ; trading data
        k_int (rec %+ %1 %+ _sub08_e - rec %+ %1 %+ _sub08_s)
        rec %+ %1 %+ _sub08_s:
            k_int 100 ; value
        rec %+ %1 %+ _sub08_e:
		
	    k_short 0x09 ; inventory data
        k_int (rec %+ %1 %+ _sub09_e - rec %+ %1 %+ _sub09_s)
        rec %+ %1 %+ _sub09_s:
            k_byte 1 ; slot count       
	        k_byte 0 ; flags  
        rec %+ %1 %+ _sub09_e:
		
	    k_short 0x0a ; icon data
        k_int (rec %+ %1 %+ _sub0a_e - rec %+ %1 %+ _sub0a_s)
        rec %+ %1 %+ _sub0a_s:
            k_bstring 'book2.png' ; icon path
        rec %+ %1 %+ _sub0a_e:
		
        k_short 0x100  ; book information
        k_int (rec %+ %1 %+ _sub100_e - rec %+ %1 %+ _sub100_s)
        rec %+ %1 %+ _sub100_s:
            k_istring %3
        rec %+ %1 %+ _sub100_e:
        
    rec %+ %1 %+ _e:
    
%endmacro

%macro r_weapon 2

    k_short 0x810                                             ; record type
    k_int (rec %+ %1 %+ _e - rec %+ %1 %+ _s)                 ; record data size
    k_short 0                                                 ; record flags
    k_int %1                                                  ; record ID
    rec %+ %1 %+ _s:
    
        k_short 0x02 ; world information
        k_int (rec %+ %1 %+ _sub02_e - rec %+ %1 %+ _sub02_s)
        rec %+ %1 %+ _sub02_s:
            k_bstring 'sword1.model'                 
        rec %+ %1 %+ _sub02_e:
    
        k_short 0x05 ; unused. put it here for testing
        k_int (rec %+ %1 %+ _sub05_e - rec %+ %1 %+ _sub05_s)
        rec %+ %1 %+ _sub05_s:
            k_sstring 'The quick brown fox jumps over the lazy dog'             
        rec %+ %1 %+ _sub05_e:
		
		k_short 0x06 ; display name
        k_int (rec %+ %1 %+ _sub06_e - rec %+ %1 %+ _sub06_s)
        rec %+ %1 %+ _sub06_s:
            k_bstring %2 ; display name            
        rec %+ %1 %+ _sub06_e:
		
		k_short 0x07 ; description
        k_int (rec %+ %1 %+ _sub07_e - rec %+ %1 %+ _sub07_s)
        rec %+ %1 %+ _sub07_s:
            k_sstring '' ; description            
        rec %+ %1 %+ _sub07_e:
        
		k_short 0x08 ; trading data
        k_int (rec %+ %1 %+ _sub08_e - rec %+ %1 %+ _sub08_s)
        rec %+ %1 %+ _sub08_s:
            k_int 100 ; value
        rec %+ %1 %+ _sub08_e:
		
		k_short 0x09 ; inventory data
        k_int (rec %+ %1 %+ _sub09_e - rec %+ %1 %+ _sub09_s)
        rec %+ %1 %+ _sub09_s:
            k_byte 2 ; slot count
	        k_byte 0x08 ; flags (weapons do not stack)
        rec %+ %1 %+ _sub09_e:
		
		k_short 0x0a ; icon data
        k_int (rec %+ %1 %+ _sub0a_e - rec %+ %1 %+ _sub0a_s)
        rec %+ %1 %+ _sub0a_s:
            k_bstring 'sword1.png' ; icon path
        rec %+ %1 %+ _sub0a_e:
        
        k_short 0x100  ; weapon information
        k_int (rec %+ %1 %+ _sub100_e - rec %+ %1 %+ _sub100_s)
        rec %+ %1 %+ _sub100_s:
            k_byte 0  ; type
            k_int 100 ; damage
            k_int 100 ; durability
            k_int 100 ; reach
        rec %+ %1 %+ _sub100_e:
        
    rec %+ %1 %+ _e:
    
%endmacro


%macro r_stuff 4

    k_short 0x821                                             ; record type
    k_int (rec %+ %1 %+ _e - rec %+ %1 %+ _s)                 ; record data size
    k_short 0                                                 ; record flags
    k_int %1                                                  ; record ID
    rec %+ %1 %+ _s:
    
        k_short 0x02 ; world information
        k_int (rec %+ %1 %+ _sub02_e - rec %+ %1 %+ _sub02_s)
        rec %+ %1 %+ _sub02_s:
            k_bstring 'empty.model'                 
        rec %+ %1 %+ _sub02_e:
    
		k_short 0x06 ; display name
        k_int (rec %+ %1 %+ _sub06_e - rec %+ %1 %+ _sub06_s)
        rec %+ %1 %+ _sub06_s:
            k_bstring %2 ; display name            
        rec %+ %1 %+ _sub06_e:
		
		k_short 0x07 ; description
        k_int (rec %+ %1 %+ _sub07_e - rec %+ %1 %+ _sub07_s)
        rec %+ %1 %+ _sub07_s:
            k_sstring '' ; description            
        rec %+ %1 %+ _sub07_e:
        
		k_short 0x08 ; trading data
        k_int (rec %+ %1 %+ _sub08_e - rec %+ %1 %+ _sub08_s)
        rec %+ %1 %+ _sub08_s:
            k_int 100 ; value
        rec %+ %1 %+ _sub08_e:
		
		k_short 0x09 ; inventory data
        k_int (rec %+ %1 %+ _sub09_e - rec %+ %1 %+ _sub09_s)
        rec %+ %1 %+ _sub09_s:
            k_byte 1 ; slot count         
            %if %4 == 0x00
                k_byte 0x00  ; flags
            %else
                k_byte 0x04
            %endif
        rec %+ %1 %+ _sub09_e:
		
		k_short 0x0a ; icon data
        k_int (rec %+ %1 %+ _sub0a_e - rec %+ %1 %+ _sub0a_s)
        rec %+ %1 %+ _sub0a_s:
            k_bstring 'default.png' ; icon path
        rec %+ %1 %+ _sub0a_e:
		
		%if %4 != 0x00
        k_short 0x0b ; identification info
        k_int (rec %+ %1 %+ _sub0b_e - rec %+ %1 %+ _sub0b_s)
        rec %+ %1 %+ _sub0b_s:
            s_uid %3 , %4  ; identified uid              
        rec %+ %1 %+ _sub0b_e:
		%endif
        
    rec %+ %1 %+ _e:
    
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

