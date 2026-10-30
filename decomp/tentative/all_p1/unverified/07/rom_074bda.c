/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074bda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_143ee(void)

{
  int iVar1;
  byte bVar2;
  undefined4 uVar3;
  byte unaff_D6b;
  undefined4 unaff_D7;
  undefined4 extraout_A0;
  int unaff_A5;
  int unaff_A6;
  undefined8 uVar4;

  candidate_128e8();
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  candidate_4ed2();
  if (((short)unaff_D7 < 0) && ((*(byte *)(unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined4 *)(unaff_A6 + 0x34) = 0x60000;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) & 0xef;
    bVar2 = *(byte *)(unaff_A6 + 0x15);
    *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xf7;
    if ((bVar2 & 8) != 0) {
      iVar1 = 0x40000;
      if (0 < *(int *)(unaff_A6 + 0x30)) {
        iVar1 = -0x40000;
      }
      *(int *)(unaff_A6 + 0x30) = (-*(int *)(unaff_A6 + 0x30) >> 1) + iVar1;
    }
  }
  *(undefined1 *)(unaff_A5 + 0x4009) = 0;
  *(undefined4 *)(unaff_A5 + 0x4004) = 0;
  *(char *)(unaff_A5 + 0x4008) = -((*(byte *)(unaff_A6 + 0x15) & 0x20) == 0);
  iVar1 = *(int *)(unaff_A6 + 0x30);
  uVar3 = *(undefined4 *)(unaff_A6 + 0x34);
  bVar2 = *(byte *)(unaff_A6 + 0x15);
  *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xf7;
  if (((bVar2 & 8) != 0) && (-1 < iVar1)) {
    iVar1 = 0;
  }
  bVar2 = *(byte *)(unaff_A6 + 0x15);
  *(byte *)(unaff_A6 + 0x15) = bVar2 & 0xef;
  if (((bVar2 & 0x10) != 0) && (iVar1 < 0)) {
    iVar1 = 0;
  }
  uVar4 = candidate_13968();
  *(undefined8 *)(unaff_A6 + 0x28) = uVar4;
  *(int *)(unaff_A6 + 0x30) = iVar1;
  *(undefined4 *)(unaff_A6 + 0x34) = uVar3;
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A0;
  bVar2 = *(byte *)(unaff_A6 + 0x5f) & 0xf8;
  if ((char)unaff_D6b < '\0') {
    bVar2 = bVar2 | 0x80;
    *(undefined1 *)(unaff_A6 + 0x5a) = 0xf;
  }
  *(byte *)(unaff_A6 + 0x15) =
       -((unaff_D6b & 2) != 0) & 0x80U |
       -((unaff_D6b & 1) != 0) & 0x40U | *(byte *)(unaff_A6 + 0x15) & 0x3f;
  *(byte *)(unaff_A6 + 0x5f) =
       -((unaff_D6b & 0x10) != 0) & 2U |
       -((unaff_D6b & 8) != 0) & 4U | -((unaff_D6b & 4) != 0) & 1U | bVar2;
  *(short *)(unaff_A6 + 0x6c) = (short)((uint)unaff_D7 >> 8);
  candidate_1abc8();
  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) {
    candidate_1ad8c();
  }
  return;
}
