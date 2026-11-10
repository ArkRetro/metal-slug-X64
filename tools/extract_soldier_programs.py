#!/usr/bin/env python3
"""Convert the soldier, death and grenade animation programs to native commands.

Reads the CPU bank window saved by tools/mame/capture_soldier_campaign.lua
(program-bank.bin, the P2 bank mapped during Mission 1) and the fixed block.
Every reachable command is converted to a typed native command; an opcode or
callback outside the recovered set becomes UNRECOVERED (it fails explicitly in
C when reached) instead of being guessed."""
import argparse,hashlib,json,struct
from pathlib import Path
ROOTS=[0x23d510,0x23d594,0x23d5e2,0x23d692,0x23d6ec,0x23d712,0x23d7ee,0x23d8c0,0x23d9ea,0x23da62,0x23db34,0x23db7e,
    0x23ddb4,0x23ddd6,0x23de6a,0x23df8e,0x23dfd2,0x23dfe4,0x23e01e,0x23e0da,0x23e1da,0x23e2ea,0x23e394,0x23e43c,0x23e47c,
    0x23e4ee,0x23e5b8,0x23e6ae,0x23e71a,0x23e7ac,0x23e82e,0x23edf4,0x23ee3c,0x23eeb6,0x23eeea,0x23ef98,0x23f0fc,0x23f15a,
    0x23f1bc,0x23f200,0x23f270,0x23dbe2,0x23dc26,0x23dc6a,0x23dcfe,
    0x2402fa,0x24039c,0x2403ee,0x240454,0x2404fe,0x240588,0x2406ec,0x240a3e,0x240ae6,0x240c6e,0x240c76,0x240d2e,0x240d3c,
    0x240dc0,0x240dee,0x240e8e,0x240f8e,0x24103c,0x241056,0x2410e2,
    0x23f3f0,0x22d2a6]
# Region entities (camera 0..1920) translated as generic actors. Their 0x14
# callbacks become CALL commands handled by the family code (an unknown one
# fails explicitly there).
ROOTS+=[0x236818,0x236846,0x2367e4,0x2367fe,0x236874,0x2368a2,0x2368d0,
    0x22cdd4,0x22ce6c,0x22cf2c,0x22d264,0x22d27a,0x22d290,0x22e48a,0x22eac2,0x22e7f2,0x22e8ae,
    0x2af47a,0x2afb56,0x2af960,0x2afc60,0x2afcca,0x2919fe,
    0x2411f2,0x24116e,0x24123e,0x2412ee,0x24124c,0x241354]
# Rest of the mission: the robed swordsman 0A8C46 (boss arena, toggles RAM
# 10C606 that starts the boss).
ROOTS+=[0x2a7372,0x2a73b4,0x2a7434,0x2a74e0,0x2a759c]
# Domed building 054904 family (camera 1920): domes 054C2C, banners 054B1C,
# hatches 054F60, launchers 055056, 05518A and missiles 055638.
ROOTS+=[0x24a334,0x24a33e,0x24a348,0x24a352,0x24a35c,0x24a366,0x24a370,0x24a450,0x24a530,
    0x24ac42,0x24abb0,0x24a610,0x24a6f2,0x24a742,0x24a7ce,0x24a910,0x24aa18,0x2a89b8,0x2a89c4,0x2a89d0,0x2a89dc,0x2a89e8,0x2a89f4,0x2a8a00]
# Covered wagon 033C98 family: drive, stop, open (callback 239ECE), wreck,
# canvas 034022 and pieces 034094.
ROOTS+=[0x239dc8,0x239df6,0x239e24,0x239e30,0x239ee4,0x239f26,0x239f32]
# Arabian swordsmen 039590/0395B6 (stand, walk/attack tables 039FB0/039FC8,
# turn, slash 039B60, flight, sword 039C62) and the shared death pieces.
ROOTS+=[0x23c8b0,0x23c8f8,0x23c97c,0x23c782,0x23c790,0x23c79e,0x23c7ac,0x23c7ba,0x23c7c8,0x23c7dc,0x23c7ea,
    0x23c7f8,0x23c806,0x23c814,0x23c822,0x23c836,0x23c9ba,0x23ca32,0x2a769c,0x269340,0x2a765a,0x2a78f0,0x2a796e,
    0x2a77f4,0x2a7872,0x2a79ec,0x2a7a6a,0x2a7ae8,0x2a7b2a,0x2a76f8,0x2a7776,
    0x23b3c6,0x23b5b8,0x23b4e4,0x23b41c]   # creature 035668
# Prisoner 099862: tied (wriggle table 099D86 by +80), salute, item gift
# (callback 099CCC), release jump and run.
ROOTS+=[0x29f6d0,0x29f9ac,0x29fa22,0x29fa40,0x29fa78,0x29f85e,0x29f874,0x29f88a,0x29f8a0,0x29f8b6,0x29f8cc,0x29f8e2,
    0x29f8f8,0x29f90e,0x29f924,0x29f93a,0x29f950,0x29f966,0x29f97c,0x29f992,0x29f99c]
# Items 08C374/08C37C of the kinds with the motion 08B9B2 (records 0E581A:
# 06, 18, 1C, 29, 2A, 2B, 2F, 4B, 4F (H), 51) and the appearance 2861BC.
ROOTS+=[0x28652c,0x28745c,0x287516,0x287814,0x2878ec,0x2879c4,0x287b40,0x2864b0,0x28635a,0x28639c,0x2861bc,0x28617e]
# Player bullets 01B6DC..01BD00: flight (H 22C1B6/22C3BE, pistol 22C12E),
# impact 22E504, wall impacts 22C884/22C81E.
ROOTS+=[0x22c1b6,0x22c3be,0x22c12e,0x22e504,0x22c884,0x22c81e]
# Grenade 01BF10/01BF46 (22C950) and its explosion 01C152 (22CB60).
ROOTS+=[0x22c950,0x22cb60]
# Armoured vehicle 087E36: wait, stop, drive, destroyed, aim, fire, flinch,
# reverse (callbacks 089412 platform offset, 08938A pieces).
ROOTS+=[0x2841b4,0x2841de,0x28425e,0x2842de,0x284416,0x2847de,0x284828,0x28454e,0x2845f2,0x2846ba,0x2848aa]
# Its children: shell 088B52, case 0896A0, smokes 088ED8/0897E2/08982A,
# pieces 08943C and the ten pieces 088F20 (table 0E516E).
ROOTS+=[0x284aba,0x284b0c,0x284b40,0x284a14,0x236272,0x24855c,0x239bca,0x247da8]
ROOTS+=[0x28499c+0xc*i for i in range(10)]
# Every item kind with the motion 08B9B2 (records 0E581A; kind 36 is dropped
# by the hidden boxes 09A67E, 54 seen in the release runs).
ROOTS+=[0x28631e,0x286336,0x28632a,0x286342,0x28634e,0x2864e0,0x28652c,0x28657c,0x286216,0x286258,0x28629a,0x2862dc,0x2875ae,0x286fc2,0x286dee,0x2866e6,0x287078,0x286eaa,0x2871c2,0x28741a,0x2876a4,0x28702c,0x28745c,0x287374,0x2867a8,0x287516,0x28756c,0x286c6c,0x286b54,0x287144,0x28724c,0x286840,0x2867ea,0x287814,0x2878ec,0x2879c4,0x287b40,0x287bb4,0x287a9c,0x28775a,0x287bc4,0x287c7e,0x28829a,0x288396,0x2887d2,0x28a720,0x24a200,0x2864a4,0x2864b0,0x2864bc,0x2864c8,0x2864d4,0x28635a,0x2863de,0x28639c,0x286420,0x286462,0x28aa54,
    0x286fbc]   # kind 55 (motion 08BCCC: the prisoners' gift and the ruin window box)
