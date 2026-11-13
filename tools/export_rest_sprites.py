#!/usr/bin/env python3
"""Export the sprite descriptors drawn by the native rest-of-mission actors
(programs of tools/extract_soldier_programs.py ROOTS for the robed swordsman
0A8C46: 2A7372, 2A73B4, 2A7434, 2A74E0, 2A759C) from the original P-ROM and
C-ROM, with their logical palettes (0x1B, 0x20B, 0x3BA, allocated by
0A8CA0), and verify them pixel by pixel against MAME VRAM dumps of a family
trace (MSX_REST_FAMILY_ROOTS=a8c46): the sprite-list entries the original
wrote during the actor's last update before each dump (PC 00A078/00A080)
are rasterized with the exported pens and must give the same palette slot
and pen as the VRAM sprite. Output: filesystem/rest-native.json (layout of
filesystem/boss-native.json) and docs/rest-sprites-validation.json."""
import argparse,collections,hashlib,json,struct
from pathlib import Path
from export_marco import ROOT,reconstruct
from export_soldier_sprites import program,descriptor,resources
from mission_tiles import rgba16
from extract_boss_fight import parse
ROOTS=(0x2a7372,0x2a73b4,0x2a7434,0x2a74e0,0x2a759c,
    # Domed building 054904 (rise, ruin, turret, banners, hatch, launcher, ender, missile).
    0x24a334,0x24a33e,0x24a348,0x24a352,0x24a35c,0x24a366,0x24a370,0x24a450,0x24a530,0x24ac42,0x24abb0,
    0x24a610,0x24a6f2,0x24a742,0x24a7ce,0x24a910,0x24aa18,
    # Destructible shop 0B6566 (building stages, ruin, overlay).
    0x2a89b8,0x2a89c4,0x2a89d0,0x2a89dc,0x2a89e8,0x2a89f4,0x2a8a00,
    # Covered wagon 033C98 (drive, stop, open, wreck, canvas 034022, pieces 034094).
    0x239dc8,0x239df6,0x239e24,0x239e30,0x239ee4,0x239f26,0x239f32,
    # Arabian swordsmen 039590/0395B6 and the shared death pieces 0A93CE.
    0x23c8b0,0x23c8f8,0x23c97c,0x23c782,0x23c790,0x23c79e,0x23c7ac,0x23c7ba,0x23c7c8,0x23c7dc,0x23c7ea,
    0x23c7f8,0x23c806,0x23c814,0x23c822,0x23c836,0x23c9ba,0x23ca32,0x2a769c,0x269340,0x2a765a,0x2a78f0,0x2a796e,
    0x2a77f4,0x2a7872,0x2a79ec,0x2a7a6a,0x2a7ae8,0x2a7b2a,0x2a76f8,0x2a7776,
    # Creature 035668.
    0x23b3c6,0x23b5b8,0x23b4e4,0x23b41c,
    # Prisoner 099862 (tied, wriggles, salute, jump, run).
    0x29f6d0,0x29f9ac,0x29fa22,0x29fa40,0x29fa78,0x29f85e,0x29f874,0x29f88a,0x29f8a0,0x29f8b6,0x29f8cc,0x29f8e2,
    0x29f8f8,0x29f90e,0x29f924,0x29f93a,0x29f950,0x29f966,0x29f97c,0x29f992,0x29f99c,
    # Items 08C374/08C37C of the native kinds and the appearance 2861BC.
    0x28652c,0x28745c,0x287516,0x287814,0x2878ec,0x2879c4,0x287b40,0x2864b0,0x28635a,0x28639c,0x2861bc,
    # Armoured vehicle 087E36 (wait, stop, drive, destroyed, aim, fire, flinch, reverse).
    0x2841b4,0x2841de,0x28425e,0x2842de,0x284416,0x2847de,0x284828,0x28454e,0x2845f2,0x2846ba,0x2848aa,
    # Its children: shell 088B52, case 0896A0, smokes 088ED8/0897E2/08982A,
    # pieces 08943C and 088F20.
    0x284aba,0x284b0c,0x284b40,0x284a14,0x236272,0x24855c,0x239bca,0x247da8,
    0x28499c,0x2849a8,0x2849b4,0x2849c0,0x2849cc,0x2849d8,0x2849e4,0x2849f0,0x2849fc,0x284a08,
    # Every item kind with the motion 08B9B2 (36 is dropped by the hidden
    # boxes 09A67E).
    0x28631e,0x286336,0x28632a,0x286342,0x28634e,0x2864e0,0x28652c,0x28657c,0x286216,0x286258,0x28629a,0x2862dc,0x2875ae,0x286fc2,0x286dee,0x2866e6,0x287078,0x286eaa,0x2871c2,0x28741a,0x2876a4,0x28702c,0x28745c,0x287374,0x2867a8,0x287516,0x28756c,0x286c6c,0x286b54,0x287144,0x28724c,0x286840,0x2867ea,0x287814,0x2878ec,0x2879c4,0x287b40,0x287bb4,0x287a9c,0x28775a,0x287bc4,0x287c7e,0x28829a,0x288396,0x2887d2,0x28a720,0x24a200,0x2864a4,0x2864b0,0x2864bc,0x2864c8,0x2864d4,0x28635a,0x2863de,0x28639c,0x286420,0x286462,0x28aa54,0x286fbc,
    # Item sparkle 08C68E; player bullets 01B6DC..01B782 (flight, impact, wall
    # impacts) and grenade 01BF46/01BF10 with its explosion 01C152.
    0x28617e,0x22c1b6,0x22c3be,0x22c12e,0x22e504,0x22c884,0x22c81e,0x22c950,0x22cb60,
    # Swordsmen 09D846/09D862/09D876, their knife 09ECCC and the death splash 01C97A.
    0x2a1200,0x2a1166,0x2a1242,0x2a124c,0x2a12ca,0x2a12ee,0x2a1382,0x2a11d0,0x2a148a,0x2a13ca,0x2a13d4,
    0x2a1430,0x2a1446,0x2a154c,0x2a159e,0x2a15be,0x2a1778,0x2a178e,0x2a17c2,0x2a1892,0x2a1962,0x2a1a32,0x2a1b02,
    0x2a1b7c,0x2a1b54,0x2a1bce,0x2a1b6a,0x2a1be4,0x2a1bf6,0x2a16a6,0x2a1724,0x2a174e,0x2a1c84,0x2a1dbc,0x22e97e,
    # Prisoners 099DCC/099E06 and the rope 09A97C.
    0x29fad8,0x29fb34,0x29fcbe,0x29fde6,0x29fe52,0x29fbec,0x29fed0,0x29feec,0x2a02b6,0x2a0260,0x29ffe4,
    0x29fffa,0x2a0074,0x2a011c,0x2a0128,0x2a01d0,0x2a024a,0x29fe0a,
    # Item kinds with the motions 08BA10, 08BA38 and 08BAB2 (the foods).
    0x2865cc,0x2876e6,0x287d64,0x28851a,0x288af2,0x288c1e,0x288e1c,0x28988e,0x28a94c,0x28a582,0x288d18,0x28a672,0x28aa8c,
    # Rocket launcher 05ED84 (idle, stop, turn, deaths, fire, reload, flinch,
    # walk), its rocket 05FD8A, aim flash 060490 and muzzle 0602E4.
    0x263bc6,0x263bf4,0x263d7a,0x263dcc,0x2640e2,0x264072,0x264b00,0x264bca,0x264c94,0x264e46,
    0x264152,0x264176,0x26419a,0x2641be,0x2641e2,0x264206,0x26422a,0x26424e,
    0x264272,0x264374,0x26443e,0x264508,0x2645d2,0x26469c,0x264766,0x264830,
    0x263fc6,0x263e1e,0x26401c,0x263ef2,0x263c78,0x263d24,0x263c22,0x263cce,0x2648fa,
    0x60370,0x60394,0x603b8,0x603dc,0x60400,0x60424,0x60448,0x6046c,0x601a0,0x60242,
    # Helicopter formation 04857C: helicopter 245178/245188, rotor 24483E,
    # dropper 244F06, bomb 244DBC, impact 245106, shell 244F9E (and the lone
    # helicopter's 24473E/244712/244DD6/244E6E and turn tables).
    0x24473e,0x24483e,0x244dd6,0x244e6e,0x244f06,0x244dbc,0x244f9e,0x244712,0x245106,0x245178,0x245188,
    0x245004,0x245012,0x245020,0x24502e,0x2450a2,0x2450b0,0x2450be,0x2450cc,
    # Camel riders 09F682: camel, rider tables A0B82/A0BBA, head, shot,
    # piece, case and the dropped sword 09EE34.
    0x2a1dd8,0x2a1e56,0x2a1ea6,0x2a1ef6,0x2a1f34,0x2a1f72,0x2a21c4,0x2a222c,0x2a22a0,0x2a2334,0x2a2380,
    0x2a23d8,0x2a2178,0x2a219e,0x2a212c,0x2a2152,0x2a1ffe,0x2a2084,0x2a2430,0x2a2266,0x2a21f8,0x2a22ea,0x2a235a,
    0x2a23b6,0x2a240e,0x2a248e,0x2a24ec,0x2a251e,0x2a2550,0x2a2578,0x2a25a0,0x2a25ce,0x2a263e,0x2a2658,0x2a26fe,0x2a1cee,
    # Village decoration 0B6298 and the market stall 0400AA.
    0x2a89ac,0x2402ca,
    # Village porter 03264E and its walker.
    0x2389ce,0x2386f0,0x237f36,
    # Explosion fragments 01D31C.., debris 01CE08.. (table 01DF20) and the flash 01E058.
    0x22d414,0x22d47e,0x22d4e8,0x22d552,0x22d5bc,0x22d626,0x22d690,0x22d6e6,0x22d73c,0x22d792,0x22d7e8,
    0x22d83e,0x22d894,0x22d8ea,0x22d940,0x22d996,0x22d9ec,0x22da42,0x22da98,0x22daee,0x22db44,0x22dc2c,0x22dc82,
    0x22dcd8,0x22dd42,0x22ddac,0x22de16,0x22de80,0x22deea,0x22ea20,
    # Score label 08D330 digits (0E56EE/0E565A).
    0x285ef2,0x285efe,0x285f0a,0x285f16,0x285f22,0x285f2e,0x285f3a,0x285f46,0x285f52,0x285f5e,0x285f6a,0x285f74,0x285f7e,0x285f88,0x285f92,0x285f9c,0x286078,0x286082,0x28608c,0x286096,0x2860a0,0x2860aa,
    # Village civilians 031D80/031D92 (figures 031F52: stand, gesture, flight).
    0x237d02,0x237d0e,0x237d6e,0x237d7a,0x237cc0,
    # MISSION COMPLETE letters 06090A (60F44 letters, 60F7C mission numbers).
    0x264ff8,0x265002,0x26500c,0x265016,0x265020,0x26502a,0x265034,0x26503e,0x265048,0x265052,0x26505c,0x265066,0x265070,0x26507a,
    0x265084,0x26508e,0x265098,0x2650a2,0x2650ac,0x2650b6,
    # Player's vehicles: Camel Slug 03458E (state programs 034368.., rider
    # graphic 034F1A, effect 034FD2, eject 23A0C6, marker 03523C and its
    # children), vulcan turret 03B4E0 (23CE0C), end piece 03B936 (23D46C),
    # spark 03BA76, bullets 01B6A0/01BE24 and the dust 059E30.
    0x23a1da,0x23a306,0x23a350,0x23a376,0x23a3c0,0x23a3fc,0x23a508,0x23a276,
    0x23a5ae,0x23a6aa,0x23a6ec,0x23a712,0x23a754,0x23a778,0x23a7d0,0x23a62c,
    0x23a88c,0x23a988,0x23a9ca,0x23a9f0,0x23aa32,0x23aa56,0x23aaae,0x23a90a,
    0x23a84e,0x23ab2c,0x23a0c6,0x23b212,0x23b19c,0x23b126,0x23b288,0x23b324,0x23b3ba,
    0x23ce0c,0x23d4e0,0x23d46c,0x22c1b8,0x22c4c6,0x22c040,0x24c49c,
    # The boss turret gun's fire 08FA4E: flash 08EFFA (22D264), projectile 08F058.
    0x22d264,0x28c32c,
    # The player's C weapon (neo_chaser.c): shell 063F32 (267F92), explosion
    # 268622 and its blast 06371E (22CE6C), trail 063D9E (26863A), muzzle
    # flash 0C58D2 (0C586C, 0C5806).
    0x267f92,0x268622,0x22ce6c,0x26863a,0xc586c,0xc5806,
    # The R weapon's rocket 06381C and its explosion 063A92.
    0x2683f2,0x2683b6,0x26858a,0x2685ca,
    # Weapon 8's rocket 0641EC (its explosion is the R's).
    0x2684d2,0x2684aa,
    # Weapon 0D (neo_pweapon.c): ball 08606A, glow 08651C, explosions
    # 2830DA / 28311A (blasts 22CDD4 / 22CE6C / 22CF2C).
    0x282c18,0x282bf0,0x282c22,0x282c2c,0x282dcc,0x282ee6,0x2830da,0x28311a,0x22cdd4,0x22cf2c,
    # Weapons 02/07: the blast 0644C4/0646B0.
    0x2686dc,0x2687e4,0x2688ec,0x2689f4,0x268760,0x268868,0x268970,0x268a78,
    # Weapons 04/09: the flame and its puffs.
    0x2a655a,0x2a696e,0x2a6574,0x2a676c,0x2a6666,0x2a6988,0x2a6868,0x2a6a8e,
    # Weapons 06/0B: the beam 01E22C and its sparks (22F476).
    0x22ebb6,0x22ec92,0x22ed6e,0x22f016,0x22f0f2,0x22f1ce,0x22ee6a,0x22ef3a,0x22f00a,0x22f2ca,0x22f39a,0x22f46a,0x22f476,
    # Bomb type 2: the molotov 01E648 (22F5B8) and its flames 01E9B6 (242B52).
    0x22f5b8,0x242b52,
    # The SV-001 092A32 (neo_tank.c; extract_soldier_programs.py TANK_ROOTS).
    0x291ce4,0x291dc2,0x2942d4,0x294c6c,0x29667e,0x2952ec,0x296cfe,0x29589a,0x2972ac,0x295f1a,
    0x29792c,0x298090,0x2984ca,0x298904,0x298d3e,0x29a202,0x2934ec,0x2947a0,0x29282a,0x299180,
    0x299756,0x29c308,0x29c746,0x29b690,0x29b6f2,0x29b734,0x29bcd6,0x2964c8,0x292200,0x290fc0,
    0x2919fe,0x29090e,0x290c9e,0x291af0,
    0x29263a,0x2926b6,0x292732,0x2927ae,0x292678,0x2926f4,0x292770,0x2927ec,
    0x25d20a,0x25dba0,0x25e536,0x25eecc,0xad8ce,0x22cf2c,
    # The Slugnoid 09692C (neo_slugnoid.c; extract_soldier_programs.py): its tables
    # 096418 / 09649C[0], dust, flames, muzzle flash and the type-3 flame 22F704.
    0x29d00e,0x29d022,0x29d104,0x29d94a,0x29d2d8,0x29d35a,0x29d4f8,0x29d522,0x29d540,0x29d55e,0x29d57c,0x29d59a,
    0x29d5b8,0x29d3ac,0x29d3b0,0x29d3ce,0x29d3ec,0x29d40a,0x29d428,0x29d6aa,0x29d8aa,0x29d710,0x29d15e,0x29d1c8,
    0x29d1b0,0x29dd22,0x29deaa,0x29df14,0x29e10a,0x29df3e,0x29dd24,0x29de94,0x29ddc6,0x29ddd0,0x29ddda,0x29dde4,
    0x29ddee,0x29ddf8,0x29dd42,0x29dd4c,0x29dd56,0x29dd60,0x29dd6a,0x29dfce,0x29e076,0x29dff8,0x29df94,0x29dfc2,
    0x29dcde,0x29daea,0x29dc30,0x29d978,0x29dbd4,0x29dbde,0x22f704,
    # Prisoner tally 0308C0: the big prisoner 237AEC, the special prisoners
    # 237B84/237BD0 and the parade icon 237C48.
    0x237aec,0x237b84,0x237bd0,0x237c48,
    # The mission actor's CONTINUE? and countdown digit 018120 and GAME OVER
    # 0181E4 (fixed block, neo_continue.c).
    0x18120,0x181e4,
    # Parachute landing: the canopy 0C49D4 and the player marker 0C519E/0C5212.
    0xc49d4,0xc519e,0xc5212,
    # Parachute descent: the canopy descending/cut away (0C4904/0C498C).
    0xc4904,0xc498c,
    # Mission 2: the GO arrow 05287C (idle 24953C, arrows of table 05285C)
    # and the stage-1 exit marker 0B6F6A (2A8B70).
    0x24953c,0x249546,0x2495fc,0x2496b2,0x249768,0x24981e,0x2498d4,0x24998a,0x2a8b70,
    # Mission 2: the rebels 0613CA/0613D8/0613E6, their shot and flash (neo_rebel2.c).
    0x265c8a,0x265f36,0x265fe2,0x266666,0x26608e,0x2660c8,0x26611e,0x266190,0x265e62,0x266162,0x266302,0x2663f8,0x26684a,0x2668c8,0x266572,0x2665ec,0x266686,0x266780,0x266a38,
    # Mission 2: the soldiers 0678B6.. / 0692B2.., the knife 067FC0, the flash 021CB6 and the dust 0695DC (neo_rebel3.c).
    0x268c0a,0x268ca8,0x268d0c,0x268fee,0x269024,0x268dcc,0x268f24,0x2309d2,0x269530,0x2696a6,0x269702,0x26975e,0x269130,0x26985a,
    # Mission 2: the gate 03882A and the water ripple 03C62A (neo_gate.c).
    0x23c55a,0x23f868,0x23f8d0,0x23f960,0x23fa2c,0x23f746,0x23f774,0x23f7a2,0x23f7d0,0x23f7fe,0x23f892,0x23f90e,0x23f9b2,
    # Mission 2: the supply crate 08C154.. (neo_crate.c).
    0x286896,0x286904,0x2868a2,0x286910,
    # Mission 2: the stage-2 prop 06851A and its overlay 0685FE (neo_m2props.c).
    0x26939a,0x2693a6,0x2693b2,0x269382,0x26938e,
    # Mission 2: the drone 03670E and its bomb 0369AC (neo_drone.c).
    0x23ba74,0x23bb52,0x23bbd0,0x23bc72,0x23bcf2,
    # Mission 2: the hostage 021B46.. of 06835C (neo_hostage.c).
    0x230984,0x230654,0x23066a,
    # Mission 2: the stage-3 boss 026254 (neo_m2boss.c): body, parts (027D74), shots.
    0x23502c,0x234f10,0x234fcc,0x234f3c,0x23508c,0x235a8a,0x235872,0x23581c,
    0x2350e6,0x2350fa,0x23510e,0x23518c,0x23520a,0x235288,0x235306,0x235384,0x23543c,0x235454,0x235478,0x2353fe,
    0x235448,0x235466,0x235484,0x235490,0x2354d8,0x23558a,0x2356ba,0x2356b0,0x23574c,0x235742,0x235762,
    # Mission 2 stage-2 gate 03A6EA (23CDD0), overlay 03A82E (23CDDC), dust 03A464 (22D372).
    0x23cdd0,0x23cddc,0x22d372,
    # Mission 2 stage-3 boulder 03A2AA: boulder/stones 23CC7E/23CD24, slab 23CDE8.
    0x23cc7e,0x23cd24,0x23cde8,
    # Mission 2 stage-2 miners 021A1C/021A26 (neo_hostage.c): digs, pause,
    # startled, gift, flight, and the sparkle 02206E (22DF54).
    0x23058a,0x2305cc,0x230608,0x2308ce,0x2306ee,0x23072c,0x2306ac,0x22df54,
    # Mission 2 bats 035B66/035C68 (neo_drone.c).
    0x23b636,0x23b654,0x23b672,0x23b838,0x23b88e,0x23b912,0x23b76c,0x23b68c,0x23b7aa,0x23b6de,0x23b7ec,0x23b720,
    # Mission 2 stage 3 (neo_m2traps.c): treasure 0995EC, wrapped soldier 032DE2.
    0x29f57a,0x29f586,0x29f5d8,0x29f602,0x239492,0x23cac2,0x23cb9e,
    # The rope soldier 082BCE, its rope end 083212 and the hit splash 0682F0.
    0x2818e4,0x281a08,0x281a5c,0x281ac6,0x281c20,0x281d66,0x281e1e,0x281dea,
    # Mission 2 prisoners hung from a rope 099DF0 and the rope 09A9C4.
    0x29fb62,0x29fbe8,0x29fc42,0x29fc8a,
    # Mission 2 signs 0B829C/0B833A (neo_m2props.c).
    0x2a8da4,0x2a8db0,0x2a8dbc,0x2a8dc8,0x2a8dd4,0x2a8de0,0x2a8dec,0x2a8df8,0x2a8e04,0x2a8e10,0x2a8e1c,0x2a8e28,0x2a8e34,0x2a8bac,0x2a8be4,0x2a8c1c,0x2a8c54,0x2a8c8c,0x2a8cc4,0x2a8cfc,0x2a8d34,0x2a8d6c,
    # Mission 3 (neo_m3props.c): wagon box 039382 (23C762), wagons 038D1E/038D26, doors
    # 038B06, targets 038F38, pieces 0390A6, platform 0B9286 and 0B93B4.
    0x23c762,0x23c624,0x23c62c,0x23c57c,0x23c5aa,0x23c5d8,0x23c640,0x23c64c,0x22d4e8,0x22d552,0x22d5bc,0x22d626,0x2a8e50,0x2a8ea6,0x2a8e40,
    # Mission 3 soldiers' grenade 03C306 (neo_m3enemy.c; G16: no MAME trace, the descriptors from the ROM).
    0x23f2a0,
    # Mission 3 grenadier 08A732 and its helmet, flash, shell and smoke (neo_m3enemy.c).
    0x285864,0x2858a6,0x2858fe,0x285a0c,0x2859ba,0x285ab2,0x285bbe,
    # Mission 3 soldiers 05C53C/05D226/05D916 and their pieces (neo_m3enemy.c).
    0x2639a4,0x262ad8,0x262b1a,0x262bcc,0x262c18,0x262ec8,0x262f26,0x262b98,0x262fc4,0x262f06,0x263062,0x263082,0x2630e2,0x263170,0x2630a2,0x2630c2,0x262dcc,0x262e32,0x262e58,0x262c64,0x262cac,0x262d2c,0x262d86,0x262da8,0x262cee,0x262e26,0x26328e,0x26333e,0x263498,0x263ad0,0x2634e0,0x2638b0,0x26395c,0x263a2e,0x263a00,0x2631de,0x2631f4,0x26320a,0x263220,0x263236,0x26324c,0x263262,0x263278,0x2632a4,0x2632ba,0x2632d0,0x2632e6,0x2632fc,0x263312,0x263328,0x2633d2,0x2633e8,0x2633fe,0x263414,0x26342a,0x263440,0x263456,0x26346c,0x263482,0x263574,0x2635d0,0x26362c,0x263688,0x2636e4,0x263740,0x26379c,0x2637f8,0x263854,
    # Mission 3 flying machine 04E990, its pieces and missiles (neo_m3enemy.c).
    0x247730,0x2477c2,0x247854,0x2478b0,0x24790c,0x247986,0x2479c4,0x247a76,0x247afe,0x247b18,0x247baa,0x247c50,0x247c6a,0x247cd4,0x247d16,0x247da8,
    # Mission 3 gunship 097FCC and its pieces (neo_m3enemy.c).
    0x29ed1a,0x29ee20,0x29ef26,0x29ef7c,0x29f032,0x29f06c,0x29f090,0x29f0e6,0x29f13c,0x29f192,
    # Mission 3 molotov throwers 0569B4.. and the flying bomber 084588 (neo_m3enemy.c).
    0x24ac4c,0x24ac56,0x24ad2a,0x24ad9a,0x24adf2,0x24aed2,0x24b106,0x24b1f6,0x24b426,0x24b50a,0x24b548,0x24b58a,0x24b5cc,0x24b616,0x24b680,0x24b730,0x24b7ae,0x24b810,0x24b8b6,0x24b966,0x24b9b8,0x24ba1a,0x281ffc,0x28207a,0x2824f4,0x282518,0x282362,0x28236e,0x282388,
    # Mission 3 armed truck 02B202 and its pieces (neo_m3enemy.c).
    0x236d00,0x236d88,0x23739e,0x2373cc,0x2373e6,0x23787c,0x2378aa,
    # Mission 3 floating platform 08AFB6 and its floor (neo_m3enemy.c).
    0x285c6a,0x285c84,0x285cb2,0x285cbc,0x285d3a,0x285d68,0x285d96,0x285da8,0x285dba,0x285dcc,0x285eaa,0x285ebe,
    # Mission 3 oil drum 0391EA (neo_m3enemy.c).
    0x23c658,0x23c6b4,
    # Mission 3 birds 03ED00.., sandbags 0214EA/0216BA and camp 01F1D4 (neo_m3enemy.c).
    0x23fe3e,0x23fecc,0x23fefa,0x23fea0,0x23feac,0x23fe6c,0x23ff04,0x230238,0x23035e,0x2303e4,0x2303ca,0x2303f0,0x230438,0x23052c,0x230512,0x230538,0x22f81a,0x22f868,0x22f89c,0x22f9d8,0x22f8c2,0x22f98a,0x22f8f6,
    # Mission 3 armed jeep 0A2F2E, gunner, turret, rider, pieces and shell (neo_m3enemy.c).
    0x2a392a,0x2a39be,0x2a3aca,0x2a3b80,0x2a3cbc,0x2a3e92,0x2a40fa,0x2a426a,0x2a4056,0x2a440a,0x2a4350,0x2a4578,0x2a45fe,0x2a3958,0x2a3a14,0x2a3b20,0x2a3be6,0x2a3d88,0x2a3eda,0x2a3f42,0x2a409e,0x2a4170,0x2a42e0,0x2a438e,0x2a4484,0x2a4592,0x2a46c0,0x2a3990,0x2a3a74,0x2a3c56,0x2a3e5e,0x2a3f58,0x2a3fb4,0x2a41e6,0x2a43cc,0x2a44fe,0x2a45ac,0x2a473a,0x2a478e,0x2a47c4,0x2a4866,0x2a48fe,
    # Mission 3 rider car 027F54 and its pieces (neo_m3enemy.c).
    0x235baa,0x235a96,0x235ad8,0x235b1a,0x235c78,0x235d58,0x235be8,0x235c30,0x235e38,0x235f3e,0x235eaa,0x235fb6,0x235fe0,0x23606e,0x23611c,0x236170,0x236244,0x23638a,0x2363d8,0x236356,0x236314,0x23643a,0x2364d4,0x236758,0x23660c,0x2366b2,0x23657a,0x2361de,
    # Mission 3 window soldiers 08DA12/08DA34 (neo_m3enemy.c).
    0x23d62a,0x23d976,0x28ab8c,0x28abd4,0x28ac56,0x28acce,0x28ad18,
    # Mission 3 shield soldiers 09AE1E/09AE2C/09BA2A, shield, overlay, shots, flashes (neo_m3enemy.c).
    0x2a037a,0x2a03c6,0x2a044e,0x2a048c,0x2a04e2,0x2a0562,0x2a05c8,0x2a0654,0x2a069e,0x2a0714,0x2a076c,0x2a07c4,0x2a0852,0x2a08e0,0x2a0938,0x2a0990,0x2a09c4,0x2a09f8,0x2a0a46,0x2a0a94,0x2a0ac8,0x2a0afc,0x2a0b64,0x2a0cc8,0x2a0d20,0x2a0d80,0x2a0dc2,0x2a0e04,0x2a0e24,0x2a0e4e,0x2a0e78,0x2a0ffa,0x2a1000,0x2a0eda,0x2a0ee2,0x2a061e,0x2a06e4,0x2a0b34,0x2a0c24,0x2a0cfa,0x2a0746,0x2a079e,0x2a0eea,0x2a0ef2,0x2a0912,0x2a096a,0x2a0efa,0x2a0810,0x2a089e,0x2a0f26,0x2a109a,0x2a1034,0x2a0f40,0x2a0f7e,0x2a0fbc,
    # Mission 3 mid-boss 0A4A94 (neo_m3mid.c): side pieces, guns, parts, debris, engine, hatch,
    # bay soldiers and their programs 2A63DC, bomb, spark; the engine's puff 01C8D2 (22E8AE).
    0x2a4d16,0x2a4dc4,0x2a4d2e,0x2a4d22,0x2a4e66,0x2a4dd0,0x2a4eee,0x2a4f26,0x2a4f5e,0x2a4f8e,0x2a4fbe,0x2a50f6,
    0x2a550e,0x2a4e72,0x2a522e,0x2a52e2,0x2a544e,0x2a4ea6,0x2a529a,0x2a539a,0x2a55c2,0x2a566e,0x2a571c,0x2a57ca,
    0x22da42,0x22da98,0x22daee,0x22dc2c,0x22dc82,0x2a5626,0x2a5950,0x2a5a62,0x2a5a90,0x2a5b14,0x2a5b66,0x2a5be8,
    0x2a5c4e,0x2a5c8c,0x2a5d3e,0x2a5df0,0x2a5ea2,0x2a5f54,0x2a595c,0x22e8ae)
