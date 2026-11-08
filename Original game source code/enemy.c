#include <pdcurses.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gamedata.h"
#include "projectile.h"
#include "window.h"
#include "enemy.h"

//Spawning pool for enemies. Values here are placeholders that will be overwritten during spawning
Enemy enemies[MAX_ENEMIES] = {
    [0] = {
.px = 0, .py = 0,
    .dx = 1, .dy = 1,
    .hp = 1,
    .symbol = '%',
    .width = 1, .height = 1,
    .cooldown_frames = 0, //Stores the firing timer
    .type = GRUNT,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &GRUNT_RIFLE,
    .fire_px=0,
    .fire_py=0,
    .aimed = FALSE
    
    }
};
//Backup of spawning pool
Enemy enemies_backup[MAX_ENEMIES] = {
    [0] = {
.px = 0, .py = 0,
    .dx = 1, .dy = 1,
    .hp = 1,
    .symbol = '%',
    .width = 1, .height = 1,
    .cooldown_frames = 0, //Stores the firing timer
    .type = GRUNT,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &GRUNT_RIFLE,
    
    }
};
//Slowly fires bullets. Low-threat enemy, mostly just scoring fodder
static const Enemy grunt_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.2,
    .hp = 1,
    .symbol = '%',
    .width = 1, .height = 1,
    .cooldown_frames = -5,
    .type = GRUNT,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &GRUNT_RIFLE,
    
};
// Rapidly fires bullets. Meant to be dangerous in swarms
static const Enemy rapidfire_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.2,
    .hp = 2,
    .symbol = '&',
    .width = 1, .height = 1,
    .cooldown_frames = -5,
    .type = RAPIDFIRE,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &RAPIDFIRE_RIFLE
};
//Shoots lasers that travel in a straight line. A dangerous, high-priority target
static const Enemy laser_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.25,
    .hp = 3,
    .symbol = '!',
    .width = 1, .height = 1,
    .cooldown_frames = -30,
    .type = LASER_ENEMY,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &LASER_RIFLE_ENEMY
};
//Fire bombs that explode into circles of bullets.A dangerous, high-priority target
static const Enemy bomber_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.25,
    .hp =3,
    .symbol = '#',
    .width = 1, .height = 1,
    .cooldown_frames = -30,
    .type = BOMBER,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &BOMB_ENEMY_WEAPON
};
//Hunts the player while rapidly firing bullets. The most dangerous basic enemy
static const Enemy hunter_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.6,
    .hp = 4,
    .symbol = 'H',
    .width = 1, .height = 1,
    .cooldown_frames = -30,
    .type = HUNTER,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &HUNTER_RIFLE
};

//Reflects bullets. Designed to shield other enemies from player bullets
static const Enemy reflector_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.5,
    .hp = 1,
    .symbol = 'R',
    .width = 1, .height = 1,
    .cooldown_frames = -30,
    .type = REFLECTOR,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = &GRUNT_RIFLE
};
//An enemy that does nothing but acts as a shield for other enemies
static const Enemy barrier_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 1,
    .hp = 20,
    .symbol = '=',
    .width = 1, .height = 1,
    .cooldown_frames = -30,
    .type = BARRIER,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = NULL,
    .weapon = NULL
};
//Layout for corvettes
static const int corvette_row0_colors[] = {2, 6, 2}; // red, amber, red

TileLayout Corvette_Layout = {
.width = 3, .height = 1,
.glyph_rows = {
    "<#>"
},
.color_rows = {
    corvette_row0_colors
}
};
static const Enemy corvette_template = {
    .px = 0, .py = 0,
    .dx = 4, .dy = 0.5,
    .hp = 10,
    .symbol = '%',
    .width = 3, .height = 1,
    .cooldown_frames = 0,
    .type = CORVETTE,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Corvette_Layout,
    .weapon = &RAPIDFIRE_RIFLE
};
static const Enemy corvette_bomber_template = {
    .px = 0, .py = 0,
    .dx = 4, .dy = 0.5,
    .hp = 10,
    .symbol = '%',
    .width = 3, .height = 1,
    .cooldown_frames = 0,
    .type = CORVETTE_BOMBER,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Corvette_Layout,
    .weapon = &BOMB_ENEMY_WEAPON
};
//Layout for jets
static const int jet_row0_colors[] = {2, 6, 2}; //red, amber, red
static const int jet_row1_colors[] = {2, 7, 2}; //red, steel gray, red
static const int jet_row2_colors[] = {2, 2, 2}; //red, red, red
// Anchor point is the # at its center
TileLayout Jet_Layout = {
.width = 3, .height = 3,
.glyph_rows = {
    " | ",
    "<#>",
    " V "
},
.color_rows = {
    jet_row0_colors,
    jet_row1_colors,
    jet_row2_colors
}
};
static const int jet_row0_colors_invuln[] = {2, 6, 2}; //red, amber, red
static const int jet_row1_colors_invuln[] = {9, 7, 9}; //red, steel gray, red
static const int jet_row2_colors_invuln[] = {2, 9, 2}; //red, red, red
// Anchor point is the # at its center
TileLayout Jet_Layout_Invuln = {
.width = 3, .height = 3,
.glyph_rows = {
    " | ",
    "<#>",
    " V "
},
.color_rows = {
    jet_row0_colors_invuln,
    jet_row1_colors_invuln,
    jet_row2_colors_invuln
}
};
static const Enemy jet_template = {
    .px = 0, .py = 0,
    .dx = 2, .dy = 2,
    .hp = 10,
    .symbol = '%',
    .width = 3, .height = 3,
    .cooldown_frames = -30,
    .type = JET,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Jet_Layout,
    .weapon = &JET_CANNON
};
//Special version of jet used as miniboss
static const Enemy jet_boss_template = {
    .px = 0, .py = 0,
    .dx = 2, .dy = 2,
    .hp = 20,
    .symbol = '%',
    .width = 3, .height = 3,
    .cooldown_frames = -30,
    .type = JET_BOSS,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Jet_Layout,
    .weapon = &JET_CANNON,
    .is_boss_part = 1,
    .is_boss_core = 0
};
//Layout for flying fortresses
static const int flyfort_row0_colors[] = {2, 6, 6, 6, 2}; //red, amber, amber,amber, red
static const int flyfort_row1_colors[] = {2, 7, 7, 7, 2}; //red, steel gray, steel gray, steel gray, red
static const int flyfort_row2_colors[] = {2, 2, 7, 2, 2}; //red, red,steel gray, red, red
// Anchor point is the # at its center
TileLayout Flying_Fortress_Layout = {
.width = 5, .height = 3,
.glyph_rows = {
    " ||| ",
    "<###>",
    " \\#/ "
},
.color_rows = {
    flyfort_row0_colors,
    flyfort_row1_colors,
    flyfort_row2_colors
}
};
static const Enemy Flying_Fortress_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.3,
    .hp = 20,
    .symbol = '%',
    .width = 5, .height = 3,
    .cooldown_frames = -60,
    .type = FLYING_FORTRESS,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Flying_Fortress_Layout,
    .weapon = &FLYFORT_CANNON
};
static const int flyfort_invuln_row0_colors[] = {9, 6, 6, 6, 9}; //purple, amber, amber, amber, purple
static const int flyfort_invuln_row1_colors[] = {9, 7, 7, 7, 9}; //purple, steel gray, steel gray, steel gray, purple
static const int flyfort_invuln_row2_colors[] = {9, 9, 7, 9, 9}; //purple, purple, steel gray, purple, purple

