
%include 'kisdf.asm'
%include 'mgf_records.asm'

;--------file header-----------------------------------------------------------
    db 'NRPG3MGF'   ; magic id
    k_bstring 'SAS' ; game id
    k_int 0         ; file flags

    k_sstring 'Zalasus'                                ; author
    k_sstring 'The main game file for Skin And Scales' ; description
    k_long 0x5649A4D3 ; timestamp

    k_short 0 ; dependency count

    k_short 1 ; resource count
    k_byte 1
    k_byte 0
    k_sstring 'sas_data/'

    k_int 6 ; record group count


    
;--------record groups---------------------------------------------------------
    k_short 0                                 ; record group header
    k_int (book_group_end - book_group_start) ; group size
    k_short 0x820                             ; group type
    k_int 0x02                                ; child record count
book_group_start:
    
    r_begin 0x820
        r_sub_begin SUB_MODEL
            k_bstring 'bookQuarto1.model'
            k_float 1.3
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'The Tale of the Dragons'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'A recent edition of the known epic poem -Tale of the Dragons- which outlines the downfall of the legendary creatures from the north of Xelis'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 50
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte 0x00 ; flags
            k_byte 1 ; slot count
            k_int 2     ; max stack size
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'book1.png'
        r_sub_end
    
        r_sub_begin SUB_DATA
            k_istring 'Thats a particulary boring book....'
        r_sub_end
    r_end


    r_begin 0x820
        r_sub_begin SUB_MODEL
            k_bstring 'bookFolio1.model'
            k_float 1.2
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'Old dirty book'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'An old book. You cant recognize the letters on the cover.'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 50
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte 0x00
            k_byte 1
            k_int 0
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'book1.png'
        r_sub_end
    
        r_sub_begin SUB_DATA
            k_istring '[This book is written in an unrecognizable script. Its pages are smeared with black grease.]'
        r_sub_end
    r_end

    
book_group_end:
    
    
    
;------------------------------------------------------------------------------    
    k_short 0
    k_int (weapon_group_end - weapon_group_start)
    k_short 0x810
    k_int 0x02
weapon_group_start:
    
    r_begin 0x810
        r_sub_begin SUB_MODEL
            k_bstring 'sword1.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'Iron Sword'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'A simple sword made of iron.'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 20
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte 0
            k_byte 2
            k_int 1
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'sword1.png'
        r_sub_end
    
        r_sub_begin SUB_DATA
            k_byte 0  ; type
            k_int 15 ; damage
            k_int 120 ; durability
            k_int 15 ; reach
        r_sub_end
    r_end

    r_begin 0x810
        r_sub_begin SUB_MODEL
            k_bstring 'dagger1.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'Lyrs Dagger'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'A legendary weapon, said to be once carried by Lyr, the god of magic himself.'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 3000
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte IS_UNIQUE | IS_ESSENTIAL
            k_byte 1
            k_int 1
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'dagger1.png'
        r_sub_end
    
        r_sub_begin SUB_DATA
            k_byte 0  ; type
            k_int 40 ; damage
            k_int 0 ; durability
            k_int 5 ; reach
        r_sub_end

        %assign lyrs_dagger_id lastUID

    r_end
    
weapon_group_end:
    
    
    
;------------------------------------------------------------------------------  
    k_short 0
    k_int (stuff_group_end - stuff_group_start)
    k_short 0x821
    k_int 0x03