# Swordsmen 09D846/09D862/09D876 (patrol, run, walk, turn, jump, slash,
# deaths 09E842/09E8C2/09E920/09EAB8, hop and jump in 09E382..09E722).
SWORD_ROOTS=[0x2a1200,0x2a1166,0x2a1242,0x2a124c,0x2a12ca,0x2a12ee,0x2a1382,0x2a11d0,0x2a148a,0x2a13ca,0x2a13d4,
    0x2a1430,0x2a1446,0x2a154c,0x2a159e,0x2a15be,0x2a1778,0x2a178e,0x2a17c2,0x2a1892,0x2a1962,0x2a1a32,0x2a1b02,
    0x2a1b7c,0x2a1b54,0x2a1bce,0x2a1b6a,0x2a1be4,0x2a1bf6,0x2a16a6,0x2a1724,0x2a174e,
    0x2a1c84,0x2a1dbc,0x22e97e]   # knife 09ECCC (flight, landing); death splash 01C97A
ROOTS+=SWORD_ROOTS
# Prisoners 099DCC/099E06 (tied, released, run, salute, fall, ladder climb
# 0402E6, rescue) and the rope 09A97C they leave.
PRISONER_ROOTS=[0x29fad8,0x29fb34,0x29fcbe,0x29fde6,0x29fe52,0x29fbec,0x29fed0,0x29feec,0x2a02b6,0x2a0260,0x29ffe4,
    0x29fffa,0x2a0074,0x2a011c,0x2a0128,0x2a01d0,0x2a024a,0x29fe0a]
ROOTS+=PRISONER_ROOTS
# Item kinds with the motions 08BA10 (1A), 08BA38 (24) and 08BAB2 (35, 38,
# 3A..3C, 3E, 43, 45..47, 58: the foods), records 0E581A +4.
ROOTS+=[0x2865cc,0x2876e6,0x287d64,0x28851a,0x288af2,0x288c1e,0x288e1c,0x28988e,0x28a94c,0x28a582,0x288d18,0x28a672,0x28aa8c]
# Rocket launcher 05ED84 (idle, stop, turn, deaths, fire by shot, reload by
# shot, flinch, walk), its rocket 05FD8A (2648FA), the aim flash 060490 and
# the muzzle 0602E4 (programs in the P1 ROM).
ROCKET_ROOTS=[0x263bc6,0x263bf4,0x263d7a,0x263dcc,0x2640e2,0x264072,0x264b00,0x264bca,0x264c94,0x264e46,
    0x264152,0x264176,0x26419a,0x2641be,0x2641e2,0x264206,0x26422a,0x26424e,
    0x264272,0x264374,0x26443e,0x264508,0x2645d2,0x26469c,0x264766,0x264830,
    0x263fc6,0x263e1e,0x26401c,0x263ef2,0x263c78,0x263d24,0x263c22,0x263cce,0x2648fa,
    0x60370,0x60394,0x603b8,0x603dc,0x60400,0x60424,0x60448,0x6046c,0x601a0,0x60242]
ROOTS+=ROCKET_ROOTS
# Helicopter formation 04857C: rotor 24483E, lone helicopter 24473E,
# helicopter 245178 (wait) / 245188 (flight by the bank +96) / 244712, the
# droppers 244DD6/244E6E/244F06 (callbacks 04A1AE/04A208/04A262), bomb
# 244DBC, impact 245106, shell 244F9E, and the lone helicopter's turn tables
# 244FF4/245092 (245004..24502E, 2450A2..2450CC).
HELI_ROOTS=[0x24473e,0x24483e,0x244dd6,0x244e6e,0x244f06,0x244dbc,0x244f9e,0x244712,0x245106,0x245178,0x245188,
    0x245004,0x245012,0x245020,0x24502e,0x2450a2,0x2450b0,0x2450be,0x2450cc]
ROOTS+=HELI_ROOTS
# Camel riders 09F682: camel walk/stand/swap/leave 2A1DD8/2A1E56/2A1EA6/
# 2A1EF6/2A1F34/2A1F72, rider tables A0B82/A0BBA (0A098C, 16 entries each:
# +8B = FF reads entry 15), head 2A24EC/2A251E/2A2550/2A2578/2A25A0/2A25CE,
# shot 2A263E, piece 2A2658, case 2A26FE, dropped sword 2A1CEE (09EE34).
CAMEL_ROOTS=[0x2a1dd8,0x2a1e56,0x2a1ea6,0x2a1ef6,0x2a1f34,0x2a1f72,0x2a21c4,0x2a222c,0x2a22a0,0x2a2334,0x2a2380,
    0x2a23d8,0x2a2178,0x2a219e,0x2a212c,0x2a2152,0x2a1ffe,0x2a2084,0x2a2430,0x2a2266,0x2a21f8,0x2a22ea,0x2a235a,
    0x2a23b6,0x2a240e,0x2a248e,0x2a24ec,0x2a251e,0x2a2550,0x2a2578,0x2a25a0,0x2a25ce,0x2a263e,0x2a2658,0x2a26fe,0x2a1cee]
ROOTS+=CAMEL_ROOTS
# Village decoration 0B6298 (camera 336) and the market stall 0400AA.
ROOTS+=[0x2a89ac,0x2402ca]
# Village porter 03264E: figure 2389CE/2386F0, walker 237F36.
ROOTS+=[0x2389ce,0x2386f0,0x237f36]
# Explosion fragments 01D31C.. (01D31C/01D37C/01D3D2 22DAEE/22DC2C, 01D428
# 22D5BC/22D626, 01D488 22D8EA/22D940, 01D4DA table 01E008), debris pieces
# 01CE08.. (table 01DF20) and the flash 01E058 (22EA20).
ROOTS+=[0x22d414,0x22d47e,0x22d4e8,0x22d552,0x22d5bc,0x22d626,0x22d690,0x22d6e6,0x22d73c,0x22d792,0x22d7e8,
    0x22d83e,0x22d894,0x22d8ea,0x22d940,0x22d996,0x22d9ec,0x22da42,0x22da98,0x22daee,0x22db44,0x22dc2c,0x22dc82,
    0x22dcd8,0x22dd42,0x22ddac,0x22de16,0x22de80,0x22deea,0x22ea20]