# The Mission 3 weapons with +93 = 2 (fat Marco): the R's rockets 063AEE 267C74 / 063B22 267E86 and explosion 26860A, the C 063F10 2681A4, the grenade 01BEDA 22CA58 (neo_chaser.c, neo_bullet.c).
ROOTS+=(0x267c74,0x267e86,0x26860a,0x2681a4,0x22ca58)
# The Mission 3 fat bomb 01C362 22CD04 and its piece 01C654 22CD5A, the fat landing dust 0C4E86 0C4E48, the fat molotov 01E680 22F65E and its flames 01EA7A 22F704 (neo_bullet.c).
ROOTS+=(0x22cd04,0x22cd5a,0x0c4e48,0x22f65e,0x22f704)
# Fat Marco's bullets (+93 = 2, table 0C42EA; neo_bullet.c): the pistol 01B62C 22C0A6, the H
# 01B668 22C000 / 01BDD2 22C4C6 and their impact 22C040 (MISSION3_DEBUG.md G21).
ROOTS+=(0x22c0a6,0x22c000,0x22c4c6,0x22c040)
# Its flames 0A7226 / 0A719C (level, vertical; neo_pweapon.c, palette 877).
ROOTS+=(0x2a6b8a,0x2a6c08,0x2a6c86,0x2a6d04)
# The splash 03C962 (neo_gate.c), palette 152 in Mission 3.
ROOTS+=(0x22e064,0x22e0f2,0x22df54,0x22e1f0)
# The Mission 3 weapon E 0DE4DA (neo_pweapon.c).
ROOTS+=(0x2b01a6,0x2b020c,0x2b01b0,0x2b0216,0x2b01de,0x2b0244)
# The Mission 3 vehicle 0AC866 (neo_m3veh.c) and the SV-001 093C9A's jump 299228.
ROOTS+=(0x0ab09e,0x0ab1aa,0x0ab29e,0x0ab0ac,0x0ab250,0x0aafa2,0x0aafb2,0x0ab028,0x0ab202,0x0ab458,0x0ab48c,0x0ab654,0x0ab576,0x0ab528,0x0ab44c,0x0ab4c0,0x0ab7d6,0x0ab80a,0x0ab9d2,0x0ab8f4,0x0ab8a6,0x0ab7ca,0x0ab83e,0x0abb54,0x0abb88,0x0abd50,0x0abc72,0x0abc24,0x0abb48,0x0abbbc,0x0abed2,0x0abf06,0x0ac0ce,0x0abff0,0x0abfa2,0x0abec6,0x0abf3a,0x0ac214,0x0ac5ca,0x0ad53a,0x0ac61c,0x0ac66e,0x0ad738,0x0ac398,0x0adc8a,0x0ac23e,0x0ac288,0x0ac2d2,0x23ae0a,0x23b0e8,0x022d372,0x022cf2c,0x022ce6c,0x0299228)
# Burning figures 08732C of the destroyed shop 0B6566 (B51): flight and landing.
ROOTS=ROOTS+(0x283b7a,0x283bf8)
# Mission 2 stage-3 creature 080170, its bite, death, pieces and egg 0808AA (neo_m2creature.c, B54).
ROOTS=ROOTS+(0x280142,0x280198,0x2801cc,0x2802c2,0x2803bc,0x2804a0,0x280436,0x28050a,0x280342,0x28034c)
# The Mission 3 final boss 04A2C2 (neo_m3boss.c, docs/JEFE_MISION3.md): programs 2451DC..247598.
ROOTS=ROOTS+(0x2451dc,0x2451e8,0x245294,0x245340,0x24569c,0x24571e,0x2457a0,0x24585a,0x2458c8,0x245936,0x2459d0,0x2459dc,0x2459e8,0x2459f4,0x245a48,0x245d86,0x245df2,0x245efc,0x2460d8,0x246262,0x246326,0x2463ea,0x2463ee,0x246480,0x246484,0x246ad2,0x246b4c,0x246eb2,0x247034,0x2470f6,0x24741e,0x247598)
# Its flamethrower (flames 245BA0, fireball 247656, trail 24767A, puddle 24855C) and missile
# pod (245D92, 245DE6, 245FEC, 2461A2, missile 24683A): pieces 2 and 3.
ROOTS=ROOTS+(0x245ba0,0x247656,0x24767a,0x24855c,0x245d92,0x245de6,0x245fec,0x2461a2,0x24683a)
# Its hatch shots (246694, 2465DA) and mortar bombs (2464BA): pieces 4 and 5.
ROOTS=ROOTS+(0x246694,0x2465da,0x2464ba)
# Its order-6 missile 04C220 (246794, palette 0x202; G15).
ROOTS=ROOTS+(0x246794,)
RESOURCES=(0x1b,0x20b,0x3ba,0x472,0x475,0x476,0x477,0x478,0x4b9,0x87f,0x883,0x46e,0x121,0x122,0x123,0x17,0x19,0x1a,0x3fd,0x93,0x9b,0x37d,0x3a6,0x380,0x37e,0x3a7,0x3ac,0xd0,0x103,0x24,0x30b,0xd3,0x208,0x113,0x13f,0x37c,0x124,0x37f,0x3c6,0x114,0x133,0x38e,0x823,0x3fd,0x202,0x27c,0x378,0x3fe,0xb8,0x37a,0x16c,0x16d,0x16e,0x381,0x382,0x3fa,0x117,0x9c,0x9d,0x1d,0x15a,0x20,0x21,0x22,0x23,0x178,0x179,0x202,0x15c,0x118,0x442,0x13b,0x92,0x114,
    # The Slugnoid 09692C: body 0x88D/0x2, dust 0x2D, muzzle flash 3, turrets
    # 0xD1/0x891 (+72 = 2, 3), death list 097E88, the type-3 flame 0x873.
    0x88d,0x2,0x2d,0x3,0xd1,0x891,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0x873,
    # Camel Slug, its rider (player 1/2), turret and bullets, marker, dust.
    0x3ff,0x376,0x377,0x3d6,0x3ad,0x884,0x170,
    # Mission 3 step 2 (neo_m3enemy.c): the rider car 027F54 (2F2/3CC, flash 305, rider 21A,
    # bomb 3CF, shell 208, engine 216/3D2/367, pieces 218/348/204), the jeep 0A2F2E (21B/21C,
    # flash 306, turret 21D, shell 223), the birds 03ED00.. (328, 2F8, 2F9), the sandbags
    # 0214EA (219, 3D5, 37A/3BA) and the camp 01F1D4 (20E, 208, 22E).
    0x2f2,0x3cc,0x305,0x21a,0x3cf,0x216,0x3d2,0x367,0x218,0x348,0x204,0x21b,0x21c,0x306,0x21d,0x223,0x328,0x2f8,0x2f9,0x219,0x3d5,0x20e,0x22e,
    # Palette of the fire's projectile 08F058.
    0x373,
    # Mission-end letters and other HEAD-only palettes.
    0x3be,0x3aa,0x200,
    # Explosion fragments, debris and flash reached in Mission 1.
    0x277,0x278,0x20a,0x29e,0x143,0x228,
    # Score label 08D330 (player 1; player 2 0x3CD/0x3CE in no capture), item kind 55.
    0x3b8,0x3b9,
    # The C weapon's blast 06371E.
    0x363,
    # The beam of weapons 06/0B and its sparks (98/99/9A; 875 with +93 = 2).
    0x98,0x99,0x9a,0x875,
    # Bomb type 2: the molotov 203, its flames D5, the hit in flight 363.
    0x203,0xd5,
    # Weapon 0D: ball 822, blast 828, glow 204.
    0x822,0x828,0x204,
    # Weapons 02/07: blast 39B (+93 = 2: 86C).
    0x39b,0x86c,
    # Weapons 04/09: flame 384 / 876.
    0x384,0x876,
    # The SV-001: body 0x3D3/0x39D/0x203 (exit list 093882: 0x396..0x39A, 0x39C..0x39E),
    # shell 0x258, dust and pieces 0x204, rider entry 0x4/0x5.
    0x3d3,0x39d,0x203,0x258,0x204,0x4,0x5,0x396,0x397,0x398,0x399,0x39a,0x39c,0x39e,
    # The Mission 3 SV-001 093C9A (neo_tank.c v3): body 0x87D/0x39D/0x87A (exit list 094A62 = 093882's).
    0x87d,0x87a,
    # The Mission 3 weapon E 0DE4DA: ball 0x81E (0x870 the other variant).
    0x81e,0x870,0x86e,
    # The Mission 3 fat bomb 01C362 (20A, its piece 204), dust 0C4E86 (2D), fat molotov 01E680 (872) and its flames (873).
    0x872,0x873,
    # The splash 03C962 (019240: 151 in Mission 2, 152 in Mission 3).
    0x151,0x152,
    # The Mission 3 vehicle 0AC866 (neo_m3veh.c): body 0xD/0xCE (0x1C2 two players), flames/bombs 0xE/0xF, dust 0x2D, trail 0x37C, marker 0x3B0, pilot 0x376/0x377, death 0xD..0x12.
    0xd,0xce,0xe,0xf,0x2d,0x37c,0x3b0,0x376,0x377,0x10,0x11,0x12,
    # Prisoner tally 0308C0: parade icon (0x381, 0x3FA, 0x9B listed above).
    0x2a2,
    # CONTINUE? letters (slot 04) and digit (slot FE) of 018120 (go2 dumps).
    0x63,0x25f,
    # Parachute canopy (0C4A58) and player 1/2 marker (0C5286).
    0x38d,0x2a3,
    # Mission 2: GO arrow 05287C, stage-1 exit marker 0B6F6A.
    0x125,0x461,
    # Mission 2 rebels 0613CA..: body 0x1F6/0x1F7, flash 0x811, burn 0x84A, flash child 0xB0.
    0x1f6,0x1f7,0x811,0x84a,0xb0,
    # Mission 2 soldiers 0678B6.. (+9C = 0): 0x3FC/0x3FD/0xF2, flash 0xAC, knife 0x3FB.
    0x3fc,0xf2,0xac,0x3fb,
    # Mission 2 stage-3 boss 026254: body 0x57/0x5B/0xB2 (flash 0x59), parts
    # 0x58/0x5C/0xB3 (flash 0x5A), 0x5D..0x61, column 0xB1, shots 0x223.
    0x57,0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0xb1,0xb2,0xb3,0x223,
    # Mission 2 gate 03882A (0x3CA/0x3D8, flash 0x3CB) and ripple 03C62A (0x14B).
    0x3ca,0x3d8,0x3cb,0x14b,
    # Mission 2: crate 08C154.., prop 06851A, drone 03670E, soldier 0678A2 (+9C = 4).
    0xcf,0x3ab,0xf5,0xf6,0x142,0x143,0x4bb,0x17e,0x17f,0x187,0x180,
    # Mission 2: the hostage 021B46.. (0x1F).
    0x1f,
    # Mission 2 stage-2 gate 03A6EA (0x445) and its overlay 03A82E (0x44B).
    0x445,0x44b,
    # Mission 2 stage-2 miners: the sparkle 02206E (0xAD; the miners 0x1F,
    # their dust 0x204).
    0xad,
    # Mission 2 signs 0B829C (0x474) and 0B833A (0x4B8, 0x4C3 with +70).
    0x474,0x4b8,0x4c3,
    # Mission 2 bats (0x94; 0xB7 the points bat, not in Mission 2's records).
    0x94,0xb7,
    # Mission 2 stage 3 (neo_m2traps.c): the treasure 0995EC (0x104) and its
    # explosion 01C7AE (0x26C), the wrapped soldier 032DE2 (0x105).
    0x104,0x26c,0x105,
    # The rope soldier 082BCE (0x3FD, flash 0xAC, 0xF2 burnt), its knife (0x3FB), the splash 0682F0 (0x16A).
    0x16a,
    # Mission 3: box 04BF, wagons/targets 04AB, doors 04AC (flash 04C0), pieces 020A, platform 04B7.
    0x4bf,0x4ab,0x4ac,0x4c0,0x20a,0x4b7,
    # Mission 3 grenadier 08A732 (0xA4, shell/flash 0xA5, smoke 0x37C).
    0xa4,0xa5,0x37c,
    # Mission 3 shield soldiers: 0x238/0x3C7 (flash 0x38C/0x171), item carrier 0x39F/0x3A3/0x20D, shot 0x22, impact 0x202.
    0x238,0x3c7,0x38c,0x171,0x39f,0x3a3,0x20d,0x22,0x202,
    # Mission 3 ripple 03C62A (019240 with 10C2F6 = 4).
    0x14c,0x152,
    # Mission 3 canopy 05DDA4, flash 05DF3E, bullets 05DF9A/05DFC2.
    0x38d,0x204,0x37b,
    # Mission 3 flying machine 04E990 (A6/AA/A7/B9/AB/A8/A9/78, pieces 20A/20B).
    0xa6,0xaa,0xa7,0xb9,0xab,0xa8,0xa9,0x78,0x20a,0x20b,
    # Mission 3 gunship 097FCC (23B/30C), bomb and missile 23C.
    0x23b,0x30c,0x23c,
    # Mission 3 throwers 9F/A2/22/178/179, bomber 244/850/220/BD/113/245.
    0x9f,0xa2,0x22,0x178,0x179,0x244,0x850,0x220,0xbd,0x113,0x245,
    # Mission 3 oil drum 4AD; truck 244/245/177.
    0x4ad,0x177,
    # Mission 3 floating platform 861/862/863.
    0x861,0x862,0x863,
    # Mission 2 stage-3 boulder 03A2AA: boulder/stones 0x4BA (03A5E6), slab 0x456 (03A528) (B49).
    0x4ba,0x456,
    # Burning figures 08732C of the destroyed shop 0B6566 (B51).
    0x852,
    # Mission 2 rebels 0678B6 with +9C = 8 (068F98: 0x894, 0x893, flash 0x895), beside the boulder (B55).
    0x893,0x894,0x895,
    # Mission 2 stage-3 creature 080170 (0x836) and its bite, death, pieces and egg (0x837) (B54).
    0x836,0x837,
    # Mission 3 mid-boss 0A4A94: pieces 0x1D, 0x26..0x32 (program callbacks 0A6F6A..0A703A),
    # bay soldiers 0x26D, bomb 0x258/0x880/0x881, the engine's puff 0x20D.
    0x1d,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x26d,0x880,0x881,0x20d)