TileLayout Flying_Fortress_Layout_Invuln = {
.width = 5, .height = 3,
.glyph_rows = {
    " ||| ",
    "<###>",
    " \\#/ "
},
.color_rows = {
    flyfort_invuln_row0_colors, flyfort_invuln_row1_colors, flyfort_invuln_row2_colors
}
};
//Special version of flying fortress to be used as miniboss
static const Enemy Flying_Fortress_Boss_template = {
    .px = 0, .py = 0,
    .dx = 0.4, .dy = 0.4,
    .hp = 40,
    .symbol = '%',
    .width = 5, .height = 3,
    .cooldown_frames = -60,
    .type = FLYFORT_SPECIAL,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Flying_Fortress_Layout,
    .weapon = &FLYFORT_CANNON,
    .is_boss_part = 1,
    .is_boss_core = 0
};
static const int laser_jet_row0_colors[] = {2, 6, 6, 2}; //red, amber, red
static const int laser_jet_row1_colors[] = {2, 7, 7, 2}; //red, steel gray, red
static const int laser_jet_row2_colors[] = {2, 3, 3, 2}; //red, yellow,yellow, red
TileLayout Laser_Jet_Layout = {
.width = 4, .height = 3,
.glyph_rows = {
    " || ",
    "<##>",
    " !! "
},
.color_rows = {
    laser_jet_row0_colors,
    laser_jet_row1_colors,
    laser_jet_row2_colors
}
};
static const Enemy laser_jet_template = {
    .px = 0, .py = 0,
    .dx = 3, .dy = 0.5,
    .hp = 8,
    .symbol = '%',
    .width = 4, .height = 3,
    .cooldown_frames = -120,
    .type = LASER_JET,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Laser_Jet_Layout,
    .weapon = &laserrifle
};
//Template for core of first boss
// Carrier boss shape — turret sockets marked '.', aircraft bays marked 'o'
// Use these positions (row, col) as reference offsets when placing turret/grunt-spawn subsystems relative to the core anchor
static  int carrier_colors_row0[10] = {7,7,6,7,7,6,7,7,6,7};
static  int carrier_colors_row1[10] = {2,2,2,2,2,2,2,2,2,2};
static  int carrier_colors_row2[10] = {7,7,7,7,7,7,7,7,7,7};
static  int carrier_colors_row3[10] = {7,7,2,7,7,7,7,2,7,7};
static  int carrier_colors_row4[10] = {7,2,2,7,7,7,7,2,2,7};
static int carrier_colors_row5[10] = {2,2,2,2,2,2,2,2,2,2};
static TileLayout carrier_layout = {
    .width = 10, .height = 6,
    .glyph_rows = {
        "  |  |  | ",
        "==========",
        "|########|",
        "|#.####.#|",
        "|..####..|",
        "=========="
    },
    .color_rows = {
        carrier_colors_row0, carrier_colors_row1, carrier_colors_row2,
        carrier_colors_row3, carrier_colors_row4, carrier_colors_row5
    }
};
static Enemy carrier_boss_core_template = {
    .px = 0, .py = 0,
    .dx = 0, .dy = 0,
    .hp = 80,
    .symbol = '%',
    .width = 10, .height = 6,
    .cooldown_frames = -120,
    .type = CARRIER_BOSS,
    .state = INACTIVE,
    .behavior = CARRIER_SPECIAL,
    .shape = &carrier_layout,
    .weapon = &HUNTER_RIFLE,
    .is_boss_part = 1, //Is part of the carrier boss
    .is_boss_core = 1 //Is core of the carrier boss
};
//Special layout to be used during the invuln state
static const int carrier_invuln_colors_row0[10] = {7,7,6,7,7,6,7,7,6,7}; // contrails unchanged
static const int carrier_invuln_colors_row1[10] = {9,9,9,9,9,9,9,9,9,9}; // top edge -> purple
static const int carrier_invuln_colors_row2[10] = {7,7,7,7,7,7,7,7,7,7}; // hull unchanged
static const int carrier_invuln_colors_row3[10] = {7,7,9,7,7,7,7,9,7,7}; // turret sockets -> purple
static const int carrier_invuln_colors_row4[10] = {7,9,9,7,7,7,7,9,9,7}; // aircraft bays -> purple
static const int carrier_invuln_colors_row5[10] = {9,9,9,9,9,9,9,9,9,9}; // bottom edge -> purple

static TileLayout carrier_layout_invuln = {
    .width = 10, .height = 6,
    .glyph_rows = {
        "  |  |  | ",
        "==========",
        "|########|",
        "|#.####.#|",
        "|..####..|",
        "=========="
    },
    .color_rows = {
        carrier_invuln_colors_row0, carrier_invuln_colors_row1, carrier_invuln_colors_row2,
        carrier_invuln_colors_row3, carrier_invuln_colors_row4, carrier_invuln_colors_row5
    }
};
//Layout for carrier's machine guns
static const int mg_colors_row0[2] = {3, 3};
static const int mg_colors_row1[2] = {2, 2};
static const int mg_colors_row2[2] = {2, 2};
static const int mg_colors_row3[2] = {3, 3};

static TileLayout flakgun_layout = {
    .width = 2, .height = 4,
    .glyph_rows = {
        "^^",
        "##",
        "##",
        "vv"
    },
    .color_rows = {
        mg_colors_row0, mg_colors_row1, mg_colors_row2, mg_colors_row3
    }
};
static Enemy carrier_boss_FLAK_template = {
    .px = 0, .py = 0,
    .dx = 0, .dy = 0,
    .hp = 15,
    .symbol = '%',
    .width = 2, .height = 4,
    .cooldown_frames = -60,
    .type = CARRIER_BOSS_FLAK,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &flakgun_layout,
    .weapon = &CARRIER_FLAK,
    .is_boss_part = 1, //Is part of the carrier boss
    .is_boss_core = 0 
};
//Bomb launcher of carrier boss
static const int bc_colors_row0[3] = {3, 3, 3};
static const int bc_colors_row1[3] = {2, 2, 2};
static const int bc_colors_row2[3] = {2, 2, 2};
static const int bc_colors_row3[3] = {2, 2, 2};
static const int bc_colors_row4[3] = {3, 3, 3};