# Score label 08D330: digit programs of 0E56EE/0E565A.
ROOTS+=[0x285ef2,0x285efe,0x285f0a,0x285f16,0x285f22,0x285f2e,0x285f3a,0x285f46,0x285f52,0x285f5e,0x285f6a,0x285f74,0x285f7e,0x285f88,0x285f92,0x285f9c,0x286078,0x286082,0x28608c,0x286096,0x2860a0,0x2860aa]
# Village civilians 031D80/031D92 (031F52: stand 237D02/237D6E, gesture
# 237D0E/237D7A, flight 237CC0).
ROOTS+=[0x237d02,0x237d0e,0x237d6e,0x237d7a,0x237cc0]
# Mission end: MISSION COMPLETE letters 06090A (tables 060F44, 060F7C) and
# the prisoner tally 0308C0 (parade 237AEC/237B84/237BD0/237C48, tables 031276).
ROOTS+=[0x264ff8+0xa*i for i in range(14)]+[0x265084+0xa*i for i in range(6)]
ROOTS+=[0x237aec,0x237b84,0x237bd0,0x237c48,0x237c52,0x237c68,0x237c7e,0x237c94,0x237caa]
# The mission actor 101364's own program (fixed block): the big CONTINUE?
# and the countdown digit 018120 (00427A at 01883C) and GAME OVER 0181E4.
ROOTS+=[0x18120,0x181e4]
# Parachute landing (0C162A): the canopy collapsing behind the player 0C49D4
# (0C4B62) and the player marker over him 0C519E/0C5212 (0C5286).
ROOTS+=[0xc49d4,0xc519e,0xc5212]
# Parachute descent: the canopy 0C4A58 (0C4904 descending, 0C498C cut away).
ROOTS+=[0xc4904,0xc498c]
# The mummy transformation's smoke 0C4DD8 (2A97A0, palette 0xB0).
ROOTS+=[0x2a97a0]
# Callbacks with a native meaning (ROM routine -> native kind, asset).
PALETTE={0x4061c:0x385,0x40632:0x214,0x40648:0x35c,0x4065e:0x20e,0x40674:0x221,0x4068a:0x21a,0x406a0:0x20e}
GENERIC={0x40948,0x40956,0x4095e,0x23e418,0x23e420,0x23e42e,0xaef14}
# Player's vehicles: Camel Slug 03458E (state programs 034368.., the rider
# graphic 034F1A of players 1/2 (tables 034388/0343A8), its no-bomb effect
# 034FD2 (23A59E), the dismount 23A0C6, the camel's smoke/spray children
# 03523C.. and 03533A/03555A), the vulcan turret 03B4E0 (23CE0C), its spark
# 03BA76 and end piece 03B936, the bullets 01B6A0/01BE24 and the dust 059E30.
ROOTS+=[0x23a1da,0x23a306,0x23a350,0x23a376,0x23a3c0,0x23a3fc,0x23a508,0x23a276,
    0x23a5ae,0x23a6aa,0x23a6ec,0x23a712,0x23a754,0x23a778,0x23a7d0,0x23a62c,
    0x23a88c,0x23a988,0x23a9ca,0x23a9f0,0x23aa32,0x23aa56,0x23aaae,0x23a90a,
    0x23a84e,0x23ab2c,0x23a0c6,0x23b212,0x23b19c,0x23b126,0x23b288,0x23b324,0x23b3ba,
    0x23ce0c,0x23d4e0,0x23d46c,0x22c1b8,0x22c4c6,0x22c040,0x24c49c]
# The generic fire 08FA4E of the boss turret's gun 0D6168: its projectile
# 08F058 (the flash 08EFFA uses 22D264, already above).
ROOTS+=[0x28c32c]
# The C weapon (neo_chaser.c): shell 063F32 (267F92), explosion 064128
# (268622, CALL 0636B0; its blast uses 22CE6C, above), trail 063D9E (26863A)
# and the muzzle flash 0C58D2 (fixed block: 0C586C up/down, 0C5806 level).
ROOTS+=[0x267f92,0x268622,0x26863a,0x0c586c,0x0c5806]
# The R weapon (neo_chaser.c): rocket 06381C (level 2683F2, vertical 2683B6)
# and its explosion 063A92 (26858A / 2685CA).
ROOTS+=[0x2683f2,0x2683b6,0x26858a,0x2685ca]
# Weapon 8 (neo_chaser.c): its rocket 0641EC (level 2684D2, vertical
# 2684AA); the explosion is the R's.
ROOTS+=[0x2684d2,0x2684aa]
# Weapon 0D (neo_pweapon.c): ball 08606A (282C18, 282BF0, 282C22, 282C2C,
# burst 282DCC), glow 08651C (282EE6), explosions 2830DA / 28311A (CALL
# 0866A6; the blasts use 22CDD4 / 22CE6C / 22CF2C, above).
ROOTS+=[0x282c18,0x282bf0,0x282c22,0x282c2c,0x282dcc,0x282ee6,0x2830da,0x28311a]
# Weapons 02/07 (neo_pweapon.c): the blast 0644C4/0646B0, programs E13F2/E1402 by +72.
ROOTS+=[0x2686dc,0x2687e4,0x2688ec,0x2689f4,0x268760,0x268868,0x268970,0x268a78]
# Weapons 04/09 (neo_pweapon.c): the flame 0A71F8/0A716E and its puffs 0A74E4/0A751C.
ROOTS+=[0x2a655a,0x2a696e,0x2a6574,0x2a676c,0x2a6666,0x2a6988,0x2a6868,0x2a6a8e]
# The flames with +93 = 2 (fat Marco) 0A7226 / 0A719C (level, vertical; G21).
ROOTS+=[0x2a6b8a,0x2a6c08,0x2a6c86,0x2a6d04]
# The SV-001 092A32 (neo_tank.c): its state programs, the shell 094EAA
# (290FC0, burst 2919FE), the muzzle flash 0952F8 (29090E / 290C9E), the dust
# 0956CE (291AF0) and the rider's entry 0954DC (tables 0954A4 / 0954B4 /
# 0954C4 by the character +93 and the rider's +6E).
TANK_ROOTS=[0x291ce4,0x291dc2,0x2942d4,0x294c6c,0x29667e,0x2952ec,0x296cfe,0x29589a,0x2972ac,0x295f1a,
    0x29792c,0x298090,0x2984ca,0x298904,0x298d3e,0x29a202,0x2934ec,0x2947a0,0x29282a,0x299180,
    0x299756,0x29c308,0x29c746,0x29b690,0x29b6f2,0x29b734,0x29bcd6,0x2964c8,0x292200,0x290fc0,
    0x2919fe,0x29090e,0x290c9e,0x291af0,
    0x29263a,0x2926b6,0x292732,0x2927ae,0x292678,0x2926f4,0x292770,0x2927ec,
    0x25d20a,0x25dba0,0x25e536,0x25eecc,
    # 093AFA: the pieces 0AD9B8 (fixed block 0AD8CE); the burst uses 22CF2C (above).
    0x0ad8ce]
