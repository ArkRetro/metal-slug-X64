/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069e5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69e5c(void)

{
  byte bVar1;
  undefined4 uVar2;
  uint uVar3;
  int iVar4;
  short sVar5;
  int unaff_A5;
  undefined4 *unaff_A6;

  *unaff_A6 = 0x69e62;
  bVar1 = *(byte *)(unaff_A6[3] + 0x7f);
  *(byte *)(unaff_A6[3] + 0x7f) = bVar1 & 0xfe;
  if ((bVar1 & 1) == 0) {
    return;
  }
  candidate_1aca2();
  candidate_1ad6e();
  uVar2 = 0x200000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    uVar2 = 0xffe00000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x6e) = uVar2;
  uVar3 = (uint)(ushort)((*(ushort *)(unaff_A5 + -0x7ff2) & 1) << 4);
  iVar4 = uVar3 * 0x10000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar4 = uVar3 * -0x10000;
  }
  unaff_A6[0xc] = iVar4;
  unaff_A6[0xd] = 0;
  while (((func_0x000143e4(), (*(byte *)((int)unaff_A6 + 0x15) & 0x80) == 0 &&
          (-1 < *(short *)(unaff_A6 + 0xf))) && (*(short *)(unaff_A6 + 0xf) < 0x141))) {
    unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x6e);
    unaff_A6[0xd] = 0;
    sVar5 = candidate_49e8();
    if (sVar5 != 0) {
      *(byte *)(unaff_A6[3] + 0x7f) = *(byte *)(unaff_A6[3] + 0x7f) | 2;
      return;
    }
  }
  return;
}
