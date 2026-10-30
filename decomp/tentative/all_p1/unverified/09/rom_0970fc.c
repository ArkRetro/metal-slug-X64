/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0970fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_970fc(void)

{
  byte bVar1;
  char cVar3;
  short sVar2;
  undefined1 extraout_D1b;
  short extraout_D1w;
  undefined4 uVar4;
  int extraout_A0;
  undefined2 *extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  *(undefined1 *)(unaff_A6 + 0x21) = 1;
  candidate_966f0();
  candidate_411c();
  *(undefined4 *)(extraout_A0 + 0x72) = 0x29dcde;
  candidate_1acf2();
  *unaff_A6 = 0x97136;
  candidate_ca2ce();
  *(char *)((int)unaff_A6 + 0x85) = (char)*extraout_A0_00;
  if ((*(byte *)((int)unaff_A6 + 0x85) & 8) == 0) {
    if ((*(byte *)((int)unaff_A6 + 0x85) & 4) == 0) {
      uVar4 = 0;
    }
    else {
      uVar4 = 0xfffe0000;
    }
  }
  else {
    uVar4 = 0x20000;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = uVar4;
  uVar5 = (*(ushort *)((int)unaff_A6 + 0x62) >> 0xc & 1) != 0;
  candidate_14850();
  if (!(bool)uVar5) {
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
    uVar5 = 0;
  }
  candidate_14594();
  candidate_1ad8c();
  candidate_427a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_96c22;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_96f6a;
  }
  if ((*(char *)(unaff_A6 + 0x24) != '\0') && ((*(byte *)((int)unaff_A6 + 0x85) & 0x20) != 0)) {
    *unaff_A6 = candidate_96f86;
  }
  if ((*(byte *)((int)unaff_A6 + 0x85) & 0x40) != 0) {
    candidate_96728();
  }
  if ((*(byte *)((int)unaff_A6 + 0x85) & 0x80) != 0) {
    *unaff_A6 = candidate_971f8;
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
  bVar6 = false;
  candidate_aa760();
  if (bVar6) {
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