ROOTS+=TANK_ROOTS
# Mission 2: the GO arrow 05287C (idle 24953C, arrows of table 05285C by
# RAM 10C44A) and the stage-1 exit marker 0B6F6A (2A8B70).
ROOTS+=[0x24953c,0x249546,0x2495fc,0x2496b2,0x249768,0x24981e,0x2498d4,0x24998a,0x2a8b70]
# Mission 2: the rebels 0613CA/0613D8/0613E6 (neo_rebel2.c) and their flash 0620AA.
ROOTS+=[0x265c8a,0x265f36,0x265fe2,0x266666,0x26608e,0x2660c8,0x26611e,0x266190,0x265e62,0x266162,0x266302,0x2663f8,0x26684a,0x2668c8,0x266572,0x2665ec,0x266686,0x266780,0x266a38]
# Mission 2: the 0678B6 / 0692B2 families (neo_rebel3.c), their children 067FC0 / 021CB6 and 0695DC.
ROOTS+=[0x268c0a,0x268ca8,0x268cc4,0x268d0c,0x268fee,0x269024,0x268dcc,0x268f24,0x2309d2,0x269530,0x2696a6,0x269702,0x26975e,0x269130,0x26985a]
# Mission 2: the gate 03882A and the water ripple 03C62A (neo_gate.c).
ROOTS+=[0x23c55a,0x23f868,0x23f8d0,0x23f960,0x23fa2c,0x23f746,0x23f774,0x23f7a2,0x23f7d0,0x23f7fe,0x23f892,0x23f90e,0x23f9b2]
# Mission 2: the supply crate 08C154.. (neo_crate.c).
ROOTS+=[0x286896,0x286904,0x2868a2,0x286910]
# Mission 2: the stage-2 prop 06851A and its overlay 0685FE (neo_m2props.c).
ROOTS+=[0x26939a,0x2693a6,0x2693b2,0x269382,0x26938e]
# Mission 2: the drone 03670E and its bomb 0369AC (neo_drone.c).
ROOTS+=[0x23ba74,0x23bb52,0x23bbd0,0x23bc72,0x23bcf2]
# Mission 2: the hostage 021B46.. of 06835C (neo_hostage.c).
ROOTS+=[0x230984,0x230654,0x23066a]
# The miners 021A1C/021A26 (neo_hostage.c): digs, pause, startled, the
# item gift (callbacks 022162/022196/0221C2), flight, and the sparkle
# 02206E (the dust 021F96 is the bullets' impact 22E504).
ROOTS+=[0x23058a,0x2305cc,0x230608,0x2308ce,0x2306ee,0x23072c,0x2306ac,0x22df54]
# The bats 035B66/035C68 (neo_drone.c): wings, perch, stretches, death, fall, crawl.
ROOTS+=[0x23b636,0x23b654,0x23b672,0x23b838,0x23b88e,0x23b912,0x23b76c,0x23b68c,0x23b7aa,0x23b6de,0x23b7ec,0x23b720]
# The prisoners hung from a rope 099DF0 (hanging, falling) and the rope
# pieces 09A9C4 falling (neo_prisoner.c).
ROOTS+=[0x29fb62,0x29fbe8,0x29fc42,0x29fc8a]
# The stage-2/3 signs 0B829C (tables B84AC) and 0B833A (B84BC, wiggle B84E0) (neo_m2props.c).
ROOTS+=[0x2a8da4,0x2a8db0,0x2a8dbc,0x2a8dc8,0x2a8dd4,0x2a8de0,0x2a8dec,0x2a8df8,0x2a8e04,0x2a8e10,0x2a8e1c,0x2a8e28,0x2a8e34,0x2a8bac,0x2a8be4,0x2a8c1c,0x2a8c54,0x2a8c8c,0x2a8cc4,0x2a8cfc,0x2a8d34,0x2a8d6c]
# Mission 2 stage-3 boss 026254 (neo_m2boss.c): body 23502C/234F10/234FCC/
# 234F3C/23508C/235A8A, parts by 027D74 (+87), shots 235872/23581C.
ROOTS+=[0x23502c,0x234f10,0x234fcc,0x234f3c,0x23508c,0x235a8a,0x235872,0x23581c,
    0x2350e6,0x2350fa,0x23510e,0x23518c,0x23520a,0x235288,0x235306,0x235384,0x23543c,0x235454,0x235478,0x2353fe,
    0x235448,0x235466,0x235484,0x235490,0x2354d8,0x23558a,0x2356ba,0x2356b0,0x23574c,0x235742,0x235762]
