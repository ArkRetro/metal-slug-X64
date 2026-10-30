/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x003f9e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3f9e(void)

{
  int iVar1;
  short sVar2;
  undefined4 *extraout_A0;
  int *extraout_A0_00;
  undefined4 *extraout_A0_01;
  undefined4 *puVar3;
  int extraout_A1;
  int *piVar5;
  undefined4 *puVar4;

  candidate_3f24();
  *extraout_A0 = uRam00003e9e;
  *(undefined2 *)((int)extraout_A0 + 0x6e) = uRam00003ea2;
  *(undefined2 *)(extraout_A0 + 0x1c) = uRam00003ea4;
  *(undefined2 *)((int)extraout_A0 + 0x72) = uRam00003ea6;
  *(undefined2 *)(extraout_A0 + 0x1d) = uRam00003ea8;
  extraout_A0[3] = 0;
  *(undefined2 *)(extraout_A0 + 4) = 0;
  extraout_A0[1] = extraout_A0;
  extraout_A0[2] = extraout_A0;
  for (piVar5 = (int *)0x3eaa; *piVar5 != 0; piVar5 = piVar5 + 4) {
    candidate_3f24();
    extraout_A0_00[3] = 0;
    *(undefined2 *)(extraout_A0_00 + 4) = 0;
    *extraout_A0_00 = piVar5[1];
    *(undefined2 *)((int)extraout_A0_00 + 0x6e) = *(undefined2 *)(piVar5 + 2);
    *(undefined2 *)(extraout_A0_00 + 0x1c) = *(undefined2 *)((int)piVar5 + 10);
    *(undefined2 *)((int)extraout_A0_00 + 0x72) = *(undefined2 *)(piVar5 + 3);
    *(undefined2 *)(extraout_A0_00 + 0x1d) = *(undefined2 *)((int)piVar5 + 0xe);
    iVar1 = *(int *)(extraout_A1 + 8);
    *(int **)(iVar1 + 4) = extraout_A0_00;
    *(int **)(extraout_A1 + 8) = extraout_A0_00;
    extraout_A0_00[1] = extraout_A1;
    extraout_A0_00[2] = iVar1;
  }
  _DAT_001063d8 = 0;
  uRam001063d4 = 0x1016d4;
  while( true ) {
    candidate_3f24();
    *extraout_A0_01 = candidate_3f1e;
    _DAT_001063d8 = _DAT_001063d8 + 1;
    if ((undefined4 *)0x1063d3 < extraout_A0_01 + 0x2c) break;
    extraout_A0_01[2] = extraout_A0_01 + 0x2c;
  }
  if (_DAT_001063d8 != 0x70) {
    __m68k_trap(0xf);
  }
  extraout_A0_01[2] = 0;
  uRam001063da = 0;
  sVar2 = 0x57;
  puVar3 = (undefined4 *)0x1016d4;
  do {
    puVar4 = puVar3;
    puVar3 = (undefined4 *)((int)puVar4 + -2);
    *(undefined2 *)puVar3 = 0;
    sVar2 = sVar2 + -1;
  } while (sVar2 != -1);
  *puVar3 = candidate_3f1e;
  *(undefined4 *)((int)puVar4 + 2) = 0x101624;
  *(undefined4 *)((int)puVar4 + 6) = 0x101624;
  *(undefined4 *)((int)puVar4 + 10) = 0x101624;
  return;
}
