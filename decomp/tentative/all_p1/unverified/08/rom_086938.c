/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086938.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86938(void)

{
  char cVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  unaff_A6[0x14] = 0x87c92;
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x8695c;
  candidate_1436a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    iVar2 = unaff_A6[0xd];
    if (-0x20000 < iVar2) {
      unaff_A6[0xd] = 0xfffe0000;
    }
    uVar3 = iVar2 < -0x20000;
    candidate_143dc();
  }
  else {
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    uVar3 = 0;
  }
  candidate_427a();
  candidate_1b284();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_86a1c;
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
  bVar4 = false;
  candidate_4c24();
  if ((bVar4) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
