/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086c3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86c3a(void)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  uVar2 = candidate_29c6();
  if ((uVar2 & 1) == 0) {
    iVar3 = *(int *)((int)unaff_A6 + 0x76);
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      iVar3 = -iVar3;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar3;
    unaff_A6[0x14] = 0x87c92;
    candidate_4256();
    *unaff_A6 = 0x86c7a;
    uVar4 = 0;
    candidate_1456a();
    candidate_427a();
    candidate_8790a();
    candidate_87964();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_86a1c;
    }
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
    if (*(short *)((int)unaff_A6 + 0x82) < 0) {
      *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
      uVar4 = false;
      candidate_87a1e();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_86a1c;
        uVar4 = 0;
      }
      candidate_87ae6();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_86f4c;
        uVar4 = 0;
      }
      candidate_87b64();
      if ((bool)uVar4) {
        *unaff_A6 = 0x86b0e;
      }
    }
  }
  else {
    *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) & 0x7f;
    *(byte *)((int)unaff_A6 + 0x8b) = *(byte *)((int)unaff_A6 + 0x8b) | 0x40;
    unaff_A6[0x14] = 0x87c92;
    candidate_4256();
    *unaff_A6 = 0x86d32;
    uVar4 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_86d4c;
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
  bVar5 = false;
  candidate_4c24();
  if ((bVar5) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar5 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
