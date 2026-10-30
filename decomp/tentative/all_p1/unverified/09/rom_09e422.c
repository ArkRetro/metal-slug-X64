/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e422.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e422(void)

{
  char cVar1;
  short sVar3;
  int iVar2;
  undefined2 uVar4;
  undefined2 extraout_D1w;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  unaff_A6[0xd] = 0xaaaaa;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xfffe38e4;
  unaff_A6[0xc] = 0;
  sVar3 = candidate_1ad7c();
  iVar2 = (int)(short)(sVar3 - *(short *)(unaff_A6 + 0xf)) << 8;
  if (iVar2 != 0) {
    iVar2 = (int)(short)(iVar2 / 0x12) << 8;
  }
  unaff_A6[0xc] = iVar2;
  *unaff_A6 = 0x9e4aa;
  func_0x000143e4();
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x72) + unaff_A6[0xd];
  uVar4 = candidate_1ad7c();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar4;
  *(undefined2 *)(unaff_A6 + 0x1e) = extraout_D1w;
  sVar3 = candidate_1b29c();
  *(short *)(unaff_A6 + 0x20) = sVar3;
  if (sVar3 < 0x15) {
    *(short *)(unaff_A6 + 10) =
         (*(short *)((int)unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0xf)) + *(short *)(unaff_A6 + 10)
    ;
    *(short *)(unaff_A6 + 0xb) =
         (*(short *)(unaff_A6 + 0x1e) - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
    candidate_1abc8();
    candidate_b64ba();
    *unaff_A6 = candidate_9e578;
  }
  if (*(short *)(unaff_A6 + 0x20) < 0x31) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xdf;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
    *unaff_A6 = candidate_9df5e;
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
