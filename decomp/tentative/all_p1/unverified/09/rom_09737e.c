/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09737e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9737e(void)

{
  byte bVar1;
  char cVar3;
  short sVar2;
  undefined1 extraout_D1b;
  short extraout_D1w;
  undefined2 *extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;
  bool bVar5;

  candidate_1b1dc();
  *(undefined1 *)(unaff_A6 + 0x21) = 1;
  candidate_966f0();
  *unaff_A6 = 0x97396;
  candidate_ca2ce();
  *(char *)((int)unaff_A6 + 0x85) = (char)*extraout_A0;
  uVar4 = 0;
  candidate_14594();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_96c22;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0x2a;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *unaff_A6 = candidate_96f6a;
  }
  bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
  *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
  if ((bVar1 & 0x40) != 0) {
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
  bVar5 = false;
  candidate_aa760();
  if (bVar5) {
    cVar3 = candidate_aaa88();
    if (cVar3 == '\x03') {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = extraout_D1b;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x8e) = 0x2a;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    *unaff_A6 = candidate_96ba6;
  }
  sVar2 = candidate_96916();
  if ((short)(extraout_D1w - sVar2) < 0) {
    *unaff_A6 = candidate_975c6;
  }
  bVar5 = false;
  candidate_aa9e4();
  if (bVar5) {
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