# The Mission 3 final boss 04A2C2: the palettes of its pieces (docs/JEFE_MISION3.md).
RESOURCES=RESOURCES+(0xba,0x157,0xbc,0x130,0xf0,0x148,0xbb,0xbd,0x113)
# Mission 3: the bombers 0DC97E (0DC8FE/0DC902: 0x898/0x899), the turret 03B4E0 of
# the tank 093C9A (03B4C8: 0x87B), the decoration 0B91FA (0x4B4) (MISSION3_DEBUG.md).
RESOURCES=RESOURCES+(0x898,0x899,0x87b,0x4b4)
# Fat Marco's weapons (+93 = 2): the H bullet 01BDD2 (0x874), the flame 0A7226 (0x877), the
# weapon 0D 0A7960 (0x871) (capture_fat_weapons.lua palette dumps; 86C/870/875 listed above).
RESOURCES=RESOURCES+(0x874,0x877,0x871)
# Mission 4 props (neo_m4props.c): the floor piece 0B99EE (2A8EFC, 0xE0), the decoration 0B9AD6
# (2A8F3E, 0x532), the lamp 0B9B24 (2A8F4A, 0x54E), the collapse pieces 0B9D88 (2A8F6E.., 0x575)
# and the debris 0BB106 (22D414.., 0x574 / 0x20A).
ROOTS=ROOTS+(0x2a8efc,0x2a8f3e,0x2a8f4a,0x2a8f6e,0x2a8f7a,0x2a8f86,0x2a8f92,0x2a8f9e,0x2a8faa,0x22d414,0x22d47e)
RESOURCES=RESOURCES+(0xe0,0x532,0x54e,0x574,0x575)
# Mission 4 flyers 083C8A / 08391C (neo_m3enemy.c): body 244 (flash 850), rotors 220, rider 177 / 20E, explosion 204, trail 01C82A 345.
ROOTS=ROOTS+(0x2820f8,0x282126,0x282154,0x282182,0x2821ac,0x2823b2,0x2823e0,0x28240a)
RESOURCES=RESOURCES+(0x345,)
# 04061C: the soldiers' palette 0x385 (the charred death of the generic node, 0406B6 with +5A = 7: the flyer's rider 0843F8 on odd frames).
RESOURCES=RESOURCES+(0x385,)
# Mission 4 bins 033526 (13D / 13E), their puff 03378A (13F / 140) and pieces 033864 (211, E1..E3).
ROOTS=ROOTS+(0x2397d8,0x239928,0x239a7a,0x239bca,0x248682,0x2487c2,0x248902,0x248a42,0x248678,0x2487b8,0x2488f8,0x248a38)
RESOURCES=RESOURCES+(0x13d,0x13e,0x13f,0x140,0x211,0xe1,0xe2,0xe3)
# Mission 4 ripple 03C62A and splash 03C962 (019240 with 10C2F6 = 5: 14A / 150).
RESOURCES=RESOURCES+(0x14a,0x150)
# Mission 4 object 036C34 and its blast 036EA8 (26C).
ROOTS=ROOTS+(0x23bdd2,0x23bddc,0x23be08,0x23bdf2,0x23bebe)
# Mission 4 van 0A9F0C and its overlay 0AA37C (B5, flash 318).
ROOTS=ROOTS+(0x2a7e1a,0x2a7e10,0x2a7f28,0x2a80d8,0x2a8192,0x2a82e4,0x2a83e4,0x2a8546,0x2a8646,0x2a8712,0x2a896c,0x2a8994,0x2a7e2c,0x2a80cc,0x2a8186,0x2a82d8,0x2a83d0,0x2a8532,0x2a8632,0x2a86fe,0x2a8880,0x2a8988)
RESOURCES=RESOURCES+(0xb5,0x318)
# Mission 4 hatches 01F828 / 01F866 (76), rocks 01FABA (74), shadows 01FF82 (75), puffs 020004 (77).
ROOTS=ROOTS+(0x22fbea,0x22fc02,0x22fc4a,0x22fc92,0x22fbf6,0x22fc26,0x22fc6e,0x22fcb6,0x22fb34,0x22fbbc,0x22fcda)
RESOURCES=RESOURCES+(0x74,0x75,0x76,0x77)
# Mission 4 jumping soldier 07E554 and its piece 07EC6A (379), the dust 07EDF0 (22DF54, splash palette 150).
ROOTS=ROOTS+(0x27f832,0x27f884,0x27f8ae,0x27f932,0x27fa3a,0x22df54)
RESOURCES=RESOURCES+(0x379,0x150)
# Mission 4 helicopters 08514A / 085522 / 08571C: body 2825CE (D2 or 88F scaled, D6, D4), death 282834, flashes 28292C (FE) / 282998 (88E), bomb 282A2C (88E), impact 282AEA (D5).
ROOTS=ROOTS+(0x2825ce,0x282834,0x28292c,0x282998,0x282a2c,0x282aea)
RESOURCES=RESOURCES+(0xd2,0x88f,0xd6,0xd4,0xfe,0x88e,0xd5)
# Mission 4 truck pieces 02A448 (palettes 02A9BC: 23A, 32F, 372; 201) and the animals' sparkle 08CF6E (289704, 124).
ROOTS=ROOTS+(0x236a88,0x236af2,0x236b48,0x236b9e,0x236bb8,0x236aba,0x236b1a,0x236b70,0x236ba8,0x236bc2,0x289704)
RESOURCES=RESOURCES+(0x23a,0x32f,0x372,0x201,0x124)
# Pickup label 08C752: the characters 08C954 (tables 0E565A / 0E56EE; palettes 0E559E by player: 3B8 / 3B9, 3CD / 3CE).
ROOTS=ROOTS+(0x285ef2,0x285efe,0x285f0a,0x285f16,0x285f22,0x285f2e,0x285f3a,0x285f46,0x285f52,0x285f5e,0x285f6a,0x285f74,0x285f7e,0x285f88,0x285f92,0x285f9c,0x285fa6,0x285fb0,0x285fba,0x285fc4,0x285fce,0x285fd8,0x285fe2,0x285fec,0x285ff6,0x286000,0x28600a,0x286014,0x28601e,0x286028,0x286032,0x28603c,0x286046,0x286050,0x28605a,0x286064,0x28606e,0x286078,0x286082,0x28608c,0x286096,0x2860a0,0x2860aa,0x2860b4,0x2860be,0x2860c8,0x2860d2,0x2860dc,0x2860e6,0x2860f0,0x2860fa,0x286104,0x28610e,0x286118,0x286122,0x28612c,0x286136,0x286140,0x28614a,0x286154,0x28615e,0x286168,0x286172,0x28617c)
RESOURCES=RESOURCES+(0x3b8,0x3b9,0x3cd,0x3ce)
# Mission 4 fighting prisoner 0DDCDA (palettes 381 / 3FA of the prisoner) and its shot 0DE0FE (3FA); the animals (item kinds 40..42) and their fruits 3D / 3F (124).
ROOTS=ROOTS+(0x2afdaa,0x2afe14,0x2aff14,0x2aff94,0x289082,0x289cd6,0x28a12c,0x289aa8,0x28992a,0x289b44)
# Mission 4 vehicle 0587C4 and its turret 0592C6 (1C, flash 30B), the muzzle flashes and shots 0594B0 / 0594CE (1E), the trail 05996A (37C), the wreck's piece 059B5C (1D); the shot's blast 059852 is 22CE6C (204, above).
ROOTS=ROOTS+(0x24ba74,0x24bb34,0x24bca0,0x24bcba,0x24bdc8,0x24bde2,0x24c1c2,0x24c2f6,0x24bd62,0x24ba5e,0x24bad4,0x24bbea,0x24bd48,0x24bedc,0x24be5c,0x24c25a,0x24be76,0x24bfbc,0x24c0fc,0x24bf42,0x24c082,0x24c3aa,0x24c53e)
RESOURCES=RESOURCES+(0x1c,0x30b,0x1e,0x37c,0x1d)
# Mission 5 sewer gates 0BC296 (59C, flash 59D) and 0BC3A6 (59F, flash 59E).
ROOTS=ROOTS+(0x2a91d0,0x2a91ee)
RESOURCES=RESOURCES+(0x59c,0x59d,0x59e,0x59f)
# Mission 5 sewer drips 08346E: the drop 0834F4 (AC8, 281E98 and the splash 281EA2).
ROOTS=ROOTS+(0x281e98,0x281ea2)
RESOURCES=RESOURCES+(0xac8,)
# Mission 5 decorations 0BBE5E (5BB), 0BBED0 (5DC), 0BC4F4 (596 / 592) and 0BC4BA (591).
ROOTS=ROOTS+(0x2a8fb6,0x2a8fc0,0x2a918a,0x2a921e,0x2a921c)
RESOURCES=RESOURCES+(0x5bb,0x5dc,0x596,0x592,0x591)
# Mission 5 street: the wrecked cars 050542 (palettes 0500FC by +6E: 211, E1..E5; program tables 04FD26..04FE2D), their
# wheels 05090A / 05092C (37C) and blast 05094E (113).
ROOTS=ROOTS+(0x22e6ae,0x247da8,0x24855c,0x247e54,0x247e8a,0x247ec0,0x247ef6,0x247f2c,0x247f62,0x248448,0x248476,0x2484a4,0x2484d2,0x248500,0x24852e,
             0x247f98,0x248054,0x248110,0x2481cc,0x248288,0x248368,0x248008,0x2480c4,0x248180,0x24823c,0x2482f8,0x2483d8,0x248566,0x2486a6,0x2487e6,
             0x248926,0x248a66,0x248b9e,0x248596,0x2486d6,0x248816,0x248956,0x2485d8,0x248718,0x248858,0x248998,0x248636,0x248776,0x2488b6,0x2489f6,
             0x248b5c,0x248c94,0x248794,0x2488d4,0x248a14,0x248682,0x2487c2,0x248902,0x248a42,0x248678,0x2487b8,0x2488f8,0x248a38)
