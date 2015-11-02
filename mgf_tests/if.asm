
%include 'kisdf.asm'
%include 'mgf_records.asm'

%assign lastUID 1
%assign groupCount 0

;--------file header-----------------------------------------------------------
    db 'NRPG3MGF'   ; magic id
    k_bstring 'SAS' ; game id
    k_int 0         ; file flags

    k_sstring 'Zalasus'                                ; author
    k_sstring 'Just another master file to test the dependency system' ; description

    k_short 1 ; dependency count
    k_sstring 'sas.mgf'
    k_short 0x00

    k_short 0 ; resource count

    k_int 2 ; record group count


    
;--------record groups---------------------------------------------------------
    k_short 0                                 ; record group header
    k_int (book_group_end - book_group_start) ; group size
    k_short 0x820                             ; group type
    k_int 0x01                                ; child record count
book_group_start:
    
    r_book lastUID,'Chronicles of Cloudkeep','[This book contains an exceptionally boring timeline of previous regents of Cloudkeep. It is totally not worth reading]'
    %assign lastUID lastUID+1

book_group_end:
    

;------------------------------------------------------------------------------
    k_short 0
    k_int (world_group_end - world_group_start)
    k_short 0xC5
    k_int 0x02
world_group_start:
    
    r_world lastUID,'Cloudkeep - Inner Courtyard'
    %assign lastUID lastUID+1

    ; world entity group
    k_short 0
    k_int (world_entity_group_end - world_entity_group_start)
    k_short 0xC0
    k_int 0x02
    world_entity_group_start:
    
        r_entity lastUID,0x0,3,0x820
        %assign lastUID lastUID+1

        r_entity lastUID,0xFFFF,1,0x820
        %assign lastUID lastUID+1
    
    world_entity_group_end:
    
world_group_end:
    
    db 0xF0 ; end marker
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    