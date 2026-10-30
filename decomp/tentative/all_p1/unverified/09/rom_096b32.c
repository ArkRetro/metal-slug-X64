/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096b32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96b32(void)

{
  uint uVar1;
  byte bVar2;
  char cVar4;
  short sVar3;
  undefined1 extraout_D1b;
  short extraout_D1w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
  candidate_1b1dc();
  *(undefined1 *)(unaff_A6 + 0x21) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8e) = 0x2a;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  candidate_966f0();
  candidate_120f8();
  *unaff_A6 = 0x96b62;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    unaff_A6[0xc] = 0;
    uVar1 = unaff_A6[0xd];
    uVar5 = 0x4fff < uVar1;
    unaff_A6[0xd] = uVar1 - 0x5000;
    candidate_143ee();
  }
  else {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar5 = 0;
    candidate_14594();
  }
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_96c22;
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
  bVar6 = false;
  candidate_aa760();
  if (bVar6) {
    cVar4 = candidate_aaa88();
    if (cVar4 == '\x03') {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = extraout_D1b;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0x2a;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *unaff_A6 = candidate_96ba6;
  }
  sVar3 = candidate_96916();
  if ((short)(extraout_D1w - sVar3) < 0) {
    *unaff_A6 = candidate_975c6;
  }
  bVar6 = false;
  candidate_aa9e4();
  if (bVar6) {
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
