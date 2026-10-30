/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x045fc4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_45fc4(void)

{
  undefined2 extraout_D0u;
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;

  sVar1 = candidate_29f2();
                    /* WARNING: Switch is manually overridden */
  switch(sVar1 * 4) {
  case 0:
    candidate_411c();
    candidate_460c2();
    return;
  case 4:
    candidate_411c();
    candidate_460c2();
    return;
  case 8:
    candidate_411c();
    uVar2 = candidate_460c2();
    *(ushort *)(extraout_A0 + 0x6e) = uVar2 ^ 1;
    *(undefined2 *)(extraout_A0 + 0x70) = 0x708;
    uVar2 = candidate_29c6();
    *(ushort *)(extraout_A0_00 + 0x72) = uVar2 & 1;
    uVar2 = candidate_29c6();
    *(ushort *)(extraout_A0_01 + 0x74) = ((uVar2 & 3) + 1) * 0x10;
    return;
  case 0xc:
    candidate_411c();
    candidate_460c2();
    *(undefined2 *)(extraout_A0_02 + 0x6e) = 0;
    candidate_29c6();
    candidate_1ac4c();
    *(undefined2 *)(extraout_A0_02 + 0x70) = extraout_D0u;
    *(undefined2 *)(extraout_A0_02 + 0x72) = 0xffff;
    *(undefined2 *)(extraout_A0_02 + 0x74) = 0;
    return;
  }
}
