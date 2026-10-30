/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x092832.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Instruction at (ram,0x0000432c) overlaps instruction at (ram,0x0000432a)
    */
/* WARNING: Removing unreachable block (ram,0x000042b2) */
/* WARNING: Removing unreachable block (ram,0x000042b8) */
/* WARNING: Removing unreachable block (ram,0x00004322) */
/* WARNING: Removing unreachable block (ram,0x00004328) */
/* WARNING: Removing unreachable block (ram,0x00004330) */
/* WARNING: Removing unreachable block (ram,0x00004338) */
/* WARNING: Removing unreachable block (ram,0x00004352) */
/* WARNING: Removing unreachable block (ram,0x00004340) */
/* WARNING: Removing unreachable block (ram,0x00004342) */
/* WARNING: Removing unreachable block (ram,0x0000434e) */
/* WARNING: Removing unreachable block (ram,0x00004348) */
/* WARNING: Removing unreachable block (ram,0x0000432a) */

void candidate_92832(void)

{
  byte bVar1;
  ushort uVar2;
  ushort uVar3;
  short sVar4;
  int iVar5;
  byte *pbVar6;
  byte *pbVar7;
  int unaff_A5;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    iVar5 = 0x927ca;
  }
  else if (*(char *)(unaff_A6 + 0x77) == '\0') {
    iVar5 = 0x92742;
  }
  else {
    iVar5 = 0x92786;
  }
  *(undefined4 *)(unaff_A6 + 0x50) =
       *(undefined4 *)(iVar5 + (short)((ushort)*(byte *)(unaff_A6 + 0x74) << 2));
  *(char *)(unaff_A6 + 0x78) = -(*(int *)(unaff_A6 + 0x30) != 0);
  uVar2 = *(short *)(unaff_A6 + 0x84) >> 5;
  if ((short)uVar2 < 0) {
    uVar3 = ~(~uVar2 & 0xe0);
  }
  else {
    uVar3 = uVar2 - 1;
    if (uVar2 != 0) {
      uVar3 = uVar3 & 0xff;
    }
    uVar3 = uVar3 + 1;
  }
  *(char *)(unaff_A6 + 0x74) = (char)((ushort)(uVar3 + 0x100) >> 5);
  sVar4 = *(short *)(unaff_A6 + 0x84);
  if (sVar4 < -0x1500) {
    *(undefined1 *)(unaff_A6 + 0x75) = 0;
  }
  else if (sVar4 < -0x500) {
    *(undefined1 *)(unaff_A6 + 0x75) = 1;
  }
  else if (sVar4 < 0x501) {
    *(undefined1 *)(unaff_A6 + 0x75) = 2;
  }
  else if (sVar4 < 0x1501) {
    *(undefined1 *)(unaff_A6 + 0x75) = 3;
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x75) = 4;
  }
  if (sVar4 < 0) {
    sVar4 = sVar4 + 0x1ff;
  }
  uVar2 = sVar4 >> 1 & 0xff00;
  *(char *)(unaff_A6 + 0x76) =
       -(uVar2 == 0xf000) |
       -(uVar2 == 0xfb00) | -(uVar2 == 0) | -(uVar2 == 0x500) | -(uVar2 == 0x1000);
  pbVar6 = *(byte **)(unaff_A6 + 0x20);
  if (pbVar6 != (byte *)0x0) {
    *(undefined1 *)(unaff_A5 + -0x1c20) = 0;
    if ('\0' < *(char *)(unaff_A6 + 0x17)) {
      *(char *)(unaff_A6 + 0x17) = *(char *)(unaff_A6 + 0x17) + -1;
    }
    while (0x33 < *pbVar6) {
      __m68k_trap(0xf);
      pbVar7 = pbVar6 + -1;
      *(short *)pbVar7 = *(short *)pbVar7 + 8;
      bVar1 = *pbVar7;
      (**(code **)pbVar6)();
      *(byte **)(unaff_A6 + 0x20) = pbVar6 + (short)(ushort)bVar1;
      pbVar6 = pbVar6 + (short)(ushort)bVar1;
    }
                    /* WARNING: Could not recover jumptable at 0x000042bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(thunk_FUN_000042f4 + (short)(ushort)*pbVar6))();
    return;
  }
  return;
}