static TileLayout bombcannon_layout = {
    .width = 3, .height = 5,
    .glyph_rows = {
        "/#\\",
        "###",
        "###",
        "###",
        "\\#/"
    },
    .color_rows = {
        bc_colors_row0, bc_colors_row1, bc_colors_row2, bc_colors_row3, bc_colors_row4
    }
};
static Enemy carrier_boss_bomb_template = {
    .px = 0, .py = 0,
    .dx = 0, .dy = 0,
    .hp = 15,
    .symbol = '%',
    .width = 3, .height = 5,
    .cooldown_frames = -10,
    .type = CARRIER_BOSS_BOMB,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &bombcannon_layout,
    .weapon = &CARRIER_CANNON,
    .is_boss_part = 1, //Is part of the carrier boss
    .is_boss_core = 0 
};
//Miniboss of stage 2
//Twin frigates that mirror each other's attacks and behaviours
// Frigate hull colors
static const int frigate_row0_colors[5] = {11, 11, 3, 11, 11};   // amber contrails, yellow turret
static const int frigate_row1_colors[5] = {11, 2, 7, 2, 11};     // red outline, steel gray interior
static const int frigate_row2_colors[5] = {2, 7, 7, 7, 2};       // red edges
static const int frigate_row3_colors[5] = {11, 2, 7, 2, 11};     // red outline, steel gray interior
static const int frigate_row4_colors[5] = {11, 11, 11, 11, 11};  // amber thruster exhaust

TileLayout Frigate_Layout = {
    .width = 5, .height = 5,
    .glyph_rows = {
        "  T  ",
        " ### ",
        "#####",
        " ### ",
        "  V  "
    },
    .color_rows = {
        frigate_row0_colors,
        frigate_row1_colors,
        frigate_row2_colors,
        frigate_row3_colors,
        frigate_row4_colors
    }
};

// Frigate 1 template
static const Enemy frigate1_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.4,
    .hp = 25,
    .symbol = '%',
    .width = 5, .height = 5,
    .cooldown_frames = -10,
    .type = FRIGATE1,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Frigate_Layout,
    .weapon = &FRIGATE_FLAK,
    .is_boss_part = 1,
    .is_boss_core = 0
};
// Frigate 1 template
static const Enemy frigate2_template = {
    .px = 0, .py = 0,
    .dx = 1, .dy = 0.4,
    .hp = 25,
    .symbol = '%',
    .width = 5, .height = 5,
    .cooldown_frames = -10,
    .type = FRIGATE2,
    .state = INACTIVE,
    .behavior = STATIC,
    .shape = &Frigate_Layout,
    .weapon = &FRIGATE_LASER,
    .is_boss_part = 1,
    .is_boss_core = 0
};
// Battleship layout — underside bristling with 3 cannon types
static const int bship_row0[40] = {[0 ... 39] = 6}; // contrails - amber
static const int bship_row1[40] = {[0 ... 39] = 7}; // hull edge - steel gray
static const int bship_row2[40] = {[0 ... 39] = 2}; // hull - red
static const int bship_row3[40] = {[0 ... 39] = 2};
static const int bship_row4[40] = {
    [0 ... 39] = 2, [15 ... 24] = 9 // core section - purple
};
static const int bship_row5[40] = {[0 ... 39] = 2};
static const int bship_row6[40] = {[0 ... 39] = 7}; // turret socket band
static const int bship_row7[40] = {
    [0 ... 39] = 2,
    [3]=6, [9]=3, [15]=6, [21]=6, [27]=3, [33]=6 // socket colors per cannon type
};
static const int bship_row8[40] = {
    [0 ... 39] = 2,
    [4]=6, [10]=3, [16]=6, [22]=6, [28]=3, [34]=6
};
static const int bship_row9[40] = {
    [0 ... 39] = 2,
    [4]=6, [10]=3, [16]=6, [22]=6, [28]=3, [34]=6
};

static TileLayout Battleship_Layout = {
    .width = 40, .height = 10,
    .glyph_rows = {
        "   |   |   |   |   |   |   |   |   |   |",
        "/======================================\\",
        "|######################################|",
        "|######################################|",
        "|##############@@@@@@@@@@##############|",
        "|######################################|",
        "|###.###.###.###.###.###.###.###.###.##|",
        "|###v#####!###########v#####!##########|",
        "    #     !     #     #     !     #     ",
        "    V     !     o     V     !     o     "
    },
    .color_rows = {
        bship_row0, bship_row1, bship_row2, bship_row3, bship_row4,
        bship_row5, bship_row6, bship_row7, bship_row8, bship_row9
    }
};

static const int bship_invuln_row1[40] = {[0 ... 39] = 9}; // hull edge -> purple
static const int bship_invuln_row6[40] = {[0 ... 39] = 9}; // socket band -> purple
static const int bship_invuln_row7[40] = {
    [0 ... 39] = 2,
    [3]=9, [9]=9, [15]=9, [21]=9, [27]=9, [33]=9 // all turret sockets -> purple
};
static TileLayout Battleship_Layout_Invuln = {
    .width = 40, .height = 10,
    .glyph_rows = {
        "   |   |   |   |   |   |   |   |   |   |",
        "/======================================\\",
        "|######################################|",
        "|######################################|",
        "|##############@@@@@@@@@@##############|",
        "|######################################|",
        "|###.###.###.###.###.###.###.###.###.##|",
        "|###v#####!#####%#####v#####!#####%####|",
        "    #     !     %     #     !     %     ",
        "    V     !     o     V     !     o     "
    },
    .color_rows = {
        bship_row0, bship_invuln_row1, bship_row2, bship_row3, bship_row4,
        bship_row5, bship_invuln_row6, bship_invuln_row7, bship_row8, bship_row9
    }
};

// Template for battleship, boss of stage 2
static const Enemy Battleship_template = {
    .px = 0, .py = 0,
    .dx = 0.1, .dy = 0.1,
    .hp = 200,
    .symbol = '%',
    .width = 40, .height = 10,
    .cooldown_frames = -30,
    .type = BATTLESHIP_BOSS,
    .state = INACTIVE,
    .behavior = BATTLESHIP_SPECIAL,
    .shape = &Battleship_Layout,
    .weapon = &shotgun,
    .is_boss_part = 1,
    .is_boss_core = 1
};
//Tile layout for the mothership, the game's final boss
static const int mship_row0_colors[30] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
static const int mship_row1_colors[30] = {9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
static const int mship_row2_colors[30] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9};
static const int mship_row3_colors[30] = {9,9,9,9,9,8,9,9,9,9,9,9,2,2,2,2,2,2,9,9,9,9,9,9,9,9,9,9,9,9};
static const int mship_row4_colors[30] = {9,9,9,9,9,9,9,9,9,9,9,9,2,2,2,2,2,2,9,9,9,9,9,8,9,9,9,9,9,9};
static const int mship_row5_colors[30] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
static const int mship_row6_colors[30] = {9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
static const int mship_row7_colors[30] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};

