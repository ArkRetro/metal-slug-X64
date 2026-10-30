/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e25a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e25a(void)

{
  short sVar1;
  char cVar2;
  uint uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  uVar3 = candidate_29c6();
  if ((uVar3 & 1) == 0) {
    candidate_4256();
    unaff_A6[0xd] = 0xa0000;
    *(undefined4 *)((int)unaff_A6 + 0x72) = 0xfffec000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      unaff_A6[0xc] = 0xffff8000;
    }
    else {
      unaff_A6[0xc] = 0x8000;
    }
    *unaff_A6 = 0x9e31e;
    func_0x000143e4();
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xdf;
      *unaff_A6 = candidate_9df5e;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x9e27e;
    uVar4 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_9e298;
    }
  }
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
