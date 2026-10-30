/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043c74.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43c74(void)

{
  uint uVar1;
  short sVar2;
  undefined1 uVar3;
  byte unaff_D2b;
  undefined2 uVar4;
  undefined4 unaff_D5;
  byte bVar6;
  int in_A0;
  int unaff_A6;
  uint uVar5;

  uVar4 = (undefined2)((uint)unaff_D5 >> 0x10);
  uVar5 = CONCAT22(uVar4,0x1ff);
  if (0xf0 < *(short *)(in_A0 + 0x3c)) {
    uVar5 = CONCAT22(uVar4,0x1e0);
  }
  if (*(short *)(in_A0 + 0x3c) < 0x50) {
    uVar5 = uVar5 & 0xffff000f;
  }
  if (0x110 < *(short *)(unaff_A6 + 0x3c)) {
    uVar5 = uVar5 & 0xffff01e0;
  }
  if (*(short *)(unaff_A6 + 0x3c) < 0x30) {
    uVar5 = uVar5 & 0xffff000f;
  }
  if (0x1d0 < *(short *)(unaff_A6 + 0x40)) {
    uVar5 = uVar5 & 0xffff0183;
  }
  if (*(char *)(unaff_A6 + 0x9c) != '\0') {
    uVar5 = uVar5 & 0xffff01c7;
  }
  if ((short)uVar5 == 0) {
    uVar5 = CONCAT22((short)(uVar5 >> 0x10),0x1ff);
  }
  candidate_2eb4();
  candidate_43d86();
  uVar1 = 8;
  bVar6 = 0;
  do {
    sVar2 = (short)uVar1;
    if ((uVar5 & 1 << (uVar1 & 0x1f)) != 0) {
      sVar2 = candidate_43dba();
      if (bVar6 < unaff_D2b) {
        *(char *)(unaff_A6 + 0x9a) = (char)sVar2;
        bVar6 = unaff_D2b;
      }
    }
    uVar1 = (uint)(ushort)(sVar2 - 1);
  } while (sVar2 != 0);
  uVar3 = candidate_43da4();
  *(undefined1 *)(unaff_A6 + 0x9a) = uVar3;
  return;
}