static TileLayout Mothership_Layout = {
    .width = 30, .height = 8,
    .glyph_rows = {
        "        ##############        ",
        "    ######~###############    ",
        "  ##################~#######  ",
        " #####~#####......########### ",
        " ###########......#####~##### ",
        "  #############~############  ",
        "    ##########~###########    ",
        "        ##############        "
    },
    .color_rows = {
        mship_row0_colors, mship_row1_colors, mship_row2_colors, mship_row3_colors,
        mship_row4_colors, mship_row5_colors, mship_row6_colors, mship_row7_colors
    }
};
static const int mship_invuln_row0_colors[30] = {0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0};
static const int mship_invuln_row1_colors[30] = {0,0,0,0,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0};
static const int mship_invuln_row2_colors[30] = {0,0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,0,0};
static const int mship_invuln_row3_colors[30] = {0,9,9,9,9,9,8,9,9,9,9,9,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,0};
static const int mship_invuln_row4_colors[30] = {0,9,9,9,9,9,9,9,9,9,9,9,8,8,8,8,8,8,9,9,9,9,9,8,9,9,9,9,9,0};
static const int mship_invuln_row5_colors[30] = {0,0,9,9,9,9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9,9,9,9,0,0};
static const int mship_invuln_row6_colors[30] = {0,0,0,0,9,9,9,9,9,9,9,9,9,9,8,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0};
static const int mship_invuln_row7_colors[30] = {0,0,0,0,0,0,0,0,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,0,0,0,0,0,0,0};

static TileLayout Mothership_Layout_Invuln = {
    .width = 30, .height = 8,
    .glyph_rows = {
        "        ##############        ",
        "    ######~###############    ",
        "  ##################~#######  ",
        " #####~#####......########### ",
        " ###########......#####~##### ",
        "  #############~############  ",
        "    ##########~###########    ",
        "        ##############        "
    },
    .color_rows = {
        mship_invuln_row0_colors, mship_invuln_row1_colors, mship_invuln_row2_colors, mship_invuln_row3_colors,
        mship_invuln_row4_colors, mship_invuln_row5_colors, mship_invuln_row6_colors, mship_invuln_row7_colors
    }
};

static const Enemy Mothership_template = {
    .px = 0, .py = 0,
    .dx = 0.1, .dy = 0.1,
    .hp = 75,
    .symbol = '%',
    .width = 30, .height = 8,
    .cooldown_frames = -30,
    .type = MOTHERSHIP,
    .state = INACTIVE,
    .behavior = MOTHERSHIP_SPECIAL,
    .shape = &Mothership_Layout,
    .weapon = NULL,
    .is_boss_part = 1,
    .is_boss_core = 1
};
//Function to find a free slot in the enemy pool
int findfreeslot(void){
    for(int i=0; i < MAX_ENEMIES; i++){
        if (enemies[i].state == INACTIVE){
            return i;
        }
    }
return -1; //No valid slot found
}
//Helper functions to spawn specific waves of enemies
//Meant for use by the mothership boss
//Spawns barriers along three sides of the boss
void Spawn_Barrier(Enemy caller){
    for(int i = 0; i < caller.width; i++){
spawn_enemy(BARRIER, STATIC, false, caller.px + i,
caller.py + caller.height + 1, 0);
    }
     for(int i = 0; i < caller.height; i++){
spawn_enemy(BARRIER, STATIC, false, caller.px -1,
caller.py + i, 0);
    }
     for(int i = 0; i < caller.height; i++){
spawn_enemy(BARRIER, STATIC, false, caller.px + caller.width + 1,
caller.py + i, 0);
    }
}
void Delete_Barriers(Enemy *enemies){
    for(int i = 0; i < MAX_ENEMIES; i++){
if(enemies[i].type == BARRIER) enemies[i].state = DEAD;
    }
}
//Spawns waves of weak fodder enemies
void Rapidfire_Wave(Enemy caller, float number, float spacing){
    for(int i = 0; i < number; i++){
spawn_enemy(RAPIDFIRE, MOVEVERTICALLY, false, caller.px + i * (caller.width/spacing),
caller.py + caller.height + 2, 0);
    }
}

//Spawns dangerous hunters
void Hunter_Wave(Enemy caller, float number, float spacing){
    for(int i = 1; i < (number/2); i++){
spawn_enemy(HUNTER, HUNT_PLAYER_FAR, false, caller.px ,
caller.py + i* (caller.height/spacing), 0);
spawn_enemy(HUNTER, HUNT_PLAYER_FAR, false, caller.px ,
caller.py + i* (caller.height/spacing), 0);
    }
}

//Spawns lots of stationary bombers
void Bomber_Wave(Enemy caller, float number, float spacing){
    for(int i = 1; i < number; i++){
spawn_enemy(BOMBER, STATIC, false, caller.px + i* (caller.width/spacing),
caller.py + caller.height + 3, 0);
    }
}

//Spawns 4 corvettes along the edges of the screen that try to flank the player
void Corvettes_flanking(Enemy caller){
    for(int i = 1; i < 2; i++){
spawn_enemy(CORVETTE, STRAFE_VERTICAL, TRUE, 5,
caller.py + caller.height + 1, 20);
spawn_enemy(CORVETTE, STRAFE_VERTICAL, TRUE, 95,
caller.py + caller.height + 1, 20);
    }   
}

