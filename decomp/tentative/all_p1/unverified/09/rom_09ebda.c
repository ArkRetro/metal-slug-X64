/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09ebda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9ebda(void)

{
  short sVar1;
  char cVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  uVar3 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x22) = (uVar3 & 0x1f) + 0x14;
  candidate_29c6();
  candidate_4256();
  *unaff_A6 = 0x9ec12;
  candidate_9f044();
  candidate_427a();
  *(short *)(unaff_A6 + 0x22) = *(short *)(unaff_A6 + 0x22) + -1;
  if (*(short *)(unaff_A6 + 0x22) < 1) {
    *unaff_A6 = candidate_9eb36;
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
