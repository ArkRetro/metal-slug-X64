/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086b52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86b52(void)

{
  char cVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  unaff_A6[0x14] = 0x87c92;
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) << 1;
  iVar2 = *(int *)((int)unaff_A6 + 0x76);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  candidate_4256();
  *unaff_A6 = 0x86b8e;
  uVar3 = 0;
  candidate_1456a();
  candidate_427a();
  candidate_8790a();
  candidate_87964();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_86a1c;
  }
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if (*(short *)((int)unaff_A6 + 0x82) < 0) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
    uVar3 = false;
    candidate_879fe();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_86a1c;
      uVar3 = 0;
    }
    candidate_87a64();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_86d08;
      uVar3 = 0;
    }
    candidate_87aa0();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_86cd8;
      uVar3 = 0;
    }
    candidate_87ae6();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_86f4c;
      uVar3 = 0;
    }
    candidate_87b64();
    if ((bool)uVar3) {
      *unaff_A6 = 0x86b0e;
    }
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