//Spawns some corvettes that strafe below the mothershi[]
void Corvette_Wave(Enemy caller, float number, float spacing){
    for(int i = 1; i < number; i++){
spawn_enemy(CORVETTE, STRAFE_HORIZONTAL, false, caller.px + i*(caller.width/spacing),
caller.py + caller.height + 3, 100);
    }
}
//Finds the address of a template and returns it. Add a new case each time a new template is made
const Enemy* get_template(EnemyType type) {
    switch (type) {
        case GRUNT:     return &grunt_template;
        case RAPIDFIRE: return &rapidfire_template;
        case LASER_ENEMY:     return &laser_template;
        case BOMBER:    return &bomber_template;
        case HUNTER:    return &hunter_template;
        case JET:       return &jet_template;
        case FLYING_FORTRESS: return &Flying_Fortress_template;
        case LASER_JET: return &laser_jet_template;
        case CARRIER_BOSS: return &carrier_boss_core_template;
        case CARRIER_BOSS_FLAK: return &carrier_boss_FLAK_template;
        case CARRIER_BOSS_BOMB: return &carrier_boss_bomb_template;
        case FRIGATE1:          return &frigate1_template;
        case FRIGATE2:          return &frigate2_template;
        case JET_BOSS:          return &jet_boss_template;
        case BATTLESHIP_BOSS:    return &Battleship_template;
        case REFLECTOR:         return &reflector_template;
        case BARRIER:           return &barrier_template;
        case CORVETTE:          return &corvette_template;
        case FLYFORT_BOSS:      return &Flying_Fortress_Boss_template;
        case MOTHERSHIP:        return &Mothership_template;
        default:        return NULL;
    }
}
//Spawns enemies. Can take arguments to set their type, behavior, spawn point, anchor point, and strafe values
void spawn_enemy(EnemyType type, EnemyBehavior behavior, bool aimed, float px, float py, float strafe){
int slot = findfreeslot();
if (slot == -1){
    return;
}
const Enemy *template = get_template(type);
if (template == NULL) {
    return; //Unknown type, exits the function
}

enemies[slot] = *template;
if(aimed == TRUE){
    enemies[slot].aimed = TRUE;
}
enemies[slot].px = px;
enemies[slot].py = py;
enemies[slot].old_px = px;
enemies[slot].old_py = py;
enemies[slot].strafe = strafe;
enemies[slot].anchor_px = enemies[slot].px;
enemies[slot].anchor_py = enemies[slot].py;
enemies[slot].fire_px = enemies[slot].px;
enemies[slot].fire_py = enemies[slot].py;
enemies[slot].behavior = behavior;
enemies[slot].state = ALIVE;
if (enemies[slot].shape != NULL){
enemies[slot].px = enemies[slot].px - (template->shape->width  / 2.0f);
enemies[slot].py = enemies[slot].py - (template->shape->height / 2.0f);
enemies[slot].fire_px = enemies[slot].px + ceilf(enemies[slot].width / 2.0f);
enemies[slot].fire_py = enemies[slot].py + ceilf(enemies[slot].height / 2.0f);
enemies[slot].anchor_px = enemies[slot].px;
enemies[slot].anchor_py = enemies[slot].py;
} 
}
//Function that moves living enemies based on their behavior type and removes dead enemies
void move_enemy(Enemy *enemies, Player *player, int max_x, int max_y){
for (int i =0; i < MAX_ENEMIES; i++ ){
    enemies[i].age++;
    if (enemies[i].state == ALIVE){
if(enemies[i].shape !=NULL){
enemies[i].fire_px = enemies[i].px + ceilf(enemies[i].width / 2.0f);
enemies[i].fire_py = enemies[i].py + ceilf(enemies[i].height / 2.0f);}
else {
enemies[i].fire_px = enemies[i].px ;
enemies[i].fire_py = enemies[i].py;
}
switch(enemies[i].behavior){
    case STATIC:
    break; //No need to move them since they are stationary by design
    case MOVEVERTICALLY: //Moves vertically downwards and then despawns
    enemies[i].py = enemies[i].py + enemies[i].dy;
    if (enemies[i].py >= PLAYFIELD_H - 1) {
enemies[i].state = DEAD; //Despawns upon hitting the bottom edge of the screen
    }
break;
case MOVEHORIZONTALLY: //Moves from left to right and then despawms
enemies[i].px = enemies[i].px + enemies[i].dx;
 if (enemies[i].px >= PLAYFIELD_W - 1) {
enemies[i].state = DEAD; //Despawns upon hitting the right edge of the screen
 }
break;
case STRAFE_HORIZONTAL:
enemies[i].px = enemies[i].px + enemies[i].dx;
//Reverses direction if it goes too far from its anchor point or hits a border
if (fabs(enemies[i].anchor_px - enemies[i].px) > enemies[i].strafe){
  enemies[i].dx = -enemies[i].dx; 
  enemies[i].px = enemies[i].px + enemies[i].dx;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W - enemies[i].width;
}
if  (enemies[i].px + enemies[i].width -1 <= 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px =  enemies[i].width;
}
break;
case STRAFE_VERTICAL:
enemies[i].py = enemies[i].py + enemies[i].dy;
//Reverses direction if it goes too far from its anchor point or hits a border
if (fabs(enemies[i].anchor_py - enemies[i].py) > enemies[i].strafe) 
enemies[i].dy = -enemies[i].dy;
enemies[i].py = enemies[i].py + enemies[i].dy;
//Boundary checking code
if ((int)enemies[i].py + enemies[i].height -1 >= PLAYFIELD_H-1){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = PLAYFIELD_H - enemies[i].height;
}
if  (enemies[i].py + enemies[i].height -1 <= 0){
enemies[i].dy = -enemies[i].dy;
enemies[i].py = enemies[i].height;
}
break;
// Hunters will attempt to get within 4 rows of the player 
// and then attack with their rapid-firing cannons.
// They will always attempt to be on the same column as the player
// They will never go below the player
case HUNT_PLAYER:
static int hunt_tick = 0;
hunt_tick++;
if (hunt_tick % 4 == 0){
if (enemies[i].px < player->px) enemies[i].px +=  enemies[i].dx;
if (enemies[i].px > player->px) enemies[i].px -=  enemies[i].dx;
float target_py = player->py - 4;
if (enemies[i].py < target_py ) enemies[i].py += enemies[i].dy;
if (enemies[i].py > target_py ) enemies[i].py -= enemies[i].dy;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W -1;
}
if  (enemies[i].px + enemies[i].width -1 == 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px -=  enemies[i].dx;
}
if ((int)enemies[i].py + enemies[i].height -1 >= PLAYFIELD_H-1){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = PLAYFIELD_H - 1;
}
if  (enemies[i].py + enemies[i].height -1 <= 0){
enemies[i].dy = -enemies[i].dy;
enemies[i].py = 1;
}
break;
case HUNT_PLAYER_FAR:
static int hunt_tick_2 = 0;
hunt_tick_2++;
if (hunt_tick_2 % 4 == 0){
if (enemies[i].px < player->px) enemies[i].px +=  enemies[i].dx;
if (enemies[i].px > player->px) enemies[i].px -=  enemies[i].dx;
float target_py = player->py - 8;
if (enemies[i].py < target_py ) enemies[i].py += enemies[i].dy;
if (enemies[i].py > target_py ) enemies[i].py -= enemies[i].dy;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W -1;
}
if  (enemies[i].px + enemies[i].width -1 == 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px -=  enemies[i].dx;
}
if ((int)enemies[i].py + enemies[i].height -1 >= PLAYFIELD_H-1){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = PLAYFIELD_H - 1;
}
if  (enemies[i].py + enemies[i].height -1 <= 0){
enemies[i].dy = -enemies[i].dy;
enemies[i].py = 1;
}
break;
case CARRIER_SPECIAL:
boss_state_timer++;
if (BOSS_NORMAL == TRUE){
break;}
if (boss_state_timer % 500 == 0){
spawn_enemy(GRUNT, HUNT_PLAYER, false, enemies[i].px + (enemies[i].width/3),
 enemies[i].py + enemies[i].height + 2, 0);
 spawn_enemy(GRUNT, HUNT_PLAYER, false, enemies[i].px + (enemies[i].width/3 + 1),
 enemies[i].py + enemies[i].height + 2, 0);
 spawn_enemy(GRUNT, HUNT_PLAYER, false, enemies[i].px + (enemies[i].width/3 + 2),
 enemies[i].py + enemies[i].height + 2, 0);
 spawn_enemy(HUNTER, HUNT_PLAYER, false, enemies[i].px + (enemies[i].width/3 -1),
 enemies[i].py + enemies[i].height + 2, 0);
} 
if (enemies[i].hp < 50 && state == BOSS_NORMAL) {
    state = SPECIAL_ATTACK_1;
    static int carrier_colors_row5[10] = {2,2,2,2,2,2,2,2,2,2};
spawn_enemy(CARRIER_BOSS_FLAK, STATIC, false, enemies[i].px + (enemies[i].width/3),
 enemies[i].py + enemies[i].height + 2, 0);
}
if (enemies[i].hp < 20 && state == SPECIAL_ATTACK_1){
for(int core = 0; core <= MAX_ENEMIES; core++){
    if(enemies[core].is_boss_core == TRUE){
    enemies[core].shape = &carrier_layout_invuln;
    }
}
    state = SPECIAL_ATTACK_2;
    boss_state_timer = 0;
    boss_invulnerable = TRUE; 
spawn_enemy(CARRIER_BOSS_BOMB, STATIC, false, enemies[i].px + (2*enemies[i].width/3 + 3),
 enemies[i].py + enemies[i].height + 3, 0);
}
if (boss_state_timer >= 180 && state == SPECIAL_ATTACK_2){
boss_invulnerable = FALSE;
for(int core = 0; core <= MAX_ENEMIES; core++){
    if(enemies[core].is_boss_core == TRUE){
    enemies[core].shape = &carrier_layout;
    }
}
}
break;
case FRIGATE1_SPECIAL:
static int frigate1_tick = 0;
frigate1_tick++;
if (frigate1_tick % 6 == 0){
if (enemies[i].px < player->px) enemies[i].px +=  enemies[i].dx;
if (enemies[i].px > player->px) enemies[i].px -=  enemies[i].dx;
float target_py = player->py - 10;
if (enemies[i].py < target_py ) enemies[i].py += enemies[i].dy;
if (enemies[i].py > target_py ) enemies[i].py -= enemies[i].dy;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].px = PLAYFIELD_W - enemies[i].width - 1;
}
if (enemies[i].px <= 0){
   enemies[i].px = 0 + enemies[i].width;
}
if ((int)enemies[i].py + enemies[i].height - 1 >= PLAYFIELD_H - 1){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = PLAYFIELD_H - enemies[i].height;
}
if (enemies[i].py + enemies[i].height <= 0){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = 0 + enemies[i].height;
}
if (frigate1_tick % 180 == 0){
if(enemies[i].weapon == &FRIGATE_FLAK)enemies[i].weapon = &FRIGATE_LASER;
else if(enemies[i].weapon == &FRIGATE_LASER)enemies[i].weapon = &FRIGATE_FLAK;
}
break;
case FRIGATE2_SPECIAL:
static int frigate2_tick = 0;
frigate2_tick++;
if (frigate2_tick % 6 == 0){
if (enemies[i].px < player->px + 30) enemies[i].px +=  enemies[i].dx;
if (enemies[i].px > player->px + 30) enemies[i].px -=  enemies[i].dx;
float target_py = player->py - 10;
if (enemies[i].py < target_py ) enemies[i].py += enemies[i].dy;
if (enemies[i].py > target_py ) enemies[i].py -= enemies[i].dy;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W -1;
}
if  (enemies[i].px + enemies[i].width -1 == 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px -=  enemies[i].dx;
}
if ((int)enemies[i].py + enemies[i].height -1 >= PLAYFIELD_H-1){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = PLAYFIELD_H - enemies[i].height;
}
if  (enemies[i].py + enemies[i].height -1 <= 0){
enemies[i].dy = -enemies[i].dy;
enemies[i].py = 0 + enemies[i].height;
}
if (frigate2_tick % 180 == 0){
if(enemies[i].weapon == &FRIGATE_FLAK)enemies[i].weapon = &FRIGATE_LASER;
else if(enemies[i].weapon == &FRIGATE_LASER)enemies[i].weapon = &FRIGATE_FLAK;
}
break;
case JET_SPECIAL:
static int jet_state_timer;
jet_state_timer++;
static int hunt_tick_jet = 0;
hunt_tick_jet++;
if (hunt_tick_jet % 12 == 0){
if (enemies[i].px < player->px) enemies[i].px +=  enemies[i].dx;
if (enemies[i].px > player->px) enemies[i].px -=  enemies[i].dx;
float target_py = player->py - 10;
if (enemies[i].py < target_py ) enemies[i].py += enemies[i].dy;
if (enemies[i].py > target_py ) enemies[i].py -= enemies[i].dy;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W -1;
}
if  (enemies[i].px + enemies[i].width -1 == 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px -=  enemies[i].dx;
}
if ((int)enemies[i].py + enemies[i].height -1 >= PLAYFIELD_H-1){
    enemies[i].dy = -enemies[i].dy;
    enemies[i].py = PLAYFIELD_H + enemies[i].height - 1;
}
if  (enemies[i].py + enemies[i].height -1 <= 0){
enemies[i].dy = -enemies[i].dy;
enemies[i].py = 1;
}
if (jet_state_timer % 120 == 0 && boss_invulnerable != TRUE){
    if (enemies[i].weapon == &JET_CANNON) enemies[i].weapon = &BOMB_ENEMY_WEAPON;
    else if (enemies[i].weapon == &BOMB_ENEMY_WEAPON) enemies[i].weapon = &JET_CANNON;
}
static bool jet_invuln_used = FALSE;
if (enemies[i].hp < 15 && boss_invulnerable != TRUE && jet_invuln_used == FALSE){
    jet_invuln_used = TRUE;
    boss_invulnerable = TRUE;
    enemies[i].shape  = &Jet_Layout_Invuln;
    enemies[i].weapon = &FLYFORT_CANNON;
    jet_state_timer = 0;
}
if (boss_invulnerable == TRUE && jet_state_timer >= 120){
    boss_invulnerable = FALSE;
    enemies[i].shape  = &Jet_Layout;
    jet_state_timer = 0;
}
break;
case BATTLESHIP_SPECIAL:
for (int row = 0; row < Battleship_Layout.height; row++) {
    int len = strlen(Battleship_Layout.glyph_rows[row]);
    if (len != Battleship_Layout.width) {
        fprintf(stderr, "Row %d has length %d, expected %d\n", row, len, Battleship_Layout.width);
    }
}
enemies[i].px = enemies[i].px + enemies[i].dx;
//Reverses direction if it goes too far from its anchor point or hits a border
if (fabs(enemies[i].anchor_px - enemies[i].px) > enemies[i].strafe){
  enemies[i].dx = -enemies[i].dx; 
  enemies[i].px = enemies[i].px + enemies[i].dx;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W - enemies[i].width;
}
if  (enemies[i].px + enemies[i].width -1 <= 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px =  enemies[i].width;
}
static bool boss_invuln_used1= FALSE, boss_invuln_used2= FALSE, boss_invuln_used3= FALSE;
static int battleship_state_tick = 0, laser_cannon_tick =0, bomb_cannon_tick =0;
battleship_state_tick++;
laser_cannon_tick++;
bomb_cannon_tick++;
if(state == BOSS_NORMAL){
if(bomb_cannon_tick % 80 == 0){
fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px +30, enemies[i].py +8, 270, FALSE);
fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px +10, enemies[i].py +8, 270, FALSE);
} 
if (bomb_cannon_tick % 120 == 0){
    fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px + 20, enemies[i].py +8, 270, FALSE);
}
}
if (enemies[i].hp < 125 && boss_invuln_used1 == FALSE){
    boss_invuln_used1 = TRUE;
    state = SPECIAL_ATTACK_1;
    boss_invulnerable = TRUE;
    enemies[i].shape = &Battleship_Layout_Invuln;
    battleship_state_tick = 0;
     //Special bomb barrage attack  
fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px +20 , enemies[i].py +8, 210, FALSE);
fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px +25, enemies[i].py +8, 240, FALSE); 
fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px +15, enemies[i].py +8, 270, FALSE);
fire_weapon(&LASER_RIFLE_ENEMY, enemies[i].px +30 , enemies[i].py +8, 300, FALSE);
fire_weapon(&LASER_RIFLE_ENEMY, enemies[i].px +10, enemies[i].py +8, 330, FALSE);  
}
if (enemies[i].hp < 50 && boss_invuln_used2 == FALSE){
    boss_invuln_used2 = TRUE;
    state = SPECIAL_ATTACK_2;
    boss_invulnerable = TRUE;
    enemies[i].shape = &Battleship_Layout_Invuln;
    battleship_state_tick = 0;
    
}