# Mission 2: the stage-2 gate 03A6EA (23CDD0), its overlay 03A82E (23CDDC)
# and the dust 03A464 (22D372) of 03A776 (neo_m2gate.c).
ROOTS+=[0x23cdd0,0x23cddc,0x22d372]
# Weapons 06 / 0B (neo_pweapon.c): the beam 01E22C (muzzle, segments and
# end cap of the firing phase 22EBB6/22EC92/22ED6E and 22F016/22F0F2/22F1CE,
# of the fading phase 22EE6A/22EF3A/22F00A and 22F2CA/22F39A/22F46A) and its
# sparks 01E582.. (22F476).
ROOTS+=[0x22ebb6,0x22ec92,0x22ed6e,0x22f016,0x22f0f2,0x22f1ce,0x22ee6a,0x22ef3a,0x22f00a,0x22f2ca,0x22f39a,0x22f46a,0x22f476]
# Mission 2 stage 3 (neo_m2traps.c): the hidden treasure 0995EC (idle, hit,
# open, taken) and the soldier wrapped as a mummy 032DE2.
ROOTS+=[0x29f57a,0x29f586,0x29f5d8,0x29f602,0x239492]
# The gas vent 03A05A: its puffs 03A1AE (23CABA[+72]).
ROOTS+=[0x23cac2,0x23cb9e]
# The rope soldier 082BCE (descent, hanging, turn, knife, death, fall, body,
# rope end 083212); the hit splash 0682F0 (22DB44) is already a root.
ROOTS+=[0x2818e4,0x281a08,0x281a5c,0x281ac6,0x281c20,0x281d66,0x281e1e,0x281dea]
# The molotov 01E648 (22F5B8) and its flames 01E9B6 (242B52), neo_bullet.c.
ROOTS+=[0x22f5b8,0x242b52]
# The Slugnoid 09692C (neo_slugnoid.c): program tables 096418 (and its
# crouch sub-tables), the pilot's table 09649C[0] (Marco: the port's only
# character; the others stop explicitly in ga_anim_start), the dust,
# legs' flames and muzzle flash, and the type-3 flame 01EA7A (22F704).
ROOTS+=[
    0x29d00e,0x29d022,0x29d104,0x29d94a,0x29d2d8,0x29d35a,0x29d4f8,0x29d522,0x29d540,0x29d55e,0x29d57c,0x29d59a,
    0x29d5b8,0x29d3ac,0x29d3b0,0x29d3ce,0x29d3ec,0x29d40a,0x29d428,0x29d6aa,0x29d8aa,0x29d710,0x29d15e,0x29d1c8,
    0x29d1b0,0x29dd22,0x29deaa,0x29df14,0x29e10a,0x29df3e,0x29dd24,0x29de94,0x29ddc6,0x29ddd0,0x29ddda,0x29dde4,
    0x29ddee,0x29ddf8,0x29dd42,0x29dd4c,0x29dd56,0x29dd60,0x29dd6a,0x29dfce,0x29e076,0x29dff8,0x29df94,0x29dfc2,
    0x29dcde,0x29daea,0x29dc30,0x29d978,0x29dbd4,0x29dbde,0x22f704,]
# The stage-3 boulder 03A2AA (neo_m2props.c): boulder and stones 03A5E6
# (23CC7E), their fall 23CD24 and the slab 03A528 (23CDE8).
ROOTS+=[0x23cc7e,0x23cd24,0x23cde8]
# Mission 3: the wagon box 039382/03938A (neo_m3props.c).
ROOTS+=[0x23c762]
# Mission 3 wagons 038D1E/038D26 (23C624/23C62C), doors 038B06 (23C574[+70],
# broken 23C5D8), targets 038F38 (23C638[rand]), pieces 0390A6 (03957A[rand])
# and the platform 0B9286/0B93B4 (2A8E50/2A8EA6), neo_m3props.c.
ROOTS+=[0x23c624,0x23c62c,0x23c57c,0x23c5aa,0x23c5d8,0x23c640,0x23c64c,0x22d4e8,0x22d552,0x22d5bc,0x22d626,0x2a8e50,0x2a8ea6]
# Mission 3 decoration 0B91FA (camera-list call 0B91D0, MISSION3_DEBUG.md G9).
ROOTS+=[0x2a8e40]
# Mission 3 grenadier 08A732 (285864/2858A6/2858FE), helmet 08AA6E (285A0C),
# flash 08AB70 (2859BA), shell 08AC54 (285AB2) and smoke 08AE70 (285BBE), neo_m3enemy.c.
ROOTS+=[0x285864,0x2858a6,0x2858fe,0x285a0c,0x2859ba,0x285ab2,0x285bbe]
# Mission 3 shield soldiers 09AE1E/09AE2C/09BA2A, their shield 09C46C (poses
# 09D804), raised-shield overlay 09C6B0, shots 09C744.. and flashes 09CAE8.. (neo_m3enemy.c).
ROOTS+=[0x2a037a,0x2a03c6,0x2a044e,0x2a048c,0x2a04e2,0x2a0562,0x2a05c8,0x2a0654,0x2a069e,0x2a0714,0x2a076c,0x2a07c4,0x2a0852,0x2a08e0,0x2a0938,0x2a0990,0x2a09c4,0x2a09f8,0x2a0a46,0x2a0a94,0x2a0ac8,0x2a0afc,0x2a0b64,0x2a0cc8,0x2a0d20,0x2a0d80,0x2a0dc2,0x2a0e04,0x2a0e24,0x2a0e4e,0x2a0e78,0x2a0ffa,0x2a1000,0x2a0eda,0x2a0ee2,0x2a061e,0x2a06e4,0x2a0b34,0x2a0c24,0x2a0cfa,0x2a0746,0x2a079e,0x2a0eea,0x2a0ef2,0x2a0912,0x2a096a,0x2a0efa,0x2a0810,0x2a089e,0x2a0f26,0x2a109a,0x2a1034,0x2a0f40,0x2a0f7e,0x2a0fbc]
# Mission 3 window soldiers 08DA12/08DA34 (neo_m3enemy.c).
ROOTS+=[0x23d62a,0x23d976,0x28ab8c,0x28abd4,0x28ac56,0x28acce,0x28ad18]
# Mission 3 soldiers 05C53C/05D226/05D916, canopy 05DDA4, helmet 05DD2A, flash 05DF3E,
# bullets 05DF9A/05DFC2 and the program tables 05E9B8/05E9F8/05EA38/05EA5C (neo_m3enemy.c).
ROOTS+=[0x2639a4,0x262ad8,0x262b1a,0x262bcc,0x262c18,0x262ec8,0x262f26,0x262b98,0x262fc4,0x262f06,0x263062,0x263082,0x2630e2,0x263170,0x2630a2,0x2630c2,0x262dcc,0x262e32,0x262e58,0x262c64,0x262cac,0x262d2c,0x262d86,0x262da8,0x262cee,0x262e26,0x26328e,0x26333e,0x263498,0x263ad0,0x2634e0,0x2638b0,0x26395c,0x263a2e,0x263a00,0x2631de,0x2631f4,0x26320a,0x263220,0x263236,0x26324c,0x263262,0x263278,0x2632a4,0x2632ba,0x2632d0,0x2632e6,0x2632fc,0x263312,0x263328,0x2633d2,0x2633e8,0x2633fe,0x263414,0x26342a,0x263440,0x263456,0x26346c,0x263482,0x263574,0x2635d0,0x26362c,0x263688,0x2636e4,0x263740,0x26379c,0x2637f8,0x263854]
# Mission 3 flying machine 04E990 and its pieces, missiles 04F44E/04F464, bomb 04F358 (neo_m3enemy.c).
ROOTS+=[0x247730,0x2477c2,0x247854,0x2478b0,0x24790c,0x247986,0x2479c4,0x247a76,0x247afe,0x247b18,0x247baa,0x247c50,0x247c6a,0x247cd4,0x247d16]
# Mission 3 mid-boss 0A4A94 (neo_m3mid.c): side pieces 0A606E/0A6078
# (tables E61BC/E61C8), guns 0A61E8/0A61F2 (2A60D2/2A60DA/2A60E2), parts
# 0A64E2 (2A601A), debris 0A6598 (2A653C), burning engine 0A5D6A, hatch
# 0A5E88, bay soldiers 0A4F3A/0A4F52 (and their table 2A63DC), bomb 0A5398
# and the spark 0A68A0.
M3MID_ROOTS=[0x2a4d16,0x2a4dc4,0x2a4d2e,0x2a4d22,0x2a4e66,0x2a4dd0,0x2a4eee,0x2a4f26,0x2a4f5e,0x2a4f8e,0x2a4fbe,0x2a50f6,
    0x2a550e,0x2a4e72,0x2a522e,0x2a52e2,0x2a544e,0x2a4ea6,0x2a529a,0x2a539a,0x2a55c2,0x2a566e,0x2a571c,0x2a57ca,
    0x22da42,0x22da98,0x22daee,0x22dc2c,0x22dc82,0x2a5626,0x2a5950,0x2a5a62,0x2a5a90,0x2a5b14,0x2a5b66,0x2a5be8,
    0x2a5c4e,0x2a5c8c,0x2a5d3e,0x2a5df0,0x2a5ea2,0x2a5f54,0x2a595c]