RESOURCES=RESOURCES+(0x211,0xe1,0xe2,0xe3,0xe4,0xe5,0x37c,0x113)
# Mission 5 metro: the signal 0BC1E6 (58F).
ROOTS=ROOTS+(0x2a91c6,0x2a9194)
RESOURCES=RESOURCES+(0x58f,)
# Mission 5 metro's end 0BBF12 (58E, flash 58D, open 587).
ROOTS=ROOTS+(0x2a8fca,0x2a906a,0x2a9080,0x2a909c,0x2a910c,0x2a90a6)
RESOURCES=RESOURCES+(0x58e,0x58d,0x587)
# Mission 5 sewer stains 032B46.. (17D, and 158 with BLOOD on: captures-m5/fam/B8 as a palette capture).
ROOTS=ROOTS+(0x2393de,0x2393ea,0x2393f6)
RESOURCES=RESOURCES+(0x17d,0x158)
# Mission 5 sewer jets 0836E8: the puff 083820 (869 / 851).
ROOTS=ROOTS+(0x281ee0,0x281f82)
RESOURCES=RESOURCES+(0x851,)   # 869 is the +77 = 0 variant (0836DE), which Mission 5 never makes: no capture has it
# Mission 5: the debris pieces 01CE50..01CE98 of the sewer gates' lists 0BBD78 / 0BBD8A (palettes 168 / 167, 01D1B0).
RESOURCES=RESOURCES+(0x167,0x168)
# Mission 5 street tank 08DCB2 / 08EB18 (22B / 347 by +89, flash 306), its flash 08EFFA (208) and shell 08F058 (373); the prizes
# of class 0x55 the wrecked cars 050542 drop (892).
ROOTS=ROOTS+(0x28aef0,0x28ad3a,0x28adf4,0x28b03c,0x28af3e,0x28b13a,0x28b250,0x28b366,0x28b5cc,0x28b6f6,0x28bcdc,0x28c10e,0x22d264,0x28c32c,
    0x28b7bc,0x28b80e,0x28b860,0x28b8b2,0x28b904,0x28b956,0x28b9a8,0x28b9fa,0x28ba4c,0x28ba9e,0x28baf0,0x28bb42,0x28bb94,0x28bbe6,0x28bc38,0x28bc8a)