if (enemies[i].hp < 25 && boss_invuln_used3 == FALSE){
    boss_invuln_used3 = TRUE;
    state = SPECIAL_ATTACK_3;
    boss_invulnerable = TRUE;
    enemies[i].shape = &Battleship_Layout_Invuln;
    battleship_state_tick = 0;
    enemies[i].weapon = &FLYFORT_CANNON;
}
if(battleship_state_tick >= 180 && boss_invulnerable == TRUE){
    boss_invulnerable = FALSE;
    enemies[i].shape = &Battleship_Layout;
}
if (state= SPECIAL_ATTACK_3 && boss_invulnerable == FALSE) state = BOSS_NORMAL;
if(state == SPECIAL_ATTACK_1){
if(battleship_state_tick % 25 == 0){
fire_weapon(&spiral_cannon, enemies[i].px +20, enemies[i].py + 8, 270, FALSE);  }}
if(state == SPECIAL_ATTACK_2){
if(battleship_state_tick%50 == 0){
    fire_weapon(&CARRIER_CANNON, enemies[i].px +20, enemies[i].py +8, 270, FALSE);
    fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px + 30, enemies[i].py +8, 270, FALSE);
    fire_weapon(&BOMB_ENEMY_WEAPON, enemies[i].px + 10, enemies[i].py +8, 270, FALSE);
    }