ROOTS+=M3MID_ROOTS
# Mission 3 boss 04A2C2 (neo_m3boss.c): body, legs, torso, arm, missile pod,
# hatch, hatch floor and the dust and step pieces (docs/JEFE_MISION3.md).
ROOTS+=[0x2451dc,0x2451e8,0x245294,0x245340,0x24585a,0x24569c,0x2459d0,0x2458c8,0x24571e,0x2459dc,0x245936,0x2457a0,
    0x2459f4,0x2459e8,0x245a48,0x245d86,0x2460d8,0x245efc,0x245df2,0x246262,0x2463ea,0x2463ee,0x246480,0x246484,0x246326,
    0x246ad2,0x246b4c,0x247034,0x246eb2,0x2470f6,0x247598,0x24741e,
    0x245ba0,0x247656,0x24767a,0x24855c,0x245d92,0x245de6,0x245fec,0x2461a2,0x24683a,0x246694,0x2465da,0x2464ba,
    0x246794]
# Mission-only programs (docs/MEMORY_OPTIMIZATION.md, rule 5): the blocks of
# ROOTS whose comment starts with one of these lines belong to one mission
# ("m2", "m3"): they go into that mission's pack (pack_mission.py, 'MSXD'),
# not into the console image (compile_soldier_ai_tables.py). A root also
# listed in another block stays in the image.
MISSION_BLOCKS={'m2':('# Mission 2: the rebels','# Mission 2: the 0678B6','# Mission 2: the stage-2 prop','# Mission 2: the drone',
    '# Mission 2: the hostage','# The miners','# The bats','# The stage-2/3 signs','# Mission 2 stage-3 boss','# Mission 2: the stage-2 gate',
    '# Mission 2 stage 3 (neo_m2traps.c)','# The gas vent','# The rope soldier','# The stage-3 boulder','# Mission 2 stage 3: the creature'),
    'm3':('# Mission 3: the wagon box','# Mission 3 wagons','# Mission 3 grenadier','# Mission 3 shield soldiers','# Mission 3 window soldiers',
    '# Mission 3 mid-boss','# Mission 3 boss 04A2C2','# Mission 3 SV-001 093C9A','# Mission 3 vehicle 0AC866','# Mission 3 weapon E 0DE4DA','# Mission 3 fat weapons','# Mission 3 soldiers 05C53C','# Mission 3 flying machine','# Mission 3 gunship','# Mission 3 molotov throwers','# Mission 3 flying bomber','# Mission 3 oil drum','# Mission 3 armed truck','# Mission 3 floating platform','# Mission 3 rider car','# Mission 3 armed jeep','# Mission 3 birds, sandbags and camp'),
    'm4':('# Mission 4 props','# Mission 4 flyers')}
def root_groups():
    import re
    text=Path(__file__).read_text();seen={};other=set()
    for com,body in re.findall(r'((?:^#[^\n]*\n)+)((?:^(?!#)[^\n]*\n)+)',text,re.M):
        if 'ROOTS' not in body:continue
        first=com.split('\n')[0]
        g=next((k for k,v in MISSION_BLOCKS.items() if first.startswith(v)),None)
        roots=[int(x,16) for x in re.findall(r'0x[0-9a-f]+',body)]
        if g:
            for r in roots:seen.setdefault(r,g)
        else:other.update(roots)
    return {r:g for r,g in seen.items() if r not in other}
# Mission 3 gunship 097FCC, its rotor 098886, bomb 09879C, missile 098974, pieces 098C0C.. and smoke 098EA2 (neo_m3enemy.c).
ROOTS+=[0x29ed1a,0x29ee20,0x29ef26,0x29ef7c,0x29f032,0x29f06c,0x29f090,0x29f0e6,0x29f13c,0x29f192]
# Mission 3 molotov throwers 0569B4/0569D0/0569E6, their shield 056D76, molotov 056B00, flash 056C94 and helmet 056A6A (neo_m3enemy.c).
ROOTS+=[0x24ac4c,0x24ac56,0x24ad2a,0x24ad9a,0x24adf2,0x24aed2,0x24b106,0x24b1f6,0x24b426,0x24b50a,0x24b548,0x24b58a,0x24b5cc,0x24b616,0x24b680,0x24b730,0x24b7ae,0x24b810,0x24b8b6,0x24b966,0x24b9b8,0x24ba1a]
# Mission 3 SV-001 093C9A (neo_tank.c v3, docs/PLAN_MISION3.md step 5): its jump program 299228 (09452C; the original's 299180 is shared).
ROOTS+=[0x299228]
# Mission 3 vehicle 0AC866 (neo_m3veh.c): its program table 0AAF72, the pilot tables 0AB40C (by character), the pieces and the marker 03533A.
ROOTS+=[0x0ab09e,0x0ab1aa,0x0ab29e,0x0ab0ac,0x0ab250,0x0aafa2,0x0aafb2,0x0ab028,0x0ab202,0x0ab458,
    0x0ab48c,0x0ab654,0x0ab576,0x0ab528,0x0ab44c,0x0ab4c0,0x0ab7d6,0x0ab80a,0x0ab9d2,0x0ab8f4,
    0x0ab8a6,0x0ab7ca,0x0ab83e,0x0abb54,0x0abb88,0x0abd50,0x0abc72,0x0abc24,0x0abb48,0x0abbbc,
    0x0abed2,0x0abf06,0x0ac0ce,0x0abff0,0x0abfa2,0x0abec6,0x0abf3a,0x0ac214,0x0ac5ca,0x0ad53a,
    0x0ac61c,0x0ac66e,0x0ad738,0x0ac398,0x0adc8a,0x0ac23e,0x0ac288,0x0ac2d2,0x23ae0a,0x23b0e8]
