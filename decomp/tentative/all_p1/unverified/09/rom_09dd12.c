/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09dd12.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9dd12(void)

{
  char cVar1;
  short sVar3;
  int iVar2;
  byte bVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  if (*(short *)(unaff_A6 + 0xf) < 0xa1) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) goto candidate_9e00a;
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
candidate_9e00a:
    *(undefined1 *)((int)unaff_A6 + 0x93) = 6;
    candidate_4256();
    *unaff_A6 = 0x9e026;
    uVar5 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_9d9c0;
    }
    goto LAB_0009e7ac;
  }
  sVar3 = candidate_260c8();
  iVar2 = sVar3 * 0x200;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = sVar3 * -0x200;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  bVar4 = candidate_29c6();
  *(byte *)((int)unaff_A6 + 0x91) = (bVar4 & 7) + 8;
  candidate_4256();
  *unaff_A6 = 0x9dd90;
  uVar5 = 0;
  candidate_9efc8();
  candidate_427a();
  candidate_9f0fa();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_9e25a;
    uVar5 = 0;
  }
  candidate_9f23a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_9dfb2;
  }
  *(char *)((int)unaff_A6 + 0x91) = *(char *)((int)unaff_A6 + 0x91) + -1;
  if (*(char *)((int)unaff_A6 + 0x91) < '\x01') {
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
LAB_0009e7ac:
  sVar3 = *(short *)(unaff_A6 + 0x22);
  bVar6 = sVar3 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar3 + -1;
  candidate_4c24();
  if (bVar6) {
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
  bVar6 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar6) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