RESOURCES=RESOURCES+(0x22b,0x892)   # 347 is the +89 = 1 variant, which no Mission 5 record makes: no capture has it
# Mission 5 lone helicopter 0474B6: its stop 0E0C32[+96 & 3] (the other programs and the palettes 20 / 21 / 22 / 15C are the formation's).
ROOTS=ROOTS+(0x244792,0x2447a0,0x2447ae,0x2447bc)
# Mission 5 tank on treads 08FF7E / 090A62 (22B, flash 306), its flash 091290 (208, 22D264) and shell 0912EE (373).
ROOTS=ROOTS+(0x28ca64,0x28c9c8,0x28ca16,0x28c440,0x28c6e0,0x28c56c,0x28c626,0x28c90e,0x28c79a,0x28c854,0x28ccae,0x28cab2,0x28cbb0,0x28cfa8,0x28cdac,0x28ceaa,
    0x28d2d2,0x28d0a6,0x28d1bc,0x28d614,0x28d3e8,0x28d4fe,0x28d7ca,0x28d72a,0x28d77a,0x28e146,0x28def2,0x28e01c,0x28e270,0x28c442,
    0x28e4b8,0x28e3ac,0x28e432,0x28e28a,0x28efc6,0x28e53e,0x28ea82,0x2905dc,0x2901a0,0x2903be,0x28fd6e,0x28f50a,0x28f93c,0x2907fa)