# Mission 3 fat weapons (+93 = 2, table 0C42EA / 0C476E): the R's rockets 063AEE 267C74 / 063B22 267E86 and their explosion 26860A, the C 063F10 2681A4, the bullets 01B62C 22C0A6 / 01B668 22C000, the grenade 01BEDA 22CA58.
ROOTS+=[0x267c74,0x267e86,0x26860a,0x2681a4,0x22c0a6,0x22c000,0x22ca58]
# Mission 3 fat bomb type 3 01C362 / 01C334 22CD04 (thin 01C3BE / 01C390 22CC34) and its piece 01C654 (22CD5A / 22CC8A), the fat landing dust 0C4E86 0C4E48, the fat molotov 01E680 / 01E69C 22F65E (neo_bullet.c).
ROOTS+=[0x22cd04,0x22cc34,0x22cd5a,0x22cc8a,0x0c4e48,0x22f65e]
# The splash 03C962 (programs 03C952: 22E064 / 22E0F2 / 22DF54 / 22E1F0; neo_gate.c).
ROOTS+=[0x22e064,0x22e0f2,0x22df54,0x22e1f0]
# Mission 3 weapon E 0DE4DA (neo_pweapon.c): the ball's programs 2B0272/2B027A/2B0282 (two variants).
ROOTS+=[0x2b01a6,0x2b020c,0x2b01b0,0x2b0216,0x2b01de,0x2b0244]
# Mission 3 flying bomber 084588, rotor 083B30, bomb 08471A, trail 0848EE, helmet 084948, debris 08497E (neo_m3enemy.c).
ROOTS+=[0x281ffc,0x28207a,0x2824f4,0x282518,0x282362,0x28236e,0x282388]
# Mission 3 oil drum 0391EA (neo_m3enemy.c).
ROOTS+=[0x23c658,0x23c6b4]
# Mission 3 armed truck 02B202, gunner 02B61C, rider 02B670, tyres 02BBBA (neo_m3enemy.c).
ROOTS+=[0x236d00,0x236d88,0x23739e,0x2373cc,0x2373e6,0x23787c,0x2378aa]
# Mission 3 floating platform 08AFB6 and its floor 08B3E6 (neo_m3enemy.c).
ROOTS+=[0x285c6a,0x285c84,0x285cb2,0x285cbc,0x285d3a,0x285d68,0x285d96,0x285da8,0x285dba,0x285dcc,0x285eaa,0x285ebe]
# Mission 3 rider car 027F54, its rider 0287FA, bomb 0286F8, shell 0287AC and pieces 028A88.. (neo_m3enemy.c; 236272 and 22D372 above).
ROOTS+=[0x235baa,0x235a96,0x235ad8,0x235b1a,0x235c78,0x235d58,0x235be8,0x235c30,0x235e38,0x235f3e,0x235eaa,0x235fb6,0x235fe0,0x23606e,0x23611c,0x236170,0x236244,0x23638a,0x2363d8,0x236356,0x236314,0x23643a,0x2364d4,0x236758,0x23660c,0x2366b2,0x23657a,0x2361de]
# Mission 3 armed jeep 0A2F2E, gunner 0A38F4 (table 0A4396), turret 0A3A0A (table 0A436A), rider, pieces 0A3BC2/0A3BD6, shell 0A3C24 (neo_m3enemy.c; 2404FE above).
ROOTS+=[0x2a392a,0x2a39be,0x2a3aca,0x2a3b80,0x2a3cbc,0x2a3e92,0x2a40fa,0x2a426a,0x2a4056,0x2a440a,0x2a4350,0x2a4578,0x2a45fe,0x2a3958,0x2a3a14,0x2a3b20,0x2a3be6,0x2a3d88,0x2a3eda,0x2a3f42,0x2a409e,0x2a4170,0x2a42e0,0x2a438e,0x2a4484,0x2a4592,0x2a46c0,0x2a3990,0x2a3a74,0x2a3c56,0x2a3e5e,0x2a3f58,0x2a3fb4,0x2a41e6,0x2a43cc,0x2a44fe,0x2a45ac,0x2a473a,0x2a478e,0x2a47c4,0x2a4866,0x2a48fe]
# Shared by Missions 2 and 3: the dust 22D372 (03A464 of Mission 2 above; the piece 028CA4 of the Mission 3 rider car): no mission group.
ROOTS+=[0x22d372]
# The soldiers' grenade 03C306 (03C2DE of the actions 0AF92C / 0AF940: 23DA62 / 23D9EA) (neo_m3enemy.c).
ROOTS+=[0x23f2a0,0x23da62,0x23d9ea]
# Item kind 28 (record 0E581A + 0x18 * 0x28): its program 2869D6 as a root (the walker 288396 falls into it by 2884C8; the Mission 3 hidden boxes 09A67E drop kind 28 directly, 08C374 starts 2869D6 itself).
ROOTS+=[0x2869d6]
# Mission 3 birds 03ED00.., sandbags 0214EA/0216BA with their pieces 0218C4, camp 01F1D4 (soldiers 01F2D6/01F386, stove 01F562, pot 01F4DA) (neo_m3enemy.c).
ROOTS+=[0x23fe3e,0x23fecc,0x23fefa,0x23fea0,0x23feac,0x23fe6c,0x23ff04,0x230238,0x23035e,0x2303e4,0x2303ca,0x2303f0,0x230438,0x23052c,0x230512,0x230538,0x22f81a,0x22f868,0x22f89c,0x22f9d8,0x22f8c2,0x22f98a,0x22f8f6]
# Burning pieces 08732C of the destroyed shop 0B6566 (0B68B4, Mission 1; neo_barrel.c, B51): flight and landing.
ROOTS+=[0x283b7a,0x283bf8]
# Mission 2 stage 3: the creature 080170 thrown by the soldiers 0678B6 of +9C = 8 (06899A -> 0689D2), its pieces and its egg 0808AA (neo_m2creature.c, B54).
ROOTS+=[0x280142,0x280198,0x2801cc,0x2802c2,0x2803bc,0x2804a0,0x280436,0x28050a,0x280342,0x28034c]
# Mission 4 props (neo_m4props.c): the decorations 0B99EE (2A8EFC), 0B9AD6 (2A8F3E) and 0B9B24 (2A8F4A), the collapse pieces 0B9D88 (tables 2A8F56 / 2A8F62) and the debris 0BB106 (table 0BB0B6).
ROOTS+=[0x2a8efc,0x2a8f3e,0x2a8f4a,0x2a8f6e,0x2a8f7a,0x2a8f86,0x2a8f92,0x2a8f9e,0x2a8faa,0x22d414,0x22d47e,0x22d4e8,0x22d552,0x22d5bc,0x22d626]
# Mission 4 flyers 083C8A / 08391C (neo_m3enemy.c, the family of the Mission 3 bomber 084588): body 2820F8 / 282154 / 2821AC, rotors 282126 / 282182, rider 2823B2 / 2823E0, explosion 28240A (281FFC, 28207A, 282362 above).
ROOTS+=[0x2820f8,0x282126,0x282154,0x282182,0x2821ac,0x2823b2,0x2823e0,0x28240a]
def convert(bank,root,fixed=None):
    commands=[];where={}
    def rd(pc,n):
        # Programs of the P1 ROM (the rocket launcher's flash 060490 and
        # muzzle 0602E4) read the fixed block.
        if pc<0x100000:assert fixed is not None;return fixed[pc:pc+n]
        assert 0x200000<=pc<0x300000;return bank[pc-0x200000:pc-0x200000+n]
    def add(pc,**k):
        c=dict(address=pc,kind='',asset=0,target=0,operand=0,flags=0,dx=0,dy=0);c.update(k)
        where.setdefault(pc,len(commands));commands.append(c);return c
    pending=[root];fixups=[]
    while pending:
        pc=pending.pop(0);fresh=pc not in where
        while pc not in where:
            op,arg=rd(pc,2)
            if op==0:add(pc,kind='WAIT',operand=arg);pc+=2
            elif op==4:
                dx,dy=struct.unpack('bb',rd(pc+2,2));add(pc,kind='POSE',flags=arg,dx=dx,dy=dy,asset=struct.unpack('>I',rd(pc+4,4))[0]);pc+=8
            elif op==8:
                t=struct.unpack('>I',rd(pc+2,4))[0];fixups.append((add(pc,kind='JUMP'),t));pending.append(t);break
            elif op==0x0c:
                f=struct.unpack('>H',rd(pc+2,2))[0];t=struct.unpack('>I',rd(pc+4,4))[0]
                fixups.append((add(pc,kind='COND_JUMP',asset=f,operand=1 if arg else 0),t));pending.append(t);pc+=8
            elif op==0x10:
                length=struct.unpack('>H',rd(pc+2,2))[0];assert length%8==0
                # 004412: pose selected by the actor byte `arg` (+61 for the soldiers).
                c=add(pc,kind='SELECT_POSE' if arg==0x61 else 'SELECT_FIELD_POSE',asset=arg,operand=length//8);c['target']=len(commands)
                for i in range(length//8):
                    at=pc+4+i*8;assert rd(at,1)[0]==4
                    dx,dy=struct.unpack('bb',rd(at+2,2));add(at,kind='POSE',flags=rd(at+1,1)[0],dx=dx,dy=dy,asset=struct.unpack('>I',rd(at+4,4))[0])
                pc+=4+length
            elif op==0x14:
                routine=struct.unpack('>I',rd(pc+2,4))[0];param=struct.unpack('>H',rd(pc+6,2))[0] if arg>=6 else 0
                if routine==0x1adaa:add(pc,kind='FLIP_FACING')
                elif routine in PALETTE:add(pc,kind='REPLACE_PALETTE',asset=PALETTE[routine],target=routine&0xffff)
                elif routine==0x3bb8e:add(pc,kind='SPAWN_THROW',asset=0x3bc28)
                # 03BBBC/03BBEA: same object with +6E=1 / 2 and offsets (0,24) / (0,32).
                elif routine in (0x3bbbc,0x3bbea):add(pc,kind='SPAWN_THROW',asset=routine)
                # The SV-001's 09280E/092820 take A0 = the table after the
                # routine (+70 = table[+74 / +75]): the low word of the
                # table's address (bank 29xxxx, asserted).
                elif routine in (0x9280e,0x92820):assert (pc+6)>>16==0x29;add(pc,kind='CALL',asset=routine,target=(pc+6)&0xffff)
                elif routine in GENERIC:add(pc,kind='CALL',asset=routine,target=param)
                else:add(pc,kind='CALL',asset=routine,target=param)
                pc+=2+arg
            elif op==0x18:add(pc,kind='SET_BYTE',target=struct.unpack('>H',rd(pc+2,2))[0],asset=arg);pc+=4
            elif op==0x1c:add(pc,kind='SET_WORD',target=arg,asset=struct.unpack('>H',rd(pc+2,2))[0]);pc+=4
            elif op==0x20:
                v=struct.unpack('>I',rd(pc+2,4))[0]
                add(pc,kind='FIELD50',asset=v) if arg==0x50 else add(pc,kind='SET_LONG',target=arg,asset=v);pc+=6
            elif op==0x24:add(pc,kind='DEC_BYTE',target=arg);pc+=2
            elif op==0x28:add(pc,kind='FINISH');break
            elif op==0x2c:
                length=struct.unpack('>H',rd(pc+2,2))[0];assert length%2==0
                add(pc,kind='SWITCH',asset=arg,operand=length//2);table=pc+4
                for i in range(length//2):
                    t=table+struct.unpack('>h',rd(table+2*i,2))[0]+length
                    fixups.append((add(table+2*i,kind='JUMP'),t));pending.append(t)
                pc=table+length
            elif op==0x30:add(pc,kind='SOUND',target=struct.unpack('>H',rd(pc+2,2))[0]);pc+=4
            else:add(pc,kind='UNRECOVERED',asset=op);break
        else:
            # Falling into an already converted address: make the flow explicit.
            if fresh and commands[-1]['kind'] not in ('JUMP','FINISH','UNRECOVERED'):
                fixups.append((add(-pc,kind='JUMP'),pc))
    for c,t in fixups:c['target']=where[t]
    return commands
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    bank=(a.capture/'program-bank.bin').read_bytes();fixed=(a.capture/'program-fixed.bin').read_bytes()
    # Programs of a group (m3mid: the Mission 3 mid-boss) are left out of
    # the console image unless the build enables it (compile_soldier_ai_tables.py).
    group=root_groups()
    programs=[dict(root=r,commands=convert(bank,r,fixed),**({"group":group[r]} if r in group else {})) for r in dict.fromkeys(ROOTS)]
    kinds={}
    for prog in programs:
        for c in prog['commands']:kinds[c['kind']]=kinds.get(c['kind'],0)+1
    out=dict(source='MAME 0.264 mslugx euro, CPU window 0x200000..0x2FFFFF saved during Mission 1 (P2 bank 1)',
        program_bank_sha256=hashlib.sha256(bank).hexdigest(),command_kinds=kinds,programs=programs,
        limits=['Callbacks without a dedicated kind are CALL commands; the owning actor handles or rejects them explicitly.',
                'Fall-through into an already converted command is expressed as an explicit JUMP (negative address marker).'])
    a.output.write_text(json.dumps(out,separators=(',',':'))+'\n')
    print(kinds)
if __name__=='__main__':main()
