/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c4372.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x000c44a2: Changing call to branch */
/* WARNING: Possible PIC construction at 0x000c4500: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000c44a4) */
/* WARNING: Removing unreachable block (ram,0x000c4504) */

void candidate_c4372(void)

{
  int iVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  int unaff_D7;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  undefined4 extraout_A0_02;
  undefined4 extraout_A0_03;
  undefined4 extraout_A0_04;
  undefined4 extraout_A0_05;
  int extraout_A0_06;
  int extraout_A1;
  int unaff_A5;
  int unaff_A6;
  bool bVar5;
  undefined1 uVar6;
  undefined4 uStack_8;
  int iStack_4;

  bVar5 = false;
  iStack_4 = 0xc4388;
  candidate_c0d64();
  if (!bVar5) {
    return;
  }
  iStack_4 = extraout_A0;
  if (*(int *)((short)((ushort)*(byte *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70)) << 2)
              + 0xc432a) != 0) {
    uStack_8 = 0xc43c0;
    candidate_411c();
    uStack_8 = 0xc43c4;
    candidate_c4244();
    *(char *)(extraout_A0_00 + 0x6e) = (char)unaff_D7;
    uStack_8 = 0xc43e4;
    candidate_1acf2();
    uStack_8 = 0xc43ea;
    candidate_1ac94();
  }
  bVar2 = *(byte *)(unaff_A5 + 0x471e + (int)*(short *)(unaff_A6 + 0x70));
  if (bVar2 == 5) {
    if (*(char *)(unaff_A6 + 0x93) == '\x02') {
      uVar6 = 0;
      iStack_4 = 0xc44d8;
      candidate_411c();
      iStack_4 = extraout_A0_04;
joined_r0x000c44d8:
      if (!(bool)uVar6) goto LAB_000c4526;
      register0x0000003c = (BADSPACEBASE *)&uStack_8;
      uStack_8 = 0xc4504;
      goto SUB_000c4434;
    }
    uVar6 = 0;
    iStack_4 = 0xc4474;
    candidate_411c();
    iStack_4 = extraout_A0_02;
  }
  else {
    if (bVar2 != 10) {
      unaff_D7 = *(int *)(iStack_4 + (short)((ushort)bVar2 << 2));
      if (unaff_D7 == 0) goto LAB_000c4526;
      bVar5 = false;
      iStack_4 = 0xc4428;
      candidate_411c();
      if (!bVar5) goto LAB_000c4526;
      *(char *)(unaff_A6 + 0x95) = *(char *)(unaff_A6 + 0x95) + '\x01';
      goto SUB_000c4434;
    }
    if (*(char *)(unaff_A6 + 0x93) == '\x02') {
      uVar6 = 0;
      iStack_4 = 0xc44f6;
      candidate_411c();
      iStack_4 = extraout_A0_05;
      goto joined_r0x000c44d8;
    }
    uVar6 = 0;
    iStack_4 = 0xc449c;
    candidate_411c();
    iStack_4 = extraout_A0_03;
  }
  if (!(bool)uVar6) {
LAB_000c4526:
    iStack_4 = 0xc4538;
    candidate_c0dfc();
    return;
  }
  register0x0000003c = (BADSPACEBASE *)&uStack_8;
  uStack_8 = 0xc44a4;
SUB_000c4434:
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc443a;
  candidate_1ac94();
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc443e;
  candidate_c4244();
  *(char *)(extraout_A0_01 + 0x6e) = (char)unaff_D7;
  *(undefined2 *)(extraout_A0_01 + 0x70) = *(undefined2 *)(unaff_A6 + 0x70);
  *(undefined1 *)(extraout_A0_01 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
  *(undefined4 *)((int)register0x0000003c + -4) = 0xc4834;
  candidate_14a02();
  iVar1 = *(int *)(unaff_A6 + 0x38);
  iVar3 = *(int *)(unaff_A6 + 0x30);
  iVar4 = *(int *)(unaff_A6 + 0x34);
  if (iVar1 != 0) {
    iVar3 = (*(int *)(iVar1 + 0x14) + iVar3) - *(int *)(iVar1 + 0x1c);
    iVar4 = (*(int *)(iVar1 + 0x18) + iVar4) - *(int *)(iVar1 + 0x20);
  }
  if (extraout_A1 != 0) {
    iVar3 = *(int *)(extraout_A1 + 0x1c) + (iVar3 - *(int *)(extraout_A1 + 0x14));
    iVar4 = *(int *)(extraout_A1 + 0x20) + (iVar4 - *(int *)(extraout_A1 + 0x18));
  }
  *(int *)(extraout_A0_06 + 0x30) = iVar3;
  *(int *)(extraout_A0_06 + 0x34) = iVar4;
  return;
}
