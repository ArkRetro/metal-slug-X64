/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09df5e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9df5e(void)

{
  short sVar1;
  char cVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  if ((*(byte *)(unaff_A6 + 0x26) & 2) == 0) {
    candidate_4256();
  }
  else {
    *(byte *)(unaff_A6 + 0x26) = *(byte *)(unaff_A6 + 0x26) & 0xfd;
    candidate_4256();
  }
  *unaff_A6 = 0x9df98;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9d9c0;
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
