/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e578.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e578(void)

{
  char cVar1;
  short sVar2;
  short extraout_D1w;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  sVar2 = candidate_1ad7c();
  *(short *)(unaff_A6 + 10) = (sVar2 - *(short *)(unaff_A6 + 0xf)) + *(short *)(unaff_A6 + 10);
  *(short *)(unaff_A6 + 0xb) =
       (extraout_D1w - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
  unaff_A6[0x14] = 0;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  if ((*(byte *)((int)unaff_A6 + 0x96) & 4) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x9e5f0;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9e60a;
  }
  sVar2 = *(short *)(unaff_A6 + 0x22);
  bVar4 = sVar2 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar2 + -1;
  candidate_4c24();
  if (bVar4) {
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
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
