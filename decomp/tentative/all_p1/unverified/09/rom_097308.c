/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x097308.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_97308(void)

{
  uint uVar1;
  byte bVar2;
  uint uVar3;
  char cVar5;
  short sVar4;
  undefined1 extraout_D1b;
  short extraout_D1w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;
  bool bVar7;

  *(undefined1 *)(unaff_A6 + 0x21) = 1;
  candidate_966f0();
  *unaff_A6 = 0x9731a;
  uVar3 = ((int)unaff_A6[0xd] >> 5) + 0x10000;
  uVar1 = unaff_A6[0xd];
  uVar6 = uVar1 < uVar3;
  unaff_A6[0xd] = uVar1 - uVar3;
  candidate_143ee();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_9737e;
    uVar6 = 0;
  }
  candidate_427a();
  if ((bool)uVar6) {
    *unaff_A6 = candidate_96f6a;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_970fc;
      candidate_1b1dc();
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    candidate_1b1dc();
  }
  bVar2 = *(byte *)((int)unaff_A6 + 0x5f);
  *(byte *)((int)unaff_A6 + 0x5f) = bVar2 & 0xbf;
  if ((bVar2 & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  if (*(char *)((int)unaff_A6 + 0x8e) != '\0') {
    if ((*(ushort *)(unaff_A5 + -0x7ff2) & 1) != 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
      *(undefined1 *)((int)unaff_A6 + 0x7a) = 0xff;
      *(undefined1 *)((int)unaff_A6 + 0x7e) = 0xff;
    }
    *(char *)((int)unaff_A6 + 0x8e) = *(char *)((int)unaff_A6 + 0x8e) + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar7 = false;
  candidate_aa760();
  if (bVar7) {
    cVar5 = candidate_aaa88();
    if (cVar5 == '\x03') {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = extraout_D1b;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0x2a;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *unaff_A6 = candidate_96ba6;
  }
  sVar4 = candidate_96916();
  if ((short)(extraout_D1w - sVar4) < 0) {
    *unaff_A6 = candidate_975c6;
  }
  bVar7 = false;
  candidate_aa9e4();
  if (bVar7) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x40) == 0) {
      candidate_977dc();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0x2a;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *unaff_A6 = candidate_9737e;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      *unaff_A6 = candidate_97308;
    }
  }
  if (*(short *)((int)unaff_A6 + 0x4e) == 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  }
  if (((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) &&
     ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0)) {
    *unaff_A6 = candidate_973e2;
  }
  return;
}