# Mission 5 metro train 051344 and its children (neo_m5train.c; palettes 10F, 110, 111, 112, 115, 116, 1D).
ROOTS=ROOTS+(0x248cd6,0x248cf0,0x248d16,0x248d3c,0x248d68,0x248d8e,0x248dd0,0x248e40,0x248e5a,0x248e74,0x248e8e,0x248ea8,0x248ec2,0x248edc,
    0x248f0a,0x248f60,0x248fbc,0x249018,0x24907e,0x2490d0,0x249118,0x24917e,0x2491da,0x24c3aa,0x24924c,0x2492f2,0x249398,0x249420,
    0x2494b2,0x2494f4,0x249518)
# Palettes of the Mission 5 metro train 051344 and machine 0D3B36 (their captures R and Q are gone: the palette RAM of
# run4 in the metro, --palette-capture of its frames 262380..271500).
RESOURCES=RESOURCES+(0x10f,0x110,0x111,0x112,0x115,0x116,0x3e5,0x30f,0x25b,0x25c,0x25d,0x25e,0x3e6,0x238,0x239,0x3cf,0x348)
# Mission 5 metro machine 0D3B36 and its children (neo_m5mech.c; palettes 3E5, 30F, 25B, 25C, 25D, 25E, 3E6, 238, 239, 208, 3CF, 348).
ROOTS=ROOTS+(0x2ab858,0x2ab888,0x2ab8b8,0x2ab8f2,0x2ab914,0x2aba16,0x2aba2e,0x2abb42,0x2abc3a,0x2abd44,0x2abe26,0x2aba3a,0x2abb4e,0x2abc46,
    0x2abd50,0x2aba78,0x2abb8c,0x2abc7a,0x2abd7a,0x2abe32,0x2abb04,0x2abbfc,0x2abd06,0x2abd8c,0x2abe44,0x2abdd4,0x2ac67a,0x2ac68c,
    0x2ac72a,0x2ac7c8,0x23643a,0x2ac1c2,0x2ac312,0x2ac462,0x2ac0bc,0x236272,0x2abe96,0x2abf38,0x2abfb2,0x2abffa,0x2ac074,0x2ac8d0,
    0x2ac92e,0x263ad0,0x2361de)
