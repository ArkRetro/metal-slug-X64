/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x090124.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_90124(void)

{
  short sVar1;
  undefined1 uVar3;
  byte bVar4;
  undefined1 uVar5;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar6;

  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  iVar2 = *(int *)((int)unaff_A6 + 0x6e);
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  bVar6 = false;
  candidate_91a20();
  if (bVar6) {
    uVar3 = candidate_919f4();
    if (!bVar6) {
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar3;
      candidate_4256();
      candidate_91ab6();
      goto LAB_000901c6;
    }
  }
  else {
    uVar3 = candidate_919f4();
    if (!bVar6) {
      *(undefined1 *)((int)unaff_A6 + 0x92) = uVar3;
      candidate_4256();
      candidate_91ab6();
LAB_000901c6:
      *unaff_A6 = 0x901cc;
      uVar3 = 0;
      candidate_91416();
      bVar4 = candidate_919f4();
      if (!(bool)uVar3) {
        uVar3 = bVar4 < *(byte *)((int)unaff_A6 + 0x92);
        if (bVar4 != *(byte *)((int)unaff_A6 + 0x92)) {
          candidate_91a20();
          if ((bool)uVar3) {
            uVar5 = candidate_919f4();
            if ((bool)uVar3) goto LAB_00090698;
            *(undefined1 *)((int)unaff_A6 + 0x92) = uVar5;
            uVar3 = 0;
            candidate_4256();
            candidate_91ab6();
          }
          else {
            uVar5 = candidate_919f4();
            if ((bool)uVar3) goto LAB_00090698;
            *(undefined1 *)((int)unaff_A6 + 0x92) = uVar5;
            uVar3 = 0;
            candidate_4256();
            candidate_91ab6();
          }
        }
        candidate_427a();
        candidate_919a4();
        if ((bool)uVar3) {
          *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) ^ 0x80;
          *unaff_A6 = candidate_902e4;
          uVar3 = false;
        }
        else if ((*(short *)(unaff_A6 + 0xf) < 0x30) &&
                ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0)) {
          *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x80;
          *unaff_A6 = candidate_902e4;
          uVar3 = false;
        }
        else {
          uVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x110;
          if ((0x110 < (short)*(ushort *)(unaff_A6 + 0xf)) &&
             ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0)) {
            *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0x7f;
            *unaff_A6 = candidate_902e4;
            uVar3 = false;
          }
        }
        candidate_91ada();
        candidate_917fc();
        if ((bool)uVar3) {
          *unaff_A6 = candidate_902e4;
          uVar3 = 0;
        }
        *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
        candidate_4c24();
        if ((bool)uVar3) {
          *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
          uVar3 = 0;
          *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x40;
        }
        goto LAB_000908c0;
      }
    }
  }
LAB_00090698:
  iVar2 = *(int *)((int)unaff_A6 + 0x6e);
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  *(undefined1 *)(unaff_A6 + 0x18) = 1;
  unaff_A6[0x14] = 0x91cec;
  candidate_4e76();
  candidate_4256();
  *unaff_A6 = 0x906da;
  uVar3 = 0;
  candidate_91416();
  candidate_919f4();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_90124;
    uVar3 = 0;
  }
  candidate_427a();
  *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  sVar1 = *(short *)(unaff_A6 + 0x20);
  bVar6 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x20) = sVar1 + -1;
  candidate_919a4();
  if (bVar6) {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) ^ 0x80;
    *unaff_A6 = candidate_90772;
    uVar3 = false;
  }
  else if ((*(short *)(unaff_A6 + 0xf) < 0x10) && ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) == 0)) {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x80;
    *unaff_A6 = candidate_90772;
    uVar3 = false;
  }
  else {
    uVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x130;
    if ((0x130 < (short)*(ushort *)(unaff_A6 + 0xf)) &&
       ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0)) {
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0x7f;
      *unaff_A6 = candidate_90772;
      uVar3 = false;
    }
  }
  candidate_91ada();
LAB_000908c0:
  candidate_90a1e();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_908e4;
    uVar3 = 0;
  }
  candidate_91a4e();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9094c;
  }
  return;
}
