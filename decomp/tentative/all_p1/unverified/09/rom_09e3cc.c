/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09e3cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9e3cc(void)

{
  short sVar1;
  char cVar2;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar3;

  candidate_9efc8();
  candidate_427a();
  candidate_9f0fa();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_9e3e8;
  }
  if (((*(byte *)(unaff_A6 + 0x26) & 4) != 0) &&
     (*(short *)((int)unaff_A6 + 0x8a) = *(short *)((int)unaff_A6 + 0x8a) + -1,
     *(short *)((int)unaff_A6 + 0x8a) < 1)) {
    *unaff_A6 = candidate_9de84;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_9e34c;
  }
  sVar1 = *(short *)(unaff_A6 + 0x22);
  bVar3 = sVar1 == 0;
  *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
  candidate_4c24();
  if (bVar3) {
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
  bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_9ec34;
  }
  return;
}
