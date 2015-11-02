
%include 'kisdf.asm'
%include 'mgf_records.asm'

%assign lastUID 1
%assign groupCount 0

;--------file header-----------------------------------------------------------
    db 'NRPG3MGF'   ; magic id
    k_bstring 'SAS' ; game id
    k_int 0         ; file flags

    k_sstring 'Zalasus'                                ; author
    k_sstring 'The main game file for Skin And Scales' ; description

    k_short 0 ; dependency count

    k_short 0 ; resource count

    k_int 4 ; record group count


    
;--------record groups---------------------------------------------------------
    k_short 0                                 ; record group header
    k_int (book_group_end - book_group_start) ; group size
    k_short 0x820                             ; group type
    k_int 0x03                                ; child record count
book_group_start:
    
    r_book lastUID,'The Tale Of The Dragons','Behold, for this is the tale of an ancient kind. Once mighty and high, now veiled in oblivion. Listen, ye who has ears to listen, for many a hero fell the last time this song was sung.'
    %assign lastUID lastUID+1
    
    r_book lastUID,'Old dirty book','[This book is written in an unrecognizable script. Its pages are smeared with black grease.]'
    %assign lastUID lastUID+1

    r_book lastUID,'readme.txt','When you can read this, the dependency system is working'
    %assign lastUID lastUID+1

    
book_group_end:
    
    
    
;------------------------------------------------------------------------------    
    k_short 0
    k_int (weapon_group_end - weapon_group_start)
    k_short 0x810
    k_int 0x02
weapon_group_start:
    
    r_weapon lastUID,'The very unique sword Excalibur'
    %assign lastUID lastUID+1
    
    r_weapon lastUID,'Lyrs Dagger'
    %assign lyrs_dagger_id lastUID
    %assign lastUID lastUID+1
    
weapon_group_end:
    
    
    
;------------------------------------------------------------------------------  
    k_short 0
    k_int (stuff_group_end - stuff_group_start)
    k_short 0x821
    k_int 0x02
stuff_group_start:
    
    r_stuff lastUID,'Gunk',0x00,0x00
    %assign lastUID lastUID+1
    
    r_stuff lastUID,'Blunt dagger',0xFFFF,lyrs_dagger_id
    %assign lastUID lastUID+1
    
stuff_group_end:
    
    
;------------------------------------------------------------------------------
    k_short 0
    k_int (world_group_end - world_group_start)
    k_short 0xC5
    k_int 0x02
world_group_start:
    
    r_world lastUID,'Plains of Vinmor'
    %assign lastUID lastUID+1

    ; world entity group
    k_short 0
    k_int (world_entity_group_end - world_entity_group_start)
    k_short 0xC0
    k_int 0x03
    world_entity_group_start:
    
        r_entity lastUID,0xFFFF,1,0x820
        %assign lastUID lastUID+1

        r_entity lastUID,0xFFFF,4,0x810
        %assign lastUID lastUID+1

        r_entity lastUID,0xFFFF,6,0x821
        %assign lastUID lastUID+1
    
    world_entity_group_end:
    
world_group_end:
    
    db 0xF0 ; end marker
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    