/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0902e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_902e4(void)

{
  short sVar1;
  byte bVar2;
  undefined1 uVar4;
  byte bVar5;
  int iVar3;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar6;

  candidate_919f4();
  if ((bool)in_CF) {
LAB_00090698:
    iVar3 = *(int *)((int)unaff_A6 + 0x6e);
    if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0) {
      iVar3 = -iVar3;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar3;
    *(undefined1 *)(unaff_A6 + 0x18) = 1;
    unaff_A6[0x14] = 0x91cec;
    candidate_4e76();
    candidate_4256();
    *unaff_A6 = 0x906da;
    uVar4 = 0;
    candidate_91416();
    candidate_919f4();
    if (!(bool)uVar4) {
      *unaff_A6 = candidate_90124;
      uVar4 = 0;
    }
    candidate_427a();
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
    candidate_4c24();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    sVar1 = *(short *)(unaff_A6 + 0x20);
    bVar6 = sVar1 == 0;
    *(short *)(unaff_A6 + 0x20) = sVar1 + -1;
    candidate_919a4();
    if (bVar6) {
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) ^ 0x80;
      *unaff_A6 = candidate_90772;
      uVar4 = false;
    }
    else if ((*(short *)(unaff_A6 + 0xf) < 0x10) && ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0))
    {
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x80;
      *unaff_A6 = candidate_90772;
      uVar4 = false;
    }
    else {
      uVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x130;
      if ((0x130 < (short)*(ushort *)(unaff_A6 + 0xf)) &&
         ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0)) {
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0x7f;
        *unaff_A6 = candidate_90772;
        uVar4 = false;
      }
    }
    candidate_91ada();
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    candidate_914a2();
    *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
    bVar6 = false;
    candidate_91a20();
    if (bVar6) {
      uVar4 = candidate_919f4();
      if (bVar6) goto LAB_00090698;
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar4;
      candidate_4256();
      candidate_91ab6();
    }
    else {
      uVar4 = candidate_919f4();
      if (bVar6) goto LAB_00090698;
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar4;
      candidate_4256();
      candidate_91ab6();
    }
    *unaff_A6 = 0x90388;
    uVar4 = 0;
    candidate_91416();
    bVar5 = candidate_919f4();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_90042;
      uVar4 = false;
    }
    else {
      bVar2 = *(byte *)((int)unaff_A6 + 0x92);
      if (bVar5 != bVar2) {
        *unaff_A6 = candidate_90042;
      }
      uVar4 = bVar5 == bVar2 && bVar5 < bVar2;
    }
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_90042;
      uVar4 = 0;
    }
    candidate_91ada();
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
    candidate_4c24();
    if ((bool)uVar4) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar4 = 0;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x40;
    }
  }
  candidate_90a1e();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_908e4;
    uVar4 = 0;
  }
  candidate_91a4e();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9094c;
  }
  return;
}
