
%include 'kisdf.asm'
%include 'mgf_records.asm'

;--------file header-----------------------------------------------------------
    db 'NRPG3SGF'   ; magic id
    k_bstring 'SAS' ; game id
    k_int 0         ; file flags

    k_long 0 ; timestamp

    k_short 0 ; dependency count

    

;------------------------------------------------------------------------------  
    k_short 0
    k_int (stuff_group_end - stuff_group_start)
    k_short 0x821
    k_int 0x03
stuff_group_