stuff_group_start:
    
    r_begin 0x821
        r_sub_begin SUB_MODEL
            k_bstring 'gunk.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'Gunk'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'Something you dont have an accurate name for.'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 0
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte 0x00
            k_byte 1
            k_int 0
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'gunk.png'
        r_sub_end
    r_end
    
    r_begin 0x821
        r_sub_begin SUB_MODEL
            k_bstring 'dagger1.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'Blunt Dagger'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'A blunt, rusty dagger. It appears to have markings on its blade, but they are too rusty to read'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 15
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte IS_UNIDENTIFIED | IS_ESSENTIAL | IS_UNIQUE
            k_byte 1
            k_int 1
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'dagger1.png'
        r_sub_end

        r_sub_begin SUB_IDENTIFICATION
            s_uid 0xFFFF,lyrs_dagger_id
        r_sub_end
    r_end

    r_begin 0x821
        r_sub_begin SUB_MODEL
            k_bstring 'teapot.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_DISPLAY
            k_bstring 'Teapot'
        r_sub_end

        r_sub_begin SUB_DESCRIPTION
            k_sstring 'An ordinary teapot made of porcelain. It appers to have a crack at the handle. One should be careful when carrying it around...'
        r_sub_end

        r_sub_begin SUB_TRADING
            k_int 120
        r_sub_end

        r_sub_begin SUB_INVENTORY
            k_byte 0x00
            k_byte 1
            k_int 0
        r_sub_end

        r_sub_begin SUB_ICON
            k_bstring 'teapot.png'
        r_sub_end
    r_end
    
stuff_group_end:
   
;------------------------------------------------------------------------------
    k_short 0
    k_int (static_group_end - static_group_start)
    k_short 0x800
    k_int 0x02
static_group_start:
    
    r_begin 0x800
        r_sub_begin SUB_MODEL
            k_bstring 'teapot.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_EDITOR
            k_sstring 'teapotCommon'
        r_sub_end
    r_end

    r_begin 0x800
        r_sub_begin SUB_MODEL
            k_bstring 'cube.model'
            k_float 1.0
        r_sub_end

        r_sub_begin SUB_EDITOR
            k_sstring 'cubeTest'
        r_sub_end
    r_end
    
static_group_end: 
    
;------------------------------------------------------------------------------
    k_short 0
    k_int (dungeon_group_end - dungeon_group_start)
    k_short REC_DUNGEON
    k_int 0x02
dungeon_group_start:
    
    r_begin REC_DUNGEON
        r_sub_begin SUB_DATA
            k_bstring 'Cloudkeep - Gatehouse'
        r_sub_end
    r_end

    ; world entity group
    k_short 0
    k_int (dungeon_entity_group_end - dungeon_entity_group_start)
    k_short REC_ENTITY
    k_int 0x03
    dungeon_entity_group_start:
    
        r_begin REC_ENTITY
            r_sub_begin SUB_ENTITY
                s_uid 0xFFFF,9
                k_short 0x800
            r_sub_end
        r_end

        r_begin REC_ENTITY
            r_sub_begin SUB_ENTITY
                s_uid 0xFFFF,1
                k_short 0x820
            r_sub_end

            r_sub_begin SUB_ENTITY_ITEM
                k_int 1
            r_sub_end
        r_end

        r_begin REC_ENTITY
            r_sub_begin SUB_ENTITY
                s_uid 0xFFFF,0x5
                k_short 0x821
            r_sub_end

            r_sub_begin SUB_ENTITY_ITEM
                k_int 64
            r_sub_end
        r_end

    dungeon_entity_group_end:
    
dungeon_group_end:


;------------------------------------------------------------------------------
    k_short 0
    k_int (world_group_end - world_group_start)
    k_short REC_OUTDOOR
    k_int 0x02
world_group_start:
    
    r_begin REC_OUTDOOR
        r_sub_begin SUB_DATA
            k_bstring 'Plains of Vinmor'
        r_sub_end
    r_end

    ; world entity group
    k_short 0
    k_int (world_entity_group_end - world_entity_group_start)
    k_short REC_ENTITY
    k_int 0x02
    world_entity_group_start:
    
        r_begin REC_ENTITY
            r_sub_begin SUB_ENTITY
                s_uid 0xFFFF,8
                k_short 0x800
            r_sub_end
        r_end

        r_begin REC_ENTITY
            r_sub_begin SUB_ENTITY
                s_uid 0xFFFF,9
                k_short 0x800
            r_sub_end
        r_end

    world_entity_group_end:
    
world_group_end:
    
    db 0xF0 ; end marker
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    