# Mission 5 sewer crank 095C98 (neo_m5sewer.c; palette 4F) and soldiers 089948 / 08995A / 08996A (palettes C4, C5, 878 and the
# lists E5372 (C4, C6..CC) / E5382 (17C, 886..88C) of 08A618).
ROOTS=ROOTS+(0x29c9d4,0x284c18,0x284cb2,0x284d36,0x284df0,0x284e7c,0x284c26,0x284f08,0x284f68,0x285088,0x285026,0x2854f2,0x285602,
    0x28563a,0x285722,0x28538a,0x285238,0x285418,0x2850da,0x285118,0x284dc0,0x284ec4)
RESOURCES=RESOURCES+(0x4f,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0x878,0x17c,0x886,0x887,0x888,0x889,0x88a,0x88b,0x88c)
# The sewer crank grabbed (capture C): 29CA7A, the wheel 095A94[+91] and the turning player 095AD4[+91] (096140, palette
# 034330[the player's +6E]).
ROOTS=ROOTS+(0x29ca7a,0x29cc3c,0x29cc82,0x29ccc8,0x29cd0e,0x29cd54,0x29cd9a,0x29cde0,0x29ce2c,0x29ce72,
    0x29ceb8,0x29cede,0x29cf04,0x29cf2a,0x29cf50,0x29cf76,0x29cf9c,0x29cfc2,0x29cfe8,0x29c98c)
RESOURCES=RESOURCES+(0x204,)
# Mission 6 stage 1 rafts 0BD822.. (floats 2A953E: 4CB / 4CC / 4CD; pictures 0BDA4E 2A954E: 509), decorations 0BD416..0BD4D4
# (2A946C 4D4, 2A947C 4D5, 2A948C 50B, 2A949C 4DF, 2A94B4 11D) and 0BDAB8 (2A95A4, no pose) (neo_m6props.c).
ROOTS=ROOTS+(0x2a953e,0x2a954e,0x2a946c,0x2a947c,0x2a948c,0x2a949c,0x2a94b4)
RESOURCES=RESOURCES+(0x4cb,0x4cc,0x4cd,0x509,0x4d4,0x4d5,0x50b,0x4df,0x11d)
# Mission 6 stage 1 hanging bridge spans 0BCFA4 / 0BCFC4 / 0BCFE4 / 0BD004 (2A923C / 2A92C8 / 2A9354 / 2A93E0, palette 4E1;
# neo_m6props.c).
ROOTS=ROOTS+(0x2a923c,0x2a92c8,0x2a9354,0x2a93e0)
RESOURCES=RESOURCES+(0x4e1,)
# The Mission 6 stage 1 mid-boss 05310E and its family (neo_m6props.c; captures-m6/fam/B).
ROOTS=ROOTS+(0x249d10,0x249d84,0x249dd0,0x249fb2,0x249a40,0x249b42,0x24a014,0x249e80,0x249e38,0x24a0be,0x249ec8,0x249f6a,0x249f8a,
    0x249f4c,0x249af0,0x249c1a,0x24a260,0x22cb60,0x244dbc,0x24a15c,0x24a1ae,0x24a20a,0x24a21e,0x24a214,0x22e1f0,0x22e064,0x24a306,0x269340)
RESOURCES=RESOURCES+(0x38e,0x38f,0x35d,0x364,0x365,0x3e0,0x22,0x178,0x179,0x13c,0x154,0x166,0x165,0x3fd)
# The Mission 6 stage 1 runners 020166 / 02011C (22FE22 / 22FD1C, palette 74) and their riders 020372 (22FEB0, 75)
# (neo_m6props.c; captures-m6/fam/C).
ROOTS=ROOTS+(0x22fe22,0x22fd1c,0x22feb0)
RESOURCES=RESOURCES+(0x74,0x75)
DRAWN_7A=(0x29fb62,)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path)
    p.add_argument('captures',nargs='+',help='capture directory[:node,...] or directory@lo-hi (family trace; nodes, or routine range, validated)')
    p.add_argument('--palette-capture',action='append',type=Path,default=[],help='capture directory read for palettes only (e.g. the BLOOD ON capture of tools/capture_blood.lua)')
    p.add_argument('--mask-auto',action='store_true',help='compare the frames with auto-animated entries (LSPC attribute bits 2/3), leaving out their area')
    p.add_argument('--report',type=Path,default=ROOT/'docs/rest-sprites-validation.json')
    p.add_argument('--output',type=Path,default=ROOT/'filesystem/rest-native.json',help='export (e.g. filesystem/mission2/rest-native.json, the Mission 2 private export)')
    p.add_argument('--keep',type=Path,action='append',default=None,help='previous export (repeatable; default filesystem/rest-native.json): its palettes stay when no given capture has them, and its descriptors when no program root of this tree has them (captures or roots of another branch)')
    a=p.parse_args();globals()["MASK_AUTO"]=a.mask_auto
    data=program(a.zip);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    progs=[q for q in json.loads((ROOT/'data/soldier_programs.json').read_text())['programs'] if q['root'] in ROOTS]
    assets=[]
    for prog in progs:
        for c in prog['commands']:
            if c['kind']=='POSE' and c['asset'] not in assets:assets.append(c['asset'])
            # 29FB62 (prisoner 099DF0 hanging) writes the rope's descriptor
            # to +7A, drawn by its rope pieces 09A9C4 through 011FF6.
            if prog['root'] in DRAWN_7A and c['kind']=='SET_LONG' and c['target']==0x7a and c['asset'] not in assets:assets.append(c['asset'])
    def columns(address):
        at=address-0x100000;groups,attr=data[at:at+2];at+=2;out=[]
        for g in range(groups):
            x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6+2*cols*rows
            out+=[x+16*c for c in range(cols)]
        return out
    descs={}
    for d in assets:
        try:descs[d]=descriptor(data,d,crom)
        except AssertionError:descs[d]=dict(descriptor=hex(d),x=0,y=0,width=0,height=0,pens=b'')
        descs[d]['columns']=columns(d)
    caps=[]
    for c in a.captures:
        if '@' in c:
            path,_,r=c.partition('@');lo,hi=(int(x,16) for x in r.split('-'));caps.append((Path(path),(lo,hi)))
        else:
            path,_,nodes=c.partition(':');caps.append((Path(path),[int(n,16) for n in nodes.split(',')] if nodes else [0x104644]))
    palettes={}
    for pal_file in sorted(f for cap in [c for c,_ in caps]+a.palette_capture for f in cap.glob('palette*.bin')):
        frame=int(pal_file.stem[7:]);ram_file=pal_file.parent/f'ram{frame}.bin'
        if not ram_file.exists():continue
        ram=ram_file.read_bytes();pw=struct.unpack('>4096H',pal_file.read_bytes())
        for slot,rid in resources(ram).items():
            if rid in RESOURCES:palettes.setdefault(rid,collections.Counter())[tuple(rgba16(pw[slot*16:slot*16+16]))]+=1
    pal_out={hex(k):list(c.most_common(1)[0][0]) for k,c in sorted(palettes.items())}
    kept_frames={}
    for keep in a.keep if a.keep is not None else [ROOT/'filesystem/rest-native.json']:
        if not keep.exists():continue
        prev=json.loads(keep.read_text())
        for k,v in prev.get('palettes',{}).items():pal_out.setdefault(k,v)
        for f in prev.get('frames',[]):kept_frames.setdefault(f['descriptor'],f)
        pal_out=dict(sorted(pal_out.items(),key=lambda kv:int(kv[0],16)))
    stats=collections.Counter();per=[]
    for capture,nodes in caps:validate(capture,nodes,descs,crom,stats,per)
    frames=[dict(descriptor=d['descriptor'],x=d['x'],y=d['y'],width=d['width'],height=d['height'],columns=d['columns'],pens_hex=d['pens'].hex()) for d in descs.values() if d['width']]
    have={f['descriptor'] for f in frames}
    frames+=[f for d,f in sorted(kept_frames.items(),key=lambda kv:int(kv[0],16)) if d not in have and d not in {hex(e) for e,g in descs.items() if not g['width']}]
    out=dict(source='Original P-ROM descriptors and C-ROM tiles of the rest-of-mission programs; palettes from MAME 0.264 mslugx euro dumps',
        p2_sha256=hashlib.sha256(data).hexdigest(),frames=frames,palettes=pal_out,empty=[hex(d) for d,g in descs.items() if not g['width']],
        missing_palettes=[hex(r) for r in RESOURCES if hex(r) not in pal_out])
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(out,separators=(',',':'))+'\n')
    report=dict(descriptors=len(frames),palettes=sorted(pal_out),missing_palettes=out['missing_palettes'],stats=stats,frames=per)
    a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps(dict(descriptors=len(frames),palettes=len(pal_out),missing=out['missing_palettes'],stats=stats)))
