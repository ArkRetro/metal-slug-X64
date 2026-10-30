/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e062.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e062(void)

{
  short sVar1;
  char cVar2;
  undefined2 uVar3;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  if ((*(byte *)((int)unaff_A6 + 0x96) & 0x80) != 0) {
    if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
      *(undefined2 *)(unaff_A6 + 0x22) = 0x7fff;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xdf;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) & 0xbf;
      *(byte *)((int)unaff_A6 + 0x96) = *(byte *)((int)unaff_A6 + 0x96) | 0x10;
      *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x200;
      *(undefined1 *)((int)unaff_A6 + 0x93) = 6;
      candidate_4256();
      *unaff_A6 = 0x9e026;
      uVar4 = 0;
      candidate_1436a();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_9d9c0;
      }
      goto LAB_0009e7ac;
    }
    *(char *)((int)unaff_A6 + 0x95) = *(char *)((int)unaff_A6 + 0x95) + -1;
  }
  candidate_9f412();
  candidate_9f3b0();
  if ((*(byte *)((int)unaff_A6 + 0x97) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9e0b2;
  uVar4 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1436a();
  }
  candidate_427a();
  if ((bool)uVar4) {
    candidate_9f432();
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x22) = uVar3;
    candidate_1b04a();
    *(undefined4 *)((int)unaff_A6 + 0x6e) = extraout_A0;
    *unaff_A6 = candidate_9d9c0;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_9e34c;
    }
  }
LAB_0009e7ac:
  sVar1 = *(short *)(unaff_A6 + 0x22);
  bVar5 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
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
