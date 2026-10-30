/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09de84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9de84(void)

{
  byte bVar1;
  char cVar2;
  short sVar4;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  *(undefined1 *)((int)unaff_A6 + 0x99) = 0x1e;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x93) = 6;
    candidate_4256();
    *unaff_A6 = 0x9e026;
    uVar6 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_9d9c0;
    }
  }
  else {
    sVar4 = candidate_260c8();
    iVar3 = sVar4 * 0x200;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      iVar3 = sVar4 * -0x200;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar3;
    if ((*(byte *)((int)unaff_A6 + 0x97) & 2) == 0) {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    *unaff_A6 = 0x9defe;
    candidate_9efc8();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 8) == 0) {
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
        *(undefined1 *)((int)unaff_A6 + 0x99) = 0x1e;
        bVar5 = false;
      }
      else {
        bVar1 = *(byte *)((int)unaff_A6 + 0x99);
        bVar5 = bVar1 == 0;
        bVar1 = bVar1 - 1;
        *(byte *)((int)unaff_A6 + 0x99) = bVar1;
        if ((short)((ushort)bVar1 << 8) < 0) {
          *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
          *(undefined1 *)((int)unaff_A6 + 0x99) = 0x1e;
          bVar5 = false;
        }
      }
    }
    else {
      bVar1 = *(byte *)((int)unaff_A6 + 0x99);
      bVar5 = bVar1 == 0;
      bVar1 = bVar1 - 1;
      *(byte *)((int)unaff_A6 + 0x99) = bVar1;
      if ((short)((ushort)bVar1 << 8) < 0) {
        *unaff_A6 = candidate_9ec34;
        bVar5 = false;
      }
    }
    candidate_9f0fa();
    if (bVar5) {
      *unaff_A6 = candidate_9e25a;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_9e34c;
    }
  }
  sVar4 = *(short *)(unaff_A6 + 0x22);
  bVar5 = sVar4 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar4 + -1;
  candidate_4c24();
  if (bVar5) {
    candidate_9f53e();
    candidate_9f5fa();
    cVar2 = *(char *)((int)unaff_A6 + 0x5a);
    if (cVar2 == '\x02') {
      *unaff_A6 = candidate_9e8c2;
    }
    if (cVar2 == '\x01') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar2 == '\f') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar2 == '\r') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar2 == '\t') {
      *unaff_A6 = candidate_9e920;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_9eab8;
    }
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_9ec34;
  }
  bVar5 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
