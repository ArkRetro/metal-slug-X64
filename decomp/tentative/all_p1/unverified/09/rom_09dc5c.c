/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09dc5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9dc5c(void)

{
  char cVar1;
  short sVar3;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  sVar3 = candidate_260c8();
  iVar2 = sVar3 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = sVar3 * -0x100;
  }
  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x10) != 0) {
    iVar2 = iVar2 * 2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  if ((*(byte *)((int)unaff_A6 + 0x97) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9dcd0;
  uVar4 = 0;
  candidate_9efc8();
  candidate_427a();
  candidate_9f0fa();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9e25a;
    uVar4 = 0;
  }
  candidate_9f1c8();
  if ((bool)uVar4) {
    candidate_9f23a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_9dfb2;
    }
  }
  else {
    *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x20;
    *unaff_A6 = candidate_9dfb2;
  }
  if (((*(byte *)(unaff_A6 + 0x26) & 4) != 0) &&
     (*(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1,
     *(short *)((int)unaff_A6 + 0x8a) < 1)) {
    *unaff_A6 = candidate_9de84;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_9e34c;
  }
  sVar3 = *(short *)(unaff_A6 + 0x22);
  bVar5 = sVar3 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar3 + -1;
  candidate_4c24();
  if (bVar5) {
    candidate_9f53e();
    candidate_9f5fa();
    cVar1 = *(char *)((int)unaff_A6 + 0x5a);
    if (cVar1 == '\x02') {
      *unaff_A6 = candidate_9e8c2;
    }
    if (cVar1 == '\x01') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar1 == '\f') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar1 == '\r') {
      *unaff_A6 = candidate_9e920;
    }
    if (cVar1 == '\t') {
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
