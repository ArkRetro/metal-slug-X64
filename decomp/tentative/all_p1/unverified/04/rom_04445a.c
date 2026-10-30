/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04445a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_4445a(void)

{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  iVar3 = *(short *)((int)unaff_A6 + 0x8a) * 0x100;
  uVar2 = *(int *)(candidate_698c + unaff_A5 + 2) +
          (uint)*(ushort *)((int)unaff_A6 + 0x86) * -0x10000;
  if (-1 < (int)uVar2) {
    iVar3 = *(short *)((int)unaff_A6 + 0x8a) * -0x100;
  }
  if ((int)(uVar2 ^ iVar3 + uVar2) < 1) {
    *(uint *)(candidate_698c + unaff_A5 + 2) = (uint)*(ushort *)((int)unaff_A6 + 0x86) << 0x10;
    iVar3 = 0;
  }
  *(int *)(unaff_A5 + 0x695c) = iVar3;
  iVar3 = *(short *)(unaff_A6 + 0x23) * 0x100;
  uVar2 = *(int *)(unaff_A5 + 0x6992) + (uint)*(ushort *)(unaff_A6 + 0x22) * -0x10000;
  if (-1 < (int)uVar2) {
    iVar3 = *(short *)(unaff_A6 + 0x23) * -0x100;
  }
  if ((int)(uVar2 ^ iVar3 + uVar2) < 1) {
    *(uint *)(unaff_A5 + 0x6992) = (uint)*(ushort *)(unaff_A6 + 0x22) << 0x10;
    iVar3 = 0;
  }
  *(int *)(unaff_A5 + 0x6960) = iVar3;
  uVar2 = *(uint *)(unaff_A5 + 0x6960) | *(uint *)(unaff_A5 + 0x695c);
  if (uVar2 != 0) {
    return uVar2;
  }
  *(undefined1 *)(unaff_A6[3] + 0x60) = 1;
  candidate_4126();
  iVar3 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar3 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
