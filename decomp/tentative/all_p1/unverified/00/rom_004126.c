/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x004126.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_4126(void)

{
  int iVar1;
  undefined1 in_D0b;
  undefined1 uVar2;
  int extraout_A0;
  int *unaff_A6;
  int *piVar3;
  bool bVar4;

  DAT_00300001 = in_D0b;
  uVar2 = candidate_3f90();
  *unaff_A6 = (int)candidate_3f1e;
  while( true ) {
    piVar3 = unaff_A6;
    bVar4 = false;
    if (*(short *)(piVar3 + 4) == 0) {
      DAT_00300001 = uVar2;
      candidate_9ea0();
      candidate_1ab60();
      candidate_164b8();
      *(undefined1 *)(extraout_A0 + 0x15) = 0;
      *(undefined1 *)(extraout_A0 + 0x5f) = 0;
      *(undefined1 *)(extraout_A0 + 0x60) = 0;
      *(undefined1 *)(extraout_A0 + 0x61) = 0;
      *(undefined1 *)(extraout_A0 + 0x12) = 0xff;
      *(undefined1 *)(extraout_A0 + 0x13) = 0xff;
      *(undefined2 *)(extraout_A0 + 0x1e) = 0xffff;
      DAT_00300001 = 0;
      *(undefined4 *)(extraout_A0 + 0x6e) = 0;
      *(undefined4 *)(extraout_A0 + 0x72) = 0;
      *(undefined4 *)(extraout_A0 + 0x76) = 0;
      *(undefined4 *)(extraout_A0 + 0x7a) = 0;
      *(undefined4 *)(extraout_A0 + 0x7e) = 0;
      *(undefined4 *)(extraout_A0 + 0x82) = 0;
      *(undefined4 *)(extraout_A0 + 0x86) = 0;
      *(undefined4 *)(extraout_A0 + 0x8a) = 0;
      *(undefined4 *)(extraout_A0 + 0x8e) = 0;
      *(undefined4 *)(extraout_A0 + 0x92) = 0;
      *(undefined4 *)(extraout_A0 + 0x96) = 0;
      *(undefined4 *)(extraout_A0 + 0x9a) = 0;
      *(undefined4 *)(extraout_A0 + 0x9e) = 0;
      *(undefined4 *)(extraout_A0 + 0xa2) = 0;
      *(undefined4 *)(extraout_A0 + 0xa6) = 0;
      *(undefined4 *)(extraout_A0 + 0xaa) = 0;
      *(undefined2 *)(extraout_A0 + 0xae) = 0;
      return;
    }
    candidate_408c();
    if (bVar4) break;
    uVar2 = candidate_3f24();
    *piVar3 = (int)candidate_3f1e;
    unaff_A6 = (int *)piVar3[1];
    iVar1 = piVar3[2];
    *(int **)(iVar1 + 4) = unaff_A6;
    unaff_A6[2] = iVar1;
    piVar3[2] = (int)piRam001063d4;
    _DAT_001063d8 = _DAT_001063d8 + 1;
    piRam001063d4 = piVar3;
    if ((code *)*unaff_A6 != candidate_3f1e) {
      return;
    }
  }
  return;
}
