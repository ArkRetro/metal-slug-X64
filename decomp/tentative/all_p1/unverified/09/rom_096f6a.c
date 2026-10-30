/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x096f6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_96f6a(void)

{
  byte bVar1;
  char cVar3;
  short sVar2;
  undefined1 extraout_D1b;
  short extraout_D1w;
  int iVar4;
  undefined2 *extraout_A0;
  undefined2 *extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  candidate_966f0();
  unaff_A6[0xc] = *(undefined4 *)((int)unaff_A6 + 0x62);
  *unaff_A6 = 0x97028;
  candidate_ca2ce();
  *(char *)((int)unaff_A6 + 0x85) = (char)*extraout_A0;
  iVar4 = unaff_A6[0xc];
  if ((*(byte *)((int)unaff_A6 + 0x85) & 8) == 0) {
    if (((*(byte *)((int)unaff_A6 + 0x85) & 4) != 0) && (iVar4 = iVar4 + -0xaaaa, iVar4 < -0x38000))
    {
      iVar4 = -0x38000;
    }
  }
  else {
    iVar4 = iVar4 + 0xaaaa;
    if (0x38000 < iVar4) {
      iVar4 = 0x38000;
    }
  }
  unaff_A6[0xc] = iVar4;
  candidate_96794();
  candidate_968c0();
  unaff_A6[0xd] = unaff_A6[0xd] - (((int)unaff_A6[0xd] >> 5) + 0x10000);
  candidate_143ee();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_970fc;
    candidate_1b1dc();
    candidate_ca2ce();
    *(char *)((int)unaff_A6 + 0x85) = (char)*extraout_A0_00;
  }
  candidate_427a();
  if ((*(short *)((int)unaff_A6 + 0x76) == 0xc) && ((*(byte *)((int)unaff_A6 + 0x85) & 0x20) != 0))
  {
    *unaff_A6 = candidate_96f7a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x85) & 0x40) != 0) {
    candidate_96728();
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
