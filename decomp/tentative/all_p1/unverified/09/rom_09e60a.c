/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e60a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e60a(void)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar4;

  if ((*(byte *)((int)unaff_A6 + 0x96) & 4) == 0) {
    candidate_4256();
    unaff_A6[0xd] = 0x80000;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xfffeaaab;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      unaff_A6[0xc] = 0x2aaaa;
    }
    else {
      unaff_A6[0xc] = 0xfffd5556;
    }
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
  }
  else {
    *(byte *)(unaff_A6 + 0x26) = *(byte *)(unaff_A6 + 0x26) | 2;
    candidate_4256();
    unaff_A6[0xd] = 0x66666;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff92c6;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      unaff_A6[0xc] = 0;
    }
    else {
      unaff_A6[0xc] = 0;
    }
    candidate_1af9a();
    iVar3 = (short)((*(int *)(extraout_A0 + 0x3c) - unaff_A6[0xf] >> 8) / 0x28) * 0x100;
    if (iVar3 < -0x40000) {
      iVar3 = -0x40000;
    }
    if (0x40000 < iVar3) {
      iVar3 = 0x40000;
    }
    if ((-0x10000 < iVar3) && (iVar3 < 0x10000)) {
      if (iVar3 < 0) {
        iVar3 = -0x10000;
      }
      else {
        iVar3 = 0x10000;
      }
    }
    unaff_A6[0xc] = iVar3;
    if (iVar3 < 0) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    }
  }
  *unaff_A6 = 0x9e722;
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  if (*(short *)(unaff_A6 + 0xd) < 0) {
    unaff_A6[0x14] = 0x9f610;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xdf;
    unaff_A6[0x14] = 0x9f610;
    *unaff_A6 = candidate_9df5e;
  }
  if ((*(byte *)((int)unaff_A6 + 0x96) & 4) != 0) {
    candidate_49e8();
  }
  sVar1 = *(short *)(unaff_A6 + 0x22);
  bVar4 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
  candidate_4c24();
  if (bVar4) {
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
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