def zoomed_slots(vram):
    """Palette slots of the shrunk sprites of a VRAM dump (the rasterizer of
    export_marco.reconstruct draws only full-size sprites)."""
    out=set();zx=zy=rows=0
    for slot in range(1,381):
        control,zoom=vram[0x8200+slot],vram[0x8000+slot]
        if control&64:zx=(zoom>>8)&15
        else:rows,zy,zx=control&63,zoom&255,(zoom>>8)&15
        if not 0<rows<=32 or (zx==15 and zy==255):continue
        for row in range(rows):out.add(vram[slot*64+row*2+1]>>8)
    return out
MASK_AUTO=False
def validate(capture,nodes,descs,crom,stats,per):
    # Sprite-list entries written during each update of the nodes; at a dump
    # every node contributes its last (or, second try, its previous) update.
    by=collections.defaultdict(list)
    for u in parse(capture/'boss-trace.txt'):
        if isinstance(nodes,tuple):
            if not nodes[0]<=u['routine']<=nodes[1]:
                # A node of the family that left the routine range (a car turned into the blast 01C71E): its entries
                # are not rasterized, but the update counts, so that "the previous update" of the second try is
                # the same update for every node.
                if u['node'] in by:by[u['node']].append((u['frame'],[]))
                continue
        elif u['node'] not in nodes:continue
        if u['post'] is None or u['post'][:4]==bytes.fromhex('00003f1e'):
            # Deleting update: nothing drawn after it (it counts as an update with no entries, as above).
            if u['node'] in by:by[u['node']].append((u['frame'],[]))
            continue
        dl=[];cur=None
        for k,ad,m,d,pc in u['recs']:
            if k=='W' and pc==0xa078 and ad<0x10f000:
                if cur is None or len(cur['w'])>=6:cur=dict(w=[]);dl.append(cur)
                cur['w'].append(d)
        by[u['node']].append((u['frame'],[dict(desc=c['w'][4]<<16|c['w'][5],x=c['w'][0],y=c['w'][1],attr=c['w'][3]) for c in dl if len(c['w'])==6]))
    for vf in sorted(capture.glob('vram*.bin'),key=lambda f:int(f.stem[4:])):
        frame=int(vf.stem[4:])
        if not (capture/f'palette{frame}.bin').exists():continue
        if not any(x[0]<=frame and x[1] for ups in by.values() for x in ups):continue
        v=struct.unpack('>34304H',vf.read_bytes());pw=struct.unpack('>4096H',(capture/f'palette{frame}.bin').read_bytes())
        best=None;zoomed=zoomed_slots(v);skipped=False
        def entries(dl):
            # Zoomed sprites are not rasterized; auto-animated entries (attr bits 2/3: the LSPC counter picks the
            # tile, e.g. the GO arrow 05287C) neither.
            dl=[d for d in dl if d['attr']>>8 not in zoomed]
            return [d for d in dl if not d['attr']&12],[d for d in dl if d['attr']&12]
        def paint(dl,auto):
            native=[0]*(320*256)
            for d in dl:
                g=descs.get(d['desc'])
                if g is None:stats['missing_descriptor']+=1;continue
                if not g['width']:continue
                # Vertical flip (attribute bit 1, the 0D ball's blasts facing
                # down): y offset mirrored, one pixel the other way than x
                # (measured on capture d1: 71222 equal vs 48551 with 0).
                mirror=d['attr']&1;flip=d['attr']&2;w,h=g['width'],g['height'];pens=g['pens']
                left=d['x']+(-(g['x']+w)-1 if mirror else g['x']);top=((512-d['y'])&511)+(1-(g['y']+h) if flip else g['y'])
                shown=set()
                for cx in g['columns']:
                    sx0=d['x']-cx-16-1 if mirror else d['x']+cx
                    if -8<=sx0<=311:shown.update(range(sx0,sx0+16))
                for yy in range(h):
                    sy=top+yy
                    if not 0<=sy<256:continue
                    for xx in range(w):
                        pen=pens[(h-1-yy if flip else yy)*w+(w-1-xx if mirror else xx)];sx=left+xx
                        if pen and 0<=sx<320 and sx in shown:
                            # Two sprites of the family on one pixel: the
                            # hardware order decides; not compared.
                            if native[sy*320+sx]:native[sy*320+sx]=-1
                            else:native[sy*320+sx]=(d['attr']>>8)*16+pen
            # --mask-auto: the area of an auto-animated entry is not compared.
            for d in auto:
                g=descs.get(d['desc'])
                if g is None or not g['width']:continue
                left=d['x']+(-(g['x']+g['width'])-1 if d['attr']&1 else g['x']);top=((512-d['y'])&511)+g['y']
                for sy in range(max(top,0),min(top+g['height'],256)):
                    for sx in range(max(left,0),min(left+g['width'],320)):native[sy*320+sx]=-1
            return native
        def score(native,mame):
            same=sum(1 for i in range(320*256) if native[i]>0 and native[i]==mame[i])
            # A pixel where the dump shows a pen of another palette slot: a sprite outside the family (drawn with a
            # slot some family entry also uses, e.g. the fire 01C9E6 and a shell's blast 01C706) lies on top of
            # the family's; counted apart, not a difference of the descriptor.
            covered=sum(1 for i in range(320*256) if native[i]>0 and mame[i] and (mame[i]>>4)!=(native[i]>>4))
            diff=sum(1 for i in range(320*256) if native[i]>0 and native[i]!=mame[i])-covered
            return same,diff,sum(1 for i in range(320*256) if native[i]<0),covered
        # Per node, the entries of its last update and of the one before (a node deleted since then draws nothing).
        cand={}
        for node,ups in by.items():
            done=[x for x in ups if x[0]<=frame]
            if done and done[-1][0]>=frame-2:cand[node]=[done[-1]]+([done[-2]] if len(done)>=2 else [(done[-1][0],[])])
        if not cand:continue
        every=[d for c in cand.values() for x in c for d in x[1]]
        stats['zoomed_entries_skipped']+=sum(1 for d in every if d['attr']>>8 in zoomed)
        stats['auto_animation_entries_skipped']+=sum(1 for d in every if d['attr']&12)
        if any(d['attr']&12 for d in every) and not MASK_AUTO:continue
        slots={d['attr']>>8 for d in entries(every)[0]}
        try:im,_=reconstruct(v,pw,crom,slots)
        except AssertionError as err:
            # Another sprite of the screen with one of these palette
            # numbers is auto-animated (Mission 3: the train's wheels):
            # the dump is not validated (counted).
            if 'auto-animation' not in str(err):raise
            stats['dumps_skipped_auto_animation']+=1;continue
        mame=struct.unpack(f'<{320*256}H',im.info['neo_palette_indices'])
        # First and second try: every node's last update, or every node's previous one (the dump shows the last
        # sprite list the game finished). Third try: the walk of one game update split by the dump (the game
        # runs slower than the screen: an update takes several frames, and a node made or deleted in it has no
        # previous entries): per node, whichever of its two updates is the one on screen.
        for rank in (1,2,3):
            if rank<3:picks={n:c[rank-1] for n,c in cand.items()}
            else:
                if best is not None and not best[1]:break
                picks={}
                for n,c in cand.items():
                    s=[score(paint(*entries(x[1])),mame)[1] for x in c]
                    picks[n]=c[1] if s[1]<s[0] else c[0]
            dl=[d for x in picks.values() for d in x[1]];f=max(x[0] for x in picks.values())
            same,diff,overlap,covered=score(paint(*entries(dl)),mame)
            if best is None or diff<best[1]:best=(same,diff,rank,f,overlap,covered)
        if best is None:continue
        stats['frames']+=1;stats['pixels_equal']+=best[0];stats['pixels_different']+=best[1];stats['pixels_overlapping']+=best[4];stats['rank_%d'%best[2]]+=1
        stats['pixels_covered']+=best[5]
        per.append(dict(capture=capture.name,frame=frame,update_frame=best[3],equal=best[0],different=best[1]))
if __name__=='__main__':main()
