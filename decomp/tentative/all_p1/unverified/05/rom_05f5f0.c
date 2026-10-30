/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05f5f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5f5f0(void)

{
  short sVar1;
  undefined2 uVar2;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8b) = 0;
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  candidate_4256();
  if (*(short *)(unaff_A6 + 0x20) == 0) {
    uVar2 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x82) = uVar2;
  }
  else {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 1;
  }
  candidate_40bc();
  candidate_1acf2();
  *(undefined1 *)(extraout_A0 + 0x88) = *(undefined1 *)(unaff_A6 + 0x22);
  *(undefined2 *)(extraout_A0 + 0x80) = *(undefined2 *)(unaff_A6 + 0x20);
  *(ushort *)(extraout_A0 + 0x80) = *(ushort *)(extraout_A0 + 0x80) & 3;
  *(undefined2 *)(extraout_A0 + 0x82) = *(undefined2 *)((int)unaff_A6 + 0x82);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)(extraout_A0 + 0x15) = *(byte *)(extraout_A0 + 0x15) | 1;
  }
  *unaff_A6 = 0x5f694;
  uVar3 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    candidate_143dc();
  }
  else {
    candidate_1456a();
  }
  candidate_4c24();
  if (((((bool)uVar3) && (*(char *)((int)unaff_A6 + 0x8b) != '\0')) &&
      ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) &&
     ((*(char *)((int)unaff_A6 + 0x89) == '\0' && (*(char *)((int)unaff_A6 + 0x8d) == '\0')))) {
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
    unaff_A6[0x21] = unaff_A6[8];
    candidate_1b206();
    candidate_4256();
  }
  if (*(char *)(unaff_A6 + 0x22) == '\0') {
    unaff_A6[0x14] = 0x5eb62;
  }
  else {
    unaff_A6[0x14] = 0x5eb46;
  }
  bVar4 = false;
  candidate_427a();
  if ((bVar4) && (*(char *)((int)unaff_A6 + 0x8d) != '\0')) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x82);
  *(short *)((int)unaff_A6 + 0x82) = sVar1 + -1;
  if ((sVar1 == 0) && (*(char *)((int)unaff_A6 + 0x8d) == '\0')) {
    *unaff_A6 = candidate_5f780;
  }
  candidate_5fd48();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