if(battleship_state_tick % 20 == 0){ 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px , enemies[i].py +8, 270, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 40, enemies[i].py +8, 270, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 30, enemies[i].py +8, 315, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 10, enemies[i].py +8, 225, FALSE);  }
}
if (state == SPECIAL_ATTACK_3){
    if(battleship_state_tick % 20 == 0){   
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px , enemies[i].py +8, 270, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 40, enemies[i].py +8, 270, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 15, enemies[i].py +8, 270, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 25, enemies[i].py +8, 270, FALSE);   
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 30, enemies[i].py +8, 315, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 10, enemies[i].py +8, 225, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 15, enemies[i].py +8, 270, FALSE); 
fire_weapon(&RAPIDFIRE_RIFLE, enemies[i].px + 25, enemies[i].py +8, 270, FALSE); 
    }
}

break;
case FLYFORT_SPECIAL:
enemies[i].px = enemies[i].px + enemies[i].dx;
//Reverses direction if it goes too far from its anchor point or hits a border
if (fabs(enemies[i].anchor_px - enemies[i].px) > enemies[i].strafe){
  enemies[i].dx = -enemies[i].dx; 
  enemies[i].px = enemies[i].px + enemies[i].dx;
}
if ((int)enemies[i].px + enemies[i].width - 1 >= PLAYFIELD_W){
   enemies[i].dx = -enemies[i].dx;
   enemies[i].px = PLAYFIELD_W - enemies[i].width;
}
if  (enemies[i].px + enemies[i].width -1 <= 0 ){
enemies[i].dx = -enemies[i].dx;
enemies[i].px =  enemies[i].width;
}
static bool flyfort_invuln_used = FALSE;
static int flyfort_tick = 0;
flyfort_tick++;
if (flyfort_tick % 180 == 0){
if(enemies[i].weapon == &FLYFORT_CANNON)enemies[i].weapon = &shotgun;
else if(enemies[i].weapon == &shotgun)enemies[i].weapon = &FLYFORT_CANNON;
}
if(enemies[i].hp < 10 && flyfort_invuln_used == FALSE){
    flyfort_invuln_used = TRUE;
    boss_invulnerable = TRUE;
    flyfort_tick = 0;
}
if(flyfort_tick % 180 == 0) boss_invulnerable = FALSE; 
break;
case MOTHERSHIP_SPECIAL:
static int mothership_tick = 0;
mothership_tick++;
#define MOTHERSHIP_INVULN_TIME 300
static bool mothership_invuln_used1= FALSE, mothership_invuln_used2= FALSE, mothership_invuln_used3= FALSE,
mothership_invuln_used4= FALSE, mothership_invuln_used5= FALSE;
if(enemies[i].hp < 60 && mothership_invuln_used1 == FALSE){
    mothership_tick = 0;
    mothership_invuln_used1 = TRUE;
    boss_invulnerable = TRUE;
    Delete_Barriers(enemies);
    Spawn_Barrier(enemies[i]);
    state = SPECIAL_ATTACK_1;
    enemies[i].shape = &Mothership_Layout_Invuln;
}
if(enemies[i].hp < 50 && mothership_invuln_used2 == FALSE){
    mothership_tick = 0;
    mothership_invuln_used2 = TRUE;
    boss_invulnerable = TRUE;
    Delete_Barriers(enemies);
    Spawn_Barrier(enemies[i]);
    state = SPECIAL_ATTACK_2;
    enemies[i].shape = &Mothership_Layout_Invuln;
}
if(enemies[i].hp < 40 && mothership_invuln_used3 == FALSE){
    mothership_tick = 0;
    mothership_invuln_used3 = TRUE;
    boss_invulnerable = TRUE;
    Delete_Barriers(enemies);
    Spawn_Barrier(enemies[i]);
    state = SPECIAL_ATTACK_3;
    enemies[i].shape = &Mothership_Layout_Invuln;
}
if(enemies[i].hp < 20 && mothership_invuln_used4 == FALSE){
    mothership_tick = 0;
    mothership_invuln_used4 = TRUE;
    boss_invulnerable = TRUE;
    Delete_Barriers(enemies);
    Spawn_Barrier(enemies[i]);
    state = SPECIAL_ATTACK_4;
    enemies[i].shape = &Mothership_Layout_Invuln;
}
if(enemies[i].hp < 10 && mothership_invuln_used5 == FALSE){
    mothership_tick = 0;
    mothership_invuln_used5 = TRUE;
    boss_invulnerable = TRUE;
    Delete_Barriers(enemies);
    Spawn_Barrier(enemies[i]);
    state = SPECIAL_ATTACK_5;
    enemies[i].shape = &Mothership_Layout_Invuln;
}
if(mothership_tick >= MOTHERSHIP_INVULN_TIME && boss_invulnerable == TRUE){
    boss_invulnerable = FALSE;
    mothership_tick = 0;
    enemies[i].shape = &Mothership_Layout;
}
if(mothership_tick % 150 == 0) Rapidfire_Wave(enemies[i], 8, 8);
if(mothership_tick % 240 == 0) Corvette_Wave(enemies[i],1,1);
if(state == SPECIAL_ATTACK_1){
 if(mothership_tick == 100) Bomber_Wave(enemies[i],3,3);  
 if(mothership_tick == 200) Corvettes_flanking(enemies[i]);  
 if(mothership_tick == 200) Hunter_Wave(enemies[i],4,4);  
 if(mothership_tick == 299) Hunter_Wave(enemies[i],4,4);   
}
if(state == SPECIAL_ATTACK_2){
 if(mothership_tick == 100) Hunter_Wave(enemies[i],4,6);   
 if(mothership_tick == 200) Corvettes_flanking(enemies[i]); 
 if(mothership_tick == 299) Corvette_Wave(enemies[i],2,2); 
}
if(state == SPECIAL_ATTACK_3){
 if(mothership_tick == 100) Bomber_Wave(enemies[i],5,5);   
 if(mothership_tick == 200) Corvettes_flanking(enemies[i]);
}
if(state == SPECIAL_ATTACK_4){
    if(mothership_tick == 50)Hunter_Wave(enemies[i],2,2);
    if(mothership_tick == 100)Bomber_Wave(enemies[i],2,2);
    if(mothership_tick == 200) Corvettes_flanking(enemies[i]);
    if(mothership_tick == 250)Hunter_Wave(enemies[i],2,2);
}
if(state == SPECIAL_ATTACK_5){
    if(mothership_tick == 50)Hunter_Wave(enemies[i],3,3);
    if(mothership_tick == 100)Bomber_Wave(enemies[i],2,2);
    if(mothership_tick == 200) Corvettes_flanking(enemies[i]);
    if(mothership_tick == 250)Hunter_Wave(enemies[i],3,3);
}
default:
break;
} 
}
}
}

