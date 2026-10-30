/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x068e14.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_68e14(void)

{
  byte bVar3;
  char cVar4;
  uint uVar1;
  ushort uVar2;
  int unaff_A6;
  undefined8 uVar5;

  bVar3 = candidate_29c6();
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    cVar4 = (bVar3 & 0x3f) - 0xf;
  }
  else {
    cVar4 = (bVar3 & 0x3f) + 0x50;
  }
  *(char *)(unaff_A6 + 0x98) = cVar4;
  uVar1 = candidate_29c6();
  *(uint *)(unaff_A6 + 0x8a) = (uVar1 & 0x1ffff) + 0x80000;
  uVar5 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x30) = uVar5;
  uVar2 = candidate_29c6();
  if ((uVar2 & 1) == 0) {
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) ^ 1;
  }
  *(ushort *)(unaff_A6 + 0x26) = (uVar2 & 0x7f) + 0xc0;
  uVar2 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x92) = (uVar2 & 0x7f) + 0x120;
  return;
}
