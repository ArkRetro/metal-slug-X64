/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086cf8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86cf8(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) | 0x80;
  *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) | 0x40;
  unaff_A6[0x14] = 0x87c92;
  candidate_4256();
  *unaff_A6 = 0x86d32;
  uVar2 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_86d4c;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = 0x86eb4;
  }
  cVar1 = *(char *)((int)unaff_A6 + 0x87);
  *(char *)((int)unaff_A6 + 0x87) = cVar1 + -1;
  if (cVar1 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 0) {
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  bVar3 = false;
  candidate_4c24();
  if ((bVar3) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