//Function that erases the old positions of enemies each frame
void erase_enemies(Enemy *enemies){
    for(int i=0; i < MAX_ENEMIES; i++){
        if (enemies[i].shape == NULL && (enemies[i].state == ALIVE || enemies[i].state == DEAD)){
if (enemies[i].shape == NULL){   
    mvaddch(offset_y + enemies[i].py, offset_x + enemies[i].px, ' ');
     enemies[i].old_px = enemies[i].px;
    enemies[i].old_py = enemies[i].py;
    if (enemies[i].state == DEAD) {
        enemies[i].state = INACTIVE;
    }
     }
             }
     if (enemies[i].shape != NULL && (enemies[i].state == ALIVE || enemies[i].state == DEAD)){
    const TileLayout *shape = enemies[i].shape;
    for (int row = 0; row < shape->height; row++) {
        for (int col = 0; col < shape->width; col++) {
            char glyph = shape->glyph_rows[row][col];
            if (glyph == ' ') continue;
            int color = shape->color_rows[row][col];
            attron(COLOR_PAIR(color));
            mvaddch(offset_y + (int)enemies[i].py + row,
                    offset_x + (int)enemies[i].px + col,
                    ' ');
            attroff(COLOR_PAIR(color));
        }
    }
    enemies[i].old_px = enemies[i].px;
    enemies[i].old_py = enemies[i].py;
    if (enemies[i].state == DEAD) {
        enemies[i].state = INACTIVE;
    }
}

    }
}
//Function that renders living enemies
void render_enemies(Enemy *enemies){
for(int i=0; i < MAX_ENEMIES; i++){
   /* if (enemies[i].state != INACTIVE){
    fprintf(stderr, "%d %d %d",i, enemies[i].state, enemies[i].shape);
    } */
    if (enemies[i].shape == NULL && enemies[i].state == ALIVE){
    attron(COLOR_PAIR(2));
     mvaddch(offset_y + enemies[i].py, offset_x + enemies[i].px, enemies[i].symbol);
     attroff(COLOR_PAIR(2));
     wnoutrefresh(stdscr);
     
    }
    //This block renders all multi-tile enemies
     if (enemies[i].shape != NULL && enemies[i].state ==ALIVE){
      //  fprintf(stderr, "rendering shape enemy at %d\n", i);
const TileLayout *shape = enemies[i].shape;
    for (int row = 0; row < shape->height; row++) {
        for (int col = 0; col < shape->width; col++) {
            char glyph = shape->glyph_rows[row][col];
            // Does not render anything in blank cells
            if (glyph == ' ') continue;
            int color = shape->color_rows[row][col];
            attron(COLOR_PAIR(color));
            mvaddch(offset_y + (int)enemies[i].py + row,
            offset_x + (int)enemies[i].px + col,
            glyph);
            attroff(COLOR_PAIR(color));
            wnoutrefresh(stdscr);
        }
    }
     }
}
}
//Function that fires enemy weapons
void fire_enemies(Enemy *enemies, Player *player){
    
    for (int i =0 ; i < MAX_ENEMIES ; i++){
    float fire_angle = 270; // default downward angle, same as current behavior
if(enemies[i].weapon == NULL) continue;
if (enemies[i].aimed == TRUE) {
    float dx = player->px - enemies[i].px;
    float dy = player->py - (enemies[i].py - 1); // matches the py-1 fire origin you already use

    float ideal_rad = atan2f(-dy, dx); // negative dy since up = decreasing py, matches your missile code
    float ideal_deg = ideal_rad * (180.0f / 3.14159265358979323846f);

    int inaccuracy = 15; // degrees of max random offset, tune to taste
    float offset = (rand() % (inaccuracy * 2 + 1)) - inaccuracy;

    fire_angle = ideal_deg + offset;
}
        if (enemies[i].state == ALIVE){
            enemies[i].cooldown_frames++;
        WeaponType *weapon = enemies[i].weapon;
         if (weapon != NULL && enemies[i].cooldown_frames >= weapon->cooldown_frames) {
            fire_weapon(weapon, enemies[i].fire_px, enemies[i].fire_py + 1, fire_angle, FALSE);
            enemies[i].cooldown_frames = 0;
            if (weapon->type == LASER) enemies[i].cooldown_frames = -30;
        }
    }
}
}
